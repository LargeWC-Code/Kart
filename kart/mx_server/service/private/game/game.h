#ifndef _moba_service_H_
#define _moba_service_H_
#include "gamedata.h"
#include "../../public/common/game_center_robj.h"
#include "../../public/common/log_robj.h"
#include "../common/game_db_robj.h"
#include "../common/game_robj.h"
#include "../common/game_match_robj.h"
#include "../common/game_secret.h"


//玩家多久不发消息，自动下线(单位秒)
#define USERDATA_TIMEOUT			60

//本地配置存档
struct CGame_LocalCfg
{
	ucINT					EnableLog;					//是否开启日志
	UCString				LogUrl;						//日志系统地址

	UCString				Key;						//密钥，防止冲突

	ucINT					SingleGate;					//是否一对一的Gate
	UCString				PrivateGate;				//内网Gate地址//结束（本机可以填空）
	UCString				DnsURL;						//DNS服路径//结束
};

//玩家数据（方便内存管理，从CUserData_RDObj派生）
class CGame_Player : public CUserData_RDObj
{
	UCVF_Service*					m_pService;
	ucDWORD							m_KeepAliveTime;		//保持GameDB活跃计时
public:
	//连接GameDB的快照
	CUserData_ExtData_RDObj			UserExtData;			//玩家装备
	CUserData_GameExt_RDObj			UserData_GameExt;		//给匹配服用的
	CUserData_GameDBExt_RDObj		GameKeyRD;				//用户快速登录Key，服务器之间使用

	//本地公开数据
	CPlayer_PubInfo					m_Player_PubInfo;
	//本地物理数据
	CPlayer_PhyInfo_RDObj			m_Player_PhyInfo_RDObj;

	ucINT							m_Offline;		//1分钟离线标记
public:
	CGame_Player(ucCONST CUserGameID& nUserGameID,ucINT iSeatID, UCVF_Service* pService)
	{
		m_KeepAliveTime = UCGetTime();
		m_pService = pService;
		m_Player_PubInfo.Clear();
		m_Player_PubInfo.UserGameID = nUserGameID.ToInt64();
		m_Player_PubInfo.SeatID = iSeatID;

		m_Player_PhyInfo_RDObj.UserGameID = nUserGameID.ToInt64();
		m_Offline = 0;
	}
	UCString Init(ucCONST UCString& strRoot, ucCONST UCString& strPrivateGate, ucINT64 iAdminKey, CBattle_RDObj* pBattleRDObj)
	{
		CUserGameID UserGameID(m_Player_PubInfo.UserGameID.Value);
		UCString strUserID = UserGameID.ToString();
		UCString strBaseUrl = strPrivateGate + UCString("dom://GameDB_") + ITOS(UserGameID.DBIndex) + UCString("//UserData/") + strUserID;
		{
			//先连接GameKey
			if(!m_pService->AppendLocalFile(m_pService->Name + UCString("//UserData/") + strUserID + UCString("/GameKey.data"), typeof(CUserData_GameDBExt_RDObj), &GameKeyRD))
				return UCString("GameKey错误") + strUserID;
			UCString strUrl = strBaseUrl + UCString("/CUserData_GameDBExt_RDObj.data");
			ucINT Code = GameKeyRD.StableLink(strUrl, iAdminKey);
			if (Code != 0)
			{
				return UCString("GameKey错误:") + ITOS(Code);
			}
		}

		{
			if (!m_pService->AppendLocalFile(m_pService->Name + UCString("//UserData/") + strUserID + UCString("/CUserData_GameExt_RDObj.data"), typeof(CUserData_GameExt_RDObj), &UserData_GameExt))
				return UCString("GameExt错误");
			UCString strUrl = strBaseUrl + UCString("/CUserData_GameExt_RDObj.data");
			ucINT Code = UserData_GameExt.StableLink(strUrl, iAdminKey);
			if (Code != 0)
			{
				return UCString("GameExt错误:") + ITOS(Code);
			}
			if (UserData_GameExt.BattleInfo.GameID != -1)
				return UCString("已在场内") + ITOS(UserData_GameExt.BattleInfo.GameID);
		}

		{
			if (!m_pService->AppendLocalFile(m_pService->Name + UCString("//UserData/") + strUserID + UCString("/CUserData_RDObj.data"), typeof(CUserData_RDObj), this))
				return UCString("UserData错误");
			UCString strUrl = strBaseUrl + UCString("/CUserData_RDObj.data");
			ucINT Code = StableLink(strUrl, iAdminKey);
			if (Code != 0)
			{
				return UCString("UserData错误:") + ITOS(Code);
			}
		}

		{
			if (!m_pService->AppendLocalFile(m_pService->Name + UCString("//UserData/") + strUserID + UCString("/CUserData_ExtData_RDObj.data"), typeof(CUserData_ExtData_RDObj), &UserExtData))
				return UCString("UserExtData错误");
			UCString strUrl = strBaseUrl + UCString("/CUserData_ExtData_RDObj.data");
			ucINT Code = UserExtData.StableLink(strUrl, iAdminKey);
			if (Code != 0)
			{
				return UCString("UserExtData错误:") + ITOS(Code);
			}
		}

		m_Player_PubInfo.NickName = NickName.Value;

		m_Player_PubInfo.AvatarData.FaceID = AvatarData.FaceID;
		m_Player_PubInfo.AvatarData.HairID = AvatarData.HairID;
		m_Player_PubInfo.AvatarData.BodyID = AvatarData.BodyID;
		m_Player_PubInfo.AvatarData.HandID = AvatarData.HandID;
		m_Player_PubInfo.AvatarData.PantID = AvatarData.PantID;
		m_Player_PubInfo.AvatarData.ShoeID = AvatarData.ShoeID;

		m_Player_PubInfo.PlatID = UserData_GameExt.PlatID.Value;

		if (!m_pService->AppendLocalFile(strRoot + UCString("/UserData/") + strUserID + UCString("/CPlayer_PhyInfo_RDObj.data"), typeof(CPlayer_PhyInfo_RDObj), &m_Player_PhyInfo_RDObj, UCVF_Guest_Permission_ReadOnly))
			return UCString("PhyInfo错误");

		return UCString("成功");
	}
	ucVOID CheckKeepAlive()
	{
		ucDWORD CurrentTime = UCGetTime();
		//每20秒刷新一次
		if (CurrentTime - m_KeepAliveTime > 200000)
		{
			//m_pService->Log(UCString("KeepValid\r\n"));
			KeepValid();
			m_KeepAliveTime = CurrentTime;
		}
	}
};

class CBattle_LocalRCObj : public CBattle_RCObj
{
private:
	UCVF_Service*						m_pService;
	ucINT64								m_AdminKey;
	UCCUrl*								m_PostCUrl;
	ucINT*								m_EnbaleTokenSwitch;

	UCFiber								m_MainFiber;				//主游戏Fiber
public:
	UCLogCall*							m_Log;
	CBattle_RDObj						Battle_RDObj;				//场内公用快照
	ucINT								MatchIndex;					//所在匹配服
	ucINT								GameIndex;					//所在游戏服
	ucINT								RoomIndex;					//房间ID
	UCString							Password;					//房间密码

	//房间相关
	CGame_Match_Room_RDObj				Game_Match_Room_RDObj;		//给匹配服的快照
	UCESimpleMap<ucINT64, CGame_Player*>		m_MapUserGameIDPlayer;			//房间内的玩家
	UCIntIntMap							m_MapSeatID_Empty;			//空闲位置

	UCEvent								OnUnValid;					//房间失效通知（所有人退出了）UCEvent(this,0)
	UCEvent								OnRepairUser;				//修复玩家状态				UCEvent(this,CUserGameID*)

	UCIntIntMap							m_ShutDoorMap;
public:
	//房间相关
	UCString	GetRoomState(ucCONST CUserGameID& UserGameID, ucINT& StartState, ucINT& iMapID)
	{
		if (Battle_RDObj.RoomInfo.RoomState.Value == 0)
			return UCString("无效房");
		if (m_MapUserGameIDPlayer.FindKey(UserGameID.ToInt64()) < 0)
			return UCString("没这个人");
		StartState = Game_Match_Room_RDObj.RoomInfo.Start.Value;
		iMapID = Battle_RDObj.MapID.Value;
		return UCString("成功");
	}
	//修复我的状态（自己断线重连失败，IsValidPlayer返回失败的时候调用）
	ucVOID		RepairMyState(ucCONST CUserGameID& UserGameID)
	{
		if (Battle_RDObj.RoomInfo.RoomState.Value == 0 || m_MapUserGameIDPlayer.FindKey(UserGameID.ToInt64()) < 0)
		{
			//通知GameDB，修复我不在这房间了
			OnRepairUser.Run(this, (UCEventArgs*)(&UserGameID));
		}
	}
	UCString	ChangePublic(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucCONST ucINT& Flag)
	{
		ucINT64 n64UserGameID = UserGameID.ToInt64();

		ucINT Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos < 0)
		{
			//m_pService->Log(UCString("无效用户\r\n"));
			return UCString("无效用户");
		}

		CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		if (pGame_Player->GameKeyRD.GameKey.Value != GameKey)
		{
			//m_pService->Log(UCString("无效Key\r\n"));
			return UCString("无效Key");
		}
		//0-无效房间，1-随便进（搜索或指定加入），2-陌生人勿进（只能指定加入），3-密码房间（只能指定加入）
		if(Battle_RDObj.RoomInfo.RoomState.Value==0)
			return UCString("无效房间");
		if (Battle_RDObj.RoomInfo.RoomState.Value == 3)
			return UCString("密码房间");
		if (Battle_RDObj.RoomInfo.Start.Value > 1 || Battle_RDObj.RoomInfo.MasterID.Value == 0)
			return UCString("不能修改");
		if (Flag)
		{
			if (Battle_RDObj.RoomInfo.RoomState.Value == 2)
			{
				Game_Match_Room_RDObj.RoomInfo.RoomState = 1;
				Battle_RDObj.RoomInfo.RoomState = 1;
			}
		}
		else
		{
			if (Battle_RDObj.RoomInfo.RoomState.Value == 1)
			{
				Game_Match_Room_RDObj.RoomInfo.RoomState = 2;
				Battle_RDObj.RoomInfo.RoomState = 2;
			}
		}
		return UCString("成功");
	}
	//游戏结束领取奖励，addGold获得金币
	UCString	GetGameResult(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucINT& addGold)
	{
		addGold = 0;
		ucINT64 n64UserGameID = UserGameID.ToInt64();
		ucINT Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos < 0)
			return UCString("无效用户");
		CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		if (pGame_Player->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效Key");
		if (pGame_Player->LockHost() == 0)
			pGame_Player->UnlockHost();
		return UCString("成功");
	}
	//强制退出房间，拿不到任何奖励
	UCString	ExitRoom(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey)
	{
		//m_pService->Log(UCString("ExitRoom\r\n"));
		ucINT64 n64UserGameID = UserGameID.ToInt64();
		ucINT Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos < 0)
		{
			return UCString("无效用户");
		}
		CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		if (pGame_Player->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效Key");
		//要支持看广告退出，所以屏蔽
		//if (Battle_RDObj.RoomInfo.Start.Value == 2)
		//	return UCString("不让退出");
		if (Battle_RDObj.RoomInfo.Start.Value == 1 && Battle_RDObj.RoomInfo.MasterID.Value == 0)
			return UCString("不让退出");

		m_MapUserGameIDPlayer.RemoveAt(Pos);
		//m_pService->Log(UCString("玩家自己离开:") + pGame_Player->NickName.Value + UCString("\r\n"));
		LeaveGame(pGame_Player);
		return UCString("成功");
	}

	UCString	SetMaxUser(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucDWORD MaxUser)
	{
		ucINT64 n64UserGameID = UserGameID.ToInt64();

		ucINT Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos < 0)
			return UCString("无效用户");
		CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		if (pGame_Player->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效Key");

		if (Game_Match_Room_RDObj.RoomInfo.MasterID.Value != n64UserGameID)
			return UCString("权限不足");

		Game_Match_Room_RDObj.RoomInfo.MaxUser = MaxUser;
		Battle_RDObj.RoomInfo.MaxUser = MaxUser;
		return UCString("成功");
	}

	UCString	KickUser(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucCONST CUserGameID& KickID)
	{
		if (Battle_RDObj.RoomInfo.Start.Value == 2)
			return UCString("游戏已经开始");

		ucINT64 n64UserGameID = UserGameID.ToInt64();
		ucINT Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos < 0)
			return UCString("无效用户");
		CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		if (pGame_Player->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效Key");

		if (Game_Match_Room_RDObj.RoomInfo.MasterID.Value != n64UserGameID)
			return UCString("权限不足");

		ucINT64 n64KickID = KickID.ToInt64();

		if (Game_Match_Room_RDObj.RoomInfo.MasterID.Value == n64KickID)
			return UCString("别想不开");//房主踢自己，一般不会发生
		Pos = m_MapUserGameIDPlayer.FindKey(n64KickID);
		if (Pos < 0)
			return UCString("无效用户");
		CGame_Player* Kick_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		m_MapUserGameIDPlayer.RemoveAt(Pos);
		//m_pService->Log(UCString("房主踢人:") + Kick_Player->NickName.Value + UCString("\r\n"));
		LeaveGame(Kick_Player);
		return UCString("成功");
	}

	UCString	SendMsg(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucCONST UCString& strMsg)
	{
		ucINT64 n64UserGameID = UserGameID.ToInt64();

		ucINT Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos < 0)
			return UCString("无效用户");
		CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		if (pGame_Player->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效Key");

		CGame_Chat Chat;
		Chat.Msg = strMsg;
		Battle_RDObj.AryChat.Add(Chat);
		if (Battle_RDObj.AryChat.GetSize() > 100)
			Battle_RDObj.AryChat.RemoveAt(0);

		return UCString("成功");
	}

	UCString	StartGame(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey)
	{
		ucINT64 n64UserGameID = UserGameID.ToInt64();

		ucINT Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos < 0)
			return UCString("无效用户");
		CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		if (pGame_Player->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效Key");
		if (Game_Match_Room_RDObj.RoomInfo.MasterID != n64UserGameID)
			return UCString("无权限");
		if (Game_Match_Room_RDObj.RoomInfo.Start.Value == 0)
			return UCString("人数不够");
		if (Game_Match_Room_RDObj.RoomInfo.Start.Value == 2)
			return UCString("已经开始");

		if (Battle_RDObj.Result.Value != 0)
			Battle_RDObj.Result = 0;
		Battle_RDObj.LeastTime = 5;
		Game_Match_Room_RDObj.RoomInfo.Start = 2;
		Battle_RDObj.RoomInfo.Start = 2;

		return UCString("成功");
	}
	UCString	ChangeName(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, UCString Name)
	{
		ucINT64 n64UserGameID = UserGameID.ToInt64();

		ucINT Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos < 0)
			return UCString("无效用户");
		CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		if (pGame_Player->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效Key");

		pGame_Player->m_Player_PubInfo.NickName = Name;
		return UCString("成功");
	}

	UCString	TryPing()
	{
		return UCString("成功");
	}
public:
	UCString	SetChar(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, UCString Msg)
	{
		ucINT64 n64UserGameID = UserGameID.ToInt64();

		ucINT Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos < 0)
			return UCString("无效用户");
		CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		if (pGame_Player->GameKeyRD.GameKey.Value != GameKey)
			return UCString("无效Key");
		if (Battle_RDObj.RoomInfo.Start.Value == 2)
			return UCString("嘘。。。");

		if (*m_EnbaleTokenSwitch)
		{
			UCString msgcode = UCString("https://developer.toutiao.com/api/v2/tags/text/antidirt");
			//"{"tasks":[{"content":"我不是毛泽东"}]}"

			UCString Body = UCString("{\"tasks\":[{\"content\":\"") + Msg + UCString("\"}]}");

			UCString result;
			m_PostCUrl->HttpPost(msgcode, Body, result);
			UCJson x_tokenjson;
			if (!x_tokenjson.Parse(result))
				return UCString("json解析错误");
			if (x_tokenjson.isMember("error_id"))
			{
				if (x_tokenjson.GetMember("code").isInt())
				{
					return UCString(ITOS(x_tokenjson.GetMember("code").asInt()));
				}
			}
			else
			{
				if (!x_tokenjson.isMember("data"))
					return UCString("error");
				UCJson datajson = x_tokenjson.GetMember("data");//得data列
				if (datajson.GetSize() <= 0)
					return UCString("error");
				UCJson firstjson = datajson.GetAt(0);//得到第一个
				if (!firstjson.isMember("predicts"))
					return UCString("error");
				UCJson predictjson = firstjson.GetMember("predicts");//得到需要的那一个组内的内容
				if (predictjson.GetSize() <= 0)
					return UCString("error");
				UCJson secondjson = predictjson.GetAt(0);//得到predicts的第一个
				if (!secondjson.isMember("hit"))
					return UCString("error");
				UCString resultstring = secondjson.GetMember("hit").asString();
				if (resultstring == "true")
				{
					return UCString("非法字符");
				}
			}
		}

		CGame_Chat pack;
		pack.PlayerID = n64UserGameID;
		pack.Msg = Msg;
		Battle_RDObj.AryChat.Add(pack);
		m_Log->Info(ITOS(RoomIndex) + UCString("号房间:") + pGame_Player->m_Player_PubInfo.NickName.Value + UCString("(") + UserGameID.ToString() + UCString(")说:") + Msg);
		return UCString("成功");
	}
	ucVOID		SyncPlayerPos(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey,
		ucCONST uc3dxVector3& Pos0, ucFLOAT RotY0, ucCONST uc3dxVector3& Pos1, ucFLOAT RotY1)
	{
		if (Game_Match_Room_RDObj.RoomInfo.Start.Value != 2)
			return;

		ucINT64 n64UserGameID = UserGameID.ToInt64();
		ucINT Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos < 0)
			return ;

		CGame_Player* Game_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		if (Game_Player->GameKeyRD.GameKey.Value != GameKey)
			return;

		ucINT SeatID = Game_Player->m_Player_PubInfo.SeatID.Value;
		CPlayer_PhyInfo* Player_PhyInfo = &(Battle_RDObj.Player_PhyInfo[SeatID]);

		Player_PhyInfo->Pos = Pos0;
		Player_PhyInfo->RotY = RotY0;
		Player_PhyInfo->FPS++;
		Player_PhyInfo->UpdateData();

		Player_PhyInfo->Pos = Pos1;
		Player_PhyInfo->RotY = RotY1;
		Player_PhyInfo->FPS++;
		Player_PhyInfo->UpdateData();

		Game_Player->m_Player_PhyInfo_RDObj.KeepValid();
	}
	UCString JoinGame(ucCONST CUserGameID& UserGameID, ucCONST UCString& strPrivateGate, CGame_Player** ppGame_Player)
	{
		*ppGame_Player = ucNULL;

		ucINT64 n64UserGameID = UserGameID.ToInt64();
		ucINT Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos >= 0)
		{
			CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
			*ppGame_Player = pGame_Player;
			return UCString("成功");
		}

		if (Game_Match_Room_RDObj.RoomInfo.Start.Value >= 2)
			return UCString("已经开始");
		if (m_MapSeatID_Empty.GetSize() <= 0)
			return UCString("没空位");
		ucINT iSeatID = m_MapSeatID_Empty.GetKeyAt(0);
		m_MapSeatID_Empty.RemoveAt(0);
		UCString strRoot = m_pService->Name + UCString("//Battle_Room/") + ITOS(RoomIndex);
		CGame_Player* pGame_Player = new CGame_Player(UserGameID, iSeatID, m_pService);
		UCString strResult = pGame_Player->Init(strRoot, strPrivateGate, m_AdminKey, &Battle_RDObj);
		if (strResult != "成功")
		{
			delete pGame_Player;
			//位置还回去
			m_MapSeatID_Empty.Add(iSeatID, 0);
			return strResult;
		}
		if (Battle_RDObj.RoomInfo.RoomState.Value == 0)
		{
			delete pGame_Player;
			//位置还回去
			m_MapSeatID_Empty.Add(iSeatID, 0);
			return UCString("房间失效");
		}
		Pos = m_MapUserGameIDPlayer.FindKey(n64UserGameID);
		if (Pos >= 0)
		{
			CGame_Player* pGamePlayer0 = m_MapUserGameIDPlayer.GetValueAt(Pos);
			delete pGame_Player;
			*ppGame_Player = pGamePlayer0;
			//位置还回去
			m_MapSeatID_Empty.Add(iSeatID, 0);
			return UCString("成功");
		}
		ucINT IsMaster = 0;

		Game_Match_Room_RDObj.RoomInfo.UserID[iSeatID] = n64UserGameID;
		Battle_RDObj.RoomInfo.UserID[iSeatID] = n64UserGameID;
		Battle_RDObj.Player_PubInfo[iSeatID].EnterTime = UCGetTime();

		Battle_RDObj.Player_PubInfo[iSeatID].UserGameID = n64UserGameID;
		Battle_RDObj.Player_PubInfo[iSeatID].NickName = pGame_Player->NickName;

		Battle_RDObj.Player_PubInfo[iSeatID].AvatarData.Copy(pGame_Player->AvatarData);

		m_MapUserGameIDPlayer.Add(n64UserGameID, pGame_Player);

		pGame_Player->m_Player_PhyInfo_RDObj.UserGameID = n64UserGameID;
		pGame_Player->m_Player_PhyInfo_RDObj.KeepValid();

		pGame_Player->m_Player_PhyInfo_RDObj.TimeOut = USERDATA_TIMEOUT * 10000;
		pGame_Player->m_Player_PhyInfo_RDObj.OnTimeOut = UCEvent(this, OnGamePlayerTimeOut);
		pGame_Player->OnRemoteRelease = UCEvent(this, OnMyUserDataRelease);

		//把这个人拉到场内
		ucINT ret = pGame_Player->UserData_GameExt.LockHost();
		if (ret)
		{
			m_pService->Log(UCString("GameExt.LockHost进入失败:") + ITOS(ret) + UCString("\r\n"));
		}
		else
		{
			pGame_Player->UserData_GameExt.BattleInfo.MatchID = MatchIndex;
			pGame_Player->UserData_GameExt.BattleInfo.GameID = GameIndex;
			pGame_Player->UserData_GameExt.BattleInfo.BattleID = RoomIndex;
			pGame_Player->UserData_GameExt.BattleInfo.UpdateData();
			pGame_Player->UserData_GameExt.UnlockHost();
			//m_pService->Log(UCString("GameExt.LockHost进入成功:") + ITOS(MatchIndex) + UCString("_") + ITOS(GameIndex) + UCString("_") + ITOS(RoomIndex) + UCString("\r\n"));
		}

		//多少人能开始游戏
		if (m_MapUserGameIDPlayer.GetSize() >= ROOM_USER_MIN)
		{
			Game_Match_Room_RDObj.RoomInfo.Start = 1;
			Battle_RDObj.RoomInfo.Start = 1;
		}
		else
		{
			Game_Match_Room_RDObj.RoomInfo.Start = 0;
			Battle_RDObj.RoomInfo.Start = 0;
		}
		*ppGame_Player = pGame_Player;
		return UCString("成功");
	}
private:
	//玩家离开游戏
	ucVOID LeaveGame(CGame_Player* pGame_Player)
	{
		ucINT64 deleteID = pGame_Player->m_Player_PubInfo.UserGameID.Value;
		if (Battle_RDObj.RoomInfo.Start.Value == 2)
		{
			if (Battle_RDObj.Result.Value == 0)
				SetGameOver(0);
		}

		ucINT SeatID = pGame_Player->m_Player_PubInfo.SeatID.Value;
		if (SeatID >= 0 && SeatID < ROOM_USER_MAX)
		{
			Game_Match_Room_RDObj.RoomInfo.UserID[SeatID] = 0;
			Battle_RDObj.RoomInfo.UserID[SeatID] = 0;
			m_MapSeatID_Empty.Add(SeatID, 0);
		}

		ucINT ret = pGame_Player->UserData_GameExt.LockHost();
		if (ret)
		{
			m_pService->Log(UCString("GameExt.LockHost离开失败:") + ITOS(ret) + UCString("\r\n"));
		}
		else
		{
			//m_pService->Log(UCString("GameExt.LockHost离开成功:") + ITOS(pGame_Player->UserData_GameExt.BattleInfo.MatchID) + UCString("_") + ITOS(pGame_Player->UserData_GameExt.BattleInfo.GameID) + UCString("_") + ITOS(pGame_Player->UserData_GameExt.BattleInfo.BattleID) + UCString("\r\n"));
			pGame_Player->UserData_GameExt.BattleInfo.MatchID = -1;
			pGame_Player->UserData_GameExt.BattleInfo.GameID = -1;
			pGame_Player->UserData_GameExt.BattleInfo.BattleID = -1;
			pGame_Player->UserData_GameExt.BattleInfo.UpdateData();
			pGame_Player->UserData_GameExt.UnlockHost();
		}
		//m_pService->Log(UCString("玩家离开:") + pGame_Player->NickName.Value + UCString("\r\n"));
		delete pGame_Player;
		if (m_MapUserGameIDPlayer.GetSize() <= 0)
		{
			m_MainFiber.SetUnValid();
			//让整个房间无效
			Game_Match_Room_RDObj.RoomInfo.RoomState = 0;
			Battle_RDObj.RoomInfo.RoomState = 0;
			//Reset();
			//m_pService->Log(UCString("房间销毁：") + ITOS(RoomIndex) + UCString("\r\n"));
			OnUnValid.Run(this, 0);
			return;
		}
		//如果自己是Master
		if (Game_Match_Room_RDObj.RoomInfo.MasterID.Value == deleteID)
		{
			if (m_MapUserGameIDPlayer.GetSize() > 0)
			{
				Game_Match_Room_RDObj.RoomInfo.MasterID = m_MapUserGameIDPlayer.GetKeyAt(randint(0, m_MapUserGameIDPlayer.GetSize() - 1));
				Battle_RDObj.RoomInfo.MasterID = Game_Match_Room_RDObj.RoomInfo.MasterID.Value;
			}
			else
			{
				Game_Match_Room_RDObj.RoomInfo.MasterID = 0;
				Battle_RDObj.RoomInfo.MasterID = 0;
			}
		}
		if (Battle_RDObj.RoomInfo.Start.Value == 2 || Battle_RDObj.RoomInfo.MasterID.Value == 0)
			return;
		//多少人能开始游戏
		if (m_MapUserGameIDPlayer.GetSize() >= ROOM_USER_MIN)
		{
			Game_Match_Room_RDObj.RoomInfo.Start = 1;
			Battle_RDObj.RoomInfo.Start = 1;
		}
		else
		{
			Game_Match_Room_RDObj.RoomInfo.Start = 0;
			Battle_RDObj.RoomInfo.Start = 0;
		}
	}
	//开始游戏
	ucVOID OnStartGame()
	{
		CRoomData* room = sGameData.GetRoomInfo(Battle_RDObj.MapID.Value);
		UCESimpleMap<ucINT64, CGame_Player*>				TempMap;
		UCInt64Array wolfarray;
		for (ucINT i = 0; i < m_MapUserGameIDPlayer.GetSize(); i++)//所有人初始化
		{
			CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(i);
			pGame_Player->m_Player_PhyInfo_RDObj.KeepValid();
			TempMap.Add(pGame_Player->m_Player_PubInfo.UserGameID.Value, pGame_Player);
		}
	}
	ucVOID OnMainFiber(UCObject* Sender, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Sender;

		ucINT fibertime = 0;
		while (FiberData->IsValid() && Battle_RDObj.RoomInfo.RoomState.Value > 0)
		{
			++fibertime;
			if (fibertime >= 10)
			{
				fibertime = 0;
				if (Battle_RDObj.RoomInfo.Start.Value == 2)
				{
					if (Battle_RDObj.LeastTime.Value > 0)
					{
						//开始游戏倒计时
						Battle_RDObj.LeastTime -= 1;
						if (Battle_RDObj.LeastTime.Value <= 0)
							OnStartGame();
					}

					if (Battle_RDObj.Result.Value)
					{
						//游戏结束了
						FiberData->Delay(1000);
						OnGameOver();
					}
				}
				else
				{
					if (Battle_RDObj.RoomInfo.MasterID.Value == 0)
					{
						if (Battle_RDObj.LeastTime.Value > 5)
						{
							//开始游戏倒计时
							Battle_RDObj.LeastTime -= 1;
							if (Battle_RDObj.LeastTime.Value <= 5)
							{
								Game_Match_Room_RDObj.RoomInfo.Start = 2;
								Battle_RDObj.RoomInfo.Start = 2;
							}
						}
					}
				}
			}

			FiberData->Every(1000);
		}
	}
	//玩家消息超时事件
	ucVOID OnGamePlayerTimeOut(UCObject* Sender, UCEventArgs*)
	{
		CPlayer_PhyInfo_RDObj* Player_RDObj = (CPlayer_PhyInfo_RDObj*)Sender;
		ucINT Pos = m_MapUserGameIDPlayer.FindKey(Player_RDObj->UserGameID.Value);
		if (Pos >= 0)
		{
			CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
			//如果游戏中。暂时屏蔽
			if (0 && Battle_RDObj.RoomInfo.Start.Value == 2)
			{
				pGame_Player->m_Player_PhyInfo_RDObj.KeepValid();
				if (pGame_Player->m_Offline == 0)
					pGame_Player->m_Offline = 1;
			}
			else
			{
				m_MapUserGameIDPlayer.RemoveAt(Pos);
				//m_pService->Log(UCString("玩家超时离开:") + pGame_Player->NickName.Value + UCString("\r\n"));
				LeaveGame(pGame_Player);
			}
		}
	}
	//GameDB下线事件
	ucVOID OnMyUserDataRelease(UCObject* o, UCEventArgs*)
	{
		CPlayer_PhyInfo_RDObj* Player_PhyInfo_RDObj = (CPlayer_PhyInfo_RDObj*)o;
		m_pService->Log(UCString("注意：不应该出现的日志，GameDB超时离开\r\n"));
		ucINT Pos = m_MapUserGameIDPlayer.FindKey(Player_PhyInfo_RDObj->UserGameID.Value);

		CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
		if (Pos >= 0 && Player_PhyInfo_RDObj == &(pGame_Player->m_Player_PhyInfo_RDObj))
		{
			m_MapUserGameIDPlayer.RemoveAt(Pos);
			LeaveGame(pGame_Player);
		}
		else
		{
			m_pService->Log(UCString("GameDB超时离开错误:") + pGame_Player->NickName.Value + UCString("\r\n"));
		}
	}
public:
	ucINT Init(ucINT iGameIndex, ucINT iRoomIndex, ucINT64 AdminKey, UCVF_Service* pService, UCCUrl* CUrl, ucINT* bEnbaleTokenSwitch)
	{
		GameIndex = iGameIndex;
		RoomIndex = iRoomIndex;
		m_AdminKey = AdminKey;
		m_pService = pService;
		m_PostCUrl = CUrl;
		m_EnbaleTokenSwitch = bEnbaleTokenSwitch;
		m_pService->AppendLocalFile(m_pService->Name + UCString("//Battle_RCObj/") + ITOS(RoomIndex) + UCString(".call"), typeof(CBattle_RCObj), typeof(CBattle_LocalRCObj), this, UCVF_Guest_Permission_ReadWrite);
		m_pService->AppendLocalFile(m_pService->Name + UCString("//Battle_RDObj/") + ITOS(RoomIndex) + UCString(".data"), typeof(CBattle_RDObj), &Battle_RDObj, UCVF_Guest_Permission_ReadOnly);
		m_pService->AppendLocalFile(m_pService->Name + UCString("//Game_Match_Room_RDObj/") + ITOS(RoomIndex) + UCString(".data"), typeof(CGame_Match_Room_RDObj), &Game_Match_Room_RDObj, UCVF_Guest_Permission_ReadOnly);
		return 1;
	}
	UCString CreateGame(ucCONST CUserGameID& UserGameID, ucCONST UCString& strPassword, ucCONST ucDWORD& iMatchIndex, ucINT iMapID, ucCONST UCString& strPrivateGate)
	{
		if (Battle_RDObj.RoomInfo.RoomState.Value != 0)
			return UCString("房间已存在");
		Password = strPassword;
		MatchIndex = iMatchIndex;
		Battle_RDObj.MapID = iMapID;
		Reset();
		//0-无效房间，1-随便进（搜索或指定加入），2-陌生人勿进（只能指定加入），3-密码房间（只能指定加入）
		if (strPassword.IsEmpty())
		{
			Battle_RDObj.RoomInfo.RoomState = 2;
			//Game_Match_Room_RDObj.RoomInfo.RoomState = 2;
		}
		else
		{
			Battle_RDObj.RoomInfo.RoomState = 3;
			//Game_Match_Room_RDObj.RoomInfo.RoomState = 3;
		}
		CGame_Player* pGame_Player = 0;
		UCString strRet = JoinGame(UserGameID, strPrivateGate, &pGame_Player);
		if (strRet == UCString("成功"))
		{
			//设置房主
			Game_Match_Room_RDObj.RoomInfo.MasterID = UserGameID.ToInt64();
			Battle_RDObj.RoomInfo.MasterID = Game_Match_Room_RDObj.RoomInfo.MasterID.Value;
			Game_Match_Room_RDObj.RoomInfo.RoomState = Battle_RDObj.RoomInfo.RoomState.Value;
			m_MainFiber.Start(0);
		}
		else
		{
			Battle_RDObj.RoomInfo.RoomState = 0;
			//Game_Match_Room_RDObj.RoomInfo.RoomState = 0;
		}
		return strRet;
	}
	UCString CreateMatchGame(ucCONST CMatchCreateRoom& createRoom, ucCONST ucDWORD& iMatchIndex, ucINT iMapID, ucCONST UCString& strPrivateGate, ucINT iLeastTime)
	{
		if (Battle_RDObj.RoomInfo.RoomState.Value != 0)
			return UCString("房间已存在");
		Password = "";
		MatchIndex = iMatchIndex;
		Battle_RDObj.MapID = iMapID;
		Reset();
		//0-无效房间，1-随便进（搜索或指定加入），2-陌生人勿进（只能指定加入），3-密码房间（只能指定加入）
		Battle_RDObj.RoomInfo.RoomState = 2;
		//Game_Match_Room_RDObj.RoomInfo.RoomState = 2;
		ucINT iTotle = 0;
		for (ucINT i = 0; i < ROOM_USER_MAX; ++i)
		{
			if (createRoom.UserID[i] == 0)
				continue;
			CGame_Player* pGame_Player = 0;
			UCString strRet = JoinGame(CUserGameID(createRoom.UserID[i]), strPrivateGate, &pGame_Player);
			if (strRet == UCString("成功"))
				++iTotle;
			else
			{
				m_pService->Log(UCString("CreateMatchGame::JoinGame失败:") + strRet + UCString("\r\n"));
				continue;
			}
		}
		//设置房主
		Game_Match_Room_RDObj.RoomInfo.MasterID = 0;
		Battle_RDObj.RoomInfo.MasterID = 0;
		Game_Match_Room_RDObj.RoomInfo.RoomState = Battle_RDObj.RoomInfo.RoomState.Value;
		//15秒倒计时
		if (iLeastTime < 1)
			iLeastTime = 1;
		Battle_RDObj.LeastTime = 5 + iLeastTime;
		m_MainFiber.Start(0);
		return UCString("成功");
	}
public:
	CBattle_LocalRCObj()
	{
		m_Log = 0;
		Reset();
		m_MainFiber.FiberEvent = UCEvent(this, OnMainFiber);
	}
	~CBattle_LocalRCObj()
	{
		Reset();
	}
	ucVOID Reset()
	{
		UCESimpleMap<ucINT64, CGame_Player*> MapPlatIDPlayer = m_MapUserGameIDPlayer;
		m_MapUserGameIDPlayer.RemoveAll();
		m_MapSeatID_Empty.RemoveAll();
		for (ucINT i = 0; i < ROOM_USER_MAX; i++)
			m_MapSeatID_Empty.Add(i, i);
		Battle_RDObj.Clear();
		Game_Match_Room_RDObj.RoomInfo.Clear();
		for (ucINT i = 0; i < MapPlatIDPlayer.GetSize(); i++)
			delete MapPlatIDPlayer.GetValueAt(i);
	}
	ucVOID SetGameOver(ucINT iResult)
	{
		if (iResult <= 0)
		{
			Battle_RDObj.Result = 0;
			return;
		}
		Battle_RDObj.Result = iResult;
	}
	ucVOID OnGameOver()
	{
		if (Battle_RDObj.RoomInfo.Start.Value != 2)
			return;

		Battle_RDObj.AryChat.RemoveAll();

		CRoomData* room = sGameData.GetRoomInfo(Battle_RDObj.MapID.Value);
		UCInt64Array aryOffline;
		for (ucINT i = 0; i < m_MapUserGameIDPlayer.GetSize(); i++)
		{
			CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(i);
			if (pGame_Player->m_Offline)
				aryOffline.Add(pGame_Player->m_Player_PubInfo.UserGameID.Value);
			pGame_Player->m_Player_PhyInfo_RDObj.KeepValid();
		}
		for (ucINT i = aryOffline.GetSize() - 1; i >= 0; --i)
		{
			ucINT64 iUserGameID = aryOffline.GetAt(i);
			ucINT Pos = m_MapUserGameIDPlayer.FindKey(iUserGameID);
			if (Pos >= 0)
			{
				CGame_Player* pGame_Player = m_MapUserGameIDPlayer.GetValueAt(Pos);
				if (pGame_Player->m_Offline)
				{
					m_MapUserGameIDPlayer.RemoveAt(Pos);
					LeaveGame(pGame_Player);
				}
			}
		}
		if (Battle_RDObj.RoomInfo.MasterID.Value == 0)
		{
			Game_Match_Room_RDObj.RoomInfo.Start = 3;
			Battle_RDObj.RoomInfo.Start = 3;
			return;
		}
		//多少人能开始游戏
		if (m_MapUserGameIDPlayer.GetSize() >= ROOM_USER_MIN)//有人，可开局
		{
			Game_Match_Room_RDObj.RoomInfo.Start = 1;
			Battle_RDObj.RoomInfo.Start = 1;
		}
		else
		{
			Game_Match_Room_RDObj.RoomInfo.Start = 0;//没有人，不用开局
			Battle_RDObj.RoomInfo.Start = 0;
		}
	}
};

class CGameService : public CGame_RCObj
{
	CGame_Center_RCObj					m_Game_Center_RCObj;				//平台中央服务器的远程调用
private:
	UCIntStringMap						m_MapDB_Info_Open;					//所有对外GameDB
	UCIntIntMap							m_MapDB_Info_UnOpen;				//所有对内GameDB
	CGameDB_Info_RDObj					m_GameDB_Info_RDObj;				//所有的GameDB信息
	CGame_DB_RCObj						m_GameDB_RCObj[GAMEDB_MAX];

	CBattle_LocalRCObj					m_Battle_LocalRCObj[BATTLE_MAX];	//所有房间内存
	UCIntIntMap							m_mapValidBattle;					//有人的房间
	UCIntIntMap							m_mapFreeBattle;					//无人房间

	//屏蔽字符用
	CToken_RDObj						m_Token_RDObj;
	UCCUrl								m_PostCUrl;
	UCString							X_Token;
	ucINT								m_EnbaleTokenSwitch;
private:
	UCEConfig<CGame_LocalCfg>			m_LocalCfg;
	UCVF_Service						m_Service;

	UCString							m_PrivateGate;				//我的私有Gate
	ucINT								m_Index;
	ucDWORD								m_CenterKey;				//中心服的临时key
	ucINT64								m_AdminKey;
	UCFiber								m_FiberInit;				//连接初始化
	UCLogCall							m_Log;
public:
	//匹配调用新建房间
	UCString	CreateRoom(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucCONST UCString& Password, ucCONST ucDWORD& MatchIndex, ucCONST ucINT MapID, ucINT& RoomIndex, ucINT iLimitRoomSize)
	{
		if (m_mapFreeBattle.GetSize() <= 0)
			return UCString("服务满");
		if (iLimitRoomSize > 0 && m_mapValidBattle.GetSize() >= iLimitRoomSize)
			return UCString("服务满");

		if (!CheckGameDBValid(UserGameID.DBIndex))
			return UCString("DB失败");

		//到GameDB校验UserKey的合法性
		UCString strRet = m_GameDB_RCObj[UserGameID.DBIndex].GameCheckValid(UserGameID, GameKey, m_Index);
		if (strRet != UCString("成功"))
		{
			return  UCString("GameKey失败");
		}
		CRoomData* room = sGameData.GetRoomInfo(MapID);
		if (room == 0)
			return UCString("非法地图ID");

		//因为有fiber切换，所以需要二次检查
		if (m_mapFreeBattle.GetSize() <= 0)
			return UCString("服务满");
		if (iLimitRoomSize > 0 && m_mapValidBattle.GetSize() >= iLimitRoomSize)
			return UCString("服务满");

		RoomIndex = m_mapFreeBattle.GetKeyAt(0);
		m_mapFreeBattle.RemoveAt(0);
		CBattle_LocalRCObj* Battle = &m_Battle_LocalRCObj[RoomIndex];
		UCString strResult = Battle->CreateGame(UserGameID, Password, MatchIndex, MapID, m_PrivateGate);
		if (strResult != "成功")
		{
			m_mapFreeBattle.Add(RoomIndex, 0);
			return strResult;
		}
		Battle->m_Log = &m_Log;
		m_mapValidBattle.Add(RoomIndex, 0);
		m_Service.SetValue(2 + m_mapValidBattle.GetSize());
		return UCString("成功");
	}
	//匹配调用指定用户创建房间
	UCString	CreateMatchRoom(ucCONST CMatchCreateRoom& createRoom, ucCONST ucINT& MatchIndex, ucCONST ucINT MapID, ucINT& RoomIndex, ucINT iLimitRoomSize, ucINT iLeastTime)
	{
		if (m_mapFreeBattle.GetSize() <= 0)
			return UCString("服务满");
		if (iLimitRoomSize > 0 && m_mapValidBattle.GetSize() >= iLimitRoomSize)
			return UCString("服务满");

		CRoomData* room = sGameData.GetRoomInfo(MapID);
		if (room == 0)
			return UCString("非法地图ID");

		RoomIndex = m_mapFreeBattle.GetKeyAt(0);
		m_mapFreeBattle.RemoveAt(0);
		CBattle_LocalRCObj* Battle = &m_Battle_LocalRCObj[RoomIndex];
		UCString strResult = Battle->CreateMatchGame(createRoom, MatchIndex, MapID, m_PrivateGate, iLeastTime);
		if (strResult != "成功")
		{
			m_mapFreeBattle.Add(RoomIndex, 0);
			return strResult;
		}
		Battle->m_Log = &m_Log;
		m_mapValidBattle.Add(RoomIndex, 0);
		m_Service.SetValue(2 + m_mapValidBattle.GetSize());
		return UCString("成功");
	}
	//匹配调用加入房间
	UCString	JoinRoom(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucCONST UCString& Password, ucCONST ucDWORD& MatchIndex, ucCONST ucINT MapID, ucCONST ucINT& RoomIndex)
	{
		if (RoomIndex >= BATTLE_MAX)
			return UCString("错误房间");

		if (m_Battle_LocalRCObj[RoomIndex].Battle_RDObj.RoomInfo.RoomState.Value == 0)
			return UCString("房间失效");
		if (m_Battle_LocalRCObj[RoomIndex].Battle_RDObj.RoomInfo.MasterID.Value == 0)
			return UCString("无效房间");

		if (!CheckGameDBValid(UserGameID.DBIndex))
			return UCString("DB失败");

		//到GameDB校验UserKey的合法性
		UCString strRet = m_GameDB_RCObj[UserGameID.DBIndex].GameCheckValid(UserGameID, GameKey, m_Index);
		if (strRet != UCString("成功"))
			return  UCString("GameKey失败");

		CBattle_LocalRCObj* Battle = &m_Battle_LocalRCObj[RoomIndex];
		if (!Battle->Password.IsEmpty() && Battle->Password != Password)
			return UCString("密码错误");

		if (Battle->MatchIndex != MatchIndex)
			return UCString("匹配服错误");

		if (Battle->Battle_RDObj.RoomInfo.RoomState.Value == 0)
			return UCString("房间失效");
		//MapID = Battle->Battle_RDObj.MapID.Value;
		if (Battle->Battle_RDObj.RoomInfo.MasterID.Value == 0)
		{
			if (Battle->m_MapUserGameIDPlayer.FindKey(UserGameID.ToInt64()) >= 0)
				return UCString("成功");
			return UCString("无效房间");
		}
		CGame_Player* pGame_Player = 0;
		return Battle->JoinGame(UserGameID, m_PrivateGate, &pGame_Player);
	}
	//匹配获取房间列表，用于匹配服重开
	UCString	GetRoomList(ucCONST ucDWORD& MatchIndex, UCEArray<ucINT>& aryRoom)
	{
		aryRoom.RemoveAll();
		for (ucINT i = m_mapValidBattle.GetSize() - 1; i >= 0; --i)
		{
			ucINT iBattleIndex = m_mapValidBattle.GetKeyAt(i);
			if (m_Battle_LocalRCObj[iBattleIndex].MatchIndex == MatchIndex)
				aryRoom.Add(iBattleIndex);
		}
		return UCString("成功");
	}
private:
	ucINT CheckGameDBValid(ucINT iDBIndex)
	{
		//需要先到GameDB校验UserKey的合法性
		if (iDBIndex < 0 || iDBIndex >= GAMEDB_MAX || m_GameDB_Info_RDObj.GameDB_Info[iDBIndex].Open == 0)
			return 0;

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
public:
	CGameService()
	{
		//UCEVariableInfo v = typeof(CGameService);
		//MBoxInt(v._Sizeof());
		m_CenterKey = 0;
		m_AdminKey = 0;
		m_EnbaleTokenSwitch = 0;
		m_FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		m_Service.OnClose = UCEvent(this, OnMyServiceClose);
	}
	~CGameService()
	{
	}
	ucINT Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("game"));
		if (!m_LocalCfg.LoadFromXML(UCString("game/game.xml")))
		{
			CGame_LocalCfg* LocalCfg = m_LocalCfg.GetData();
			LocalCfg->EnableLog = 1;
			LocalCfg->LogUrl = "GameLog//";
			LocalCfg->Key = "123456";
			LocalCfg->SingleGate = 0;
			LocalCfg->PrivateGate = "";
			LocalCfg->DnsURL = "Dns//";
			m_LocalCfg.SaveToXML(UCString("game/game.xml"));
		}
		CGame_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->EnableLog && LocalCfg->LogUrl.Right(2) != UCString("//"))
		{
			WBox(UCString("LogUrl路径错误：") + LocalCfg->LogUrl);
			return 0;
		}
		m_Service.SetTTL(1);
		m_Index = UCGetApp()->Index;
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
		UCGetSystemExt()->SetWindowCaption(UCString("Game/") + m_Service.Name);
		m_FiberInit.Start(0);
		return 1;
	}
	ucVOID OnMyServiceClose(UCObject*, UCEventArgs*)
	{
		if (m_Game_Center_RCObj.Linked)
			m_Game_Center_RCObj.UnRefeshGame(m_Index, m_CenterKey);
	}
	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;
		m_Service.Log(UCString("start Game:") + m_Service.Name + UCString("\r\n"));
		m_Service.SetValue(1);		//先对内状态

		CGame_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (m_Index < 0 || m_Index >= GAME_MAX)
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
			UCString strRet = logRCObj.AddModule(UCString("GameBattle"), m_Service.Name, m_Service.StateObj->PID.Value, bEnablePosWindows);
			if (strRet == UCString("成功"))
			{
				//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
				if (bEnablePosWindows == 0)
					SetExceptionMode(2);
				m_Log.SetService(&m_Service);
				ucINT ret = m_Log.Init(LocalCfg->LogUrl, UCString("GameBattle"), m_Service.Name, ADMIN_PASSWORD);
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
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CGame_RCObj.call"), typeof(CGame_RCObj), typeof(CGameService), this);
		//连接游戏中心服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_Center_RCObj.call"), typeof(CGame_Center_RCObj), &m_Game_Center_RCObj);
		//连接账号DB信息
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGameDB_Info_RDObj.data"), typeof(CGameDB_Info_RDObj), &m_GameDB_Info_RDObj);

		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CToken_RDObj.data"), typeof(CToken_RDObj), &m_Token_RDObj, m_AdminKey);
		for (ucINT i = 0; i < GAMEDB_MAX; i++)
		{
			m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_DB_RCObj/") + ITOS(i, UCString("%04d.call")), typeof(CGame_DB_RCObj), &m_GameDB_RCObj[i]);
			m_GameDB_Info_RDObj.GameDB_Info[i].BindOnSet(UCEvent(this, OnGameDB_InfoOnChanged));
		}
		m_GameDB_Info_RDObj.OnReset = UCEvent(this, OnMyGameDBInfoReset);
		m_Token_RDObj.OnReset = UCEvent(this, OnResetTokenToCur);
		m_Token_RDObj.m_strToken.BindOnSet(UCEvent(this, OnResetTokenToCur));

		m_Service.Log(UCString("开始连接中心服务器...\r\n"));
		while (FiberData->IsValid())
		{
			//连接中心服务器
			ucINT linkresult = m_Game_Center_RCObj.Link(m_PrivateGate + UCString("dom://GameCenter//CGame_Center_RCObj.call"));
			if (linkresult == 0)
				break;
			//m_Service.Log(UCString("中心服连接失败") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("中心服连接成功\r\n"));

		UCString Result = m_Game_Center_RCObj.RefeshGame(m_Service.Name, m_Index, LocalCfg->Key, m_AdminKey, m_CenterKey);
		if (Result != "成功")
		{
			m_Service.Log(UCString("中心服验证失败，请检查配置后重启服务 : ") + Result + UCString("\r\n"));
			return;
		}
		m_Service.Log(UCString("验证成功\r\n"));
		m_Service.SetAdminKey(m_AdminKey);

		while (FiberData->IsValid())
		{
			ucINT linkresult = m_GameDB_Info_RDObj.StableLink(m_PrivateGate + UCString("dom://GameCenter//CGameDB_Info_RDObj.data"), m_AdminKey);
			if (linkresult == 0)
				break;
			m_Service.Log(UCString("中心的gamedb列表连接失败:") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("中心的gamedb列表连接成功\r\n"));

		while (FiberData->IsValid())
		{
			ucINT linkresult = m_Token_RDObj.StableLink(m_PrivateGate + UCString("dom://GameCenter//CToken_RDObj.data"), m_AdminKey);
			if (linkresult == 0)
				break;
			m_Service.Log(UCString("中心的CToken_RDObj连接失败:") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}

		for (ucINT i = 0; i < BATTLE_MAX; ++i)
		{
			m_Battle_LocalRCObj[i].Init(m_Index, i, m_AdminKey, &m_Service, &m_PostCUrl, &m_EnbaleTokenSwitch);
			m_Battle_LocalRCObj[i].OnUnValid = UCEvent(this, OnMyGameRoomUnValid);
			m_Battle_LocalRCObj[i].OnRepairUser = UCEvent(this, OnMyGameRoomRepairUser);
			m_mapFreeBattle.Add(i, 0);
		}

		m_Service.Log(UCString("-----服务器启动完成-----\r\n\r\n"));
		m_Service.SetValue(2);		//可以对外了
		m_Service.SetTTL(DEF_DNS_TTL);
	}
	ucVOID OnMyGameRoomUnValid(UCObject* o, UCEventArgs*)
	{
		CBattle_LocalRCObj* pBattleRoomRCObj = (CBattle_LocalRCObj*)o;
		m_mapValidBattle.Remove(pBattleRoomRCObj->RoomIndex);
		m_mapFreeBattle.Add(pBattleRoomRCObj->RoomIndex, 0);
	}
	ucVOID OnMyGameRoomRepairUser(UCObject* o, UCEventArgs* e)
	{
		CBattle_LocalRCObj* pBattleRoomRCObj = (CBattle_LocalRCObj*)o;
		CUserGameID* pUserGameID = (CUserGameID*)e;
		if (!CheckGameDBValid(pUserGameID->DBIndex))
			return;
		m_GameDB_RCObj[pUserGameID->DBIndex].RepairMyState(*pUserGameID, m_Index, pBattleRoomRCObj->RoomIndex);
	}
	ucVOID OnResetTokenToCur(UCObject*, UCEventArgs* e)
	{
		//UCVF_PropertyBaseCmd_Args* Args = (UCVF_PropertyBaseCmd_Args*)e;

		//UCVF_String* pInfo = (UCVF_String*)Args->Data;

		//m_Service.Log(m_Token_RDObj.m_strToken.Value + UCString("\r\n"));

		m_PostCUrl.ClearHeader();
		if (m_Token_RDObj.m_strToken.Value.IsEmpty())
		{
			m_EnbaleTokenSwitch = 0;
		}
		else
		{
			m_EnbaleTokenSwitch = 1;
			m_PostCUrl.AppendHeader(UCString("X-Token:") + m_Token_RDObj.m_strToken.Value);
			//m_PostCUrl.AppendHeader(UCString("Content-Type=application/json"));
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

		CGameDB_Info* pInfo = (CGameDB_Info*)Args->Data;
		ucINT Index = m_GameDB_Info_RDObj.GameDB_Info[0].GetArrayIndex(pInfo);

		m_Service.Log(UCString("收到服务消息 GameDB[") + ITOS(Index) + UCString("].url=") + pInfo->Url + UCString("\r\n"));

		//如果服务地址为空，不认为是合法的地址
		if (pInfo->Url.IsEmpty())
		{
			m_MapDB_Info_Open.Remove(Index);
			m_MapDB_Info_UnOpen.Add(Index, 0);
			return;
		}

		if (pInfo->Open)
		{
			m_MapDB_Info_Open.Add(Index, pInfo->Url);
			m_MapDB_Info_UnOpen.Remove(Index);
		}
		else
		{
			m_MapDB_Info_Open.Remove(Index);
			m_MapDB_Info_UnOpen.Add(Index, 0);
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


#endif	//_moba_service_H_