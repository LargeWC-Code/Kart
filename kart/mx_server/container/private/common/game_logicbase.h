#ifndef _game_logicbase_
#define _game_logicbase_

#include "game_userdatabase.h"

#define BATTLE_USER_MAX		8
#define BATTLE_USER_MIN		1
#define BATTLE_USER_READY	2

class UCRGameUserPubInfo : public UCRData
{
public:
	UCRInt64			GameUserID;				//ID
	UCRString			Nickname;				//名字

	UCRProAvatarData	AvatarData;				//Avatar

	UCRInt				SeatID;
	ucVOID	Clear()
	{
		GameUserID = -1;
		Nickname = UCString();

		AvatarData.FaceID = -1;
		AvatarData.HairID = -1;
		AvatarData.BodyID = -1;
		AvatarData.HandID = -1;
		AvatarData.PantID = -1;
		AvatarData.ShoeID = -1;
		AvatarData.CarID = -1;

		SeatID = -1;
	}
};

struct UCRGameUserPhyInfoFrame : UCRProData
{
	ucINT				FPS;
	ucFLOAT				PosX;
	ucFLOAT				PosY;
	ucFLOAT				PosZ;
	ucFLOAT				RotY;
	ucVOID	Clear()
	{
		FPS = 0;
		PosX = 0.0f;
		PosY = 0.0f;
		PosZ = 0.0f;
		RotY = 0.0f;
	}
};
/*
* 玩家物理数据 - 单帧
*/
struct UCRGameUserPhyInfo : public UCRProData
{
	UCRGameUserPhyInfoFrame				RGameUserPhyInfoFrame[8];
	ucVOID	Clear()
	{
		for (ucINT i = 0; i < 8; i++)
			RGameUserPhyInfoFrame[i].Clear();
	}
};

class UCRObjGameUserPhyInfo : public UCRObject
{
public:
	UCRInt64							GameUserID;
	UCRGameUserPhyInfoFrame				RGameUserPhyInfoFrame[8];
};

enum BATTLE_TYPE
{
	BATTLE_TYPE_UNVALID = 0,
	BATTLE_TYPE_PUBLIC,
	BATTLE_TYPE_PRIVATE,
	BATTLE_TYPE_PASSWORD
};

enum BATTLE_STATE
{
	BATTLE_STATE_WAITING = 0,
	BATTLE_STATE_READY,
	BATTLE_STATE_GAMING,
	BATTLE_STATE_ENDGAME
};

struct UCGameMatch_BattleBase
{
	UCGameUserID		GameUserID[BATTLE_USER_MAX];		//用户ID
};

//战斗服务器，匹配用
class UCGameBattle_MatchInfo
{
public:
	//房间状态
	UCRInt			Type;				// BATTLE_TYPE
	//是否开始
	UCRInt			State;				// BATTLE_STATE

	UCRInt			MapID;

	//房主用户ID
	UCRInt64		MasterID;
	//用户ID
	UCRInt64		GameUserID[BATTLE_USER_MAX];

	//最大游戏用户数量
	UCRInt			MaxCount;
public:
	UCGameBattle_MatchInfo()
	{
		Clear();
	}
	ucVOID Clear()
	{
		Type = BATTLE_TYPE_UNVALID;
		State = BATTLE_STATE_WAITING;

		MasterID = 0xFFFFFFFFFFFFFFFFU;
		MaxCount = BATTLE_USER_MAX;

		for (ucINT i = 0; i < BATTLE_USER_MAX; ++i)
			GameUserID[i] = 0;
	}
};

class UCRObjGameBattle : public UCRObject
{
public:
	UCRInt					LeastTime;

	UCGameBattle_MatchInfo	GameBattle_MatchInfo;

	UCRGameUserPubInfo		UserPubInfo[BATTLE_USER_MAX];
	UCRGameUserPhyInfo		UserPhyInfo[BATTLE_USER_MAX];

	UCString				StartGame(ucCONST UCGameUserID& GameUserID, ucUINT64 Token)
	{
		return UCString("local");
	}

	UCString				Sync(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucCONST UCRGameUserPhyInfo& RGameUserPhyInfo)
	{
		return UCString("local");
	}

	UCString				ExitGame(ucCONST UCGameUserID& GameUserID, ucUINT64 Token)
	{
		return UCString("local");
	}
};

//给匹配用，房间快照
class UCRObjGameBattle_MatchInfo : public UCRObject
{
public:
	UCGameBattle_MatchInfo			GameBattle_MatchInfo;			//房间状态信息
};

class UCRObjGameLogic : public UCRObject
{
public:
	UCString	CreateBattle(ucCONST UCGameUserID& GameUserID, ucDWORD GameKey, ucINT MatchIndex, ucINT MapID, ucINT& BattleIndex)
	{
		return UCString("local");
	}
	UCString	JoinBattle(ucCONST UCGameUserID& GameUserID, ucDWORD GameKey, ucINT MatchIndex, ucINT MapID, ucINT BattleIndex)
	{
		return UCString("local");
	}
	//直接创建房间
	UCString	Direct_CreateBattle(ucCONST UCGameMatch_BattleBase& Match_BattleBase, ucINT MatchIndex, ucINT MapID, ucINT LeastTime, ucINT& BattleIndex)
	{
		return UCString("local");
	}
};

#endif