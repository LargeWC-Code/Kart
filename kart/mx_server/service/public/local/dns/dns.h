#ifndef _dns_server_H_
#define _dns_server_H_

//给一个默认密码，日志和工具使用的
#define ADMIN_PASSWORD 123456789

//本地配置存档
struct CDns_LocalCfg
{
	int						Port;						//端口
	UCString				HostFile;					//域名列表文件路径（csv格式）
};

//监控服务器
class CDnsService
{
	UCEConfig<CDns_LocalCfg>		m_LocalCfg;			//配置
	UCRContainer_NET				m_Container;			//服务

	UCFiber							m_FiberInit;		//连接初始化
public:
	CDnsService()
	{
		m_FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
	}
	~CDnsService()
	{
	}
	int Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("dns"));
		if (!m_LocalCfg.LoadFromXML(UCString("dns/dns.xml")))
		{
			CDns_LocalCfg* LocalCfg = m_LocalCfg.GetData();
			LocalCfg->Port = 8888;
			LocalCfg->HostFile = "../../csv/host.csv";
			m_LocalCfg.SaveToXML(UCString("dns/dns.xml"));
		}
		CDns_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		UCString strExt = LocalCfg->HostFile.Right(4);
		strExt.MakeLower();
		if (strExt != UCString(".csv"))
		{
			WBox(UCString("HOST文件路径错误：") + LocalCfg->HostFile);
			return 0;
		}
		UCCsv csv;
		if (!csv.ReadFromFile(LocalCfg->HostFile))
		{
			WBox(UCString("打开HOST文件失败：") + LocalCfg->HostFile);
			return 0;
		}
		UCString strHost, strAddress;
		int iStaticValue = 0;
		int iSize = csv.GetRowSize();
		for (int i = 2; i <= iSize; ++i)
		{
			if (!csv.GetString(i, 1, strHost) || strHost.IsEmpty())
				continue;
			if (!csv.GetString(i, 2, strAddress) || strAddress.IsEmpty())
				continue;
			if (!csv.GetInt(i, 3, iStaticValue))
				continue;
			m_Container.AddHost(strHost, strAddress, iStaticValue);
		}
		//m_Container.AddHost(UCString("c"), UCString("GameHall_0"));
		//m_Container.AddHost(UCString("d"), UCString("Game_0"));
		//m_Container.AddHost(UCString("cc"), UCString("dom://c"));
		//m_Container.AddHost(UCString("dd"), UCString("dom://d"));

		m_Container.Name = UCGetApp()->Name;
		UCGetSystemExt()->SetWindowCaption(UCString("Dns/") + m_Container.Name);
		//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
		SetExceptionMode(2);

		if (LocalCfg->Port > 0)
			m_Container.StartTCP(LocalCfg->Port, 4);
		m_Container.EnableTCPClient(1, 4);
		m_Container.SetTCPMaxTime(60 * 60 * 24);
		m_Container.SetTCPMerge(1, 1);

		m_FiberInit.Start(0);
		return 1;
	}
	void OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		m_Container.Log(UCString("start Dns:") + m_Container.Name + UCString("\r\n"));

		m_Container.SetAdminKey(ADMIN_PASSWORD);

		m_Container.Log(UCString("-----服务器启动完成-----\r\n\r\n"));
	}
public:
	void Run()
	{
		//初始化
		if (!Init())
			return;
		m_Container.Run(10);
	}
};

#endif	//_log_H_