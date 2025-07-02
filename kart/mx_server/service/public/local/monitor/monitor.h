#ifndef _monitor_server_H_
#define _monitor_server_H_
#include "../../common/monitor_robj.h"
#include "../../common/log_robj.h"
#include "../../common/game_center_robj.h"


//本地配置存档
struct CMonitor_LocalCfg
{
	UCString				Password;						//管理员密码

	int						GameVER[_CHANNEL_MAX];			//服务器版本号
	int						GameVEREnd[_CHANNEL_MAX];		//服务器版本号结束区间（<=GameVER忽略）
	int						MaintainTag[_CHANNEL_MAX];		//服务器维护状态：0-全开放，1-游戏对内（好友开放？），2-可游戏关闭好友系统，3-全对内
	UCString				GameNote[_CHANNEL_MAX];			//服务器维护时的公告
	UCString				PlatformGate;					//平台gate

	int						LimitPCU;						//限制全服PCU（0为不限制）
	int						LimitSinglePCU;					//限制单服PCU（0为不限制）
	int						LimitGameRoomSize;				//限制游戏单服房间数

	int						MatchServiceNum;				//设置匹配服数量，这样按取余管理各自的Game
	int						MatchRandRoomNum;				//设置匹配服随机匹配前几个房间数量

	//指定GameDB，需要压测0号大厅，把0号大厅压到最大值，非选中GameDB用随机大厅，但不包括0号大厅
	int						TestGameDBIndex;				//用来压测的GameDB服，-1为不进行压测
	int						TestGameDB_AllocTo_Hall0;		//0号大厅压测人数最大值，0为不限制数量

	//0号匹配服优先进0号Game，把0号Game压到最大值，其它匹配服用随机，但不包括0号Game
	int						LimitMatch0_Alloc_Game0;		//Game压测房间最大值，不受LimitGameRoomSize影响

	UCString				WorkPath;						//日志输出路径
};

class CCenterKeyRCObj : public CCenterKey_RCObj
{
public:
	UCString				m_Password;					//管理员密码
	ucINT64					m_AdminKey;					//管理员密钥
public:
	CCenterKeyRCObj()
	{
		m_AdminKey = 0;
	}
public:
	//工具使用，由密码获取管理员密钥
	ucINT64		GetAdminKey(ucCONST UCString& strPassword)
	{
		if (strPassword.IsEmpty())
			return 0;
		if (m_Password != strPassword)
			return 0;
		return m_AdminKey;
	}
};

//监控服务器
class CMonitorService :public CMonitor_RCObj
{
	UCEConfig<CMonitor_LocalCfg>	m_LocalCfg;				//配置
	UCVF_Service					m_Service;				//服务

	CCenterKeyRCObj					m_CenterKeyRCObj;		//给工具用，获取密钥
	CMonitor_RDObj					m_Monitor_RDObj;		//监控设置快照
	CMonitor_REAL_RDObj				m_Reala_RDObj;			//监控实时快照

	ucDWORD							m_GateKey[_GAME_GATE];	//每个Gate的key

	UCLogCallManage					m_LogCallManage;		//日志管理
	UCLogCall						m_Log;
	UCFiber							m_FiberInit;			//连接初始化
public:
	//注册Gate
	void RegisterGate(int& iIndex, ucDWORD& randKey, int& iLimitPCU, int& iLimitSinglePCU, int& iCurrentPCU)
	{
		iIndex = -1;
		for (int i = 0; i < _GAME_GATE; ++i)
		{
			if (m_GateKey[i] == 0)
			{
				iIndex = i;
				break;
			}
		}
		if (iIndex == -1)
			return;
		iLimitPCU = m_Monitor_RDObj.LimitPCU.Value;
		iLimitSinglePCU = m_Monitor_RDObj.LimitSinglePCU.Value;
		iCurrentPCU = m_Reala_RDObj.CurrentPCU.Value;
		randKey = randint(0, 65535) << 16 | randint(1, 65535);
		m_GateKey[iIndex] = randKey;
		m_Reala_RDObj.GateCount += 1;
	}
	//注销Gate
	void UnRegisterGate(int iIndex, ucDWORD randKey)
	{
		if (iIndex < 0 || iIndex >= _GAME_GATE)
			return;
		if (m_GateKey[iIndex] != randKey)
			return;
		m_GateKey[iIndex] = 0;
		m_Reala_RDObj.GateCount -= 1;
		if (m_Reala_RDObj.GatePCU[iIndex].Value != 0)
		{
			m_Reala_RDObj.GatePCU[iIndex] = 0;
			m_Reala_RDObj.CurrentPCU -= m_Reala_RDObj.GatePCU[iIndex].Value;
		}
	}
	//上传数据
	void UploadGateInfo(int iIndex, ucDWORD randKey, int iPCU, int& iLimitPCU, int& iLimitSinglePCU, int& iCurrentPCU)
	{
		iLimitPCU = m_Monitor_RDObj.LimitPCU.Value;
		iLimitSinglePCU = m_Monitor_RDObj.LimitSinglePCU.Value;
		iCurrentPCU = 0;
		if (iIndex < 0 || iIndex >= _GAME_GATE)
			return;
		if (m_GateKey[iIndex] != randKey)
			return;
		int iDetPCU = iPCU - m_Reala_RDObj.GatePCU[iIndex].Value;
		if (iDetPCU != 0)
		{
			m_Reala_RDObj.GatePCU[iIndex] = iPCU;
			m_Reala_RDObj.CurrentPCU += iDetPCU;
		}
		iCurrentPCU = m_Reala_RDObj.CurrentPCU.Value - iPCU;
	}
	//上传GameDB在线人数数据
	void UploadGameDBInfo(int iGameDBIndex, int iPCU, int iDAU, int iDNU)
	{
		if (iGameDBIndex < 0 || iGameDBIndex >= _GAMEDB_MAX)
			return;
		if (m_Reala_RDObj.GameDBPCU[iGameDBIndex].Value != iPCU)
			m_Reala_RDObj.GameDBPCU[iGameDBIndex] = iPCU;
		if (m_Reala_RDObj.GameDBDAU[iGameDBIndex].Value != iDAU)
			m_Reala_RDObj.GameDBDAU[iGameDBIndex] = iDAU;
		if (m_Reala_RDObj.GameDBDNU[iGameDBIndex].Value != iDNU)
			m_Reala_RDObj.GameDBDNU[iGameDBIndex] = iDNU;
	}
	//上传Hall在线人数数
	void UploadHallInfo(int iHallIndex, int iPCU)
	{
		if (iHallIndex < 0 || iHallIndex >= _HALL_MAX)
			return;
		m_Reala_RDObj.HallPCU[iHallIndex] = iPCU;
	}
	//上传Match信息（只能Match上传）
	void UploadMatchInfo(int iMatchIndex, int iPublicRoomNum, int iStartGameNum)
	{
		if (iMatchIndex < 0 || iMatchIndex >= _MATCH_MAX)
			return;
		if (m_Reala_RDObj.MatchPublicRoomNum[iMatchIndex].Value != iPublicRoomNum)
			m_Reala_RDObj.MatchPublicRoomNum[iMatchIndex] = iPublicRoomNum;
		if (m_Reala_RDObj.MatchStartGameNum[iMatchIndex].Value != iStartGameNum)
			m_Reala_RDObj.MatchStartGameNum[iMatchIndex] = iStartGameNum;
	}
	//上传Game信息（只能Match上传）
	void UploadGameInfo(int iGameIndex, int iRoomNum, int iPCU)
	{
		if (iGameIndex < 0 || iGameIndex >= _GAME_MAX)
			return;
		if (m_Reala_RDObj.GameRoomNum[iGameIndex].Value != iRoomNum)
			m_Reala_RDObj.GameRoomNum[iGameIndex] = iRoomNum;
		if (m_Reala_RDObj.GamePCU[iGameIndex].Value != iPCU)
			m_Reala_RDObj.GamePCU[iGameIndex] = iPCU;
	}
	//重置信息
	void ResetInfo(const UCString& strPassword)
	{
		if (strPassword != m_LocalCfg.GetData()->Password)
			return;
		for (int i = 0; i < _GAME_GATE; ++i)
		{
			m_Reala_RDObj.GatePCU[i] = 0;
		}
		m_Reala_RDObj.CurrentPCU = 0;
	}
	//设置全服PCU，0为不限制
	void SetPCU(const UCString& strPassword, int iLimitPCU)
	{
		if (iLimitPCU < 0)
			return;
		if (strPassword != m_LocalCfg.GetData()->Password)
			return;
		m_LocalCfg.GetData()->LimitPCU = iLimitPCU;
		m_Monitor_RDObj.LimitPCU = iLimitPCU;
		m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
	}
	//设置单服PCU，0为不限制
	void SetSinglePCU(const UCString& strPassword, int iLimitSinglePCU)
	{
		if (iLimitSinglePCU < 0)
			return;
		if (strPassword != m_LocalCfg.GetData()->Password)
			return;
		m_LocalCfg.GetData()->LimitSinglePCU = iLimitSinglePCU;
		m_Monitor_RDObj.LimitSinglePCU = iLimitSinglePCU;
		m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
	}
	//获取当前PCU
	void GetPCU(int& iCurrentPCU, int& iGateCount)
	{
		iCurrentPCU = m_Reala_RDObj.CurrentPCU.Value;
		iGateCount = m_Reala_RDObj.GateCount.Value;
	}
	//设置渠道版本号
	void SetGameVer(int iChannelID, int iGameVer)
	{
		if (iChannelID < 0 || iChannelID >= _CHANNEL_MAX)
			return;
		if (m_Monitor_RDObj.GameVER[iChannelID].Value != iGameVer)
		{
			m_Monitor_RDObj.GameVER[iChannelID] = iGameVer;
			m_LocalCfg.GetData()->GameVER[iChannelID] = iGameVer;
			m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
		}
	}
	//设置渠道结束版本号
	void SetGameVerEnd(int iChannelID, int iGameVer)
	{
		if (iChannelID < 0 || iChannelID >= _CHANNEL_MAX)
			return;
		if (m_Monitor_RDObj.GameVEREnd[iChannelID].Value != iGameVer)
		{
			m_Monitor_RDObj.GameVEREnd[iChannelID] = iGameVer;
			m_LocalCfg.GetData()->GameVEREnd[iChannelID] = iGameVer;
			m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
		}
	}
	//设置渠道维护状态，0-全开放，1-游戏对内（好友开放？），2-可游戏关闭好友系统，3-全对内
	void SetMaintainTag(int iChannelID, int iTag)
	{
		if (iChannelID < 0 || iChannelID >= _CHANNEL_MAX)
			return;
		if (m_Monitor_RDObj.MaintainTag[iChannelID].Value != iTag)
		{
			m_Monitor_RDObj.MaintainTag[iChannelID] = iTag;
			m_LocalCfg.GetData()->MaintainTag[iChannelID] = iTag;
			m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
		}
	}
	//设置渠道维护状态公告
	void SetGameNote(int iChannelID, const UCString& strNote)
	{
		if (iChannelID < 0 || iChannelID >= _CHANNEL_MAX)
			return;
		if (m_Monitor_RDObj.GameNote[iChannelID].Value != strNote)
		{
			m_Monitor_RDObj.GameNote[iChannelID] = strNote;
			m_LocalCfg.GetData()->GameNote[iChannelID] = strNote;
			m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
		}
	}
	//设置平台Gate路径
	void SetPlatformGate(const UCString& strGate)
	{
		if (strGate.IsEmpty() || strGate.Right(2) != UCString("//"))
			return;
		if (m_Monitor_RDObj.PlatformGate.Value != strGate)
		{
			m_Monitor_RDObj.PlatformGate = strGate;
			m_LocalCfg.GetData()->PlatformGate = strGate;
			m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
		}
	}
	//设置游戏单服房间数，0为不限制
	void SetGameRoomSize(const UCString& strPassword, int iLimitRoom)
	{
		if (strPassword != m_LocalCfg.GetData()->Password)
			return;
		if (iLimitRoom < 0)
			iLimitRoom = 0;
		else if (iLimitRoom > BATTLE_MAX)
			iLimitRoom = BATTLE_MAX;
		m_LocalCfg.GetData()->LimitGameRoomSize = iLimitRoom;
		m_Monitor_RDObj.LimitGameRoomSize = iLimitRoom;
		m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
	}
	//设置匹配服数量
	void SetMatchServiceNum(const UCString& strPassword, int iNum)
	{
		if (strPassword != m_LocalCfg.GetData()->Password)
			return;
		if (iNum < 1)
			return;
		m_LocalCfg.GetData()->MatchServiceNum = iNum;
		m_Monitor_RDObj.MatchServiceNum = iNum;
		m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
	}
	//设置随机房间前几
	void SetMatchRandRoomNum(const UCString& strPassword, int iRandNum)
	{
		if (strPassword != m_LocalCfg.GetData()->Password)
			return;
		if (iRandNum < 1)
			return;
		m_LocalCfg.GetData()->MatchRandRoomNum = iRandNum;
		m_Monitor_RDObj.MatchRandRoomNum = iRandNum;
		m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
	}
	//设置压测GameDB序号
	void SetTestGameDBIndex(const UCString& strPassword, int iGameDBIndex)
	{
		if (strPassword != m_LocalCfg.GetData()->Password)
			return;
		CMonitor_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		LocalCfg->TestGameDBIndex = iGameDBIndex;
		m_Monitor_RDObj.TestGameDBIndex = iGameDBIndex;
		m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
	}
	//设置大厅压测人数
	void SetTestLimitHall(const UCString& strPassword, int iLimitHall)
	{
		if (strPassword != m_LocalCfg.GetData()->Password)
			return;
		CMonitor_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		LocalCfg->TestGameDB_AllocTo_Hall0 = iLimitHall;
		m_Monitor_RDObj.TestGameDB_AllocTo_Hall0 = iLimitHall;
		m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
	}
	//设置压测Game房间数
	void SetMatchToGame(const UCString& strPassword, int iLimitRoom)
	{
		if (strPassword != m_LocalCfg.GetData()->Password)
			return;
		if (iLimitRoom < 0)
			iLimitRoom = 0;
		else if (iLimitRoom > BATTLE_MAX)
			iLimitRoom = BATTLE_MAX;
		CMonitor_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		LocalCfg->LimitMatch0_Alloc_Game0 = iLimitRoom;
		m_Monitor_RDObj.LimitMatch0_Alloc_Game0 = iLimitRoom;
		m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
	}
public:
	CMonitorService()
	{
		m_Reala_RDObj.CurrentPCU = 0;
		m_Reala_RDObj.GateCount = 0;
		for (int i = 0; i < _GAME_GATE; ++i)
		{
			m_Reala_RDObj.GatePCU[i] = 0;
			m_GateKey[i] = 0;
		}
		for (int i = 0; i < _GAMEDB_MAX; ++i)
		{
			m_Reala_RDObj.GameDBPCU[i] = 0;
		}
		for (int i = 0; i < _HALL_MAX; ++i)
		{
			m_Reala_RDObj.HallPCU[i] = 0;
		}
		for (int i = 0; i < _MATCH_MAX; ++i)
		{
			m_Reala_RDObj.MatchPublicRoomNum[i] = 0;
			m_Reala_RDObj.MatchStartGameNum[i] = 0;
		}
		for (int i = 0; i < _GAME_MAX; ++i)
		{
			m_Reala_RDObj.GameRoomNum[i] = 0;
			m_Reala_RDObj.GamePCU[i] = 0;
		}

		for (int i = 0; i < _CHANNEL_MAX; ++i)
		{
			m_Monitor_RDObj.GameVER[i] = 0;
			m_Monitor_RDObj.GameVEREnd[i] = 0;
			m_Monitor_RDObj.MaintainTag[i] = 0;
		}
		m_Monitor_RDObj.LimitPCU = 0;
		m_Monitor_RDObj.LimitSinglePCU = 0;
		m_Monitor_RDObj.LimitGameRoomSize = 0;
		m_Monitor_RDObj.MatchServiceNum = 1;
		m_Monitor_RDObj.MatchRandRoomNum = 5;
		m_Monitor_RDObj.TestGameDBIndex = 0;
		m_Monitor_RDObj.TestGameDB_AllocTo_Hall0 = 0;
		m_Monitor_RDObj.LimitMatch0_Alloc_Game0 = 0;
		m_FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
	}
	~CMonitorService()
	{
	}
	int Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("monitor"));
		if (!m_LocalCfg.LoadFromXML(UCString("monitor/monitor.xml")))
		{
			CMonitor_LocalCfg* LocalCfg = m_LocalCfg.GetData();
			LocalCfg->Password = "Jump123456";
			for (int i = 0; i < _CHANNEL_MAX; ++i)
			{
				LocalCfg->GameVER[i] = 2021010901;
				LocalCfg->GameVEREnd[i] = 0;
				LocalCfg->MaintainTag[i] = 0;
			}
			LocalCfg->PlatformGate = "tcp://127.0.0.1:10010//";
			LocalCfg->LimitPCU = 100000;
			LocalCfg->LimitSinglePCU = 2000;
			LocalCfg->LimitGameRoomSize = BATTLE_MAX;
			LocalCfg->MatchServiceNum = 1;
			LocalCfg->MatchRandRoomNum = 5;
			LocalCfg->TestGameDBIndex = 0;
			LocalCfg->TestGameDB_AllocTo_Hall0 = 1000;
			LocalCfg->LimitMatch0_Alloc_Game0 = 200;
			LocalCfg->WorkPath = "D:\\Log\\GameLog\\";
			m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
		}
		CMonitor_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		for (int i = 0; i < _CHANNEL_MAX; ++i)
		{
			m_Monitor_RDObj.GameVER[i] = LocalCfg->GameVER[i];
			m_Monitor_RDObj.GameVEREnd[i] = LocalCfg->GameVEREnd[i];
			m_Monitor_RDObj.MaintainTag[i] = LocalCfg->MaintainTag[i];
			m_Monitor_RDObj.GameNote[i] = LocalCfg->GameNote[i];
		}
		int bSave = 0;
		if (LocalCfg->PlatformGate.IsEmpty() || LocalCfg->PlatformGate.Right(2) != UCString("//"))
		{
			LocalCfg->PlatformGate = "tcp://127.0.0.1:10010//";
			bSave = 1;
		}
		if (LocalCfg->MatchServiceNum < 1)
		{
			LocalCfg->MatchServiceNum = 1;
			bSave = 1;
		}
		if (LocalCfg->MatchRandRoomNum < 1)
		{
			LocalCfg->MatchRandRoomNum = 1;
			bSave = 1;
		}
		if (bSave)
			m_LocalCfg.SaveToXML(UCString("monitor/monitor.xml"));
		m_Service.Name = UCGetApp()->Name;
		m_CenterKeyRCObj.m_Password = LocalCfg->Password;
		m_CenterKeyRCObj.m_AdminKey = ADMIN_PASSWORD;
		UCGetSystemExt()->SetWindowCaption(UCString("Monitor/") + m_Service.Name);
		m_Monitor_RDObj.PlatformGate = LocalCfg->PlatformGate;
		m_Monitor_RDObj.LimitPCU = LocalCfg->LimitPCU;
		m_Monitor_RDObj.LimitSinglePCU = LocalCfg->LimitSinglePCU;
		m_Monitor_RDObj.LimitGameRoomSize = LocalCfg->LimitGameRoomSize;
		m_Monitor_RDObj.MatchServiceNum = LocalCfg->MatchServiceNum;
		m_Monitor_RDObj.MatchRandRoomNum = LocalCfg->MatchRandRoomNum;
		m_Monitor_RDObj.TestGameDBIndex = LocalCfg->TestGameDBIndex;
		m_Monitor_RDObj.TestGameDB_AllocTo_Hall0 = LocalCfg->TestGameDB_AllocTo_Hall0;
		m_Monitor_RDObj.LimitMatch0_Alloc_Game0 = LocalCfg->LimitMatch0_Alloc_Game0;
		//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
		SetExceptionMode(2);

		if (!LocalCfg->WorkPath.IsEmpty())
		{
			if (LocalCfg->WorkPath.Right(1) != UCString("/") && LocalCfg->WorkPath.Right(1) != UCString("\\"))
			{
				MBox("WorkPath路径错误");
				return 0;
			}
			if (!m_LogCallManage.SetOutputPath(LocalCfg->WorkPath))
			{
				MBox("Log输出路径设置失败");
				return 0;
			}
			m_LogCallManage.Start(&m_Service);
		}

		m_FiberInit.Start(0);
		return 1;
	}
	void OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		m_Service.Log(UCString("start Monitor:") + m_Service.Name + UCString("\r\n"));
		m_Service.SetValue(1);		//先对内状态

		m_Service.AppendLocalFile(m_Service.Name + UCString("//CenterKeyRCObj.call"), typeof(CCenterKey_RCObj), typeof(CCenterKeyRCObj), &m_CenterKeyRCObj, UCVF_Guest_Permission_ReadWrite);
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CMonitor_RCObj.call"), typeof(CMonitor_RCObj), typeof(CMonitorService), this);
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CMonitor_RDObj.data"), typeof(CMonitor_RDObj), &m_Monitor_RDObj);
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CMonitor_REAL_RDObj.data"), typeof(CMonitor_REAL_RDObj), &m_Reala_RDObj);
		m_Service.SetAdminKey(ADMIN_PASSWORD);

		CMonitor_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		int bEnableLog = 0;
		if (!LocalCfg->WorkPath.IsEmpty())
		{
			if (m_LogCallManage.AddModule(UCString("Monitor"), m_Service.Name))
			{
				m_Log.SetService(&m_Service);
				int ret = m_Log.Init(m_Service.Name + UCString("//"), UCString("Monitor"), m_Service.Name, ADMIN_PASSWORD);
				if (ret)
				{
					m_Service.Log(UCString("监控服日志初始化失败") + ITOS(ret) + UCString("\r\n"));
				}
				else
				{
					bEnableLog = 1;
				}
			}
		}

		if (LocalCfg->LimitPCU)
			m_Service.Log(UCString("-----全服在线人数限制：") + ITOS(LocalCfg->LimitPCU) + UCString("\r\n"));
		else
			m_Service.Log(UCString("-----全服在线人数限制：不限制\r\n"));
		if (LocalCfg->LimitSinglePCU)
			m_Service.Log(UCString("-----单服在线人数限制：") + ITOS(LocalCfg->LimitSinglePCU) + UCString("\r\n"));
		else
			m_Service.Log(UCString("-----单服在线人数限制：不限制\r\n"));

		if (LocalCfg->LimitGameRoomSize)
			m_Service.Log(UCString("-------单服房间数限制：") + ITOS(LocalCfg->LimitGameRoomSize) + UCString("\r\n"));
		else
			m_Service.Log(UCString("-------单服房间数限制：不限制\r\n"));
		m_Service.Log(UCString("-------设置匹配服数量：") + ITOS(LocalCfg->MatchServiceNum) + UCString("\r\n"));
		m_Service.Log(UCString("       设置房间随机前几：") + ITOS(LocalCfg->MatchRandRoomNum) + UCString("\r\n"));
		if (LocalCfg->TestGameDBIndex >= 0 && LocalCfg->TestGameDB_AllocTo_Hall0 > 0)
		{
			m_Service.Log(UCString("------启用压测0号大厅：GameDB_") + ITOS(LocalCfg->TestGameDBIndex) + UCString("\r\n"));
			m_Service.Log(UCString("      0号大厅压测人数：") + ITOS(LocalCfg->TestGameDB_AllocTo_Hall0) + UCString("\r\n"));
		}
		else
			m_Service.Log(UCString("------启用压测0号大厅：未启用\r\n"));
		if (LocalCfg->LimitMatch0_Alloc_Game0 > 0)
			m_Service.Log(UCString("-----压测Game_0房间数：") + ITOS(LocalCfg->LimitMatch0_Alloc_Game0) + UCString("\r\n\r\n"));
		else
			m_Service.Log(UCString("-----压测Game_0房间数：未启用\r\n\r\n"));

		m_Service.Log(UCString("-----服务器启动完成-----\r\n\r\n"));
		m_Service.SetValue(2);		//可以对外了

		if (bEnableLog == 0)
			return;

		UCTime currTime;
		int iSec = currTime.GetSecond();
		FiberData->Delay((60 - iSec) * 10000);

		while (FiberData->IsValid())
		{
			m_Log.Info(UCString("playnum = ") + ITOS(m_Reala_RDObj.CurrentPCU.Value));

			FiberData->Delay(10000 * 60);
		}
	}
public:
	void Run()
	{
		//初始化
		if (!Init())
			return;
		m_Service.Run(10);
	}
};

#endif	//_log_H_