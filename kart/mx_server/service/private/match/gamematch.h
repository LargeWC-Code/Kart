#ifndef _game_match_H_
#define _game_match_H_

#include "../../public/common/game_center_robj.h"
#include "../../public/common/log_robj.h"
#include "../../public/common/monitor_robj.h"

#include "../common/game_db_robj.h"
#include "../common/game_match_robj.h"
#include "../common/game_robj.h"

//本地配置存档
struct CMatch_LocalCfg
{
	ucINT					EnableLog;					//是否开启日志
	UCString				LogUrl;						//日志系统地址

	UCString				Key;						//密钥，防止冲突

	UCString				PrivateGate;				//内网Gate地址//结束（本机可以填空）
	UCString				DnsURL;						//DNS服路径//结束
};

class CMatch_Debug_RDObj : public UCVF_RemoteDataObj
{
public:
	UCVF_INT		PublicRoomNum;				//匹配服可加房间数
	UCVF_INT		ValidRoomNum;				//匹配服有效房间数
	UCVF_INT		ValidGameNum;				//有效Game数
	UCVF_INT		CreateRoomNum;				//统计创建房间数
	UCVF_INT		CreateRoomOld;				//旧模式统计创建房间数
	UCVF_INT		MatchNum;					//正在匹配人数
	UCVF_INT		TotleMatchNum;				//统计匹配次数
	UCVF_INT		TotleCancelMatchNum;		//统计取消匹配数
	UCVF_INT		TotleCancelMatchType;		//统计取消匹配类型
};

//单局需要匹配的玩家数量
class CRoomInfoRDObj : public CGame_Match_Room_RDObj
{
	UCVF_Service*			m_pService;
	ucINT64					m_AdminKey;
public:
	ucINT					GameIndex;			//连接的Game服序号
	ucINT					BattleIndex;		//连接的战场序号
public:
	ucINT					WaitState;			//房间等待状态：0-无效列表，1-公开列表，2-有效房间列表
	UCEvent					OnWaitStateChange;	//状态有变化通知 UCEvent(this,ucINT* OldState)
	UCEListPtr*				ListPos;			//给外部管理器UCEList的索引

	UCInt64IntMap			mapInRoom;			//已经在房间的玩家
	UCInt64IntMap			mapBlockUser;		//玩家阻塞状态的请求
public:
	CRoomInfoRDObj()
	{
		m_pService = 0;
		WaitState = 0;
		ListPos = ucNULL;
	}
	ucINT Init(UCVF_Service* pService, ucINT iGameIndex, ucINT iBattleIndex, ucINT64 AdminKey)
	{
		m_pService = pService;
		GameIndex = iGameIndex;
		BattleIndex = iBattleIndex;
		m_AdminKey = AdminKey;
		ucINT ret = m_pService->AppendLocalFile(m_pService->Name + UCString("//Game_RCObj/") + ITOS(GameIndex) + UCString("/Battle/") + ITOS(BattleIndex) + UCString(".data"), typeof(CGame_Match_Room_RDObj), this);
		if (!ret)
		{
			m_pService->Log(UCString("AppendLocalFile房间出错：") + m_pService->Name + UCString("//Game_RCObj/") + ITOS(GameIndex) + UCString("/Battle/") + ITOS(BattleIndex) + UCString(".data\r\n"));
			return 0;
		}
		for (ucINT i = 0; i < ROOM_USER_MAX; ++i)
		{
			RoomInfo.UserID[i] = 0;
			RoomInfo.UserID[i].BindOnSet(UCEvent(this, OnMyUserIDSet));
		}
		OnReset = UCEvent(this, OnMyRoonReset);
		RoomInfo.RoomState.BindOnSet(UCEvent(this, OnMyRoomStateSet));
		RoomInfo.Start.BindOnSet(UCEvent(this, OnMyStartSet));
		return 1;
	}
	ucINT CheckLink(const UCString& strPrivateGate)
	{
		if (Linked)
			return 0;
		ucINT ret = StableLink(strPrivateGate + UCString("dom://Game_") + ITOS(GameIndex) + UCString("//Game_Match_Room_RDObj/") + ITOS(BattleIndex) + UCString(".data"), m_AdminKey);
		if (ret)
		{
			m_pService->Log(UCString("连接dom://Game_") + ITOS(GameIndex) + UCString("//Game_Match_Room_RDObj/") + ITOS(BattleIndex) + UCString(".data失败:") + ITOS(ret) + UCString("\r\n"));
		}
		return ret;
	}
private:
	ucVOID CheckWaitState()
	{
		ucINT iWaitState = 0;
		//0-无效房间，1-随便进（搜索或指定加入），2-陌生人勿进（只能指定加入），3-密码房间（只能指定加入）
		if (RoomInfo.RoomState.Value)
		{
			iWaitState = 2;
			if (RoomInfo.RoomState.Value == 1)9
			{
				//0-不能开始（人数不足），1-可开始，2-游戏中，3-新匹配游戏结束```
				if (RoomInfo.Start.Value <= 1 && RoomInfo.MasterID.Value != 0)
				{
					if (mapInRoom.GetSize() /*+ mapBlockUser.GetSize()*/ < RoomInfo.MaxUser.Value)
						iWaitState = 1;
				}
			}
		}
		//m_pService->Log(UCString("房间状态：") + ITOS(iWaitState) + UCString(",RoomState：") + ITOS(RoomInfo.RoomState.Value) + UCString("\r\n"));
		if (iWaitState != WaitState)
		{
			ucINT iOldState = WaitState;
			WaitState = iWaitState;
			OnWaitStateChange.Run(this, (UCEventArgs*)(&iOldState));
		}
	}
	ucVOID OnMyRoonReset(UCObject*, UCEventArgs*)
	{
		OnMyRoomStateSet(0, 0);
	}
	ucVOID OnMyRoomStateSet(UCObject*, UCEventArgs*)
	{
		mapInRoom.RemoveAll();
		//0-无效房间，1-随便进（搜索或指定加入），2-陌生人勿进（只能指定加入），3-密码房间（只能指定加入）
		if (RoomInfo.RoomState.Value)
		{
			for (ucINT i = 0; i < ROOM_USER_MAX; ++i)
			{
				if (RoomInfo.UserID[i].Value != 0)
				{
					mapInRoom.Add(RoomInfo.UserID[i].Value, 0);
					mapBlockUser.Remove(RoomInfo.UserID[i].Value);
				}
			}
		}
		CheckWaitState();
	}
	ucVOID OnMyStartSet(UCObject*, UCEventArgs*)
	{
		CheckWaitState();
	}
	ucVOID OnMyUserIDSet(UCObject*, UCEventArgs* e)
	{
		//0-无效房间，1-随便进（搜索或指定加入），2-陌生人勿进（只能指定加入），3-密码房间（只能指定加入）
		if (RoomInfo.RoomState.Value == 0)
			return;
		UCVF_PropertyBaseCmd_Args* Args = (UCVF_PropertyBaseCmd_Args*)e;
		UCVF_INT64* pUserID = (UCVF_INT64*)Args->Data;
		if (pUserID->Value > 0)
			mapInRoom.Add(pUserID->Value, 0);
		else
		{
			mapInRoom.RemoveAll();
			for (ucINT i = 0; i < ROOM_USER_MAX; ++i)
			{
				if (RoomInfo.UserID[i].Value != 0)
				{
					mapInRoom.Add(RoomInfo.UserID[i].Value, 0);
					mapBlockUser.Remove(RoomInfo.UserID[i].Value);
				}
			}
		}
		CheckWaitState();
	}
};

//一个Game服连接，同时需要管理旗下所有房间
class CGameRCObj : public CGame_RCObj
{
public:
	UCVF_Service*			m_pService;
	ucINT64					m_AdminKey;
	ucINT					m_bInit;
	CRoomInfoRDObj			m_RoomRDObj[BATTLE_MAX];	//房间的缓存
public:
	ucINT					GameIndex;					//连接的服务器序号
	UCIntIntMap				mapValidRoom;				//有效房间(等待、私有、密码、人满，游戏中等)
	UCIntIntMap				mapCreateTempRoom;			//临时创建的房间

	UCEvent					OnRoomStateChange;			//房间状态有变化通知 UCEvent(CRoomInfoRDObj*,ucINT* OldState)

	UCInt64IntMap			mapBlockCreateUser;			//玩家阻塞状态的请求
public:
	CGameRCObj()
	{
		m_pService = 0;
		m_bInit = 0;
		for (ucINT i = 0; i < BATTLE_MAX; ++i)
		{
			m_RoomRDObj[i].OnWaitStateChange = UCEvent(this, OnMyRoomWaitStateChange);
		}
	}
	ucINT Init(UCVF_Service* pService, ucINT iIndex, ucINT64 AdminKey)
	{
		m_pService = pService;
		GameIndex = iIndex;
		m_AdminKey = AdminKey;
		if (!m_pService->AppendLocalFile(m_pService->Name + UCString("//Game_RCObj/") + ITOS(GameIndex) + UCString("/CGame_RCObj.obj"), typeof(CGame_RCObj), this))
		{
			return 0;
		}
		for (ucINT i = 0; i < BATTLE_MAX; ++i)
		{
			m_RoomRDObj[i].Init(m_pService, GameIndex, i, m_AdminKey);
		}
		return 1;
	}
	ucINT CheckLink(ucINT iMatchIndex, const UCString& strPrivateGate)
	{
		if (Linked && m_bInit)
			return 0;
		ucINT ret = Link(strPrivateGate + UCString("dom://Game_") + ITOS(GameIndex) + UCString("//CGame_RCObj.call"), m_AdminKey);
		if (ret)
		{
			m_pService->Log(UCString("连接dom://Game_") + ITOS(GameIndex) + UCString("失败:") + ITOS(ret) + UCString("\r\n"));
			return ret;
		}
		//匹配获取房间列表，用于匹配服重开
		UCEArray<ucINT> aryRoom;
		UCString strRet = GetRoomList(iMatchIndex, aryRoom);
		if (strRet != UCString("成功"))
			return -100;
		for (ucINT i = aryRoom.GetSize() - 1; i >= 0; --i)
		{
			ucINT iBattleIndex = aryRoom.GetAt(i);
			m_RoomRDObj[iBattleIndex].CheckLink(strPrivateGate);
		}
		m_bInit = 1;
		return 0;
	}
	CRoomInfoRDObj* FindRoom(ucINT iBattleIndex, const UCString& strPrivateGate)
	{
		ucINT iFind = mapValidRoom.FindKey(iBattleIndex);
		if (iFind < 0)
			return ucNULL;
		CRoomInfoRDObj * pRoomRD = &m_RoomRDObj[iBattleIndex];
		if (pRoomRD->CheckLink(strPrivateGate))
			return ucNULL;
		return pRoomRD;
	}
	ucINT GetPCU()
	{
		ucINT iTotlePCU = 0;
		for (ucINT i = mapValidRoom.GetSize() - 1; i >= 0; --i)
		{
			ucINT iIndex = mapValidRoom.GetKeyAt(i);
			if (m_RoomRDObj[iIndex].RoomInfo.RoomState.Value == 0)
				continue;
			iTotlePCU += m_RoomRDObj[iIndex].mapInRoom.GetSize();
		}
		return iTotlePCU;
	}
private:
	//检查房间状态，放到对应列表
	ucVOID OnMyRoomWaitStateChange(UCObject* o, UCEventArgs* e)
	{
		CRoomInfoRDObj* pRoomRDObj = (CRoomInfoRDObj*)o;
		ucINT iOldState = *((ucINT*)e);
		//房间等待状态：0-无效列表，1-公开列表，2-有效房间列表
		mapCreateTempRoom.Remove(pRoomRDObj->BattleIndex);
		if (pRoomRDObj->WaitState == 0)
		{
			//m_pService->Log(UCString("删除有效房间：") + ITOS(pRoomRDObj->BattleIndex) + UCString("\r\n"));
			mapValidRoom.Remove(pRoomRDObj->BattleIndex);
		}
		else //if (iOldState == 0)
		{
			//m_pService->Log(UCString("添加有效房间：") + ITOS(pRoomRDObj->BattleIndex) + UCString("\r\n"));
			mapValidRoom.Add(pRoomRDObj->BattleIndex, 0);
		}
		OnRoomStateChange.Run(o, e);
	}
};

struct CMatchInfo
{
	ucINT64		UserGameID;			//玩家ID
	UCEListPtr* ListPos;			//给外部管理器UCEList的索引
	ucDWORD		MatchTime;			//匹配时间
public:
	CMatchInfo()
	{
		MatchTime = UCGetTime();
		ListPos = ucNULL;
	}
	CMatchInfo(const CMatchInfo& info)
	{
		UserGameID = info.UserGameID;
		ListPos = info.ListPos;
		MatchTime = info.MatchTime;
	}
};

class CGameMatchService : public CGame_Match_RCObj
{
public:
	CGame_Center_RCObj					m_Game_Center_RCObj;		//中央服务器的远程调用
	CMonitor_RDObj						m_Monitor_RDObj;			//监控快照
	CMonitor_RCObj						m_Monitor_RCObj;			//监控上报
	CMatch_Debug_RDObj					m_Debug_RDObj;				//调试用快照
private:
	//大厅相关信息，用于压力均衡
	UCIntStringMap						m_MapGame_Info_Open;		//所有对外游戏服务器
	UCIntIntMap							m_MapGame_Info_UnOpen;		//所有对内游戏服务器
	CGame_Info_RDObj					m_Game_Info_RDObj;			//中央存储的所有的大厅信息

	CGameRCObj*							m_GameRCObj[GAME_MAX];		//和Game连接的对象
	UCIntIntMap							m_mapValidGame;				//有效的Game序号（不包含0号）

	UCEList<CRoomInfoRDObj*>			m_listWaitPublic;			//可加入的房间

	UCESimpleMap<ucINT64, CMatchInfo>	m_mapMatchInfo;				//正在匹配的玩家
	UCEList<CMatchInfo*>				m_listMatch;				//匹配队列
	UCFiber								m_MatchFiber;				//匹配处理Fiber
	UCGroupFiber						m_StartFiber;				//匹配成功开始游戏Fiber

	UCTime								m_DayTime;					//计算当天重置的时间
private:
	UCEConfig<CMatch_LocalCfg>			m_LocalCfg;
	UCVF_Service						m_Service;

	ucINT									m_Index;
	ucDWORD								m_CenterKey;				//中心服的临时key
	ucINT64								m_AdminKey;
	UCFiber								m_FiberInit;				//连接初始化
	UCLogCall							m_Log;
public:
	//创建房间
	UCString		CreateRoom(const CUserGameID& UserGameID, const ucDWORD& GameKey, const UCString& Password, const ucINT MapID, ucINT& GameIndex, ucINT& BattleIndex)
	{
		if (m_Index == 0 && m_Monitor_RDObj.LimitMatch0_Alloc_Game0.Value > 0)
		{
			//优先进入压测流程
			CGameRCObj* pGameRCObj = CheckGameRCObj(0, m_Index);
			if (pGameRCObj == 0)
				return UCString("无效服");
			//0号匹配服优先进0号Game，把0号Game压到最大值，其它匹配服用随机，但不包括0号Game
			if (pGameRCObj->mapValidRoom.GetSize() + pGameRCObj->mapCreateTempRoom.GetSize() + pGameRCObj->mapBlockCreateUser.GetSize() < m_Monitor_RDObj.LimitMatch0_Alloc_Game0.Value)
			{
				ucINT Key = UserGameID.ToInt64();
				if (pGameRCObj->mapBlockCreateUser.FindKey(Key) >= 0)
					return UCString("正在创建");
				pGameRCObj->mapBlockCreateUser.Add(Key, 0);
				UCString strResult = pGameRCObj->CreateRoom(UserGameID, GameKey, Password, m_Index, MapID, BattleIndex, m_Monitor_RDObj.LimitMatch0_Alloc_Game0.Value);
				pGameRCObj->mapBlockCreateUser.Remove(Key);
				if (strResult == UCString("成功"))
				{
					if (pGameRCObj->mapValidRoom.FindKey(BattleIndex) < 0)
						pGameRCObj->mapCreateTempRoom.Add(BattleIndex, 0);
					ucINT ret = pGameRCObj->m_RoomRDObj[BattleIndex].CheckLink(m_LocalCfg.GetData()->PrivateGate);
					if (ret)
						return UCString("连接房间失败：") +  ITOS(BattleIndex);
					GameIndex = 0;
					m_Debug_RDObj.CreateRoomOld += 1;
					return strResult;
				}
				else if (strResult != UCString("服务器满"))
				{
					return strResult;
				}
			}
		}
		//随机一个Game服务器
		GameIndex = RandGameIndex();
		CGameRCObj* pGameRCObj = CheckGameRCObj(GameIndex, m_Index);
		if (pGameRCObj == 0)
			return UCString("无效服") + ITOS(GameIndex);
		if (m_Monitor_RDObj.LimitGameRoomSize.Value > 0)
		{
			if (pGameRCObj->mapValidRoom.GetSize() + pGameRCObj->mapCreateTempRoom.GetSize() + pGameRCObj->mapBlockCreateUser.GetSize() >= m_Monitor_RDObj.LimitGameRoomSize.Value)
				return UCString("服务满");
			ucINT Key = UserGameID.ToInt64();
			if (pGameRCObj->mapBlockCreateUser.FindKey(Key) >= 0)
				return UCString("正在创建");
			pGameRCObj->mapBlockCreateUser.Add(Key, 0);
			UCString strResult = pGameRCObj->CreateRoom(UserGameID, GameKey, Password, m_Index, MapID, BattleIndex, m_Monitor_RDObj.LimitGameRoomSize.Value);
			pGameRCObj->mapBlockCreateUser.Remove(Key);
			if (strResult == UCString("成功"))
			{
				if (pGameRCObj->mapValidRoom.FindKey(BattleIndex) < 0)
					pGameRCObj->mapCreateTempRoom.Add(BattleIndex, 0);
				ucINT ret = pGameRCObj->m_RoomRDObj[BattleIndex].CheckLink(m_LocalCfg.GetData()->PrivateGate);
				if (ret)
					return UCString("连接房间失败：") + ITOS(BattleIndex);
				m_Debug_RDObj.CreateRoomOld += 1;
			}
			return strResult;
		}
		UCString strResult = pGameRCObj->CreateRoom(UserGameID, GameKey, Password, m_Index, MapID, BattleIndex, m_Monitor_RDObj.LimitGameRoomSize.Value);
		if (strResult == UCString("成功"))
		{
			ucINT ret = pGameRCObj->m_RoomRDObj[BattleIndex].CheckLink(m_LocalCfg.GetData()->PrivateGate);
			if (ret)
				return UCString("连接房间失败：") + ITOS(BattleIndex);
			m_Debug_RDObj.CreateRoomOld += 1;
		}
		return strResult;
	}
	//加入房间
	UCString		JoinRoom(const CUserGameID& UserGameID, const ucDWORD& GameKey, const UCString& Password, const ucINT MapID, ucINT GameIndex, ucINT BattleIndex, const CUserGameID& shareUserGameID)
	{
		CGameRCObj* pGameRCObj = CheckGameRCObj(GameIndex, m_Index);
		if (pGameRCObj == 0)
			return UCString("无效服") + ITOS(GameIndex);
		CRoomInfoRDObj* pRoomInfo = pGameRCObj->FindRoom(BattleIndex, m_LocalCfg.GetData()->PrivateGate);
		if (pRoomInfo == 0)
		{
			//找不到可加入房间，看看房间是否有效，有效的话就观战
			if (shareUserGameID.ToInt64() != 0)
			{
				//有分享者，可以观战
				if (BattleIndex >= 0 && BattleIndex < BATTLE_MAX && pGameRCObj->m_RoomRDObj[BattleIndex].Linked != 0)
				{
					if (pGameRCObj->m_RoomRDObj[BattleIndex].RoomInfo.RoomState.Value != 0)
					{
						//确认分享者在房间里
						if (pRoomInfo->mapInRoom.FindKey(shareUserGameID.ToInt64()) >= 0)
							return UCString("观战吧");
					}
				}
			}
			return UCString("无效房") + ITOS(BattleIndex);
		}
		if (pRoomInfo->RoomInfo.RoomState.Value == 0)
			return UCString("无效房");
		//m_Service.Log(UCString("房间状态：") + ITOS(pRoomInfo->RoomInfo.Start.Value) + UCString("分享：") + shareUserGameID.ToString() + UCString("\r\n"));
		if (shareUserGameID.ToInt64() != 0)
		{
			if (pRoomInfo->RoomInfo.Start.Value >= 2 || pRoomInfo->RoomInfo.MasterID.Value == 0)
			{
				//有分享者，可以观战，确认分享者在房间里
				if (pRoomInfo->mapInRoom.FindKey(shareUserGameID.ToInt64()) >= 0)
					return UCString("观战吧");
				if (pRoomInfo->mapInRoom.FindKey(UserGameID.ToInt64()) < 0)		//断线重连用户
					return UCString("观战无效");
			}
			else
			{
				//有分享者，确认分享者在房间里
				if (pRoomInfo->mapInRoom.FindKey(shareUserGameID.ToInt64()) < 0)
					return UCString("好友不在");
			}
		}
		ucINT Key = UserGameID.ToInt64();
		if (pRoomInfo->mapBlockUser.FindKey(Key) >= 0)
			return UCString("正在加入");
		if (pRoomInfo->mapInRoom.FindKey(Key) >= 0)		//断线重连用户
			return pGameRCObj->JoinRoom(UserGameID, GameKey, Password, m_Index, MapID, BattleIndex);
		if (pRoomInfo->mapInRoom.GetSize() + pRoomInfo->mapBlockUser.GetSize() >= pRoomInfo->RoomInfo.MaxUser.Value)
			return UCString("房间满");
		pRoomInfo->mapBlockUser.Add(Key, 0);
		ucINT bRemove = 0;
		if (pRoomInfo->mapInRoom.GetSize() + pRoomInfo->mapBlockUser.GetSize() >= pRoomInfo->RoomInfo.MaxUser.Value)
		{
			//先从组列表删除
			if (pRoomInfo->ListPos != ucNULL)
			{
				bRemove = 1;
				pRoomInfo->WaitState = 2;
				m_listWaitPublic.RemoveAt(pRoomInfo->ListPos);
				pRoomInfo->ListPos = ucNULL;
				m_Debug_RDObj.PublicRoomNum = m_listWaitPublic.GetCount();
				//m_Service.Log(UCString("临时删除房间：") + ITOS(pRoomInfo->BattleIndex) + UCString("\r\n"));
			}
		}
		UCString strResult = pGameRCObj->JoinRoom(UserGameID, GameKey, Password, m_Index, MapID, BattleIndex);
		pRoomInfo->mapBlockUser.Remove(Key);
		if (strResult != "成功")
		{
			//再插入到主列表
			if (bRemove == 1 && pRoomInfo->RoomInfo.RoomState.Value == 1 && pRoomInfo->RoomInfo.Start.Value <= 1 &&
				pRoomInfo->mapInRoom.GetSize() + pRoomInfo->mapBlockUser.GetSize() < pRoomInfo->RoomInfo.MaxUser.Value)
			{
				if (pRoomInfo->ListPos == ucNULL)
				{
					pRoomInfo->WaitState = 1;
					pRoomInfo->ListPos = m_listWaitPublic.AddHead(pRoomInfo);
					//m_Service.Log(UCString("临时恢复房间：") + ITOS(pRoomInfo->BattleIndex) + UCString("\r\n"));
					m_Debug_RDObj.PublicRoomNum = m_listWaitPublic.GetCount();
				}
			}
		}
		return strResult;
	}
	//随机加入房间
	UCString		JoinRoomRand(const CUserGameID& UserGameID, const ucDWORD& GameKey, const ucINT MapID, ucINT& GameIndex, ucINT& BattleIndex)
	{
		ucINT iRandSize = m_listWaitPublic.GetCount();
		//如果没有空房间，创建一个无密码的
		if (iRandSize <= 0)
			return CreateRoom(UserGameID, GameKey, UCString(""), MapID, GameIndex, BattleIndex);
		if (iRandSize > 10)
			iRandSize = 10;
		if (iRandSize < 1)
			iRandSize = 1;
		ucINT iRandCount = randint(0, iRandSize - 1);
		UCEListPtr* pPos = m_listWaitPublic.GetHeadPosition();
		while (iRandCount > 0)
		{
			pPos = m_listWaitPublic.GetNext(pPos);
			--iRandCount;
		}
		CRoomInfoRDObj* pRoomInfo = (CRoomInfoRDObj*)m_listWaitPublic.GetAt(pPos);
		//CRoomInfoRDObj* pRoomInfo = m_listWaitPublic.GetHead();
		UCString strResult = JoinRoom(UserGameID, GameKey, UCString(), MapID, pRoomInfo->GameIndex, pRoomInfo->BattleIndex, CUserGameID(0));
		if (strResult != "成功")
		{
			GameIndex = -1;
			BattleIndex = -1;
		}
		else
		{
			GameIndex = pRoomInfo->GameIndex;
			BattleIndex = pRoomInfo->BattleIndex;
		}
		return strResult;
	}
	//开始匹配
	UCString StartMatch(const CUserGameID& UserGameID, const ucDWORD& GameKey)
	{
		ucINT64 iKey = UserGameID.ToInt64();
		ucINT iFind = m_mapMatchInfo.FindKey(iKey);
		if (iFind >= 0)
			return UCString("成功");
		CMatchInfo newMatchInfo;
		m_Debug_RDObj.TotleMatchNum += 1;

		newMatchInfo.UserGameID = iKey;
		iFind = m_mapMatchInfo.Add(iKey, newMatchInfo);
		CMatchInfo* pMatchInfo = &m_mapMatchInfo.GetValueAt(iFind);
		pMatchInfo->ListPos = m_listMatch.AddTail(pMatchInfo);
		m_Debug_RDObj.MatchNum = m_listMatch.GetCount();
		return UCString("成功");
	}
	//取消匹配
	UCString StopMatch(const CUserGameID& UserGameID, const ucDWORD& GameKey)
	{
		ucINT64 iKey = UserGameID.ToInt64();
		if (iKey == 0)
			return 0;
		ucINT iFind = m_mapMatchInfo.FindKey(iKey);
		if (iFind < 0)
			return 0;
		CMatchInfo* pMatchInfo = &m_mapMatchInfo.GetValueAt(iFind);
		m_listMatch.RemoveAt(pMatchInfo->ListPos);
		m_Debug_RDObj.MatchNum = m_listMatch.GetCount();
		m_Debug_RDObj.TotleCancelMatchNum += 1;
		m_mapMatchInfo.RemoveAt(iFind);
		m_Debug_RDObj.TotleCancelMatchType += 1;
		return UCString("成功");
	}
public:
	CGameMatchService()
	{
		//UCEVariableInfo v = typeof(CGameRCObj);
		//MBoxInt(v._Sizeof());
		for (ucINT i = 0; i < GAME_MAX; ++i)
		{
			m_GameRCObj[i] = 0;
		}
		m_Debug_RDObj.ValidRoomNum = 0;
		m_Debug_RDObj.CreateRoomNum = 0;
		m_Debug_RDObj.CreateRoomOld = 0;
		m_Debug_RDObj.MatchNum = 0;
		m_Debug_RDObj.TotleMatchNum = 0;
		m_Debug_RDObj.TotleCancelMatchNum = 0;
		m_Debug_RDObj.TotleCancelMatchType = 0;
		UCTime curTime;
		m_DayTime = UCTime(curTime.GetYear(), curTime.GetMonth(), curTime.GetDay() + 1, 0, 0, 0);
		m_CenterKey = 0;
		m_AdminKey = 0;
		m_FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		m_MatchFiber.FiberEvent = UCEvent(this, OnMyMatchFiber);
		m_StartFiber.FiberEvent = UCEvent(this, OnMyGameStartFiber);
		m_Service.OnClose = UCEvent(this, OnMyServiceClose);
	}
	~CGameMatchService()
	{
		for (ucINT i = 0; i < GAME_MAX; i++)
			delete m_GameRCObj[i];
	}
private:
	ucINT RandGameIndex()
	{
		if (m_mapValidGame.GetSize() > 0)
		{
			ucINT Pos = randint(0, m_mapValidGame.GetSize() - 1);
			return m_mapValidGame.GetKeyAt(Pos);
		}
		return -1;
	}
	ucINT CheckMatchResult()
	{
		ucDWORD dwMaxTime = 0;
		UCVoidArray aryMatch;
		UCEListPtr* pPos = m_listMatch.GetHeadPosition();

		for (ucINT i = 10; i > 0 && pPos != ucNULL; --i)
		{
			CMatchInfo* pMatchInfo = (CMatchInfo*)m_listMatch.GetAt(pPos);
			ucDWORD dwTime = UCGetTime() - pMatchInfo->MatchTime;
			if (dwMaxTime < dwTime)
				dwMaxTime = dwTime;
			aryMatch.Add(pMatchInfo);
			pPos = m_listMatch.GetNext(pPos);
		}
		ucINT iTotleSize = aryMatch.GetSize();
		m_Service.Log(ITOS(iTotleSize) + UCString("\r\n"));
		//少于ROOM_USER_MIN不能开始
		if (iTotleSize < ROOM_USER_MIN)
			return 0;

		//人数没满，且没过10秒
		if (iTotleSize < 10 && dwMaxTime < 100000)
			return 0;

		CMatchCreateRoom* pCreateRoom = new CMatchCreateRoom;
		//需要随机
		ucINT iSize = 0;
		while (aryMatch.GetSize() > 0)
		{
			ucINT iRandIndex = randint(0, aryMatch.GetSize() - 1);
			CMatchInfo* pMatchInfo = (CMatchInfo*)aryMatch.GetAt(iRandIndex);
			aryMatch.RemoveAt(iRandIndex);
			pCreateRoom->UserID[iSize] = pMatchInfo->UserGameID;
			++iSize;
		}
		m_StartFiber.RunNew((UCEventArgs*)pCreateRoom);
		return 1;
	}
	ucVOID OnMyMatchFiber(UCObject* o, UCEventArgs*)
	{
		UCTimeFiberData* pData = (UCTimeFiberData*)o;
		ucDWORD dwStartTime = UCGetTime();
		while (pData->IsValid())
		{
			while (CheckMatchResult() && pData->IsValid())
			{
				//pData->Every(1);
			}
			pData->Every(1000);
		}
	}
	ucINT RemoveMatch(ucINT64 iKey)
	{
		if (iKey == 0)
			return 0;
		ucINT iFind = m_mapMatchInfo.FindKey(iKey);
		if (iFind < 0)
			return 0;
		CMatchInfo* pMatchInfo = &m_mapMatchInfo.GetValueAt(iFind);
		m_listMatch.RemoveAt(pMatchInfo->ListPos);
		m_Debug_RDObj.MatchNum = m_listMatch.GetCount();
		m_mapMatchInfo.RemoveAt(iFind);
		return 1;
	}
	ucVOID ClearCreate(CMatchCreateRoom* pCreateRoom)
	{
		for (ucINT i = 0; i < ROOM_USER_MAX; ++i)
		{
			RemoveMatch(pCreateRoom->UserID[i]);
		}
	}
	ucVOID OnMyGameStartFiber(UCObject*, UCEventArgs* e)
	{
		CMatchCreateRoom* pCreateRoom = (CMatchCreateRoom*)e;
		ClearCreate(pCreateRoom);
		if (!OnGameStart(pCreateRoom))
		{
			//失败的用户是否需要加回来
			;
		}
		delete pCreateRoom;
	}
	ucINT OnGameStart(CMatchCreateRoom* pCreateRoom)
	{
		ucINT BattleIndex = 0;
		if (m_Index == 0 && m_Monitor_RDObj.LimitMatch0_Alloc_Game0.Value > 0)
		{
			//优先进入压测流程
			CGameRCObj* pGameRCObj = CheckGameRCObj(0, m_Index);
			//0号匹配服优先进0号Game，把0号Game压到最大值，其它匹配服用随机，但不包括0号Game
			if (pGameRCObj != 0 && pGameRCObj->mapValidRoom.GetSize() + pGameRCObj->mapCreateTempRoom.GetSize() + pGameRCObj->mapBlockCreateUser.GetSize() < m_Monitor_RDObj.LimitMatch0_Alloc_Game0.Value)
			{
				UCString strResult = pGameRCObj->CreateMatchRoom(*pCreateRoom, m_Index, 101, BattleIndex, m_Monitor_RDObj.LimitMatch0_Alloc_Game0.Value, m_Monitor_RDObj.MatchRandRoomNum.Value);
				if (strResult == UCString("成功"))
				{
					if (pGameRCObj->mapValidRoom.FindKey(BattleIndex) < 0)
						pGameRCObj->mapCreateTempRoom.Add(BattleIndex, 0);
					ucINT ret = pGameRCObj->m_RoomRDObj[BattleIndex].CheckLink(m_LocalCfg.GetData()->PrivateGate);
					if (ret)
					{
						m_Service.Log(UCString("连接房间失败1：") + ITOS(BattleIndex) + UCString("\r\n"));
					}
					m_Debug_RDObj.CreateRoomNum += 1;
					//CreateSuccess(pCreateRoom);
					return 1;
				}
				else if (strResult != UCString("服务器满"))
				{
					m_Service.Log(UCString("创建失败1：") + strResult + UCString("\r\n"));
					return 0;
				}
			}
		}
		//随机一个Game服务器
		ucINT GameIndex = RandGameIndex();
		CGameRCObj* pGameRCObj = CheckGameRCObj(GameIndex, m_Index);
		if (pGameRCObj == 0)
		{
			m_Service.Log(UCString("无效服1：") + ITOS(GameIndex) + UCString("\r\n"));
			return 0;
		}
		if (m_Monitor_RDObj.LimitGameRoomSize.Value > 0)
		{
			if (pGameRCObj->mapValidRoom.GetSize() + pGameRCObj->mapCreateTempRoom.GetSize() + pGameRCObj->mapBlockCreateUser.GetSize() >= m_Monitor_RDObj.LimitGameRoomSize.Value)
			{
				m_Service.Log(UCString("服务满1：") + ITOS(GameIndex) + UCString("\r\n"));
				return 0;
			}
			UCString strResult = pGameRCObj->CreateMatchRoom(*pCreateRoom, m_Index, 101, BattleIndex, m_Monitor_RDObj.LimitGameRoomSize.Value, m_Monitor_RDObj.MatchRandRoomNum.Value);
			if (strResult == UCString("成功"))
			{
				if (pGameRCObj->mapValidRoom.FindKey(BattleIndex) < 0)
					pGameRCObj->mapCreateTempRoom.Add(BattleIndex, 0);
				ucINT ret = pGameRCObj->m_RoomRDObj[BattleIndex].CheckLink(m_LocalCfg.GetData()->PrivateGate);
				if (ret)
					m_Service.Log(UCString("连接房间失败1：") + ITOS(BattleIndex) + UCString("\r\n"));
				m_Debug_RDObj.CreateRoomNum += 1;
				//CreateSuccess(pCreateRoom);
				return 1;
			}
			return 0;
		}
		UCString strResult = pGameRCObj->CreateMatchRoom(*pCreateRoom, m_Index, 101, BattleIndex, m_Monitor_RDObj.LimitGameRoomSize.Value, m_Monitor_RDObj.MatchRandRoomNum.Value);
		if (strResult == UCString("成功"))
		{
			ucINT ret = pGameRCObj->m_RoomRDObj[BattleIndex].CheckLink(m_LocalCfg.GetData()->PrivateGate);
			if (ret)
				m_Service.Log(UCString("连接房间失败1：") + ITOS(BattleIndex) + UCString("\r\n"));
			m_Debug_RDObj.CreateRoomNum += 1;
			//CreateSuccess(pCreateRoom);
			return 1;
		}
		m_Service.Log(UCString("创建失败1：") + strResult + UCString("\r\n"));
		return 0;
	}
	ucINT Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("game_match"));
		if (!m_LocalCfg.LoadFromXML(UCString("game_match/game_match.xml")))
		{
			CMatch_LocalCfg* LocalCfg = m_LocalCfg.GetData();
			LocalCfg->EnableLog = 1;
			LocalCfg->LogUrl = "GameLog//";

			LocalCfg->Key = "123456";
			LocalCfg->PrivateGate = "";
			LocalCfg->DnsURL = "Dns//";
			m_LocalCfg.SaveToXML(UCString("game_match/game_match.xml"));
		}
		CMatch_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->EnableLog && LocalCfg->LogUrl.Right(2) != UCString("//"))
		{
			WBox(UCString("LogUrl路径错误：") + LocalCfg->LogUrl);
			return 0;
		}
		if (!LocalCfg->PrivateGate.IsEmpty() && LocalCfg->PrivateGate.Right(2) != UCString("//"))
		{
			WBox(UCString("PrivateGate路径错误：") + LocalCfg->PrivateGate);
			return 0;
		}
		m_Service.SetTTL(1);
		if (LocalCfg->DnsURL.Right(2) != UCString("//"))
		{
			WBox(UCString("DnsURL路径错误：") + LocalCfg->DnsURL);
			return 0;
		}
		if (!m_Service.AddDNS(LocalCfg->DnsURL))
		{
			m_Service.Log(UCString("添加域名服务器失败：") + LocalCfg->DnsURL + UCString("\r\n"));
			WBox(UCString("添加域名服务器失败：") + LocalCfg->DnsURL);
			return 0;
		}
		m_Service.Log(UCString("添加域名服务器成功：") + LocalCfg->DnsURL + UCString("\r\n"));
		//也可以添加本机域名
		//m_Service.AddHost(UCString("a"), UCString("GameHall_0"));

		m_Index = UCGetApp()->Index;
		m_Service.Name = UCGetApp()->Name + ITOS(m_Index, UCString("_%d"));
		UCGetSystemExt()->SetWindowCaption(UCString("game_db/") + m_Service.Name);

		m_FiberInit.Start(0);
		return 1;
	}
	CGameRCObj* CheckGameRCObj(ucINT iGameIndex, ucINT iMatchIndex)
	{
		if (iGameIndex < 0 || iGameIndex >= GAME_MAX)
			return 0;
		if (m_Game_Info_RDObj.Game_Info[iGameIndex].Open == 0)
			return 0;
		if (m_GameRCObj[iGameIndex] == 0)
		{
			m_GameRCObj[iGameIndex] = new CGameRCObj;
			m_GameRCObj[iGameIndex]->OnRoomStateChange = UCEvent(this, OnMyRoomStateChange);
			if (!m_GameRCObj[iGameIndex]->Init(&m_Service, iGameIndex, m_AdminKey))
			{
				return 0;
			}
		}
		ucINT ret = m_GameRCObj[iGameIndex]->CheckLink(iMatchIndex, m_LocalCfg.GetData()->PrivateGate);
		if (ret)
			return 0;
		return m_GameRCObj[iGameIndex];
	}
	ucVOID OnMyRoomStateChange(UCObject* o, UCEventArgs* e)
	{
		CRoomInfoRDObj* pRoomRDObj = (CRoomInfoRDObj*)o;
		ucINT iOldState = *((ucINT*)e);
		//房间等待状态：0-无效列表，1-公开列表，2-有效房间列表
		if (pRoomRDObj->WaitState)
		{
			if (iOldState == 0)
				m_Debug_RDObj.ValidRoomNum += 1;
		}
		else
		{
			if (iOldState)
				m_Debug_RDObj.ValidRoomNum -= 1;
		}
		if (pRoomRDObj->WaitState == 1)
		{
			if (pRoomRDObj->ListPos == ucNULL)
			{
				pRoomRDObj->ListPos = m_listWaitPublic.AddTail(pRoomRDObj);
				//m_Service.Log(UCString("添加房间：") + ITOS(pRoomRDObj->BattleIndex) + UCString("\r\n"));
				m_Debug_RDObj.PublicRoomNum = m_listWaitPublic.GetCount();
			}
		}
		else
		{
			if (pRoomRDObj->ListPos != ucNULL)
			{
				//m_Service.Log(UCString("删除房间：") + ITOS(pRoomRDObj->BattleIndex) + UCString("\r\n"));
				m_listWaitPublic.RemoveAt(pRoomRDObj->ListPos);
				pRoomRDObj->ListPos = ucNULL;
				m_Debug_RDObj.PublicRoomNum = m_listWaitPublic.GetCount();
			}
		}
	}
	ucVOID OnMyServiceClose(UCObject*, UCEventArgs*)
	{
		if (m_Game_Center_RCObj.Linked)
			m_Game_Center_RCObj.UnRefeshMatch(m_Index, m_CenterKey);
	}
	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		m_Service.Log(UCString("start GameMatch:") + m_Service.Name + UCString("\r\n"));
		m_Service.SetValue(1);		//先对内状态

		CMatch_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (m_Index < 0 || m_Index >= MATCH_MAX)
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
			UCString strRet = logRCObj.AddModule(UCString("GameMatch"), m_Service.Name, m_Service.StateObj->PID.Value, bEnablePosWindows);
			if (strRet == UCString("成功"))
			{
				//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
				if (bEnablePosWindows == 0)
					SetExceptionMode(2);
				m_Log.SetService(&m_Service);
				ucINT ret = m_Log.Init(LocalCfg->LogUrl, UCString("GameMatch"), m_Service.Name, ADMIN_PASSWORD);
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
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CGame_Match_RCObj.call"), typeof(CGame_Match_RCObj), typeof(CGameMatchService), this, UCVF_Guest_Permission_ReadWrite);
		//连接中心服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_Center_RCObj.call"), typeof(CGame_Center_RCObj), &m_Game_Center_RCObj);
		//连接中心服务的Game列表
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_Info_RDObj.data"), typeof(CGame_Info_RDObj), &m_Game_Info_RDObj);
		//获取监控服的参数
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CMonitor_RCObj.call"), typeof(CMonitor_RCObj), &m_Monitor_RCObj);
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CMonitor_RDObj.data"), typeof(CMonitor_RDObj), &m_Monitor_RDObj);

		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CMatch_Debug_RDObj.data"), typeof(CMatch_Debug_RDObj), &m_Debug_RDObj);

		m_Monitor_RDObj.OnReset = UCEvent(this, OnMyMoniRDObjReset);
		m_Monitor_RDObj.MatchServiceNum.BindOnSet(UCEvent(this, OnMyMatchServiceNumSet));
		m_Monitor_RDObj.LimitMatch0_Alloc_Game0.BindOnSet(UCEvent(this, OnMyLimitMatch0_Alloc_Game0Set));

		for (ucINT i = 0; i < GAME_MAX; ++i)
		{
			m_Game_Info_RDObj.Game_Info[i].BindOnSet(UCEvent(this, OnGame_InfoOnChanged));
		}
		m_Game_Info_RDObj.OnReset = UCEvent(this, OnMyGameInfoReset);

		//连接中心服务器
		m_Service.Log(UCString("开始连接中心服务器...\r\n"));
		while (FiberData->IsValid())
		{
			ucINT linkresult = m_Game_Center_RCObj.Link(LocalCfg->PrivateGate + UCString("dom://GameCenter//CGame_Center_RCObj.call"));
			if (linkresult == 0)
				break;
			//m_Service.Log(UCString("中心连接失败") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("中心服连接成功\r\n"));

		UCString Result = m_Game_Center_RCObj.RefeshMatch(m_Service.Name, m_Index, LocalCfg->Key, m_AdminKey, m_CenterKey);
		if (Result != "成功")
		{
			m_Service.Log(UCString("中心服验证失败，请检查配置后重启服务 : ") + Result + UCString("\r\n"));
			return;
		}
		m_Service.Log(UCString("验证成功\r\n"));
		m_Service.SetAdminKey(m_AdminKey);

		m_Service.Log(UCString("开始连接监控服...\r\n"));
		while (FiberData->IsValid())
		{
			ucINT linkresult = m_Monitor_RCObj.Link(LocalCfg->PrivateGate + UCString("dom://Monitor//CMonitor_RCObj.call"), ADMIN_PASSWORD);
			if (linkresult == 0)
			{
				break;
			}
			//m_Service.Log(UCString("监控服连接失败:") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		while (FiberData->IsValid())
		{
			ucINT linkresult = m_Monitor_RDObj.StableLink(LocalCfg->PrivateGate + UCString("dom://Monitor//CMonitor_RDObj.data"), ADMIN_PASSWORD);
			if (linkresult == 0)
			{
				m_Service.Log(UCString("监控服连接成功\r\n"));
				break;
			}
			//m_Service.Log(UCString("监控服连接失败:") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}

		while (FiberData->IsValid())
		{
			//连接中心服务器，获得Game的情况
			ucINT linkresult = m_Game_Info_RDObj.StableLink(LocalCfg->PrivateGate + UCString("dom://GameCenter//CGame_Info_RDObj.data"), m_AdminKey);
			if (linkresult == 0)
			{
				m_Service.Log(UCString("中心的Game列表连接成功\r\n"));
				break;
			}
			m_Service.Log(UCString("中心的Game列表连接失败:") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}

		m_Service.Log(UCString("-----服务器启动完成-----\r\n\r\n"));
		m_Service.SetValue(2);		//可以对外了
		m_Service.SetTTL(DEF_DNS_TTL);

		m_MatchFiber.Start(0);
		while (FiberData->IsValid())
		{
			//m_Service.GetHostByNames(UCString("Game"), m_aryGameDom);
			if (UCTime() > m_DayTime)
			{
				m_Debug_RDObj.CreateRoomNum = 0;
				m_Debug_RDObj.CreateRoomOld = 0;
				m_Debug_RDObj.TotleMatchNum = 0;
				m_Debug_RDObj.TotleCancelMatchNum = 0;
				m_Debug_RDObj.TotleCancelMatchType = 0;
				UCTime curTime;
				m_DayTime = UCTime(curTime.GetYear(), curTime.GetMonth(), curTime.GetDay() + 1, 0, 0, 0);
			}

			m_Monitor_RCObj.UploadMatchInfo(m_Index, m_Debug_RDObj.MatchNum.Value, m_Debug_RDObj.CreateRoomNum.Value);
			//更新0号压测测试服信息
			if (m_GameRCObj[0] != 0)
				m_Monitor_RCObj.UploadGameInfo(0, m_GameRCObj[0]->mapValidRoom.GetSize(), m_GameRCObj[0]->GetPCU());
			for (ucINT i = m_mapValidGame.GetSize() - 1; i >= 0; --i)
			{
				ucINT iIndex = m_mapValidGame.GetKeyAt(i);
				if (m_GameRCObj[iIndex])
				{
					m_Monitor_RCObj.UploadGameInfo(iIndex, m_GameRCObj[iIndex]->mapValidRoom.GetSize(), m_GameRCObj[iIndex]->GetPCU());
				}
			}

			FiberData->Delay(10000);
		}
	}
	ucVOID RefeshValidGame()
	{
		m_mapValidGame.RemoveAll();
		CMatch_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		ucINT iMatchServiceNum = m_Monitor_RDObj.MatchServiceNum.Value;
		if (iMatchServiceNum < 1)
			iMatchServiceNum = 1;
		for (ucINT i = m_MapGame_Info_Open.GetSize() - 1; i >= 0; --i)
		{
			ucINT iIndex = m_MapGame_Info_Open.GetKeyAt(i);
			if (m_Index == iIndex % iMatchServiceNum)
			{
				if (iIndex != 0 || m_Monitor_RDObj.LimitMatch0_Alloc_Game0.Value <= 0)
					m_mapValidGame.Add(iIndex, 0);
			}
		}
		m_Debug_RDObj.ValidGameNum = m_mapValidGame.GetSize();
	}
	ucVOID OnMyMoniRDObjReset(UCObject*, UCEventArgs*)
	{
		RefeshValidGame();
	}
	ucVOID OnMyMatchServiceNumSet(UCObject*, UCEventArgs*)
	{
		RefeshValidGame();
	}
	ucVOID OnMyLimitMatch0_Alloc_Game0Set(UCObject*, UCEventArgs*)
	{
		RefeshValidGame();
	}
	ucVOID OnMyGameInfoReset(UCObject*, UCEventArgs*)
	{
		m_MapGame_Info_Open.RemoveAll();
		m_MapGame_Info_UnOpen.RemoveAll();
		for (ucINT i = 0; i < GAME_MAX; ++i)
		{
			if (m_Game_Info_RDObj.Game_Info[i].Url.IsEmpty())
				continue;
			if (m_Game_Info_RDObj.Game_Info[i].Open)
			{
				m_MapGame_Info_Open.Add(i, m_Game_Info_RDObj.Game_Info[i].Url);
			}
			else
			{
				m_MapGame_Info_UnOpen.Add(i, 0);
			}
		}
		RefeshValidGame();
	}
	ucVOID OnGame_InfoOnChanged(UCObject*, UCEventArgs* e)
	{
		UCVF_PropertyBaseCmd_Args* Args = (UCVF_PropertyBaseCmd_Args*)e;

		CGame_Info* pInfo = (CGame_Info*)Args->Data;
		ucINT Index = m_Game_Info_RDObj.Game_Info[0].GetArrayIndex(pInfo);

		m_Service.Log(UCString("收到服务消息 Game[") + ITOS(Index) + UCString("].url=") + pInfo->Url + UCString("\r\n"));

		//如果服务地址为空，不认为是合法的地址
		if (pInfo->Url.IsEmpty())
		{
			m_MapGame_Info_Open.Remove(Index);
			m_MapGame_Info_UnOpen.Add(Index, 0);
			m_mapValidGame.Remove(Index);
			m_Debug_RDObj.ValidGameNum = m_mapValidGame.GetSize();
			return;
		}

		if (pInfo->Open)
		{
			m_MapGame_Info_Open.Add(Index, pInfo->Url);
			m_MapGame_Info_UnOpen.Remove(Index);
			ucINT iMatchServiceNum = m_Monitor_RDObj.MatchServiceNum.Value;
			if (iMatchServiceNum < 1)
				iMatchServiceNum = 1;
			if (m_Index == Index % iMatchServiceNum)
			{
				if (Index != 0 || m_Monitor_RDObj.LimitMatch0_Alloc_Game0.Value <= 0)
				{
					m_mapValidGame.Add(Index, 0);
					m_Debug_RDObj.ValidGameNum = m_mapValidGame.GetSize();
				}
			}
		}
		else
		{
			m_MapGame_Info_Open.Remove(Index);
			m_MapGame_Info_UnOpen.Add(Index, 0);
			m_mapValidGame.Remove(Index);
			m_Debug_RDObj.ValidGameNum = m_mapValidGame.GetSize();
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


#endif	//_game_match_H_