/*
	file : hall.robj
	author : LargeWC
	created : 2020/04/16
*/
#ifndef _hall_robj_
#define _hall_robj_

class CGame_Hall_RCObj : public UCVF_RemoteCallObj
{
public:
	//进入大厅
	UCString Enter(const CUserGameID& UserGameID, const ucDWORD& GameKey)
	{
		return UCString("本地");
	}
	UCString ChangeName(const CUserGameID& UserGameID, const ucDWORD& GameKey, UCString Name)
	{
		return UCString("本地");
	}
	UCString UpdateOnllineTime(const CUserGameID& UserGameID, const ucDWORD& GameKey)//上传在线时长
	{
		return UCString("本地");
	}
	UCString CheckOnlinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey, int& onlinetime)//获得在线时长
	{
		return UCString("本地");
	}
	UCString GetOnlinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey)//得到在线奖励
	{
		return UCString("本地");
	}
	UCString CheckOfflinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey, int& offlinetime)//获得离线时长
	{
		return UCString("本地");
	}
	UCString GetOfflinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey, int WithAD)//得到离线奖励，withad为看不看广告，为1为看了
	{
		return UCString("本地");
	}
	UCString CheckDailyOnlinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey, int& steptime, int& onlinetime)//得到在线奖励信息，steptime为10分钟30分钟60分钟的档位，分别为1，2，3
	{
		return UCString("本地");
	}
	UCString GetDailyOnlinePrize(const CUserGameID& UserGameID, const ucDWORD& GameKey, int WithAD)//得到在线奖励，withad为看不看广告，为1为看了
	{
		return UCString("本地");
	}
};

#endif