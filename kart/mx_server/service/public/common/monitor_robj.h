#ifndef _monitor_robj_
#define _monitor_robj_

//定义渠道最大数量
#define _CHANNEL_MAX		0x10

//监控快照数据
class CMonitor_RDObj : public UCVF_RemoteDataObj
{
public:
	UCVF_INT		GameVER[_CHANNEL_MAX];			//服务器版本号
	UCVF_INT		GameVEREnd[_CHANNEL_MAX];		//服务器版本号结束区间（<=GameVER忽略）
	UCVF_INT		MaintainTag[_CHANNEL_MAX];		//服务器维护状态：0-全开放，1-游戏对内（好友开放？），2-可游戏关闭好友系统，3-全对内
	UCVF_String		GameNote[_CHANNEL_MAX];			//服务器维护时的公告
	UCVF_String		PlatformGate;					//平台gate

	UCVF_INT		LimitPCU;						//限制全服PCU（0为不限制）
	UCVF_INT		LimitSinglePCU;					//限制单服PCU（0为不限制）
	UCVF_INT		LimitGameRoomSize;				//限制游戏单服房间数

	//指定GameDB，需要压测0号大厅，把0号大厅压到最大值，非选中GameDB用随机大厅，但不包括0号大厅
	UCVF_INT		TestGameDBIndex;				//用来压测的GameDB服，-1为不进行压测
	UCVF_INT		TestGameDB_AllocTo_Hall0;		//0号大厅压测人数最大值，0为不限制数量

	UCVF_INT		MatchServiceNum;				//设置匹配服数量，这样按取余管理各自的Game
	UCVF_INT		MatchRandRoomNum;				//设置匹配服随机匹配前几个房间数量

	//0号匹配服优先进0号Game，把0号Game压到最大值，其它匹配服用随机，但不包括0号Game
	UCVF_INT		LimitMatch0_Alloc_Game0;		//Game压测房间最大值，不受LimitGameRoomSize影响，0为不压测按随机
};

//暂定Gate最多是1024个
#define _GAME_GATE			0x400
//暂定游戏数据服务器最多是256个
#define _GAMEDB_MAX			0x100
//暂定大厅服务器最多是512个
#define _HALL_MAX			0x200
//暂定匹配服务器最多是16个
#define _MATCH_MAX			0x10
//暂定游戏服务器最多是1024个
#define _GAME_MAX			0x400

//监控实时数据
class CMonitor_REAL_RDObj : public UCVF_RemoteDataObj
{
public:
	UCVF_INT		CurrentPCU;							//全服当前PCU
	UCVF_INT		GateCount;							//对外Gate数量
	UCVF_INT		GatePCU[_GAME_GATE];				//Gate网络连接数

	UCVF_INT		GameDBPCU[_GAMEDB_MAX];				//GameDB的在线人数
	UCVF_INT		GameDBDAU[_GAMEDB_MAX];				//GameDB的活跃人数
	UCVF_INT		GameDBDNU[_GAMEDB_MAX];				//GameDB的新增人数
	UCVF_INT		HallPCU[_HALL_MAX];					//0号大厅的在线人数

	UCVF_INT		MatchPublicRoomNum[_MATCH_MAX];		//匹配服可加房间数
	UCVF_INT		MatchStartGameNum[_MATCH_MAX];		//匹配服开始游戏数

	UCVF_INT		GameRoomNum[_GAME_MAX];				//游戏服的房间数
	UCVF_INT		GamePCU[_GAME_MAX];					//游戏服的在线数
};


//监控管理
class CMonitor_RCObj : public UCVF_RemoteCallObj
{
public:
	//注册Gate
	void RegisterGate(int& iIndex, ucDWORD& randKey, int& iLimitPCU, int& iLimitSinglePCU, int& iCurrentPCU)
	{
		iIndex = -1;
		iLimitPCU = -1;
		iCurrentPCU = 0;
	}
	//注销Gate
	void UnRegisterGate(int iIndex, ucDWORD randKey)
	{
	}
	//上传Gate数据
	void UploadGateInfo(int iIndex, ucDWORD randKey, int iPCU, int& iLimitPCU, int& iLimitSinglePCU, int& iCurrentPCU)
	{
		iLimitPCU = -1;
		iCurrentPCU = -1;
	}
	//上传GameDB在线人数数据
	void UploadGameDBInfo(int iGameDBIndex, int iPCU, int iDAU, int iDNU)
	{
	}
	//上传Hall在线人数数
	void UploadHallInfo(int iHallIndex, int iPCU)
	{
	}
	//上传Match信息（只能Match上传）
	void UploadMatchInfo(int iMatchIndex, int iMatchNum, int iStartGameNum)
	{
	}
	//上传Game信息（只能Match上传）
	void UploadGameInfo(int iGameIndex, int iRoomNum, int iPCU)
	{
	}
	//重置信息
	void ResetInfo(const UCString& strPassword)
	{
	}
	//设置全服PCU，0为不限制
	void SetPCU(const UCString& strPassword, int iLimitPCU)
	{
	}
	//设置单服PCU，0为不限制
	void SetSinglePCU(const UCString& strPassword, int iLimitSinglePCU)
	{
	}
	//获取当前PCU
	void GetPCU(int& iCurrentPCU, int& iGateCount)
	{
	}
	//设置渠道版本号
	void SetGameVer(int iChannelID, int iGameVer)
	{
	}
	//设置渠道结束版本号
	void SetGameVerEnd(int iChannelID, int iGameVer)
	{
	}
	//设置渠道维护状态，0-全开放，1-游戏对内（好友开放？），2-可游戏关闭好友系统，3-全对内
	void SetMaintainTag(int iChannelID, int iTag)
	{
	}
	//设置渠道维护状态公告
	void SetGameNote(int iChannelID, const UCString& strNote)
	{
	}
	//设置平台Gate路径
	void SetPlatformGate(const UCString& strGate)
	{
	}
	//设置游戏单服房间数，0为不限制
	void SetGameRoomSize(const UCString& strPassword, int iLimitRoom)
	{
	}
	//设置匹配服数量
	void SetMatchServiceNum(const UCString& strPassword, int iNum)
	{
	}
	//设置随机房间前几
	void SetMatchRandRoomNum(const UCString& strPassword, int iRandNum)
	{
	}
	//设置压测GameDB序号
	void SetTestGameDBIndex(const UCString& strPassword, int iGameDBIndex)
	{
	}
	//设置大厅压测人数
	void SetTestLimitHall(const UCString& strPassword, int iLimitHall)
	{
	}
	//设置压测Game房间数
	void SetMatchToGame(const UCString& strPassword, int iLimitRoom)
	{
	}
};

#endif _monitor_robj_