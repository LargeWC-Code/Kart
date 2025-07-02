#ifndef _game_db_
#define _game_db_

#include "private_base.h"

//暂定账号服务器最多是16个
#define PLATFORMACCOUNT_MAX			0x10
//暂定游戏数据服务器最多是1024个
#define PLATFORMDB_MAX				0x400

struct CItem : public UCVF_ProData
{
	ucINT					ID;		//货品ID
	ucINT					Count;
};

class UCVF_UserBag : public UCVF_GroupData
{
public:
	CItem				Item[BAG_MAXITEM];
};


struct UCVF_AvatarData : public UCVF_ProData
{
	ucINT		FaceID;
	ucINT		HairID;

	ucINT		BodyID;
	ucINT		HandID;

	ucINT		PantID;
	ucINT		ShoeID;

	ucINT		CarID;
	ucVOID		Copy(ucCONST UCVF_AvatarData& in)
	{
		FaceID = in.FaceID;
		FaceID = in.HairID;
		FaceID = in.BodyID;
		FaceID = in.HandID;
		FaceID = in.PantID;
		FaceID = in.ShoeID;
		FaceID = in.CarID;
	}
};

class CUserData_RDObj : public UCVF_RemoteDataObj
{
public:
	UCVF_String			NickName;

	UCVF_INT			Gold;					//金币
	UCVF_INT			Level;					//等级
	UCVF_INT64			Exp;					//经验

	UCVF_AvatarData		AvatarData;

	UCVF_INT			ChannelID;				//渠道ID
};
struct StoreInfoID :public UCVF_TplData
{
	ucINT		StoreID;

	StoreInfoID()
	{

	}
	StoreInfoID(const StoreInfoID& pack)
	{
		StoreID = pack.StoreID;
	}
};

class CUserData_ExtData_RDObj:UCVF_RemoteDataObj
{
public:
	UCVF_INT			OnlinePrizeMinute;//在线时长(在线放置用)

	UCVF_TIME			LastOfflineTime;//最后一次离线时间点

	UCVF_INT			OfflineMinutes;//离线的时长

	UCVF_INT			DailyOnlineMinute;//在线时长奖励用

	UCVF_INT			DailyOnlineTimePrizeTimes;//在线时长奖励领取次数(从0开始）

	UCVF_INT			DailyGetStarCoinTimes;

	UCVF_TIME			DailyFreshTime;//刷新时间
	UCVF_Array<StoreInfoID>		ItemAry;
};

//扩展数据，GameDB存放不用存盘的数据
class CUserData_Ext_RDObj : public UCVF_RemoteDataObj
{
public:
	//登录计数，给客户端提供断线重连标记的
	UCVF_INT			LoginCount;
};

//用户服务器之间的通用玩家数据，客户端无权拿到
class CUserData_GameDBExt_RDObj : public UCVF_RemoteDataObj
{
public:
	UCVF_DWORD			GameKey;

	CUserData_GameDBExt_RDObj()
	{
	}
};

struct CBattleInfo :public UCVF_ProData
{
	ucINT			MatchID;			//用户所在的匹配服ID
	ucINT			GameID;				//用户当前所在的游戏服务器
	ucINT			BattleID;			//用户所在的战场房间ID
};

//游戏扩展数据，场内专用LockHost
class CUserData_GameExt_RDObj : public UCVF_RemoteDataObj
{
public:
	CBattleInfo			BattleInfo;
	UCVF_INT64			PlatID;
};

class CGame_DB_RCObj : public UCVF_RemoteCallObj
{
public:
	//激活，返回一个游戏动态钥匙
	UCString	Activate(const CUserGameID& UserGameID, const ucDWORD& AccountID, const ucDWORD& AccountKey, ucDWORD& GameKey, ucINT& HallIndex)
	{
		return UCString("本地");
	}
	//快速登录
	UCString	ReActivate(const CUserGameID& UserGameID, const ucDWORD& GameKey, ucDWORD& newGameKey, ucINT& HallIndex)
	{
		return UCString("本地");
	}
	//验证UserGameID的合法性，给GameHall校验用户合法性
	UCString	HallCheckValid(const CUserGameID& UserGameID, const ucDWORD& GameKey, ucINT HallIndex)
	{
		return UCString("本地");
	}
	//验证UserGameID的合法性，给Game校验用户合法性
	UCString	GameCheckValid(const CUserGameID& UserGameID, const ucDWORD& GameKey, ucINT GameIndex)
	{
		return UCString("本地");
	}
	//修复指定玩家的状态，Game调用过来的
	ucVOID		RepairMyState(const CUserGameID& UserGameID, ucINT GameIndex, ucINT BattleIndex)
	{
	}
	
	//验证礼包码是否领取
	ucINT		PlayerCodeLoad(const CUserGameID& UserGameID, const ucDWORD& GameKey, UCString Code)
	{
		return -1;
	}

	//写入礼包码
	ucINT		PlayerCodeSet(const CUserGameID& UserGameID, const ucDWORD& GameKey, UCString Code)
	{
		return -1;
	}
};

#endif