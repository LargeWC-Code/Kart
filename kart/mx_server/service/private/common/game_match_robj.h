/*
	file : match.robj
	author : LargeWC
	created : 2020/04/16
*/

#ifndef _match_robj_
#define _match_robj_

#include "../../public/common/public_base.h"

//只给大厅调用
class CGame_Match_RCObj : public UCVF_RemoteCallObj
{
public:
	//创建房间
	UCString		CreateRoom(const CUserGameID& UserGameID, const ucDWORD& GameKey, const UCString& Password, const ucINT MapID, ucINT& GameIndex, ucINT& BattleIndex)
	{
		return UCString("本地");
	}
	//加入指定房间
	UCString		JoinRoom(const CUserGameID& UserGameID, const ucDWORD& GameKey, const UCString& Password, const ucINT MapID, ucINT GameIndex, ucINT BattleIndex, const CUserGameID& shareUserGameID)
	{
		return UCString("本地");
	}
	//随机加入房间
	UCString		JoinRoomRand(const CUserGameID& UserGameID, const ucDWORD& GameKey, const ucINT MapID, ucINT& GameIndex, ucINT& BattleIndex)
	{
		return UCString("本地");
	}
	//开始匹配
	UCString StartMatch(const CUserGameID& UserGameID, const ucDWORD& GameKey)
	{
		return UCString("本地");
	}
	//取消匹配
	UCString StopMatch(const CUserGameID& UserGameID, const ucDWORD& GameKey)
	{
		return UCString("本地");
	}

};

#endif