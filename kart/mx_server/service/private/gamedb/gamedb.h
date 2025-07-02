#ifndef _moba_db_H_
#define _moba_db_H_

#include "../common/game_db_robj.h"
#include "../common/game_bi_robj.h"
#include "../../public/common/monitor_robj.h"
#include "../../public/common/game_center_robj.h"
#include "../../public/common/game_account_robj.h"
#include "../../public/common/game_account_db_robj.h"
#include "../../public/common/log_robj.h"
#include "../../public/common/monitor_robj.h"

//定义游戏密钥，高12位游戏ID，第20位注册随机值
#define GAME_KEY			(1<<20 | 509648)

//用户数据超时时间(单位秒)
#define	 USERDATA_TIMEOUT	600

//本地配置存档
struct CGameDB_LocalCfg
{
	ucINT						EnableLog;					//是否开启日志`
	UCString				LogUrl;						//日志系统地址

	UCString				Key;						//密钥，防止冲突

	ucINT						SingleGate;					//是否一对一的Gate
	UCString				PrivateGate;				//内网Gate地址//结束（本机可以填空）
	UCString				DnsURL;						//DNS服路径//结束
	UCString				GameBIUrl;					//GameBI地址//结束
};

//自动存储的管理类
class UCEDataManage : public UCETemplate
{
public:
	ucINT						StepTime;		//间隔保存时间
	UCEvent					OnSave;
public:
	UCEDataManage(const UCEVariableInfo& Key, const UCEVariableInfo& Value)
	{
		//间隔300秒全存一遍
		StepTime = 300;

		MapData = new UCESimpleMap(Key, Value);

		ArySave = new UCEArray(Key);
		FiberSave.FiberEvent = UCEvent(this, OnFiberSave);
		FiberSave.Start(0);
	}
	~UCEDataManage()
	{
		Close();
		delete MapData;
		delete ArySave;
	}
	ucVOID Close()
	{
		FiberSave.Stop();
		//这里需要强制存档
		while (ArySave->GetSize())
		{
			const UCETemplateArg0* Key = &ArySave->GetAt(ArySave->GetSize() - 1);
			ucINT Pos = MapData->FindKey(*Key);
			ArySave->RemoveAt(ArySave->GetSize() - 1);
			if (Pos < 0)
				continue;
			OnSave.Run(this, (UCEventArgs*)(&MapData->GetKeyAt(Pos)));
		}
	}
	ucINT Add(const UCETemplateArg0& Key, const UCETemplateArg1& Value)
	{
		ucINT nRet = MapData->Add(Key, Value);
		//ArySave->Add(Key);
		return nRet;
	}
	ucVOID Change(const UCETemplateArg0& Key)
	{
		ArySave->Add(Key);
	}
	ucVOID Remove(const UCETemplateArg0& Key)
	{
		MapData->Remove(Key);
	}
	ucVOID RemoveAt(ucINT Index)
	{
		MapData->RemoveAt(Index);
	}
	ucINT FindKey(const UCETemplateArg0& Key)
	{
		return MapData->FindKey(Key);
	}
	UCETemplateArg0& GetKeyAt(ucINT Pos)
	{
		return MapData->GetKeyAt(Pos);
	}
	UCETemplateArg1& GetValueAt(ucINT Pos)
	{
		return MapData->GetValueAt(Pos);
	}
	ucINT GetSize()
	{
		return MapData->GetSize();
	}
	ucVOID OnFiberSave(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;
		//每10ms 保存一个玩家数据，一分钟可以备份6000个数据
		while (FiberData->IsValid())
		{
			while (ArySave->GetSize())
			{
				//每X秒存一次，最快每100ms 存一个
				//ucINT CurrStepTime = this->StepTime * 10000 / ArySave->GetSize();
				//if (CurrStepTime < 1000)
				//	CurrStepTime = 1000;

				const UCETemplateArg0* Key = &ArySave->GetAt(ArySave->GetSize() - 1);
				ucINT Pos = MapData->FindKey(*Key);
				ArySave->RemoveAt(ArySave->GetSize() - 1);
				if (Pos < 0)
					continue;
				OnSave.Run(this, (UCEventArgs*)(&MapData->GetKeyAt(Pos)));
				//FiberData->Every(CurrStepTime);
				FiberData->Every(20);
			}
			//FiberData->Every(100);
			FiberData->Every(StepTime * 10000);
		}
	}
private:
	UCESimpleMap*	MapData;		//Map数据<key,value>
	UCEArray*		ArySave;		//有变化需要存档的数据<key>
	UCFiber			FiberSave;		//定时存档Fiber
};

//用户基础信息Data列族
class CUserData_Value
{
public:
	CUserData_RDObj					DataRD;				//基础数据
	ucDWORD							GameKey;			//用户快速登录Key，需要存档
	UCString						UnionID;			//平台唯一ID
	//ucINT							PlatloginKey;			//快速登陆Key
};

//用户其它信息ExtData列族
class CUserDataExt_Value
{
public:
	CUserData_ExtData_RDObj			DataRD;				//其它数据
};

class CUserDataMem
{
public:
	UCEDataManage*					m_pMamage;
	ucINT								m_bUserDataChange;		//用户基础数据
	ucINT								m_bExtDataChange;		//用户扩展数据
	ucINT								m_bValid;				//数据是否还有效（超时就无效，无效存档后可以删除）
public:
	CUserData_Value					UserData_RDObj;			//DataCF存档
	CUserDataExt_Value				UserExtData_RDObj;		//ExtDataCF存档
	CUserData_Ext_RDObj				Ext_RDObj;				//用于顶号的快照
	CUserData_GameDBExt_RDObj		GameKeyRD;				//用户快速登录Key，服务器之间使用
	CUserData_GameExt_RDObj			GameExt_RDObj;			//专门给场内修改用的快照
	ucINT64							UserGameID;				//玩家ID
	ucINT								HallIndex;				//所在大厅序号，不需要存档
	ucDWORD							PlatloginKey;			//
public:
	CUserDataMem()
	{
		m_pMamage = 0;
		m_bUserDataChange = 0;
		m_bExtDataChange = 0;
		m_bValid = 1;
		UserGameID = 0;
		HallIndex = -1;
		//UserData_RDObj.PlatloginKey = 0;
		//UserData_RDObj.DataRD.PlatID = 0;
		UserData_RDObj.DataRD.ChannelID = -1;
		UserData_RDObj.DataRD.TimeOut = USERDATA_TIMEOUT * 10000;		//设置超时时间
		UserData_RDObj.DataRD.OnTimeOut = UCEvent(this, OnMyTimeOut);
		UserData_RDObj.DataRD.OnTick = UCEvent(this, OnMyUserDataChange);
		UserExtData_RDObj.DataRD.OnTick = UCEvent(this, OnMyExtDataChange);
		GameExt_RDObj.BattleInfo.MatchID = -1;
		GameExt_RDObj.BattleInfo.GameID = -1;
		GameExt_RDObj.BattleInfo.BattleID = -1;
		GameExt_RDObj.PlatID = 0;
		PlatloginKey = 0;
	}
	//新注册用户调用
	ucVOID RegNew(const CUserGameID& nPlatID)
	{
		UserGameID = nPlatID.ToInt64();
		//UserData_RDObj.DataRD.NickName = UCString("玩家") + ITOS64(nPlatID.ToInt64());// +UCString(":") + ITOS(nPlatID.Index);
		UserData_RDObj.DataRD.NickName = UCString("玩家") + ITOS(nPlatID.Index) + ITOS(nPlatID.DBIndex, UCString("%02d"));
		UserData_RDObj.DataRD.Gold = 100;
		UserData_RDObj.DataRD.Level = 1;
		UserData_RDObj.DataRD.Exp = 0;

		UCTime NowTime;
		UserExtData_RDObj.DataRD.OnlinePrizeMinute = 0;
		UserExtData_RDObj.DataRD.DailyOnlineMinute = 0;
		UserExtData_RDObj.DataRD.DailyOnlineTimePrizeTimes = 1;
		UserExtData_RDObj.DataRD.DailyGetStarCoinTimes = 5;
		UserExtData_RDObj.DataRD.DailyFreshTime = UCTime(NowTime.GetYear(), NowTime.GetMonth(), NowTime.GetDay() + 1, 0, 0, 0);
		UserExtData_RDObj.DataRD.LastOfflineTime = NowTime;
		UserExtData_RDObj.DataRD.OfflineMinutes = 0;
		Ext_RDObj.LoginCount = 0;
		UserDataChange();
		ExtDataChange(); 
	}
	//用户数据有变化时调用
	ucVOID UserDataChange()
	{
		m_bValid = 1;
		if (m_bUserDataChange)
			return;
		if (m_pMamage == 0)
			return;
		m_bUserDataChange = 1;
		m_pMamage->Change((const UCETemplateArg0&)UserGameID);
	}
	//扩展数据有变化时调用
	ucVOID ExtDataChange()
	{
		m_bValid = 1;
		if (m_bExtDataChange)
			return;
		if (m_pMamage == 0)
			return;
		m_bExtDataChange = 1;
		m_pMamage->Change((const UCETemplateArg0&)UserGameID);
	}
	ucINT HasChange()
	{
		if (m_bUserDataChange || m_bExtDataChange)
			return 1;
		return 0;
	}
private:
	ucVOID OnMyTimeOut(UCObject*, UCEventArgs*)
	{
		m_bValid = 0;
		UserExtData_RDObj.DataRD.LastOfflineTime = UCTime() - UCTimeSpan(0, 0, 0, USERDATA_TIMEOUT);
		m_bExtDataChange = 1;
		m_pMamage->Change((const UCETemplateArg0&)UserGameID);
	}
	ucVOID OnMyUserDataChange(UCObject*, UCEventArgs*)
	{
		UserDataChange();
	}
	ucVOID OnMyExtDataChange(UCObject*, UCEventArgs*)
	{
		ExtDataChange();
	}

};

//
class CGameDB_Debug_RDObj : public UCVF_RemoteDataObj
{
public:
	UCVF_INT		PCU;			//在线人数
	UCVF_INT		DAU;			//当日活跃用户数
	UCVF_INT		DNU;			//当日新增
	UCVF_INT		ChannelDAU[CANAL_MAX];	//当日渠道活跃
	UCVF_INT		ChannelDNU[CANAL_MAX];	//当日渠道新增
};

struct CPlatInfo
{
	ucINT64			PlatID;
	ucDWORD			loginKey;
};

//游戏数据服务器
class CGameDBService : public CGame_DB_RCObj
{
public:
	UCRdbColumnFamily*		m_DataCF;				//数据的列，用于账号数据
	UCRdbColumnFamily*		m_ExtDataCF;			//数据的列，用于账号数据
	UCRocksDB				m_DB;					//数据库引擎
	UCRocksDB				m_PlatDB;				//平台相关数据库
	UCRdbColumnFamily*		m_PlatCF;				//平台数据的列
	UCRocksDB				m_CodeDB;				//礼包码
	UCRdbColumnFamily*		m_PlayerCodeCF;			//礼包码
	UCRdbWriteOptions		m_WriteOptions;			//写参数
	UCRdbReadOptions		m_ReadOptions;			//读参数

	CGame_Center_RCObj		m_Game_Center_RCObj;	//中央服务器的远程调用
	CMonitor_RDObj			m_Monitor_RDObj;		//监控服
	CMonitor_RCObj			m_Monitor_RCObj;		//监控上报

	CGameDB_Debug_RDObj		m_Debug_RDObj;			//调试用快照
	UCTime					m_DayTime;				//计算当天重置的时间

	CGame_BI_RCObj			m_Game_BI_RCObj;
private:
	//游戏相关信息，根据UserGameID索引
	UCEDataManage<ucINT64, CUserDataMem*>	m_UserDataManage;
	UCInt64IntMap					m_mapUserDataHall0;			//进入0号大厅的所有玩家
private:
	//大厅相关信息，用于压力均衡
	UCIntStringMap					m_MapHall_Info_Open;		//所有对外大厅
	UCIntIntMap						m_MapHall_Info_UnOpen;		//所有对内大厅
	CHall_Info_RDObj				m_Hall_Info_RDObj;			//中央存储的所有的大厅信息

	//UCVF_DOMArray					m_aryGameHallDom;			//域名解析出的有效大厅服

	CGame_Account_RCObj				Game_Account_RCObj[ACCOUNT_MAX];		//
private:
	UCEConfig<CGameDB_LocalCfg>		m_LocalCfg;
	UCVF_Service					m_Service;

	UCString						m_PrivateGate;				//我的私有Gate
	ucINT								m_Index;
	ucDWORD							m_CenterKey;				//中心服的临时key
	ucINT64							m_AdminKey;					//管理员密钥
	UCFiber							m_FiberInit;				//连接初始化
	UCLogCall						m_Log;
public:
	//仍然需要一次登录，最终创建账号
	UCString Activate(const CUserGameID& UserGameID, const ucDWORD& AccountID, const ucDWORD& AccountKey, ucDWORD& GameKey, ucINT& HallIndex)
	{
		HallIndex = -1;
		if (UserGameID.DBIndex != m_Index)
			return UCString("ID错误");

		CGame_Account_RCObj* pGame_Account_RCObj = CheckGame_AccountRCObj(AccountID);
		if (pGame_Account_RCObj == ucNULL)
			return UCString("链接不通");

		ucINT iChannelID = -1;
		UCString strExtInfo;
		UCString strUnionID;
		//检查是否合法
		UCString strResult = pGame_Account_RCObj->CheckValid(UserGameID, AccountKey, iChannelID, strExtInfo, strUnionID);
		if (strResult != "成功")
		{
			m_Service.Log(UCString("激活数据失败:") + strResult + UCString("\r\n"));
			return strResult;
		}

		CUserDataMem* UserData = LoadUser(UserGameID, 1, iChannelID);
		if (UserData == ucNULL)
			return UCString("数据初始化错误");

		UserData->UserData_RDObj.DataRD.ChannelID = iChannelID;
		//if (UserData->UserData_RDObj.DataRD.ChannelID.Value == -1)
		//{
		//	UserData->UserData_RDObj.DataRD.ChannelID = iChannelID;
		//}
		//else if (UserData->UserData_RDObj.DataRD.ChannelID.Value != iChannelID)
		//{
		//	if (iChannelID == CHANNEL_4399_GameBox || iChannelID == CHANNEL_4399_Web)
		//	{
		//		if (UserData->UserData_RDObj.DataRD.ChannelID.Value != CHANNEL_4399_GameBox && UserData->UserData_RDObj.DataRD.ChannelID.Value != CHANNEL_4399_Web)
		//			return UCString("渠道错误");
		//	}
		//	else
		//		return UCString("渠道错误");
		//}

		//激活生成新的key
		GameKey = randint(0, 65535) << 16 | randint(1, 65535);

		UserData->Ext_RDObj.LoginCount = UserData->Ext_RDObj.LoginCount.Value + 1;
		UserData->UserData_RDObj.UnionID = strUnionID;
		UserData->UserData_RDObj.GameKey = GameKey;
		UserData->GameKeyRD.GameKey = GameKey;
		UserData->UserDataChange();

		HallIndex = UserData->HallIndex;

		m_Game_BI_RCObj.Login(UserGameID.ToInt64(), 0, 1, UCString(""), 0, iChannelID);

		if (HallIndex == -1)
		{
			if (!AllocHallIndex(UserData))
				return UCString("没有有效的大厅服");
			HallIndex = UserData->HallIndex;
		}
		return UCString("成功");
	}
	//快速登录
	UCString ReActivate(const CUserGameID& UserGameID, const ucDWORD& GameKey, ucDWORD& newGameKey, ucINT& HallIndex)
	{
		HallIndex = -1;
		ucINT64 Key = UserGameID.ToInt64();
		ucINT Pos = m_UserDataManage.FindKey(Key);
		if (Pos >= 0)
		{
			//已经被缓存
			CUserDataMem* UserData = m_UserDataManage.GetValueAt(Pos);
			if (GameKey != UserData->UserData_RDObj.GameKey)
				return UCString("密钥错误");
			//激活生成新的key
			newGameKey = randint(0, 65535) << 16 | randint(1, 65535);
			UserData->Ext_RDObj.LoginCount = UserData->Ext_RDObj.LoginCount.Value + 1;
			UserData->UserData_RDObj.GameKey = newGameKey;
			UserData->GameKeyRD.GameKey = newGameKey;
			HallIndex = UserData->HallIndex;
			UserData->UserDataChange();
			m_Game_BI_RCObj.Login(UserGameID.ToInt64(), 0, 1, UCString(""), 0, 0);
			if (HallIndex == -1)
			{
				if (!AllocHallIndex(UserData))
					return UCString("没有有效的大厅服");
				HallIndex = UserData->HallIndex;
			}
			return UCString("成功");
		}
		//需要到数据库看看
		UCRdbSlicePin Value;
		ucINT ret = m_DataCF->Get(m_ReadOptions, UCRdbSlicePin(&UserGameID.Index, sizeof(ucDWORD)), Value);
		if (ret)
		{
			return UCString("不存在") + ITOS(ret);
		}
		CUserData_Value dataValue;
		if (!Value.LoadFromBin(&dataValue, typeof(CUserData_Value)))
		{
			return UCString("解析失败");
		}
		if (GameKey != dataValue.GameKey)
			return UCString("密钥错误");
		//激活生成新的key
		newGameKey = randint(0, 65535) << 16 | randint(1, 65535);
		CUserDataMem* UserData = LoadUser(UserGameID, 0, 0);
		if (UserData == ucNULL)
			return UCString("数据初始化错误");
		UserData->Ext_RDObj.LoginCount = UserData->Ext_RDObj.LoginCount.Value + 1;
		UserData->UserData_RDObj.GameKey = newGameKey;
		UserData->GameKeyRD.GameKey = newGameKey;
		HallIndex = UserData->HallIndex;
		UserData->UserDataChange();
		m_Game_BI_RCObj.Login(UserGameID.ToInt64(), 0, 1, UCString(""), 0, 0);
		if (HallIndex == -1)
		{
			if (!AllocHallIndex(UserData))
				return UCString("没有有效的大厅服");
			HallIndex = UserData->HallIndex;
		}
		return UCString("成功");
	}
	//验证UserGameID的合法性，给Hall校验用户合法性
	UCString HallCheckValid(const CUserGameID& UserGameID, const ucDWORD& GameKey, ucINT HallIndex)
	{
		if (UserGameID.DBIndex != m_Index)
			return UCString("失败");
		ucINT64 Key = UserGameID.ToInt64();
		//已经被缓存
		ucINT Pos = m_UserDataManage.FindKey(Key);
		if (Pos < 0)
			return UCString("不存在");
		CUserDataMem* UserData = m_UserDataManage.GetValueAt(Pos);
		if (GameKey == UserData->UserData_RDObj.GameKey)
		{
			if (HallIndex == UserData->HallIndex)
			{
				return UCString("成功");
			}
			//如果进入的Hall和我记录的Hall不一致，并且记录的已经失效
			if (m_MapHall_Info_Open.FindKey(UserData->HallIndex) < 0)
			{
				//修改成合法的大厅序号
				if (m_MapHall_Info_Open.FindKey(HallIndex) >= 0)
				{
					UserData->HallIndex = HallIndex;
					return UCString("成功");
				}
			}
		}
		return UCString("失败");
	}
	//验证UserGameID的合法性，给Game校验用户合法性
	UCString GameCheckValid(const CUserGameID& UserGameID, const ucDWORD& GameKey, ucINT GameIndex)
	{
		if (UserGameID.DBIndex != m_Index)
			return UCString("失败");
		ucINT64 Key = UserGameID.ToInt64();
		//已经被缓存
		ucINT Pos = m_UserDataManage.FindKey(Key);
		if (Pos < 0)
			return UCString("不存在");
		CUserDataMem* UserData = m_UserDataManage.GetValueAt(Pos);
		if (GameKey == UserData->UserData_RDObj.GameKey)
		{
			if (GameIndex == UserData->GameExt_RDObj.BattleInfo.GameID || UserData->GameExt_RDObj.BattleInfo.GameID == -1)
			{
				return UCString("成功");
			}
		}
		return UCString("失败");
	}
	//修复指定玩家的状态，Game调用过来的
	ucVOID RepairMyState(const CUserGameID& UserGameID, ucINT GameIndex, ucINT BattleIndex)
	{
		if (UserGameID.DBIndex != m_Index)
			return;
		ucINT64 Key = UserGameID.ToInt64();
		//已经被缓存
		ucINT Pos = m_UserDataManage.FindKey(Key);
		if (Pos < 0)
			return;
		CUserDataMem* UserData = m_UserDataManage.GetValueAt(Pos);
		if (UserData->GameExt_RDObj.BattleInfo.GameID == GameIndex && UserData->GameExt_RDObj.BattleInfo.BattleID == BattleIndex)
		{
			UserData->GameExt_RDObj.BattleInfo.MatchID = -1;
			UserData->GameExt_RDObj.BattleInfo.GameID = -1;
			UserData->GameExt_RDObj.BattleInfo.BattleID = -1;
			UserData->GameExt_RDObj.BattleInfo.UpdateData();
		}
	}

	ucINT PlayerCodeLoad(const CUserGameID& UserGameID, const ucDWORD& GameKey, UCString Code)//验证玩家有没有拿过
	{
		if (UserGameID.DBIndex != m_Index)
		{
			m_Service.Log(UCString("连接平台错误：") + UCString("UserGameID.DBIndex != m_Index\r\n"));
			return 1;
		}
		ucINT64 Key = UserGameID.ToInt64();
		//已经被缓存
		ucINT Pos = m_UserDataManage.FindKey(Key);
		if (Pos < 0)
		{
			m_Service.Log(UCString("连接平台错误：") + UCString("m_UserDataManage.FindKey<0\r\n"));
			return 1;
		}
		CUserDataMem* UserData = m_UserDataManage.GetValueAt(Pos);
		if (GameKey != UserData->UserData_RDObj.GameKey)
		{
			m_Service.Log(UCString("连接平台错误：") + UCString("Key错误\r\n"));
			return 2;
		}

		UCString KeyWord = ITOS64(Key) + UCString("_") + Code;
		UCRdbSlicePin pinCodeValue;
		ucINT ret = m_PlayerCodeCF->Get(m_ReadOptions, UCRdbSlicePin(KeyWord), pinCodeValue);
		if (ret == 0)
		{
			return 3;
		}
		return 0;
	}

	ucINT PlayerCodeSet(const CUserGameID& UserGameID, const ucDWORD& GameKey, UCString Code)//写入玩家拿过礼包码记录
	{
		if (UserGameID.DBIndex != m_Index)
		{
			m_Service.Log(UCString("连接平台错误：") + UCString("UserGameID.DBIndex != m_Index\r\n"));
			return 1;
		}
		ucINT64 Key = UserGameID.ToInt64();
		//已经被缓存
		ucINT Pos = m_UserDataManage.FindKey(Key);
		if (Pos < 0)
		{
			m_Service.Log(UCString("连接平台错误：") + UCString("m_UserDataManage.FindKey<0\r\n"));
			return 1;
		}
		CUserDataMem* UserData = m_UserDataManage.GetValueAt(Pos);
		if (GameKey != UserData->UserData_RDObj.GameKey)
		{
			m_Service.Log(UCString("连接平台错误：") + UCString("Key错误\r\n"));
			return 2;
		}

		UCString KeyWord = ITOS64(Key) + UCString("_") + Code;
		//CCodeConfig_RDObj pack;
		UCRdbSlicePin keyslice;// (&pack, sizeof(CCodeConfig_RDObj));
		ucINT ret = m_PlayerCodeCF->Put(m_WriteOptions, UCRdbSlicePin(KeyWord), keyslice);
		if (ret == 0)
		{
			return 0;
		}
		return 3;
}

public:
	CGameDBService()
	{
		m_DataCF = ucNULL;
		m_ExtDataCF = ucNULL;
		m_CenterKey = 0;
		m_AdminKey = 0;

		m_Debug_RDObj.PCU = 0;
		m_Debug_RDObj.DAU = 0;
		m_Debug_RDObj.DNU = 0;
		for (ucINT i = 0; i < CANAL_MAX; ++i)
		{
			m_Debug_RDObj.ChannelDAU[i] = 0;
			m_Debug_RDObj.ChannelDNU[i] = 0;
		}
		UCTime curTime;
		m_DayTime = UCTime(curTime.GetYear(), curTime.GetMonth(), curTime.GetDay() + 1, 0, 0, 0);

		m_UserDataManage.OnSave = UCEvent(this, OnUserDataSave);
		m_FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		m_Service.OnClose = UCEvent(this, OnMyServiceClose);
	}
	~CGameDBService()
	{
		m_UserDataManage.Close();
	}
	ucINT FindValidHall()
	{
		//不要包含大厅0
		//if (m_aryGameHallDom.GetSize() > 0)
		//{
		//	UCString strHallURL = m_aryGameHallDom.GetAt(0)->OutName;
		//	ucINT iFind = strHallURL.Find(UCString("_"));
		//	if (iFind > 0)
		//	{
		//		ucINT iHallIndex = STOI(strHallURL.Mid(iFind + 1));
		//		if (iHallIndex >= 0)
		//			return iHallIndex;
		//	}
		//}
		if (m_MapHall_Info_Open.GetSize() > 0)
		{
			ucINT Pos = randint(0, m_MapHall_Info_Open.GetSize() - 1);
			return m_MapHall_Info_Open.GetKeyAt(Pos);
		}
		return -1;
	}
	ucINT AllocHallIndex(CUserDataMem* UserData)
	{
		if (UserData->HallIndex == -1)
		{
			//优先进压测服
			if (m_Monitor_RDObj.TestGameDBIndex == m_Index)
			{
				if (m_Monitor_RDObj.TestGameDB_AllocTo_Hall0.Value > 0 && m_mapUserDataHall0.GetSize() < m_Monitor_RDObj.TestGameDB_AllocTo_Hall0.Value)
				{
					UserData->HallIndex = 0;
					m_mapUserDataHall0.Add(UserData->UserGameID, 0);
					return 1;
				}
			}
			UserData->HallIndex = FindValidHall();
			if (UserData->HallIndex == -1)
				return 0;
			if (UserData->HallIndex == 0 && m_Monitor_RDObj.TestGameDBIndex == m_Index && m_Monitor_RDObj.TestGameDB_AllocTo_Hall0.Value > 0)
				m_mapUserDataHall0.Add(UserData->UserGameID, 0);
		}
		return 1;
	}
	CGame_Account_RCObj* CheckGame_AccountRCObj(ucDWORD Game_AccountIndex)
	{
		if (Game_AccountIndex < 0 || Game_AccountIndex >= ACCOUNT_MAX)
			return 0;

		if (Game_Account_RCObj[Game_AccountIndex].Linked)
			return &Game_Account_RCObj[Game_AccountIndex];

		UCString strURL = m_PrivateGate + UCString("dom://GameAccount_") + ITOS(Game_AccountIndex) + UCString("//CGame_Account_RCObj.call");
		ucINT ret = Game_Account_RCObj[Game_AccountIndex].Link(strURL);
		if (ret)
		{
			m_Service.Log(UCString("连接Platform_Userdata失败:") + strURL + UCString(",ret=") + ITOS(ret) + UCString("\r\n"));
			return 0;
		}
		return &Game_Account_RCObj[Game_AccountIndex];
	}
	CUserDataMem* NewUser(const CUserGameID& UserGameID, ucINT ChannelID)
	{
		ucINT64 Key = UserGameID.ToInt64();

		CUserDataMem* UserData = new CUserDataMem;
		UserData->m_pMamage = &m_UserDataManage;
		UserData->RegNew(UserGameID);
		m_UserDataManage.Add(Key, UserData);
		m_Service.SetValue(2 + m_UserDataManage.GetSize());
		UserData->UserData_RDObj.DataRD.ChannelID = ChannelID;

		UCString strBaseURL = m_Service.Name + UCString("//UserData/") + UserGameID.ToString();
		m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_RDObj.data"), typeof(CUserData_RDObj), &UserData->UserData_RDObj.DataRD, UCVF_Guest_Permission_ReadOnly);
		m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_ExtData_RDObj.data"), typeof(CUserData_ExtData_RDObj), &UserData->UserExtData_RDObj.DataRD, UCVF_Guest_Permission_ReadOnly);
		m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_Ext_RDObj.data"), typeof(CUserData_Ext_RDObj), &UserData->Ext_RDObj, UCVF_Guest_Permission_ReadOnly);
		m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_GameDBExt_RDObj.data"), typeof(CUserData_GameDBExt_RDObj), &UserData->GameKeyRD, UCVF_Guest_Permission_ReadOnly);
		m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_GameExt_RDObj.data"), typeof(CUserData_GameExt_RDObj), &UserData->GameExt_RDObj, UCVF_Guest_Permission_ReadOnly);

		m_Game_BI_RCObj.NewReg(UserGameID.ToInt64(), 0, 0, UserGameID.ToString(), ChannelID, UCString(""));

		m_Debug_RDObj.DNU += 1;
		m_Debug_RDObj.DAU += 1;
		m_Debug_RDObj.ChannelDAU[ChannelID] += 1;
		m_Debug_RDObj.ChannelDNU[ChannelID] += 1;

		return UserData;
	}
	CUserDataMem* LoadUser(const CUserGameID& UserGameID, ucINT bAutoNew,ucINT ChannelID)
	{
		ucINT64 Key = UserGameID.ToInt64();

		//已经被缓存
		ucINT Pos = m_UserDataManage.FindKey(Key);
		if (Pos >= 0)
		{
			CUserDataMem* UserData = m_UserDataManage.GetValueAt(Pos);
			UCTime NowTime;
			if (UserData->UserExtData_RDObj.DataRD.DailyFreshTime.Value < NowTime)
			{
				UserData->UserExtData_RDObj.DataRD.OnlinePrizeMinute = 0;
				UserData->UserExtData_RDObj.DataRD.DailyOnlineMinute = 0;
				UserData->UserExtData_RDObj.DataRD.DailyOnlineTimePrizeTimes = 1;
				UserData->UserExtData_RDObj.DataRD.DailyGetStarCoinTimes = 5;
				UserData->UserExtData_RDObj.DataRD.DailyFreshTime = UCTime(NowTime.GetYear(), NowTime.GetMonth(), NowTime.GetDay() + 1, 0, 0, 0);
				m_Debug_RDObj.ChannelDAU[ChannelID] += 1;
				m_Debug_RDObj.DAU += 1;
				UserData->ExtDataChange();
			}
			return UserData;
		}

		UCRdbSlicePin Value;
		ucINT ret = m_DataCF->Get(m_ReadOptions, UCRdbSlicePin(&UserGameID.Index, sizeof(ucDWORD)), Value);
		if (ret)
		{
			if (ret == 1 && bAutoNew)
				return NewUser(UserGameID, ChannelID);
			m_Service.Log(UCString("获取Data数据失败:") + ITOS(ret) + UCString("\r\n"));
			return 0;
		}
		UCRdbSlicePin ExtValue;
		ret = m_ExtDataCF->Get(m_ReadOptions, UCRdbSlicePin(&UserGameID.Index, sizeof(ucDWORD)), ExtValue);
		if (ret)
		{
			if (ret == 1 && bAutoNew)
				return NewUser(UserGameID, ChannelID);
			m_Service.Log(UCString("获取ExtData数据失败:") + ITOS(ret) + UCString("\r\n"));
			return 0;
		}

		CUserDataMem* UserData = new CUserDataMem;
		UserData->m_pMamage = &m_UserDataManage;
		UserData->UserGameID = Key;
		if (!Value.LoadFromBin(&UserData->UserData_RDObj, typeof(CUserData_Value)))
		{
			delete UserData;
			m_Service.Log(UCString("解析Data数据失败\r\n"));
			return ucNULL;
		}

		if (!ExtValue.LoadFromBin(&UserData->UserExtData_RDObj, typeof(CUserDataExt_Value)))
		{
			delete UserData;
			m_Service.Log(UCString("解析ExtData数据失败\r\n"));
			return ucNULL;
		}

		UserData->Ext_RDObj.LoginCount = 0;
		UCTime NowTime;
		if (UserData->UserExtData_RDObj.DataRD.DailyFreshTime.Value < NowTime)
		{
			UserData->UserExtData_RDObj.DataRD.OnlinePrizeMinute = 0;
			UserData->UserExtData_RDObj.DataRD.DailyOnlineMinute = 0;
			UserData->UserExtData_RDObj.DataRD.DailyOnlineTimePrizeTimes = 1;
			UserData->UserExtData_RDObj.DataRD.DailyGetStarCoinTimes = 5;
			UserData->UserExtData_RDObj.DataRD.DailyFreshTime = UCTime(NowTime.GetYear(), NowTime.GetMonth(), NowTime.GetDay() + 1, 0, 0, 0);
			m_Debug_RDObj.ChannelDAU[ChannelID] += 1;
			m_Debug_RDObj.DAU += 1;
		}
		UCTimeSpan Span = NowTime - UserData->UserExtData_RDObj.DataRD.LastOfflineTime.Value;
		UserData->UserExtData_RDObj.DataRD.LastOfflineTime = NowTime;
		ucINT AllMinute = Span.GetTotalMinutes();
		if (AllMinute < 0)
			AllMinute = 0;
		if (UserData->UserExtData_RDObj.DataRD.OfflineMinutes.Value + AllMinute > 480)
		{
			UserData->UserExtData_RDObj.DataRD.OfflineMinutes = 480;
		}
		else
		{
			UserData->UserExtData_RDObj.DataRD.OfflineMinutes += AllMinute;
		}
		UserData->ExtDataChange();
		//if (UserData->UserData_RDObj.DataRD.NickName.Value == UCString("奥西留斯"))
		//{
		//	UserData->UserData_RDObj.DataRD.Gold = 10000000;
		//	UserData->UserData_RDObj.DataRD.StarCoin = 100000;
		//}
		m_UserDataManage.Add(Key, UserData);
		m_Service.SetValue(2 + m_UserDataManage.GetSize());

		UCString strBaseURL = m_Service.Name + UCString("//UserData/") + UserGameID.ToString();
		m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_RDObj.data"), typeof(CUserData_RDObj), &UserData->UserData_RDObj.DataRD, UCVF_Guest_Permission_ReadOnly);
		m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_ExtData_RDObj.data"), typeof(CUserData_ExtData_RDObj), &UserData->UserExtData_RDObj.DataRD, UCVF_Guest_Permission_ReadOnly);
		m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_Ext_RDObj.data"), typeof(CUserData_Ext_RDObj), &UserData->Ext_RDObj, UCVF_Guest_Permission_ReadOnly);
		m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_GameDBExt_RDObj.data"), typeof(CUserData_GameDBExt_RDObj), &UserData->GameKeyRD, UCVF_Guest_Permission_ReadOnly);
		m_Service.AppendLocalFile(strBaseURL + UCString("/CUserData_GameExt_RDObj.data"), typeof(CUserData_GameExt_RDObj), &UserData->GameExt_RDObj, UCVF_Guest_Permission_ReadOnly);

		return UserData;
	}
	ucINT SaveUser(CUserDataMem* UserData)
	{
		ucINT bSave = 0;
		ucINT iIndex = UserData->UserGameID;
		UCRdbWriteBatch writebatch;	//批量处理，保证原子性
		if (UserData->m_bUserDataChange)
		{
			UCRdbSlicePin pinValue;
			if (!pinValue.SaveToBin(&UserData->UserData_RDObj, typeof(CUserData_Value)))
			{
				m_Service.Log(UCString("Data SaveToBin 失败\r\n"));
				return 0;
			}

			ucINT ret = writebatch.Put(UCRdbSlicePin(&iIndex, sizeof(ucDWORD)), pinValue, m_DataCF);
			if (ret)
			{
				m_Service.Log(UCString("Data writebatch.Put 失败:") + ITOS(ret) + UCString("\r\n"));
				return 0;
			}
			bSave = 1;
		}

		if (UserData->m_bExtDataChange)
		{
			UCRdbSlicePin pinValue;
			if (!pinValue.SaveToBin(&UserData->UserExtData_RDObj, typeof(CUserDataExt_Value)))
			{
				m_Service.Log(UCString("ExtData SaveToBin 失败\r\n"));
				return 0;
			}

			ucINT ret = writebatch.Put(UCRdbSlicePin(&iIndex, sizeof(ucDWORD)), pinValue, m_ExtDataCF);
			if (ret)
			{
				m_Service.Log(UCString("Data writebatch.Put 失败:") + ITOS(ret) + UCString("\r\n"));
				return 0;
			}
			bSave = 1;
		}
		//没有数据修改
		if (bSave == 0)
			return 1;
		ucINT ret = m_DB.Write(m_WriteOptions, writebatch);
		if (ret)
		{
			m_Service.Log(UCString("m_DB.Write 失败:") + ITOS(ret) + UCString("\r\n"));
			return 0;
		}

		UserData->m_bUserDataChange = 0;
		UserData->m_bExtDataChange = 0;

		return 1;
	}
	ucVOID OnUserDataSave(UCObject*, UCEventArgs* e)
	{
		ucINT64 key = *((ucINT64*)e);
		ucINT UserPos = m_UserDataManage.FindKey(key);
		//缓存已经释放
		if (UserPos < 0)
			return;
		CUserDataMem* UserData = m_UserDataManage.GetValueAt(UserPos);
		if (!SaveUser(UserData))
		{
			//存档失败
			return;
		}
		//m_Service.Log(UCString("OnUserDataSave:") + ITOS(UserData->m_bValid) + UCString("\r\n"));
		if (UserData->m_bValid == 0)
		{
			m_UserDataManage.RemoveAt(UserPos);
			if (UserData->HallIndex == 0)
				m_mapUserDataHall0.Remove(UserData->UserGameID);
			delete UserData;
			m_Service.SetValue(2 + m_UserDataManage.GetSize());
		}
	}
	ucINT Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("game_db"));
		if (!m_LocalCfg.LoadFromXML(UCString("game_db/game_db.xml")))
		{
			CGameDB_LocalCfg* LocalCfg = m_LocalCfg.GetData();
			LocalCfg->EnableLog = 1;
			LocalCfg->LogUrl = "GameLog//";

			LocalCfg->Key = "123456";
			LocalCfg->SingleGate = 0;
			LocalCfg->PrivateGate = "";
			LocalCfg->DnsURL = "Dns//";
			LocalCfg->GameBIUrl = "GameBI//";
			m_LocalCfg.SaveToXML(UCString("game_db/game_db.xml"));
		}
		CGameDB_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->EnableLog && LocalCfg->LogUrl.Right(2) != UCString("//"))
		{
			WBox(UCString("LogUrl路径错误：") + LocalCfg->LogUrl);
			return 0;
		}
		if (LocalCfg->GameBIUrl.Right(2) != UCString("//"))
		{
			WBox(UCString("GameBIUrl路径错误：") + LocalCfg->GameBIUrl);
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
		UCGetSystemExt()->SetWindowCaption(UCString("game_db/") + m_Service.Name);

		if (LocalCfg->EnableLog)
		{
			m_Log.SetService(&m_Service);
			m_Log.Debug(UCString("打开RocksDB:") + UCTime().Format("(%Y-%m-%d %H:%M:%S)"));
		}
		if (!InitDB())
			return 0;

		if (LocalCfg->EnableLog)
			m_Log.Debug(UCString("初始化完成:") + UCTime().Format("(%Y-%m-%d %H:%M:%S)"));
		m_FiberInit.Start(0);
		return 1;
	}
	ucINT InitDB()
	{
		if (m_Index < 0 || m_Index >= GAMEDB_MAX)
			return 0;

		if (m_DB.IsOpen())
			return 1;

		UCCsv csv;
		if (!csv.ReadFromFile("../../csv/rocksdbadress.csv"))
		{
			MBox("读取rocksdbadress.csv失败");
			return 0;
		}
		UCString dbaddress;
		if (!csv.GetString(2, 2, dbaddress) || dbaddress.IsEmpty())
		{
			MBox("读取rocksdbadress.csv文件内容失败");
			return 0;
		}

		//打开参数
		UCRdbOptions option;
		option.create_if_missing = 1;
		//option.error_if_exists = 1;
		option.create_missing_column_families = 1;

		UCRdbColumnFamilyOptions cfOptions;
		UCRdbColumnFamilyOptionsMap mapCF;
		mapCF.Add(UCString("default"), cfOptions);
		mapCF.Add(UCString("Data"), cfOptions);
		mapCF.Add(UCString("ExtData"), cfOptions);
		UCString strDB = dbaddress + UCString("gamedb_") + ITOS(m_Index, UCString("%04d"));
		m_Service.Log(UCString("game RocksDB路径:") + strDB + UCString("\r\n"));
		UCFileExt_GetStatic().CreateFullPath(strDB);
		ucINT openret = m_DB.Open(option, strDB, mapCF);
		if (openret)
		{
			WBox(UCString("gamedb数据库打开失败:") + ITOS(openret));
			return 0;
		}
		m_DataCF = m_DB.GetColumnFamily(UCString("Data"));
		if (m_DataCF == 0)
		{
			MBox("打开Data列族失败");
			return 0;
		}
		m_ExtDataCF = m_DB.GetColumnFamily(UCString("ExtData"));
		if (m_ExtDataCF == 0)
		{
			MBox("打开ExtData列族失败");
			return 0;
		}

		//平台相关数据库
		UCRdbColumnFamilyOptionsMap mapPlatCF;
		mapPlatCF.Add(UCString("default"), cfOptions);
		mapPlatCF.Add(UCString("Plat"), cfOptions);
		UCString strPlatDB = dbaddress + UCString("plat_") + ITOS(m_Index, UCString("%04d"));
		m_Service.Log(UCString("game_plat RocksDB路径:") + strPlatDB + UCString("\r\n"));
		UCFileExt_GetStatic().CreateFullPath(strPlatDB);
		openret = m_PlatDB.Open(option, strPlatDB, mapPlatCF);
		if (openret)
		{
			WBox(UCString("gamedb_plat数据库打开失败:") + ITOS(openret));
			return 0;
		}
		m_PlatCF = m_PlatDB.GetColumnFamily(UCString("Plat"));
		if (m_PlatCF == 0)
		{
			MBox("打开m_PlatCF列族失败");
			return 0;
		}

		//礼包码数据库
		//UCRocksDB               m_CodeDB;               //礼包码
		//UCRdbColumnFamily* m_PlayerCodeCF;         //礼包码
		UCRdbColumnFamilyOptionsMap mapCodeCF;
		mapCodeCF.Add(UCString("default"), cfOptions);
		mapCodeCF.Add(UCString("PlayerCode"), cfOptions);
		UCString strCodeDB = dbaddress + UCString("playercode_") + ITOS(m_Index, UCString("%04d"));
		m_Service.Log(UCString("game_code RocksDB路径:") + strCodeDB + UCString("\r\n"));
		UCFileExt_GetStatic().CreateFullPath(strCodeDB);
		openret = m_CodeDB.Open(option, strCodeDB, mapCodeCF);
		if (openret)
		{
			WBox(UCString("gamedb_code数据库打开失败:") + ITOS(openret));
			return 0;
		}
		m_PlayerCodeCF = m_CodeDB.GetColumnFamily(UCString("PlayerCode"));
		if (m_PlayerCodeCF == 0)
		{
			MBox("打开m_PlayerCodeCF列族失败");
			return 0;
		}
		return 1;
	}
	ucVOID OnMyServiceClose(UCObject*, UCEventArgs*)
	{
		m_Log.Debug(UCString("开始关闭服务:") + UCTime().Format("(%Y-%m-%d %H:%M:%S)"));
		m_UserDataManage.Close();
		m_Log.Debug(UCString("存档结束:") + UCTime().Format("(%Y-%m-%d %H:%M:%S)"));
		if (m_Game_Center_RCObj.Linked)
			m_Game_Center_RCObj.UnRefeshGameDB(m_Index, m_CenterKey);
		m_Log.Debug(UCString("服务结束:") + UCTime().Format("(%Y-%m-%d %H:%M:%S)"));
	}
	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;
		m_Service.Log(UCString("start GameDB:") + m_Service.Name + UCString("\r\n"));
		m_Service.SetValue(1);		//先对内状态

		CGameDB_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (m_Index < 0 || m_Index >= GAMEDB_MAX)
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
			UCString strRet = logRCObj.AddModule(UCString("GameDB"), m_Service.Name, m_Service.StateObj->PID.Value, bEnablePosWindows);
			if (strRet == UCString("成功"))
			{
				//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
				if (bEnablePosWindows == 0)
					SetExceptionMode(2);
				//m_Log.SetService(&m_Service);
				ucINT ret = m_Log.Init(LocalCfg->LogUrl, UCString("GameDB"), m_Service.Name, ADMIN_PASSWORD);
				if (ret)
					m_Service.Log(UCString("游戏日志服初始化失败") + ITOS(ret) + UCString("\r\n"));
			}
			else
			{
				m_Service.Log(UCString("注册游戏日志服失败:") + strRet + UCString("\r\n"));
			}
		}

		//暂停一段时间，让GameCenter启动完成
		//FiberData->Delay(1000);

		//添加本地服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CGame_DB_RCObj.call"), typeof(CGame_DB_RCObj), typeof(CGameDBService), this, UCVF_Guest_Permission_ReadWrite);
		//连接中心服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_Center_RCObj.call"), typeof(CGame_Center_RCObj), &m_Game_Center_RCObj);
		//连接中心服务的Hall列表
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CHall_Info_RDObj.data"), typeof(CHall_Info_RDObj), &m_Hall_Info_RDObj);
		//连接BI
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_BI_RCObj.data"), typeof(CGame_BI_RCObj), &m_Game_BI_RCObj);
		//监控服
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CMonitor_RCObj.call"), typeof(CMonitor_RCObj), &m_Monitor_RCObj);
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CMonitor_RDObj.data"), typeof(CMonitor_RDObj), &m_Monitor_RDObj);
		m_Monitor_RDObj.OnReset = UCEvent(this, OnMyMoniRDObjReset);
		m_Monitor_RDObj.TestGameDBIndex.BindOnSet(UCEvent(this, OnMyTestGameDBIndexSet));

		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGameDB_Debug_RDObj.data"), typeof(CGameDB_Debug_RDObj), &m_Debug_RDObj);

		for (ucINT i = 0; i < GAMEDB_MAX; ++i)
		{
			m_Hall_Info_RDObj.Hall_Info[i].BindOnSet(UCEvent(this, OnHall_InfoOnChanged));
		}
		m_Hall_Info_RDObj.OnReset = UCEvent(this, OnMyHallInfoReset);

		for (ucINT i = 0; i < ACCOUNT_MAX; ++i)
		{
			m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_Account_RCObj/") + ITOS(i) + UCString(".call"), typeof(CGame_Account_RCObj), &Game_Account_RCObj[i]);
		}

		//连接中心服务器
		m_Service.Log(UCString("开始连接中心服务器...\r\n"));
		while (FiberData->IsValid())
		{
			ucINT linkresult = m_Game_Center_RCObj.Link(m_PrivateGate + UCString("dom://GameCenter//CGame_Center_RCObj.call"));
			if (linkresult == 0)
				break;
			//m_Service.Log(UCString("中心连接失败") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("中心服连接成功\r\n"));

		UCString Result = m_Game_Center_RCObj.RefeshGameDB(m_Service.Name, m_Index, LocalCfg->Key, m_AdminKey, m_CenterKey);
		if (Result != "成功")
		{
			m_Service.Log(UCString("中心服验证失败，请检查配置后重启服务 : ") + Result + UCString("\r\n"));
			return;
		}
		m_Service.Log(UCString("验证成功\r\n"));
		m_Service.SetAdminKey(m_AdminKey);

		while (FiberData->IsValid())
		{
			//连接中心服务器，获得Hall的注册情况
			ucINT linkresult = m_Hall_Info_RDObj.StableLink(m_PrivateGate + UCString("dom://GameCenter//CHall_Info_RDObj.data"), m_AdminKey);
			if (linkresult == 0)
			{
				m_Service.Log(UCString("中心的Hall列表连接成功\r\n"));
				break;
			}
			m_Service.Log(UCString("中心的Hall列表连接失败:") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.SetValue(2);		//可以对外了

		m_Service.Log(UCString("开始连接GameBI服...\r\n"));
		while (FiberData->IsValid())
		{
			ucINT linkresult = m_Game_BI_RCObj.Link(LocalCfg->GameBIUrl + UCString("CGame_BI_RCObj.call"), m_AdminKey);
			if (linkresult == 0)
				break;
			//m_Service.Log(UCString("GameBI连接失败:") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("GameBI连接成功\r\n"));

		m_Service.Log(UCString("开始连接监控服务器...\r\n"));
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
				break;
			//m_Service.Log(UCString("监控服连接失败") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("监控连接成功\r\n"));

		m_Service.Log(UCString("-----服务器启动完成-----\r\n\r\n"));
		m_Service.SetTTL(DEF_DNS_TTL);

		while (FiberData->IsValid())
		{
			//m_Service.GetHostByNames(UCString("GameHall"), m_aryGameHallDom);
			if (UCTime() > m_DayTime)
			{
				m_Debug_RDObj.DAU = 0;
				m_Debug_RDObj.DNU = 0;
				for (ucINT i = 0; i < CANAL_MAX; ++i)
				{
					if (m_Debug_RDObj.ChannelDAU[i].Value != 0)
						m_Debug_RDObj.ChannelDAU[i] = 0;
					if (m_Debug_RDObj.ChannelDNU[i].Value != 0)
						m_Debug_RDObj.ChannelDNU[i] = 0;
				}
				UCTime curTime;
				m_DayTime = UCTime(curTime.GetYear(), curTime.GetMonth(), curTime.GetDay() + 1, 0, 0, 0);
			}
			m_Debug_RDObj.PCU = m_UserDataManage.GetSize();
			m_Monitor_RCObj.UploadGameDBInfo(m_Index, m_Debug_RDObj.PCU.Value, m_Debug_RDObj.DAU.Value, m_Debug_RDObj.DNU.Value);

			FiberData->Delay(10000);
		}
	}
	ucVOID OnMyMoniRDObjReset(UCObject*, UCEventArgs*)
	{
		OnMyTestGameDBIndexSet(0, 0);
	}
	ucVOID OnMyTestGameDBIndexSet(UCObject*, UCEventArgs*)
	{
		if (m_Monitor_RDObj.TestGameDBIndex == m_Index)
		{
			for (ucINT i = m_UserDataManage.GetSize() - 1; i >= 0; --i)
			{
				CUserDataMem* UserData =  m_UserDataManage.GetValueAt(i);
				if (UserData->HallIndex == 0)
					m_mapUserDataHall0.Add(UserData->UserGameID,0);
			}
		}
		else
		{
			m_mapUserDataHall0.RemoveAll();
		}
	}
	ucVOID OnMyHallInfoReset(UCObject*, UCEventArgs*)
	{
		m_MapHall_Info_Open.RemoveAll();
		m_MapHall_Info_UnOpen.RemoveAll();
		for (ucINT i = 0; i < HALL_MAX; ++i)
		{
			if (m_Hall_Info_RDObj.Hall_Info[i].Url.IsEmpty())
				continue;
			if (m_Hall_Info_RDObj.Hall_Info[i].Open)
			{
				m_MapHall_Info_Open.Add(i, m_Hall_Info_RDObj.Hall_Info[i].Url);
			}
			else
			{
				m_MapHall_Info_UnOpen.Add(i, 0);
			}
		}
	}
	ucVOID OnHall_InfoOnChanged(UCObject*, UCEventArgs* e)
	{
		UCVF_PropertyBaseCmd_Args* Args = (UCVF_PropertyBaseCmd_Args*)e;

		CHall_Info* Hall_Info = (CHall_Info*)Args->Data;
		ucINT Index = m_Hall_Info_RDObj.Hall_Info[0].GetArrayIndex(Hall_Info);

		m_Service.Log(UCString("收到服务消息 Hall[") + ITOS(Index) + UCString("].url=") + Hall_Info->Url + UCString("\r\n"));

		//如果服务地址为空，不认为是合法的地址
		if (Hall_Info->Url.IsEmpty())
		{
			m_MapHall_Info_Open.Remove(Index);
			m_MapHall_Info_UnOpen.Add(Index, 0);
			return;
		}

		if (Hall_Info->Open)
		{
			m_MapHall_Info_Open.Add(Index, Hall_Info->Url);
			m_MapHall_Info_UnOpen.Remove(Index);
		}
		else
		{
			m_MapHall_Info_Open.Remove(Index);
			m_MapHall_Info_UnOpen.Add(Index, 0);
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

#endif	//_moba_db_H_