#ifndef _game_monitorbase_
#define _game_monitorbase_

//定义渠道最大数量
#define _CHANNEL_MAX		0x10

struct UCRObjGameMonitor_Game
{
	UCRInt		PCU_MaxTotal;
	UCRInt		PCU_MaxChannel;
	UCRInt		MaxRoom;
};

class UCRObjGameMonitor : public UCRObject
{
public:
	UCRObjGameMonitor_Game		Game[_CHANNEL_MAX];
	UCRString					Plat_GateURL;
public:
	UCString Gate_Register(ucINT& Index, ucDWORD& RandKey, ucINT& PCU_MaxTotal, ucINT& PCU_MaxChannal, ucINT& PCU_Current)
	{
		Index = -1;
		PCU_MaxTotal = -1;
		PCU_MaxChannal = -1;
		PCU_Current = 0;
		return UCString("local");
	}
	UCString Gate_Unregister(ucINT Index, ucDWORD RandKey)
	{
		return UCString("local");
	}
	UCString Gate_Upload(ucINT Index, ucDWORD RandKey, ucINT PCU_CurrentChannel, ucINT& PCU_MaxTotal, ucINT& PCU_MaxChannel, ucINT& PCU_CurrentTotal)
	{
		PCU_MaxTotal = -1;
		PCU_MaxChannel = -1;
		PCU_CurrentTotal = 0;
		return UCString("local");
	}
// 	//上传GameDB在线人数数据
// 	ucVOID UploadGameDBInfo(ucINT iGameDBIndex, ucINT iPCU, ucINT iDAU, ucINT iDNU)
// 	{
// 	}
// 	//上传Hall在线人数数
// 	ucVOID UploadHallInfo(ucINT iHallIndex, ucINT iPCU)
// 	{
// 	}
// 	//上传Match信息（只能Match上传）
// 	ucVOID UploadMatchInfo(ucINT iMatchIndex, ucINT iMatchNum, ucINT iStartGameNum)
// 	{
// 	}
// 	//上传Game信息（只能Match上传）
// 	ucVOID UploadGameInfo(ucINT iGameIndex, ucINT iRoomNum, ucINT iPCU)
// 	{
// 	}
// 	//重置信息
// 	ucVOID ResetInfo(ucCONST UCString& strPassword)
// 	{
// 	}
// 	//设置全服PCU，0为不限制
// 	ucVOID SetPCU(ucCONST UCString& strPassword, ucINT LimitPCU)
// 	{
// 	}
// 	//设置单服PCU，0为不限制
// 	ucVOID SetSinglePCU(ucCONST UCString& strPassword, ucINT LimitSinglePCU)
// 	{
// 	}
// 	//获取当前PCU
// 	ucVOID GetPCU(ucINT& CurrentPCU, ucINT& iGateCount)
// 	{
// 	}
// 	//设置渠道版本号
// 	ucVOID SetGameVer(ucINT iChannelID, ucINT iGameVer)
// 	{
// 	}
// 	//设置渠道结束版本号
// 	ucVOID SetGameVerEnd(ucINT iChannelID, ucINT iGameVer)
// 	{
// 	}
// 	//设置渠道维护状态，0-全开放，1-游戏对内（好友开放？），2-可游戏关闭好友系统，3-全对内
// 	ucVOID SetMaintainTag(ucINT iChannelID, ucINT iTag)
// 	{
// 	}
// 	//设置渠道维护状态公告
// 	ucVOID SetGameNote(ucINT iChannelID, ucCONST UCString& strNote)
// 	{
// 	}
// 	//设置平台Gate路径
// 	ucVOID SetPlatformGate(ucCONST UCString& strGate)
// 	{
// 	}
// 	//设置游戏单服房间数，0为不限制
// 	ucVOID SetGameRoomSize(ucCONST UCString& strPassword, ucINT iLimitRoom)
// 	{
// 	}
// 	//设置匹配服数量
// 	ucVOID SetMatchServiceNum(ucCONST UCString& strPassword, ucINT iNum)
// 	{
// 	}
// 	//设置随机房间前几
// 	ucVOID SetMatchRandRoomNum(ucCONST UCString& strPassword, ucINT iRandNum)
// 	{
// 	}
// 	//设置压测GameDB序号
// 	ucVOID SetTestGameDBIndex(ucCONST UCString& strPassword, ucINT iGameDBIndex)
// 	{
// 	}
// 	//设置大厅压测人数
// 	ucVOID SetTestLimitHall(ucCONST UCString& strPassword, ucINT iLimitHall)
// 	{
// 	}
// 	//设置压测Game房间数
// 	ucVOID SetMatchToGame(ucCONST UCString& strPassword, ucINT iLimitRoom)
// 	{
// 	}
};

#endif