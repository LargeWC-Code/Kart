#ifndef _log_H_
#define _log_H_
#include "../../common/log_robj.h"

//企业微信机器人消息推送地址
#define WXROBOT_URL  UCString("https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=a67c9293-0382-44c4-b7b7-f3f333e514ad")

//本地配置存档
struct CLog_LocalCfg
{
	UCString				WorkPath;					//日志输出路径

	int						EnablePopWindows;			//报错是否弹窗
	int						EnableMessage;				//报错是否推送消息（目前仅企业微信）
};

//日志服务器
class CLogService :public CLog_RCObj
{
	UCEConfig<CLog_LocalCfg>		m_LocalCfg;			//配置
	UCVF_Service					m_Service;			//服务

	UCLogCallManage					m_LogCallManage;	//日志管理
	UCFiber							m_FiberInit;		//连接初始化
	UCCUrl							m_CUrl;				//Http服务

	UCStringIntMap					m_mapModule;		//记录服务器启动
	int								m_bChange;			//列表是否有变动
	ucDWORD							m_dwChangeTime;		//列表变动时间
public:
	//注册日志模块
	UCString AddModule(const UCString& strModName, const UCString& strServerName, int iPID, int& bEnablePosWindows)
	{
		m_LogCallManage.AddModule(strModName, strServerName);
		m_mapModule.Add(strServerName, iPID);
		m_bChange = 1;
		m_dwChangeTime = UCGetTime();
		bEnablePosWindows = m_LocalCfg.GetData()->EnablePopWindows;
		return UCString("成功");
	}
public:
	CLogService()
	{
		m_bChange = 0;
		m_FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		m_LogCallManage.OnFatal = UCEvent(this, OnMyFatal);
	}
	~CLogService()
	{
	}
	int Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("game_log"));
		if (!m_LocalCfg.LoadFromXML(UCString("game_log/game_log.xml")))
		{
			CLog_LocalCfg* LocalCfg = m_LocalCfg.GetData();
			LocalCfg->WorkPath = "D:\\Log\\GameLog\\";
			LocalCfg->EnablePopWindows = 1;
			LocalCfg->EnableMessage = 0;
			m_LocalCfg.SaveToXML(UCString("game_log/game_log.xml"));
		}
		CLog_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->WorkPath.Right(1) != UCString("/") && LocalCfg->WorkPath.Right(1) != UCString("\\"))
		{
			WBox(LocalCfg->WorkPath);
			MBox("WorkPath路径错误");
			return 0;
		}
		if (!m_LogCallManage.SetOutputPath(LocalCfg->WorkPath))
		{
			MBox("Log输出路径设置失败");
			return 0;
		}
		m_Service.Name = UCGetApp()->Name;
		UCGetSystemExt()->SetWindowCaption(UCString("GameLog/") + m_Service.Name);
		//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
		SetExceptionMode(2);

		m_LogCallManage.Start(&m_Service);
		m_FiberInit.Start(0);
		return 1;
	}
	//异常处理
	void OnMyFatal(UCObject*, UCEventArgs* e)
	{
		if (m_LocalCfg.GetData()->EnableMessage == 0)
			return;
		UCLogCallArgs* pArg = (UCLogCallArgs*)e;
		//UCString strPost = "{\"msgtype\": \"text\",\"text\" : {\"content\": \"";
		//strPost += pArg->Log;
		//strPost += UCString("\",\"mentioned_list\" : [\"陈子君\", \"@all\"]}}");
		UCString servername = UCGetApp()->Owner;
		UCString strPost = "{\"msgtype\": \"markdown\",\"markdown\" : {\"content\": \"";
		strPost += UCString("# 监测到<font color=\\\"warning\\\">") + servername + UCString("</font>异常，请相关同事注意\n");
		strPost += UCString(">时间：<font color=\\\"comment\\\">") + pArg->Time.Format("%Y-%m-%d %H:%M:%S") + UCString("</font>\\n");
		strPost += UCString(">模块：<font color=\\\"comment\\\">") + pArg->ModuleName + UCString("</font>\\n");
		strPost += UCString(">实例：<font color=\\\"comment\\\">") + pArg->FileName + UCString("</font>\\n");
		strPost += UCString(">内容：<font color=\\\"warning\\\">") + pArg->Log + UCString("</font>");
		strPost += UCString("\"}}");
		m_CUrl.HttpPost(WXROBOT_URL, strPost, UCEvent());
	}
	void OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		m_Service.Log(UCString("start GameLog:") + m_Service.Name + UCString("\r\n"));

		m_Service.AppendLocalFile(m_Service.Name + UCString("//CLog_RCObj.call"), typeof(CLog_RCObj), typeof(CLogService), this);
		m_Service.SetAdminKey(ADMIN_PASSWORD);

		m_Service.Log(UCString("-----服务器启动完成-----\r\n\r\n"));

		CLog_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->EnableMessage == 0)
			return;
		while (FiberData->IsValid())
		{
			if (m_bChange)
			{
				//10秒后，推送所有启动服消息
				if (UCGetTime() - m_dwChangeTime > 10000 * 10)
				{
					UCString servername = UCGetApp()->Owner;
					UCString strPost = "{\"msgtype\": \"markdown\",\"markdown\" : {\"content\": \"";
					strPost += UCString("# 游戏<font color=\\\"warning\\\">") + servername + UCString("</font>开服启动提醒\n");
					strPost += UCString(">时间：<font color=\\\"comment\\\">") + UCTime().Format("%Y-%m-%d %H:%M:%S") + UCString("</font>（共") + ITOS(m_mapModule.GetSize()) + UCString("个进程）\\n");
					if (LocalCfg->EnablePopWindows)
						strPost += UCString(">异常弹窗：<font color=\\\"info\\\">开启（正式服请关闭）</font>\\n");
					else
						strPost += UCString(">异常弹窗：<font color=\\\"comment\\\">关闭</font>\\n");
					strPost += UCString(">进程ID：<font color=\\\"comment\\\">模块名</font>\\n");
					for (int i = m_mapModule.GetSize() - 1; i >= 0; --i)
					{
						strPost += UCString(">") + ITOS(m_mapModule.GetValueAt(i),UCString("%-6d")) + UCString(": <font color=\\\"comment\\\">") + m_mapModule.GetKeyAt(i) + UCString("</font>\\n");
					}
					strPost += UCString("\"}}");
					m_CUrl.HttpPost(WXROBOT_URL, strPost, UCEvent());
					m_bChange = 0;
				}
			}
			FiberData->Delay(10000);
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