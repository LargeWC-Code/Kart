#ifndef _gate_H_
#define _gate_H_

#include "../../common/gate_robj.h"
#include "../../common/monitor_robj.h"
#include "../../common/game_center_robj.h"
#include "../../common/log_robj.h"

#define GATETYPE_TCP	0
#define GATETYPE_WS		1
#define GATETYPE_WSS	2


//本地配置存档
struct CGate_LocalCfg
{
	UCString				UserName;					//连接中心服的账号
	UCString				Password;					//连接中心服的密码  

	ucINT						Public;						//是否对外，否则仅路由

	ucINT						Type;						//0 TCP 1 WS 2 WSS
	ucINT						Port;						//端口
	UCString				CerFile;					//wss用证书
	UCString				KeyFile;					//wss用证书密钥

	UCString				DnsURL;						//DNS服路径//结束
};

//游戏网关服务器
class CGateService : public CGate_RCObj
{
private:
	UCEConfig<CGate_LocalCfg>			m_LocalCfg;
	UCVF_ServiceNet						m_Service;

	CMonitor_RDObj						m_Monitor_RDObj;		//监控服
	CMonitor_RCObj						m_Monitor_RCObj;
	CGame_Center_RCObj					m_Center_RCObj;

	UCIntStringMap						m_MapAccount_Open;			//对外Account
	UCIntIntMap							m_MapAccount_UnOpen;		//对内Account
	CAccount_Info_RDObj					m_Account_Info_RDObj;		//所有的Account信息

	ucINT64								m_AdminKey;					//管理员密钥
	UCFiber								m_FiberInit;				//连接初始化

	ucINT									m_Index;
	ucINT									m_NetType;					//我的服务类型
	ucINT									m_MonitorIndex;				//监控的注册序号
	ucDWORD								m_MonitorKey;				//监控的注册Key
	ucINT									m_LimitPCU;					//限制全服PCU（0为不限制）
	ucINT									m_LimitSinglePCU;			//限制单服PCU（0为不限制）
	ucINT									m_CurrentPCU;				//全服PCU
	UCVF_DOMArray						m_aryGameAccountDom;		//域名解析出的有效游戏账号登录服
public:
	//新的登录方式，第1步，获取一个账号服地址，附带版本校验和维护公告
	UCString	GetPlatformAccountUrlInfo(ucINT iChannelID, UCString& AccoutUrl, ucINT ClientVerNum, ucINT& ServerVerNum)
	{
		if (iChannelID < 0 || iChannelID >= _CHANNEL_MAX)
			return UCString("非法渠道");
		CGate_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->Public == 0)
			return UCString("非法请求");
		ucINT iGameVerEnd = m_Monitor_RDObj.GameVER[iChannelID].Value;
		if (m_Monitor_RDObj.GameVER[iChannelID].Value < m_Monitor_RDObj.GameVEREnd[iChannelID].Value)
		{
			iGameVerEnd = m_Monitor_RDObj.GameVEREnd[iChannelID].Value;
		}
		if (ClientVerNum < m_Monitor_RDObj.GameVER[iChannelID].Value)
		{
			ServerVerNum = m_Monitor_RDObj.GameVER[iChannelID].Value;
			return UCString("版本不匹配");
		}
		else if (ClientVerNum > iGameVerEnd)
		{
			ServerVerNum = iGameVerEnd;
			return UCString("版本不匹配");
		}
		ServerVerNum = ClientVerNum;
		if (m_Monitor_RDObj.MaintainTag[iChannelID].Value & 1)
		{
			AccoutUrl = m_Monitor_RDObj.GameNote[iChannelID].Value;
			return UCString("维护中");
		}
		ucINT iPCU = m_Service.GetNetDataCount(m_NetType);
		if (m_LimitSinglePCU > 0 && iPCU >= m_LimitSinglePCU)
			return UCString("服务器满");
		if (m_LimitPCU > 0 && m_CurrentPCU + iPCU >= m_LimitPCU)
			return UCString("服务器满");
		//优先通过域名找到合法的服务器，简单动态均衡
		if (m_aryGameAccountDom.GetSize() > 0)
		{
			ucINT randIndex = randint(0, m_aryGameAccountDom.GetSize() - 1);
			AccoutUrl = m_aryGameAccountDom.GetAt(randIndex)->OutName;
			return UCString("成功");
		}
		if (m_Account_Info_RDObj.Linked == 0 || m_MapAccount_Open.GetSize() <= 0)
			return UCString("Gate没有连接账号信息");
		ucINT iAccountIndex = randint(0, m_MapAccount_Open.GetSize() - 1);
		AccoutUrl = UCString("dom://GameAccount_") + ITOS(iAccountIndex);
		return UCString("成功");
	}
public:
	CGateService()
	{
		m_AdminKey = 0;
		m_NetType = 7;	//eWssServer
		m_MonitorIndex = -1;
		m_MonitorKey = 0;
		m_LimitPCU = 0;
		m_LimitSinglePCU = 0;
		m_CurrentPCU = 0;
		m_FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		m_Service.OnClose = UCEvent(this, OnMyServiceClose);
	}
	~CGateService()
	{
	}
	ucINT Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("gate"));
		if (!m_LocalCfg.LoadFromXML(UCString("gate/gate.xml")))
		{
			CGate_LocalCfg* LocalCfg = m_LocalCfg.GetData();
			LocalCfg->UserName = "GameGate";
			LocalCfg->Password = "123456";
			LocalCfg->Public = 1;
			LocalCfg->Type = GATETYPE_WS;
			LocalCfg->Port = 9000;		//默认端口
			LocalCfg->CerFile = "3865703__jumpw.com.pem";
			LocalCfg->KeyFile = "3865703__jumpw.com.key";
			LocalCfg->DnsURL = "Dns//";
			m_LocalCfg.SaveToXML(UCString("gate/gate.xml"));
		}
		CGate_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->Type < GATETYPE_TCP || LocalCfg->Type > GATETYPE_WSS)
		{
			WBox(UCString("网络类型错误：") + ITOS(LocalCfg->Type));
			return 0;
		}

		if (!LocalCfg->DnsURL.IsEmpty())
		{
			m_Service.SetTTL(1);
			if (LocalCfg->DnsURL.Right(2) != UCString("//"))
			{
				WBox(UCString("DnsURL路径错误：") + LocalCfg->DnsURL);
				return 0;
			}
			if (!m_Service.AddDNS(LocalCfg->DnsURL))
			{
				m_Service.Log(UCString("添加域名服务器失败：") + LocalCfg->DnsURL + UCString("\r\n"));
				WBox(UCString("添加域名服务器失败：") + LocalCfg->DnsURL);
				return 0;
			}
			m_Service.Log(UCString("添加域名服务器成功：") + LocalCfg->DnsURL + UCString("\r\n"));
		}

		//也可以添加本机域名
		//m_Service.AddHost(UCString("a"), UCString("GameHall_0"));
		//m_Service.AddHost(UCString("b"), UCString("Game_0"));
		//m_Service.AddHost(UCString("aa"), UCString("dom://a"));
		//m_Service.AddHost(UCString("bb"), UCString("dom://b"));

		m_Index = UCGetApp()->Index;
		if (m_Index >= 0)
			m_Service.Name = UCGetApp()->Name + ITOS(m_Index, UCString("_%d"));
		else
		{
			m_Service.Name = UCGetApp()->Name;
			m_Index = 0;
		}
		UCGetSystemExt()->SetWindowCaption(UCString("Game_Gate/") + m_Service.Name);
		//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
		SetExceptionMode(2);

		if (LocalCfg->Public)
		{
			m_Service.SetRouteOnly(ucTRUE);
		}

		m_FiberInit.Start(0);
		return 1;
	}
	ucVOID OnMyServiceClose(UCObject*, UCEventArgs*)
	{
		m_Monitor_RCObj.UnRegisterGate(m_MonitorIndex, m_MonitorKey);
	}
	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		m_Service.Log(UCString("start GameGate:") + m_Service.Name + UCString("\r\n"));
		m_Service.SetValue(1);		//先对内状态

		//暂停一段时间，让Center启动完成
		//FiberData->Delay(100);

		CGate_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->Type == GATETYPE_TCP)
		{
			if (!m_Service.StartTCP(LocalCfg->Port + m_Index, 4))
			{
				WBox(UCString("启动TCP服务失败：") + ITOS(LocalCfg->Port + m_Index));
			}
			m_Service.EnableTCPClient(1, 4);
			m_Service.SetTCPMaxTime(60 * 60 * 24);
			m_Service.SetTCPMerge(1, 1);
			m_NetType = 1;		//eTcpServer
		}
		else if (LocalCfg->Type == GATETYPE_WS)
		{
			if (!m_Service.StartWS(LocalCfg->Port + m_Index, 8))
			{
				WBox(UCString("启动WS服务失败：") + ITOS(LocalCfg->Port + m_Index));
			}
			m_Service.SetWSServerMerge(20, 1);
			m_Service.EnableTCPClient(1, 8);
			m_Service.SetTCPMaxTime(60 * 60 * 24);
			m_Service.SetTCPMerge(1, 1);
			m_NetType = 5;		//eWsServer
		}
		else if (LocalCfg->Type == GATETYPE_WSS)
		{
			if (!m_Service.StartWSS(LocalCfg->Port + m_Index, LocalCfg->CerFile, LocalCfg->KeyFile, 8))
			{
				WBox(UCString("启动WSS服务失败：") + ITOS(LocalCfg->Port + m_Index));
			}
			m_Service.SetWSServerMerge(20, 1);
			m_Service.EnableTCPClient(1, 8);
			m_Service.SetTCPMaxTime(60 * 60 * 24);
			m_Service.SetTCPMerge(1, 1);
			m_NetType = 7;		//eWssServer
		}

		//连接游戏中心服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_Center_RCObj.call"), typeof(CGame_Center_RCObj), &m_Center_RCObj);
		if (LocalCfg->Public)
		{
			//添加本地服务
			m_Service.AppendLocalFile(m_Service.Name + UCString("//CGate_RCObj.call"), typeof(CGate_RCObj), typeof(CGateService), this, UCVF_Guest_Permission_ReadWrite);
			if (LocalCfg->DnsURL.IsEmpty())
			{
				m_Service.Log(UCString("-----服务器启动完成 纯GATE-----\r\n\r\n"));
				m_Service.SetValue(2);		//可以对外了
				m_Service.SetTTL(DEF_DNS_TTL);
				return;
			}
			//监控服
			m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CMonitor_RCObj.call"), typeof(CMonitor_RCObj), &m_Monitor_RCObj);
			m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CMonitor_RDObj.data"), typeof(CMonitor_RDObj), &m_Monitor_RDObj);
			//连接中心服务的Account列表
			m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CAccount_Info_RDObj.data"), typeof(CAccount_Info_RDObj), &m_Account_Info_RDObj);

			for (ucINT i = 0; i < ACCOUNT_MAX; i++)
			{
				m_Account_Info_RDObj.Account_Info[i].BindOnSet(UCEvent(this, OnAccount_InfoOnChanged));
			}
			m_Account_Info_RDObj.OnReset = UCEvent(this, OnMyAccountInfoReset);

			m_Service.Log(UCString("开始连接监控服务器...\r\n"));
			while (FiberData->IsValid())
			{
				ucINT linkresult = m_Monitor_RCObj.Link(UCString("dom://Monitor//CMonitor_RCObj.call"), ADMIN_PASSWORD);
				if (linkresult == 0)
					break;
				//m_Service.Log(UCString("监控服连接失败") + ITOS(linkresult) + UCString("\r\n"));
				FiberData->Every(1000);
			}
			while (FiberData->IsValid())
			{
				ucINT linkresult = m_Monitor_RDObj.StableLink(UCString("dom://Monitor//CMonitor_RDObj.data"), ADMIN_PASSWORD);
				if (linkresult == 0)
					break;
				//m_Service.Log(UCString("监控服连接失败") + ITOS(linkresult) + UCString("\r\n"));
				FiberData->Every(1000);
			}
			m_Service.Log(UCString("监控连接成功\r\n"));
			ucINT bEnablePosWindows = 1;
			m_Monitor_RCObj.RegisterGate(m_MonitorIndex, m_MonitorKey, m_LimitPCU, m_LimitSinglePCU, m_CurrentPCU);
			if (m_MonitorIndex < 0 || m_LimitPCU < 0)
			{
				m_Service.Log(UCString("注册监控服失败\r\n"));
			}
			else
			{
				m_Service.Log(UCString("注册监控服成功\r\n"));
			}
		}

		m_Service.Log(UCString("开始连接中心服务器...\r\n"));
		while (FiberData->IsValid())
		{
			ucINT linkresult = m_Center_RCObj.Link(UCString("dom://GameCenter//CGame_Center_RCObj.call"));
			if (linkresult == 0)
				break;
			m_Service.Log(UCString("中心服连接失败") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("中心服连接成功\r\n"));
		UCString Result = m_Center_RCObj.RefeshService(LocalCfg->UserName, LocalCfg->Password, m_AdminKey);
		if (Result != "成功")
		{
			m_Service.Log(UCString("中心服验证失败，请检查配置后重启服务 : ") + Result + UCString("\r\n"));
			return;
		}
		m_Service.Log(UCString("验证成功\r\n"));
		m_Service.SetAdminKey(m_AdminKey);

		if (LocalCfg->Public)
		{
			while (FiberData->IsValid())
			{
				ucINT linkresult = m_Account_Info_RDObj.StableLink(UCString("dom://GameCenter//CAccount_Info_RDObj.data"), m_AdminKey);
				if (linkresult == 0)
					break;
				m_Service.Log(UCString("平台的Account列表连接失败:") + ITOS(linkresult) + UCString("\r\n"));
				FiberData->Every(1000);
			}
			m_Service.Log(UCString("平台的Account列表连接成功\r\n"));
		}

		m_Service.Log(UCString("-----服务器启动完成-----\r\n\r\n"));
		m_Service.SetValue(2);		//可以对外了
		m_Service.SetTTL(DEF_DNS_TTL);

		if (LocalCfg->Public)
		{
			while (FiberData->IsValid())
			{
				m_Monitor_RCObj.UploadGateInfo(m_MonitorIndex, m_MonitorKey, m_Service.GetNetDataCount(m_NetType), m_LimitPCU, m_LimitSinglePCU, m_CurrentPCU);
				m_Service.GetHostByNames(UCString("GameAccount"), m_aryGameAccountDom);

				FiberData->Delay(10000);
			}
		}
	}
	ucVOID OnMyAccountInfoReset(UCObject*, UCEventArgs* e)
	{
		m_MapAccount_Open.RemoveAll();
		m_MapAccount_UnOpen.RemoveAll();
		for (ucINT i = 0; i < ACCOUNT_MAX; i++)
		{
			if (m_Account_Info_RDObj.Account_Info[i].Url.IsEmpty())
				continue;
			if (m_Account_Info_RDObj.Account_Info[i].Open)
			{
				m_MapAccount_Open.Add(i, m_Account_Info_RDObj.Account_Info[i].Url);
			}
			else
			{
				m_MapAccount_UnOpen.Add(i, 0);
			}
		}
	}
	ucVOID OnAccount_InfoOnChanged(UCObject*, UCEventArgs* e)
	{
		UCVF_PropertyBaseCmd_Args* Args = (UCVF_PropertyBaseCmd_Args*)e;

		CAccount_Info* pInfo = (CAccount_Info*)Args->Data;
		ucINT Index = m_Account_Info_RDObj.Account_Info[0].GetArrayIndex(pInfo);
		m_Service.Log(UCString("收到服务消息 Account[") + ITOS(Index) + UCString("].URL=") + pInfo->Url + UCString("\r\n"));

		//如果服务地址为空，不认为是合法的地址
		if (pInfo->Url.IsEmpty())
		{
			m_MapAccount_Open.Remove(Index);
			m_MapAccount_UnOpen.Add(Index, 0);
			return;
		}

		if (pInfo->Open)
		{
			m_MapAccount_Open.Add(Index, pInfo->Url);
			m_MapAccount_UnOpen.Remove(Index);
		}
		else
		{
			m_MapAccount_Open.Remove(Index);
			m_MapAccount_UnOpen.Add(Index, 0);
		}
	}
public:
	ucVOID Run()
	{
		//初始化
		if (!Init())
			return;
		m_Service.Run(10);
	}
};

#endif	//_gate_H_