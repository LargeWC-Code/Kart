/*
	file : game.robj
	author : LargeWC
	created : 2020/04/16
*/

#ifndef _game_robj_
#define _game_robj_

//#include "game_match_robj.h"
#include "../../public/common/public_base.h"

struct CPlayer_PhyInfo : public UCVF_ProData
{
	ucINT				FPS;
	uc3dxVector3		Pos;
	ucFLOAT				RotY;
	ucVOID	Clear()
	{
		FPS = 0;
		Pos = uc3dxVector3(0.0f, 0.0f, 0.0f);
		RotY = 0.0f;
	}
};

class CPlayer_PhyInfo_RDObj : public UCVF_RemoteDataObj
{
public:
	UCVF_INT64			UserGameID;
	CPlayer_PhyInfo		PhyInfo0;
	CPlayer_PhyInfo		PhyInfo1;
};

struct CVTempInt :public UCVF_TplData
{
	ucINT			m_iTarget;
	ucINT			m_iValue;
	CVTempInt()
	{
	}
	CVTempInt(ucCONST CVTempInt& pack)
	{
		m_iTarget = pack.m_iTarget;
		m_iValue = pack.m_iValue;
	}
};

class CPlayer_PubInfo : public UCVF_GroupData
{
public:
	UCVF_INT64			UserGameID;				//角色唯一ID
	UCVF_INT64			PlatID;
	UCVF_String			NickName;				//名字

	UCVF_AvatarData		AvatarData;

	UCVF_INT			EnterTime;
	UCVF_INT			SeatID;					//座位下标
	ucVOID	Clear()
	{
		UserGameID = -1;
		PlatID = -1;
		NickName = UCString();

		AvatarData.FaceID = -1;
		AvatarData.HairID = -1;
		AvatarData.BodyID = -1;
		AvatarData.HandID = -1;
		AvatarData.PantID = -1;
		AvatarData.ShoeID = -1;
		AvatarData.CarID = -1;

		EnterTime = 0;
		SeatID = -1;
	}
};

struct CGame_Chat : public UCVF_TplData
{
	ucINT64			PlayerID;
	UCString		Msg;
	CGame_Chat()
	{
	}
	CGame_Chat(ucCONST CGame_Chat& in)
	{
		PlayerID = in.PlayerID;
		Msg = in.Msg;
	}
};

//房间信息
class CRoomInfo : public UCVF_GroupData
{
public:
	//房间状态
	UCVF_INT		RoomState;			//0-无效房间，1-随便进（搜索或指定加入），2-陌生人勿进（只能指定加入），3-密码房间（只能指定加入）
	//是否开始
	UCVF_INT		Start;				//0-不能开始（人数不足），1-可开始，2-游戏中，3-新匹配游戏结束

	//房主用户ID
	UCVF_INT64		MasterID;
	//用户ID
	UCVF_INT64		UserID[ROOM_USER_MAX];

	//最大游戏用户数量
	UCVF_INT		MaxUser;
public:
	CRoomInfo()
	{
		Clear();
	}
	ucVOID Clear()
	{
		RoomState = 0;
		Start = 0;
		MasterID = 0;
		MaxUser = ROOM_USER_MAX;
		for (ucINT i = 0; i < ROOM_USER_MAX; ++i)
			UserID[i] = 0;
	}
};

//给匹配用，房间快照
class CGame_Match_Room_RDObj : public UCVF_RemoteDataObj
{
public:
	CRoomInfo			RoomInfo;			//房间状态信息
};

//给玩家的房间同步快照
class CBattle_RDObj : public UCVF_RemoteDataObj
{
public:
	//用户坐标信息 - 放到起始是为了缩小索引范围，减少数据通讯
	CPlayer_PhyInfo			Player_PhyInfo[ROOM_USER_MAX];
	//用户公开信息
	CPlayer_PubInfo			Player_PubInfo[ROOM_USER_MAX];

	CRoomInfo				RoomInfo;			//房间状态信息

	UCVF_INT				LeastTime;			//开始游戏倒计时
	UCVF_INT				Result;				//结果

	//聊天消息
	UCVF_Array<CGame_Chat>	AryChat;

	//MapID
	UCVF_INT				MapID;

	CBattle_RDObj()
	{
		Clear();
	}
	ucVOID Clear()
	{
		RoomInfo.Clear();

		LeastTime = 0;
		Result = 0;

		AryChat.RemoveAll();
		for (ucINT i = 0; i < ROOM_USER_MAX; i++)
		{
			Player_PhyInfo[i].Clear();
			Player_PubInfo[i].Clear();
		}
	}
};

class CBattle_RCObj : public UCVF_RemoteCallObj
{
public:
	//检查指定玩家是否在场内，如果是自己，填自己id，否则填观战玩家id
	UCString	GetRoomState(ucCONST CUserGameID& UserGameID, ucINT& StartState, ucINT& MapID)
	{
		return UCString("本地");
	}
	//修复我的状态（自己断线重连失败，IsValidPlayer返回失败的时候调用）
	ucVOID		RepairMyState(ucCONST CUserGameID& UserGameID)
	{
	}
	//设置房间能不能被查找到
	UCString	ChangePublic(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucCONST ucINT& Flag)
	{
		return UCString("本地");
	}
	//游戏结束领取奖励
	UCString	GetGameResult(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucINT& addGold)
	{
		return UCString("本地");
	}
	//强制退出房间，拿不到任何奖励
	UCString	ExitRoom(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey)
	{
		return UCString("本地");
	}

	UCString	SetMaxUser(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucDWORD MaxPlayer)
	{
		return UCString("本地");
	}

	UCString	KickUser(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucCONST CUserGameID& KickID)
	{
		return UCString("本地");
	}

	UCString	SendMsg(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucCONST UCString& strMsg)
	{
		return UCString("本地");
	}
	UCString	StartGame(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey)
	{
		return UCString("本地");
	}
	UCString	ChangeName(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, UCString Name)
	{
		return UCString("本地");
	}
	UCString	TryPing()
	{
		return UCString("本地");
	}
	UCString	SetChar(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, UCString Msg)
	{
		return UCString("本地");
	}
	//同步坐标
	ucVOID SyncPlayerPos(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, 
		ucCONST uc3dxVector3& Pos0, ucFLOAT RotY0,
		ucCONST uc3dxVector3& Pos1, ucFLOAT RotY1)
	{
	}
};

struct CMatchCreateRoom
{
	ucINT64		UserID[ROOM_USER_MAX];		//用户ID
public:
	CMatchCreateRoom()
	{
		for (ucINT i = 0; i < ROOM_USER_MAX; ++i)
			UserID[i] = 0;
	}
};

//接口不对外
class CGame_RCObj : public UCVF_RemoteCallObj
{
public:
	//匹配调用新建房间
	UCString	CreateRoom(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucCONST UCString& Password, ucCONST ucINT& MatchIndex, ucCONST ucINT MapID, ucINT& RoomIndex, ucINT iLimitRoomSize)
	{
		return UCString("本地");
	}
	//匹配调用加入房间
	UCString	JoinRoom(ucCONST CUserGameID& UserGameID, ucCONST ucDWORD& GameKey, ucCONST UCString& Password, ucCONST ucINT& MatchIndex, ucCONST ucINT MapID, ucCONST ucINT& RoomIndex)
	{
		return UCString("本地");
	}
	//匹配调用指定用户创建房间
	UCString	CreateMatchRoom(ucCONST CMatchCreateRoom& createRoom, ucCONST ucINT& MatchIndex, ucCONST ucINT MapID, ucINT& RoomIndex, ucINT iLimitRoomSize, ucINT iLeastTime)
	{
		return UCString("本地");
	}
	//匹配获取房间列表，用于匹配服重开
	UCString	GetRoomList(ucCONST ucDWORD& MatchIndex, UCEArray<ucINT>& aryRoom)
	{
		return UCString("本地");
	}
};

#endif