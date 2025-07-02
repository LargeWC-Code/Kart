#ifndef _game_hall_H_
#define _game_hall_H_

#include "../common/game_db_robj.h"
#include "../common/game_hall_robj.h"
#include "../common/game_match_robj.h"
#include "../../public/common/game_center_robj.h"
#include "../../public/common/log_robj.h"
#include "../../public/common/monitor_robj.h"

//本地配置存档
struct CGameHall_LocalCfg
{
	ucINT						EnableLog;					//是否开启日志
	UCString				LogUrl;						//日志系统地址

	UCString				Key;						//密钥，防止冲突

	ucINT						SingleGate;					//是否一对一的Gate
	UCString				PrivateGate;				//内网Gate地址//结束（本机可以填空）
	UCString				DnsURL;						//DNS服路径//结束
};

class CGameExtRDObj : public CUserData_GameExt_RDObj
{
public:
	ucINT								MatchIndex;				//开始匹配的匹配服
	ucDWORD							MatchTime;				//开始匹配时间
	ucDWORD							LastKeepalive;			//保持匹配的心跳，防止挂机用户继续匹配
	UCEListPtr*						ListPos;				//给外部管理器UCEList的索引
};
class CUserData_Local : public CUserData_RDObj
{
public:
	CUserData_ExtData_RDObj			UserExtData_RDObj;		//扩展数据
	CUserData_Ext_RDObj				Ext_RDObj;				//用于顶号的快照
	CUserData_GameDBExt_RDObj		GameKeyRD;				//用户快速登录Key，服务器之间使用
	CGameExtRDObj					GameExt_RDObj;			//专门给场内修改用的快照
	ucINT64							UserGameID;				//标记玩家ID
	ucDWORD							OnlineTimeWord;			//最后计算在线时间点
public:
	CUserData_Local()
	{
		OnlineTimeWord = UCGetTime();
		GameExt_RDObj.MatchIndex = -1;
		GameExt_RDObj.MatchTime = OnlineTimeWord;
		GameExt_RDObj.LastKeepalive = OnlineTimeWord;
		GameExt_RDObj.ListPos = ucNULL;
	}
};

class CGameHallService : public CGame_Hall_RCObj
{
public:
	CGame_Center_RCObj				m_Game_Center_RCObj;			//中央服务器的远程调用
	CMonitor_RDObj					m_Monitor_RDObj;				//监控快照
	CMonitor_RCObj					m_Monitor_RCObj;				//监控上报
private:
	//大厅相关信息，用于压力均衡
	UCIntStringMap					m_MapDB_Info_Open;				//所有对外GameDB
	UCIntIntMap						m_MapDB_Info_UnOpen;			//所有对内GameDB
	CGameDB_Info_RDObj				m_GameDB_Info_RDObj;			//中央存储的所有的大厅信息
	CGame_DB_RCObj					m_GameDB_RCObj[GAMEDB_MAX];

	UCIntStringMap					m_MapMatch_Info_Open;			//所有对外游戏服务器
	UCIntIntMap						m_MapMatch_Info_UnOpen;			//所有对内游戏服务器
	CMatch_Info_RDObj				m_Match_Info_RDObj;				//中央存储的所有的匹配信息
	CGame_Match_RCObj				m_GameMatchRCObj[MATCH_MAX];	//匹配服缓存
	UCVF_DOMArray					m_aryGameMatchDom;				//域名解析出的有效匹配服

	UCESimpleMap<ucINT64, CUserData_Local*>	m_MapKeyUserData;
	UCEList<CUserData_Local*>			m_matchUserData;			//正在匹配的玩家，按时间排列
	UCFiber								m_CheckMatchFiber;			//检查匹配玩家是否还活着
private:
	UCEConfig<CGameHall_LocalCfg>	m_LocalCfg;

	UCVF_Service					m_Service;

	UCString						m_PrivateGate;				//我的私有Gate
	ucINT								m_Index;
	ucDWORD							m_CenterKey;				//中心服的临时key
	ucINT64							m_AdminKey;
	UCFiber							m_FiberInit;				//连接初始化
	UCLogCall						m_Log;
public:
	//激活用户
	UCString Enter(const CUserGameID& UserGameID, const ucDWORD& GameKey)
	{
		if (!CheckGameDBValid(UserGameID.DBIndex))
			return UCString("登陆失败");

		//到GameDB校验UserKey的合法性
		UCString strRet = m_GameDB_RCObj[UserGameID.DBIndex].HallCheckValid(UserGameID, GameKey, m_Index);
		if (strRet != UCString("成功"))
			return UCString("登陆失败1");

		return LoadUseInfo(UserGameID, GameKey);
	}

	UCString ChangeName(const CUserGameID& UserGameID, const ucDWORD& GameKey, UCString Name)
	{
		ucINT64 Key = UserGameID.ToInt64();
		ucINT Pos = m_MapKeyUserData.FindKey(Key);
		if (Pos < 0)
			return UCString("无效的用户ID");
		CUserData_Local* UserData = m_MapKeyUserData.GetValueAt(Pos);
		if (UserData->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效的Key");
		if (UserData->LockHost() == 0)
		{
			UserData->NickName = Name;
			UserData->UnlockHost();
		}
		return UCString("成功");
	}

	UCString UpdateOnllineTime(const CUserGameID& UserGameID, const ucDWORD& GameKey)//上传在线时间(1分钟一次)
	{
		ucINT64 Key = UserGameID.ToInt64();
		ucINT Pos = m_MapKeyUserData.FindKey(Key);
		if (Pos < 0)
			return UCString("无效的用户ID");
		CUserData_Local* UserData = m_MapKeyUserData.GetValueAt(Pos);
		if (UserData->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效的Key");
		UCTime NowTime;
		if (NowTime > UserData->UserExtData_RDObj.DailyFreshTime.Value)
		{
			//重置
			if (UserData->UserExtData_RDObj.LockHost() == 0)
			{
				UserData->UserExtData_RDObj.OnlinePrizeMinute = 0;
				UserData->UserExtData_RDObj.DailyOnlineMinute = 0;
				UserData->UserExtData_RDObj.DailyOnlineTimePrizeTimes = 1;
				UserData->UserExtData_RDObj.DailyGetStarCoinTimes = 5;
				UserData->UserExtData_RDObj.DailyFreshTime = UCTime(NowTime.GetYear(), NowTime.GetMonth(), NowTime.GetDay() + 1, 0, 0, 0);
				UserData->UserExtData_RDObj.UnlockHost();
			}
			if (UserData->LockHost() == 0)
				UserData->UnlockHost();
			UserData->OnlineTimeWord = UCGetTime();
		}
		ucINT lasttime = UCGetTime() - UserData->OnlineTimeWord;
		//m_Service.Log(ITOS(UserData->LastUpdateTime) + UCString("//////")+ ITOS(lasttime) + UCString("\r\n"));
		if (lasttime < 600000)
		{
			return UCString("");
		}
		ucINT iAdd = lasttime / 600000;
		UserData->OnlineTimeWord += iAdd * 600000;
		ucDWORD dwTime = UCGetTime();
		if (UserData->UserExtData_RDObj.LockHost() == 0)
		{
			UserData->UserExtData_RDObj.OnlinePrizeMinute += iAdd;
			UserData->UserExtData_RDObj.DailyOnlineMinute += iAdd;
			UserData->UserExtData_RDObj.UnlockHost();
			ucDWORD dwtTime = UCGetTime() - dwTime;
			if (dwtTime > 10000)
				m_Service.Log(UCString("UpdateOnllineTime LockHost时间超过1秒：") + ITOS(dwtTime) + UCString("\r\n"));
		}
		else
		{
			m_Service.Log(UCString("UpdateOnllineTime LockHost失败\r\n"));
		}
		return UCString("成功");
	}
	UCString CheckOnlinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey, ucINT& onlinetime)//得到在线奖励的信息
	{
		ucINT64 Key = UserGameID.ToInt64();
		ucINT Pos = m_MapKeyUserData.FindKey(Key);
		if (Pos < 0)
			return UCString("无效的用户ID");
		CUserData_Local* UserData = m_MapKeyUserData.GetValueAt(Pos);
		if (UserData->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效的Key");
		onlinetime = UserData->UserExtData_RDObj.OnlinePrizeMinute.Value;
		return UCString("成功");
	}
	UCString GetOnlinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey)//获取在线奖励
	{
		ucINT64 Key = UserGameID.ToInt64();
		ucINT Pos = m_MapKeyUserData.FindKey(Key);
		if (Pos < 0)
			return UCString("无效的用户ID");
		CUserData_Local* UserData = m_MapKeyUserData.GetValueAt(Pos);
		if (UserData->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效的Key");
		UpdateOnllineTime(UserGameID, GameKey);
		if (UserData->UserExtData_RDObj.LockHost() == 0)
		{
			ucINT getgold = UserData->UserExtData_RDObj.OnlinePrizeMinute.Value * 2;
			UserData->UserExtData_RDObj.OnlinePrizeMinute = 0;
			if (getgold > 0)
			{
				if (UserData->LockHost() == 0)
				{
					UserData->Gold += getgold;
					UserData->UnlockHost();
				}
			}
			UserData->UserExtData_RDObj.UnlockHost();
		}
		return UCString("成功");
	}
	UCString CheckOfflinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey, ucINT& offlinetime)//得到离线时间的时��
	{
		ucINT64 Key = UserGameID.ToInt64();
		ucINT Pos = m_MapKeyUserData.FindKey(Key);
		if (Pos < 0)
			return UCString("无效的用户ID");
		CUserData_Local* UserData = m_MapKeyUserData.GetValueAt(Pos);
		if (UserData->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效的Key");
		offlinetime = UserData->UserExtData_RDObj.OfflineMinutes.Value;
		return UCString("成功");
	}
	UCString GetOfflinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey,ucINT WithAD)//获得离线奖励
	{
		ucINT64 Key = UserGameID.ToInt64();
		ucINT Pos = m_MapKeyUserData.FindKey(Key);
		if (Pos < 0)
			return UCString("无效的用户ID");
		CUserData_Local* UserData = m_MapKeyUserData.GetValueAt(Pos);
		if (UserData->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效的Key");
		ucINT allminute = UserData->UserExtData_RDObj.OfflineMinutes.Value;
		if (allminute < 60)
			return UCString("离线时间不够");

		//ucINT highminute = allminute;
		ucINT midminute = allminute;
		ucINT lowminute = 0;
		if (midminute > 240)
		{
			lowminute = midminute - 240;
			midminute = 240;
		}

		ucINT getmid = 1;
		ucINT getlow = 1;
		if (WithAD == 1)
		{
			getmid = 3;
			getlow = 3;
		}

		ucINT getgold = 4 * getmid * (midminute / 6) + 3 * getlow * (lowminute / 6);
		if (UserData->UserExtData_RDObj.LockHost() == 0)
		{
			UserData->UserExtData_RDObj.OfflineMinutes = 0;
			UserData->UserExtData_RDObj.UnlockHost();
		}
		if (UserData->LockHost() == 0)
		{
			UserData->Gold += getgold;
			UserData->UnlockHost();
		}
		return UCString("成功");
	}
	UCString CheckDailyOnlinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey, ucINT& steptime, ucINT& onlinetime)//得到��日在线奖励信息10,30,60三档
	{
		ucINT64 Key = UserGameID.ToInt64();
		ucINT Pos = m_MapKeyUserData.FindKey(Key);
		if (Pos < 0)
			return UCString("无效的用户ID");
		CUserData_Local* UserData = m_MapKeyUserData.GetValueAt(Pos);
		if (UserData->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效的Key");
		steptime = UserData->UserExtData_RDObj.DailyOnlineTimePrizeTimes.Value;
		onlinetime = UserData->UserExtData_RDObj.DailyOnlineMinute.Value;
		return UCString("成功");
	}
	UCString GetDailyOnlinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey, ucINT WithAD)//得到奖励，是否看了广告
	{
		ucINT64 Key = UserGameID.ToInt64();
		ucINT Pos = m_MapKeyUserData.FindKey(Key);
		if (Pos < 0)
			return UCString("无效的用户ID");
		CUserData_Local* UserData = m_MapKeyUserData.GetValueAt(Pos);
		if (UserData->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效的Key");
		UpdateOnllineTime(UserGameID, GameKey);
		if (UserData->UserExtData_RDObj.DailyOnlineTimePrizeTimes.Value == 0)
			return UCString("已领完");
		ucINT getgold = 0;
		ucINT getstart = 0;
		ucINT jumpstep = 1;
		if (UserData->UserExtData_RDObj.DailyOnlineTimePrizeTimes.Value == 1)
		{
			if (UserData->UserExtData_RDObj.DailyOnlineMinute.Value < 10)
				return UCString("时间未到");
			getgold = 80;
			getstart = 1;
			jumpstep = 2;
		}
		else if (UserData->UserExtData_RDObj.DailyOnlineTimePrizeTimes.Value == 2)
		{
			if (UserData->UserExtData_RDObj.DailyOnlineMinute.Value < 30)
				return UCString("时间未到");
			getgold = 160;
			getstart = 1;
			jumpstep = 3;
		}
		else if (UserData->UserExtData_RDObj.DailyOnlineTimePrizeTimes.Value == 3)
		{
			if (UserData->UserExtData_RDObj.DailyOnlineMinute.Value < 60)
				return UCString("时间未到");
			getgold = 240;
			getstart = 1;
			jumpstep = 0;
		}

		if (UserData->UserExtData_RDObj.LockHost() == 0)
		{
			UserData->UserExtData_RDObj.DailyOnlineTimePrizeTimes = jumpstep;
			UserData->UserExtData_RDObj.UnlockHost();
		}
		if (UserData->LockHost() == 0)
		{
			if (WithAD == 1)
			{
				getgold *= 3;
			}
			UserData->Gold += getgold;
			UserData->UnlockHost();
		}
		return UCString("成功");
	}
public:
	CGameHallService()
	{
		m_CenterKey = 0;
		m_AdminKey = 0;
		m_CheckMatchFiber.FiberEvent = UCEvent(this, OnMyCheckMatchFiber);
		m_FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		m_Service.OnClose = UCEvent(this, OnMyServiceClose);
	}
	~CGameHallService()
	{
	}
	ucINT RandMatchServer(ucINT bDesc)
	{
		if (m_Monitor_RDObj.MatchServiceNum.Value > 0)
		{
			ucINT iRandIndex = randint(0, m_Monitor_RDObj.MatchServiceNum.Value - 1);
			if (m_Match_Info_RDObj.Match_Info[iRandIndex].Open)
				return iRandIndex;
		}
		if (m_aryGameMatchDom.GetSize() > 0)
		{
			ucINT iIndex = 0;
			if (bDesc)
				iIndex = m_aryGameMatchDom.GetSize() - 1;
			UCString strDomURL = m_aryGameMatchDom.GetAt(iIndex)->OutName;
			ucINT iFind = strDomURL.Find(UCString("_"));
			if (iFind > 0)
			{
				ucINT iIndex = STOI(strDomURL.Mid(iFind + 1));
				if (iIndex >= 0)
					return iIndex;
			}
		}
		if (m_MapMatch_Info_Open.GetSize() > 0)
		{
			ucINT Pos = randint(0, m_MapMatch_Info_Open.GetSize() - 1);
			return m_MapMatch_Info_Open.GetKeyAt(Pos);
		}
		return -1;
	}
	ucINT CheckMatchValid(ucINT iMatchIndex)
	{
		if (iMatchIndex < 0 || iMatchIndex >= MATCH_MAX || m_Match_Info_RDObj.Match_Info[iMatchIndex].Open == 0)
			return 0;
		if (m_GameMatchRCObj[iMatchIndex].Linked == 0)
		{
			ucINT ret = m_GameMatchRCObj[iMatchIndex].Link(m_PrivateGate + UCString("dom://GameMatch_") + ITOS(iMatchIndex) + UCString("//CGame_Match_RCObj.call"), m_AdminKey);
			if (ret)
			{
				//m_Service.Log(UCString("连接GameMatch失败:") + m_PrivateGate + UCString("dom://GameMatch_") + ITOS(iMatchIndex) + UCString("//CGame_Match_RCObj.call,ret=") + ITOS(ret) + UCString("\r\n"));
				return 0;
			}
		}
		return 1;
	}
	ucINT CheckGameDBValid(ucINT iDBIndex)
	{
		//需要先到GameDB校验UserKey的合法性
		if (iDBIndex < 0 || iDBIndex >= GAMEDB_MAX || m_GameDB_Info_RDObj.GameDB_Info[iDBIndex].Open == 0)
		{
			//m_Service.Log(UCString("无效的序号:") + ITOS(iDBIndex) + UCString("\r\n"));
			return 0;
		}

		if (m_GameDB_RCObj[iDBIndex].Linked == 0)
		{
			ucINT ret = m_GameDB_RCObj[iDBIndex].Link(m_PrivateGate + UCString("dom://GameDB_") + ITOS(iDBIndex) + UCString("//CGame_DB_RCObj.call"), m_AdminKey);
			if (ret)
			{
				m_Service.Log(UCString("连接GameDB失败:") + m_PrivateGate + UCString("dom://GameDB_") + ITOS(iDBIndex) + UCString("//CGame_DB_RCObj.call,ret=") + ITOS(ret) + UCString("\r\n"));
				return 0;
			}
		}
		return 1;
	}
	UCString LoadUseInfo(const CUserGameID& UserGameID, const ucDWORD& GameKey)
	{
		ucINT64 Key = UserGameID.ToInt64();
		ucINT Pos = m_MapKeyUserData.FindKey(Key);
		if (Pos >= 0)
		{
			CUserData_Local* UserData = m_MapKeyUserData.GetValueAt(Pos);
			return UCString("成功");
		}

		UCString strUserID = UserGameID.ToString();

		CUserData_Local* UserData = new CUserData_Local;
		UserData->OnRemoteRelease = UCEvent(this, OnMyUserDataRelease);
		//UserData->OnTimeOut = UCEvent(this, OnUserDataTimeOut);
		UserData->UserGameID = Key;

		UCString strBaseURL = m_Service.Name + UCString("//UserData/") + UserGameID.ToString();
		if (!m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_RDObj.data"), typeof(CUserData_RDObj), UserData, UCVF_Guest_Permission_ReadOnly))
		{
			delete UserData;
			return UCString("Append UserData失败");
		}
		if (!m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_ExtData_RDObj.data"), typeof(CUserData_ExtData_RDObj), &UserData->UserExtData_RDObj, UCVF_Guest_Permission_ReadOnly))
		{
			delete UserData;
			return UCString("Append UserData失败");
		}
		if (!m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_Ext_RDObj.data"), typeof(CUserData_Ext_RDObj), &UserData->Ext_RDObj, UCVF_Guest_Permission_ReadOnly))
		{
			delete UserData;
			return UCString("Append GameDBExt失败");
		}
		if (!m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_GameDBExt_RDObj.data"), typeof(CUserData_GameDBExt_RDObj), &UserData->GameKeyRD))
		{
			delete UserData;
			return UCString("Append GameDBExt失败");
		}
		if (!m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_GameExt_RDObj.data"), typeof(CUserData_GameExt_RDObj), &UserData->GameExt_RDObj))
		{
			delete UserData;
			return UCString("Append GameDBExt失败");
		}
		UserData->GameExt_RDObj.OnReset = UCEvent(this, OnMyUserDataGameExtReset);
		UserData->GameExt_RDObj.BattleInfo.BindOnSet(UCEvent(this, OnMyUserDataGameExtChanged));

		UCString strDBURL = m_PrivateGate + UCString("dom://GameDB_") + ITOS(UserGameID.DBIndex) + UCString("//UserData/") + UserGameID.ToString();
		if (UserData->StableLink(strDBURL + UCString("/CUserData_RDObj.data"), m_AdminKey) != 0)
		{
			m_Service.Log(strDBURL + UCString("/CUserData_RDObj.data 连接失败\r\n"));
			delete UserData;
			return UCString("连接失败");
		}
		if (UserData->UserExtData_RDObj.StableLink(strDBURL + UCString("/CUserData_ExtData_RDObj.data"), m_AdminKey) != 0)
		{
			m_Service.Log(strDBURL + UCString("/CUserData_ExtData_RDObj.data 连接失败\r\n"));
			delete UserData;
			return UCString("连接失败");
		}
		if (UserData->Ext_RDObj.StableLink(strDBURL + UCString("/CUserData_Ext_RDObj.data"), m_AdminKey) != 0)
		{
			m_Service.Log(strDBURL + UCString("/CUserData_Ext_RDObj.data 连接失败\r\n"));
			delete UserData;
			return UCString("连接失败");
		}
		if (UserData->GameKeyRD.StableLink(strDBURL + UCString("/CUserData_GameDBExt_RDObj.data"), m_AdminKey) != 0)
		{
			m_Service.Log(strDBURL + UCString("/CUserData_GameDBExt_RDObj.data 连接失败\r\n"));
			delete UserData;
			return UCString("连接失败");
		}
		if (UserData->GameExt_RDObj.StableLink(strDBURL + UCString("/CUserData_GameExt_RDObj.data"), m_AdminKey) != 0)
		{
			m_Service.Log(strDBURL + UCString("/CUserData_GameExt_RDObj.data 连接失败\r\n"));
			delete UserData;
			return UCString("连接失败");
		}
		//因为前面有fiber切换，需要确保列表里没有
		Pos = m_MapKeyUserData.FindKey(Key);
		if (Pos < 0)
		{
			m_MapKeyUserData.Add(Key, UserData);
			m_Service.SetValue(2 + m_MapKeyUserData.GetSize());
		}
		else
		{
			delete UserData;
			UserData = m_MapKeyUserData.GetValueAt(Pos);
		}
		return UCString("成功");
	}
	ucVOID OnMyUserDataGameExtReset(UCObject* o, UCEventArgs*)
	{
		CGameExtRDObj* pGameExtRDObj = (CGameExtRDObj*)o;
		if (pGameExtRDObj->BattleInfo.GameID != -1)
		{
			pGameExtRDObj->MatchIndex = -1;
			if (pGameExtRDObj->ListPos)
			{
				m_matchUserData.RemoveAt(pGameExtRDObj->ListPos);
				pGameExtRDObj->ListPos = ucNULL;
			}
		}
	}
	ucVOID OnMyUserDataGameExtChanged(UCObject*, UCEventArgs* e)
	{
		UCVF_PropertyBaseCmd_Args* Args = (UCVF_PropertyBaseCmd_Args*)e;
		CBattleInfo* pBattleInfo = (CBattleInfo*)Args->Data;
		if (pBattleInfo->GameID != -1)
		{
			CGameExtRDObj* pGameExtRDObj = (CGameExtRDObj*)pBattleInfo->GetDataBase();
			if (pGameExtRDObj)
			{
				pGameExtRDObj->MatchIndex = -1;
				if (pGameExtRDObj->ListPos)
				{
					m_matchUserData.RemoveAt(pGameExtRDObj->ListPos);
					pGameExtRDObj->ListPos = ucNULL;
				}
			}
		}
	}
	ucVOID OnMyUserDataRelease(UCObject* Object, UCEventArgs*)
	{
		CUserData_Local* UserData = (CUserData_Local*)Object;
		ucINT Pos = m_MapKeyUserData.FindKey(UserData->UserGameID);
		if (Pos < 0 || UserData != m_MapKeyUserData.GetValueAt(Pos))
			return;
		//m_Service.Log(UCString("超时玩家 : ") + CPlatID(iPlatID).ToString() + UCString("，自动注销\r\n"));
		if (UserData->GameExt_RDObj.ListPos)
		{
			m_matchUserData.RemoveAt(UserData->GameExt_RDObj.ListPos);
			UserData->GameExt_RDObj.ListPos = ucNULL;
		}
		m_MapKeyUserData.RemoveAt(Pos);
		m_Service.SetValue(2 + m_MapKeyUserData.GetSize());
		delete UserData;
	}
	ucVOID OnMyCheckMatchFiber(UCObject* o, UCEventArgs*)
	{
		UCTimeFiberData* pData = (UCTimeFiberData*)o;
		while (pData->IsValid())
		{
			while (m_matchUserData.GetCount() > 0 && pData->IsValid())
			{
				CUserData_Local* UserData = m_matchUserData.GetHead();
				if (UserData->GameExt_RDObj.ListPos == ucNULL)
					UserData->GameExt_RDObj.ListPos = m_matchUserData.GetHeadPosition();
				if (UCGetTime() - UserData->GameExt_RDObj.LastKeepalive < 15 * 10000)
					break;
				m_matchUserData.RemoveAt(UserData->GameExt_RDObj.ListPos);
				UserData->GameExt_RDObj.ListPos = ucNULL;
			}
			pData->Every(10000);
		}
	}
	ucINT Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("game_hall"));
		if (!m_LocalCfg.LoadFromXML(UCString("game_hall/game_hall.xml")))
		{
			CGameHall_LocalCfg* LocalCfg = m_LocalCfg.GetData();
			LocalCfg->EnableLog = 1;
			LocalCfg->LogUrl = "GameLog//";

			LocalCfg->Key = "123456";
			LocalCfg->SingleGate = 0;
			LocalCfg->PrivateGate = "";
			LocalCfg->DnsURL = "Dns//";
			m_LocalCfg.SaveToXML(UCString("game_hall/game_hall.xml"));
		}
		CGameHall_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->EnableLog && LocalCfg->LogUrl.Right(2) != UCString("//"))
		{
			WBox(UCString("LogUrl路径错误：") + LocalCfg->LogUrl);
			return 0;
		}

		m_Service.SetTTL(1);
		m_Index = UCGetApp()->Index;
		if (m_Index < 0)
			m_Index = 0;
		UCString strDnsURL;
		if (LocalCfg->SingleGate)
		{
			if (LocalCfg->PrivateGate.IsEmpty() || LocalCfg->PrivateGate.Right(2) == UCString("//"))
			{
				WBox(UCString("PrivateGate路径错误：") + LocalCfg->PrivateGate);
				return 0;
			}
			m_PrivateGate = LocalCfg->PrivateGate + ITOS(m_Index, UCString("_%d//"));
			strDnsURL = m_PrivateGate;
		}
		else
		{
			if (!LocalCfg->PrivateGate.IsEmpty() && LocalCfg->PrivateGate.Right(2) != UCString("//"))
			{
				WBox(UCString("PrivateGate路径错误：") + LocalCfg->PrivateGate);
				return 0;
			}
			if (LocalCfg->DnsURL.Right(2) != UCString("//"))
			{
				WBox(UCString("DnsURL路径错误：") + LocalCfg->DnsURL);
				return 0;
			}
			m_PrivateGate = LocalCfg->PrivateGate;
			strDnsURL = LocalCfg->DnsURL;
		}
		if (!m_Service.AddDNS(strDnsURL))
		{
			m_Service.Log(UCString("添加域名服务器失败：") + strDnsURL + UCString("\r\n"));
			WBox(UCString("添加域名服务器失败：") + strDnsURL);
			return 0;
		}
		m_Service.Log(UCString("添加域名服务器成功：") + strDnsURL + UCString("\r\n"));
		//也可以添加本机域名
		//m_Service.AddHost(UCString("a"), UCString("GameHall_0"));

		m_Service.Name = UCGetApp()->Name + ITOS(m_Index, UCString("_%d"));
		UCGetSystemExt()->SetWindowCaption(UCString("game_hall/") + m_Service.Name);

		m_FiberInit.Start(0);
		return 1;
	}
	ucVOID OnMyServiceClose(UCObject*, UCEventArgs*)
	{
		if (m_Game_Center_RCObj.Linked)
			m_Game_Center_RCObj.UnRefeshHall(m_Index, m_CenterKey);
	}
	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		m_Service.Log(UCString("start GameHall:") + m_Service.Name + UCString("\r\n"));
		m_Service.SetValue(1);		//先对内状态

		CGameHall_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (m_Index < 0 || m_Index >= HALL_MAX)
		{
			m_Service.Log(UCString("服务器ID错误，请检查配置后重启\r\n"));
			return;
		}
		//优先连接日志系统
		if (LocalCfg->EnableLog)
		{
			CLog_RCObj logRCObj;
			m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CLog_RCObj.call"), typeof(CLog_RCObj), &logRCObj);
			m_Service.Log(UCString("开始连接游戏日志服务器...\r\n"));
			UCString linkadress = LocalCfg->LogUrl + UCString("CLog_RCObj.call");
			while (FiberData->IsValid())
			{
				ucINT linkresult = logRCObj.Link(linkadress, ADMIN_PASSWORD);
				if (linkresult == 0)
					break;
				m_Service.Log(UCString("游戏日志服连接失败") + ITOS(linkresult) + UCString("\r\n"));
				FiberData->Every(10000);
			}
			m_Service.Log(UCString("游戏日志服连接成功\r\n"));
			ucINT bEnablePosWindows = 1;
			UCString strRet = logRCObj.AddModule(UCString("GameHall"), m_Service.Name, m_Service.StateObj->PID.Value, bEnablePosWindows);
			if (strRet == UCString("成功"))
			{

				if (bEnablePosWindows == 0)
					SetExceptionMode(2);
				m_Log.SetService(&m_Service);
				ucINT ret = m_Log.Init(LocalCfg->LogUrl, UCString("GameHall"), m_Service.Name, ADMIN_PASSWORD);
				if (ret)
					m_Service.Log(UCString("游戏日志服初始化失败") + ITOS(ret) + UCString("\r\n"));
			}
			else
			{
				m_Service.Log(UCString("注册游戏日志服失败:") + strRet + UCString("\r\n"));
			}
		}

		//暂停一段时间，让Center启动完成
		//FiberData->Delay(1000);

		//添加本地服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CGame_Hall_RCObj.call"), typeof(CGame_Hall_RCObj), typeof(CGameHallService), this, UCVF_Guest_Permission_ReadWrite);
		//连接中心服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_Center_RCObj.call"), typeof(CGame_Center_RCObj), &m_Game_Center_RCObj);
		//连接中心服务的Hall列表
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CMatch_Info_RDObj.data"), typeof(CMatch_Info_RDObj), &m_Match_Info_RDObj);
		//连接中心服务的GameDB列表
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGameDB_Info_RDObj.data"), typeof(CGameDB_Info_RDObj), &m_GameDB_Info_RDObj);

		//获取监控服的参数
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CMonitor_RDObj.data"), typeof(CMonitor_RDObj), &m_Monitor_RDObj);
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CMonitor_RCObj.call"), typeof(CMonitor_RCObj), &m_Monitor_RCObj);

		for (ucINT i = 0; i < GAMEDB_MAX; ++i)
		{
			m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_DB_RCObj/") + ITOS(i, UCString("%04d.call")), typeof(CGame_DB_RCObj), &m_GameDB_RCObj[i]);
			m_GameDB_Info_RDObj.GameDB_Info[i].BindOnSet(UCEvent(this, OnGameDB_InfoOnChanged));
		}
		m_GameDB_Info_RDObj.OnReset = UCEvent(this, OnMyGameDBInfoReset);

		for (ucINT i = 0; i < MATCH_MAX; ++i)
		{
			m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_Match_RCObj/") + ITOS(i, UCString("%04d.call")), typeof(CGame_Match_RCObj), &m_GameMatchRCObj[i]);
			m_Match_Info_RDObj.Match_Info[i].BindOnSet(UCEvent(this, OnMatch_InfoOnChanged));
		}
		m_Match_Info_RDObj.OnReset = UCEvent(this, OnMyGameMatchInfoReset);

		//连接中心服务器
		m_Service.Log(UCString("开始连接中心服务器...\r\n"));
		while (FiberData->IsValid())
		{
			ucINT linkresult = m_Game_Center_RCObj.Link(m_PrivateGate + UCString("dom://GameCenter//CGame_Center_RCObj.call"));
			if (linkresult == 0)
				break;
			//m_Service.Log(UCString("中心服连接失败") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("中心服连接成功\r\n"));

		UCString Result = m_Game_Center_RCObj.RefeshHall(m_Service.Name, m_Index, LocalCfg->Key, m_AdminKey, m_CenterKey);
		if (Result != "成功")
		{
			m_Service.Log(UCString("中心服验证失败，请检查配置后重启服务 : ") + Result + UCString("\r\n"));
			return;
		}
		m_Service.Log(UCString("验证成功\r\n"));
		m_Service.SetAdminKey(m_AdminKey);

		while (FiberData->IsValid())
		{
			//连接中心服务器，获得game的注册情况
			if (m_Match_Info_RDObj.StableLink(m_PrivateGate + UCString("dom://GameCenter//CMatch_Info_RDObj.data"), m_AdminKey) == 0)
				break;
			m_Service.Log(UCString("match 中心的Hall列表连接失败\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("match列表连接成功\r\n"));

		while (FiberData->IsValid())
		{
			ucINT linkresult = m_GameDB_Info_RDObj.StableLink(m_PrivateGate + UCString("dom://GameCenter//CGameDB_Info_RDObj.data"), m_AdminKey);
			if (linkresult == 0)
				break;
			m_Service.Log(UCString("中心的gamedb列表连接失败:") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("中心的gamedb列表连接成功\r\n"));

		m_Service.Log(UCString("开始连接监控服...\r\n"));
		while (FiberData->IsValid())
		{
			ucINT linkresult = m_Monitor_RCObj.Link(m_PrivateGate + UCString("dom://Monitor//CMonitor_RCObj.call"), ADMIN_PASSWORD);
			if (linkresult == 0)
			{
				break;
			}
			//m_Service.Log(UCString("监控服连接失败:") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		while (FiberData->IsValid())
		{
			ucINT linkresult = m_Monitor_RDObj.StableLink(m_PrivateGate + UCString("dom://Monitor//CMonitor_RDObj.data"), ADMIN_PASSWORD);
			if (linkresult == 0)
			{
				m_Service.Log(UCString("监控服连接成功\r\n"));
				break;
			}
			//m_Service.Log(UCString("监控服连接失败:") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}

		m_Service.Log(UCString("-----服务器启动完成-----\r\n\r\n"));
		m_Service.SetValue(2);		//可以对外了
		m_Service.SetTTL(DEF_DNS_TTL);

		m_CheckMatchFiber.Start(0);

		while (FiberData->IsValid())
		{
			m_Service.GetHostByNames(UCString("GameMatch"), m_aryGameMatchDom);
			m_Monitor_RCObj.UploadHallInfo(m_Index, m_MapKeyUserData.GetSize());

			FiberData->Delay(10000);
		}
	}
	ucVOID OnMyGameMatchInfoReset(UCObject*, UCEventArgs*)
	{
		m_MapMatch_Info_Open.RemoveAll();
		m_MapMatch_Info_UnOpen.RemoveAll();
		for (ucINT i = 0; i < MATCH_MAX; i++)
		{
			if (m_Match_Info_RDObj.Match_Info[i].Url.IsEmpty())
				continue;
			if (m_Match_Info_RDObj.Match_Info[i].Open)
			{
				m_MapMatch_Info_Open.Add(i, m_Match_Info_RDObj.Match_Info[i].Url);
			}
			else
			{
				m_MapMatch_Info_UnOpen.Add(i, 0);
			}
		}
	}
	ucVOID OnMatch_InfoOnChanged(UCObject*, UCEventArgs* e)
	{
		UCVF_PropertyBaseCmd_Args* Args = (UCVF_PropertyBaseCmd_Args*)e;

		CMatch_Info* Match_Info = (CMatch_Info*)Args->Data;
		ucINT Index = m_Match_Info_RDObj.Match_Info[0].GetArrayIndex(Match_Info);

		m_Service.Log(UCString("收到服务消息 Match[") + ITOS(Index) + UCString("].url=") + Match_Info->Url + UCString("\r\n"));

		//如果服务地址为空，不认为是合法的地址
		if (Match_Info->Url.IsEmpty())
		{
			m_MapMatch_Info_Open.Remove(Index);
			m_MapMatch_Info_UnOpen.Add(Index, 0);
			return;
		}

		if (Match_Info->Open)
		{
			m_MapMatch_Info_Open.Add(Index, Match_Info->Url);
			m_MapMatch_Info_UnOpen.Remove(Index);
		}
		else
		{
			m_MapMatch_Info_Open.Remove(Index);
			m_MapMatch_Info_UnOpen.Add(Index, 0);
		}
	}
	ucVOID OnMyGameDBInfoReset(UCObject*, UCEventArgs*)
	{
		m_MapDB_Info_Open.RemoveAll();
		m_MapDB_Info_UnOpen.RemoveAll();
		for (ucINT i = 0; i < GAMEDB_MAX; i++)
		{
			if (m_GameDB_Info_RDObj.GameDB_Info[i].Url.IsEmpty())
				continue;
			if (m_GameDB_Info_RDObj.GameDB_Info[i].Open)
			{
				m_MapDB_Info_Open.Add(i, m_GameDB_Info_RDObj.GameDB_Info[i].Url);
			}
			else
			{
				m_MapDB_Info_UnOpen.Add(i, 0);
			}
		}
	}
	ucVOID OnGameDB_InfoOnChanged(UCObject*, UCEventArgs* e)
	{
		UCVF_PropertyBaseCmd_Args* Args = (UCVF_PropertyBaseCmd_Args*)e;

		CGameDB_Info* GameDB_Info = (CGameDB_Info*)Args->Data;
		ucINT Index = m_GameDB_Info_RDObj.GameDB_Info[0].GetArrayIndex(GameDB_Info);

		m_Service.Log(UCString("收到服务消息 GameDB[") + ITOS(Index) + UCString("].url=") + GameDB_Info->Url + UCString("\r\n"));

		//如果服务地址为空，不认为是合法的地址
		if (GameDB_Info->Url.IsEmpty())
		{
			m_MapDB_Info_Open.Remove(Index);
			m_MapDB_Info_UnOpen.Add(Index, 0);
			return;
		}

		if (GameDB_Info->Open)
		{
			m_MapDB_Info_Open.Add(Index, GameDB_Info->Url);
			m_MapDB_Info_UnOpen.Remove(Index);
		}
		else
		{
			m_MapDB_Info_Open.Remove(Index);
			m_MapDB_Info_UnOpen.Add(Index, 0);
		}
	}

	
	//CStoreData* GetStoreConfig(ucINT StroeID)
	//{
	//	ucINT Pos = MapStoreData.FindKey(StroeID);
	//	if (Pos < 0)
	//		return 0;
	//	return (CStoreData*)MapStoreData.GetValueAt(Pos);
	//}

	ucVOID Split(UCString str, ucTCHAR c, UCStringArray& array)
	{
		ucINT iStart = 0;
		ucINT index = str.Find(c, iStart);
		while (index >= 0)
		{
			UCString itemif = str.Mid(iStart, index - iStart);
			//ucTrace(itemif + UCString("\n"));
			array.Add(itemif);
			iStart = index + 1;
			index = str.Find(c, iStart);
		}
		if (iStart < str.GetLength())
		{
			UCString itemif = str.Mid(iStart);
			array.Add(itemif);
		}
	}
public:
	ucVOID Run()
	{
		//初始化
		if (!Init())
			return;
		m_Service.Run(100);
	}
};

#endif	//_game_hall_H_