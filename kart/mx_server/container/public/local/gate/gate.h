#ifndef _gate_H_
#define _gate_H_

#include "../../common/game_gatebase.h"
#include "../../common/game_centerbase.h"
#include "../../common/game_monitorbase.h"

//本地配置存档
struct UCGameGateInfo
{
	ucINT					Index;
	UCString				Password;					//连接中心服的密码  

	ucINT					Mode;						//0 Mirror 1 Route

	ucINT					Type;						//0 TCP 1 WS 2 WSS
	ucINT					Port;						//端口
	UCString				CerFile;					//wss用证书
	UCString				KeyFile;					//wss用证书密钥
};

class UCRObjGameGateLocal : public UCRObjGameGate
{
public:
	UCString	GetAccountID(ucINT& AccountID)
	{
		UCGameGateInfo* GateInfo = GameGateCfg.GetData();
// 		if (GateInfo->Mode == GATEMODE_MIRROR)
// 			return UCString("非法请求");

		if (MapAccount_Valid.GetSize() == 0)
			return UCString("无有效账号服务器");

		ucINT Pos = randint(0, MapAccount_Valid.GetSize() - 1);

		AccountID = MapAccount_Valid.GetValueAt(Pos);
		return UCString("succeed");
	}
	UCString	GetHomeID(ucINT& HomeID)
	{
		UCGameGateInfo* GateInfo = GameGateCfg.GetData();
// 		if (GateInfo->Mode == GATEMODE_MIRROR)
// 			return UCString("非法请求");

		if (MapHome_Valid.GetSize() <= 0)
			return UCString("没有有效的Home");

		ucINT Pos = randint(0, MapHome_Valid.GetSize() - 1);

		HomeID = MapHome_Valid.GetValueAt(Pos);
		return UCString("succeed");
	}
	UCString	GetMatchID(ucINT& MatchID)
	{
		MatchID = 0;
		return UCString("succeed");
	}
public:
	UCEApp*						App;
	UCEConfig<UCGameGateInfo>	GameGateCfg;
	UCRContainer_NET			RContainer_NET;

	UCRObjGameCenter			RObjGameCenter;
	UCRObjGameMonitor			RObjGameMonitor;

	UCIntIntMap					MapAccount_Valid;
	UCIntIntMap					MapAccount_Unvalid;
	UCRObjGameAccount_Infos		RObjGameAccount_Infos;

	UCIntIntMap					MapHome_Valid;
	UCIntIntMap					MapHome_Unvalid;
	UCRObjGameHome_Infos		RObjGameHome_Infos;
public:
	//通用Key
	ucINT64						AdminKey;

	ucDWORD						Monitor_Key;
	ucINT						Monitor_PCU_MaxTotal;
	ucINT						Monitor_PCU_MaxChannel;
	ucINT						Monitor_PCU_Current;
public:
	UCFiber						FiberInit;
public:
	ucVOID OnAccountInfo_Reset(UCObject*, UCEventArgs*)
	{
		MapAccount_Valid.RemoveAll();
		MapAccount_Unvalid.RemoveAll();
		for (ucINT i = 0; i < ACCOUNT_MAX; i++)
		{
			if (RObjGameAccount_Infos.GameAccount_Infos[i].Valid)
				MapAccount_Valid.Add(i, i);
			else
				MapAccount_Unvalid.Add(i, i);
		}
	}
	ucVOID OnAccountInfo_Changed(UCObject*, UCEventArgs* e)
	{
		UCRPropertyArgs* Args = (UCRPropertyArgs*)e;

		UCGameAccount_Info* GameAccount_Info = (UCGameAccount_Info*)Args->Data;
		ucINT Index = RObjGameAccount_Infos.GameAccount_Infos[0].GetArrayIndex(GameAccount_Info);
		RContainer_NET.Log(UCString("收到服务消息 Account[") + ITOS(Index) + UCString("]\r\n"));

		if (GameAccount_Info->Valid == ucFALSE)
		{
			MapAccount_Valid.Remove(Index);
			MapAccount_Unvalid.Add(Index, Index);
		}
		else
		{
			MapAccount_Valid.Add(Index, Index);
			MapAccount_Unvalid.Remove(Index);
		}
	}
	ucVOID OnHomeInfo_Reset(UCObject*, UCEventArgs*)
	{
		MapHome_Valid.RemoveAll();
		MapHome_Unvalid.RemoveAll();
		for (ucINT i = 0; i < HOME_MAX; i++)
		{
			if (RObjGameHome_Infos.GameHome_Infos[i].Valid)
				MapHome_Valid.Add(i, i);
			else
				MapHome_Unvalid.Add(i, i);
		}
	}
	ucVOID OnHomeInfo_Changed(UCObject*, UCEventArgs* e)
	{
		UCRPropertyArgs* Args = (UCRPropertyArgs*)e;

		UCGameHome_Info* GameHome_Info = (UCGameHome_Info*)Args->Data;
		ucINT Index = RObjGameHome_Infos.GameHome_Infos[0].GetArrayIndex(GameHome_Info);
		RContainer_NET.Log(UCString("收到服务消息 Home[") + ITOS(Index) + UCString("]\r\n"));

		if (GameHome_Info->Valid == ucFALSE)
		{
			MapHome_Valid.Remove(Index);
			MapHome_Unvalid.Add(Index, Index);
		}
		else
		{
			MapHome_Valid.Add(Index, Index);
			MapHome_Unvalid.Remove(Index);
		}
	}
public:
	UCRObjGameGateLocal()
	{
		App = ucNULL;
		FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		RContainer_NET.OnEnd = UCEvent(this, OnRContainer_End);
	}
	~UCRObjGameGateLocal()
	{
	}
	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		if (App->Index < 0)
			RContainer_NET.Log(UCString("start:") + App->Name + UCString("\r\n"));
		else
			RContainer_NET.Log(UCString("start:") + App->Name + ITOS(App->Index, UCString(":%d")) + UCString("\r\n"));

		UCGameGateInfo* GateInfo = GameGateCfg.GetData();
		ucINT AppIndex = App->Index;
		if (AppIndex < 0)
			AppIndex = 0;
		if (GateInfo->Type == GATETYPE_TCP)
		{
			if (RContainer_NET.TCP_Run(4))
			{
				if (!RContainer_NET.TCP_Add(GateInfo->Port + AppIndex))
					WBox(UCString("启动TCP服务失败：") + ITOS(GateInfo->Port + AppIndex));
				RContainer_NET.TCP_SetMaxTime(60 * 60 * 24);
			}
		}
		else if (GateInfo->Type == GATETYPE_WS)
		{
			if (RContainer_NET.WS_Run(4))
			{
				if (!RContainer_NET.WS_Add(GateInfo->Port + AppIndex))
					WBox(UCString("启动WS服务失败：") + ITOS(GateInfo->Port + AppIndex));
			}
		}
		else if (GateInfo->Type == GATETYPE_WSS)
		{
			if (RContainer_NET.WS_Run(4))
			{
				if (!RContainer_NET.WS_AddSafe(GateInfo->Port + AppIndex, GateInfo->CerFile, GateInfo->KeyFile))
					WBox(UCString("启动WSS服务失败：") + ITOS(GateInfo->Port + AppIndex));
			}
		}

		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));
		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameGate.obj"), typeof(UCRObjGameGate), typeof(UCRObjGameGateLocal), this, ucTRUE);

		RContainer_NET.AppendClient(LocalUrl + UCString("//Local/UCRObjGameCenter.obj"), typeof(UCRObjGameCenter), &RObjGameCenter);
		//if (GateInfo->Mode == GATEMODE_ROUTE)
		{
			RContainer_NET.AppendClient(LocalUrl + UCString("//Local/UCRObjGameAccount_Infos.obj"), typeof(UCRObjGameAccount_Infos), &RObjGameAccount_Infos);
			for (ucINT i = 0; i < ACCOUNT_MAX; i++)
				RObjGameAccount_Infos.GameAccount_Infos[i].AppendEvent(UCEvent(this, OnAccountInfo_Changed));
			RObjGameAccount_Infos.OnReset = UCEvent(this, OnAccountInfo_Reset);

			RContainer_NET.AppendClient(LocalUrl + UCString("//Local/UCRObjGameHome_Infos.obj"), typeof(UCRObjGameHome_Infos), &RObjGameHome_Infos);
			for (ucINT i = 0; i < HOME_MAX; i++)
				RObjGameHome_Infos.GameHome_Infos[i].AppendEvent(UCEvent(this, OnHomeInfo_Changed));
			RObjGameHome_Infos.OnReset = UCEvent(this, OnHomeInfo_Reset);

//			RContainer_NET.AppendObject(LocalUrl + UCString("//Local/UCRObjGameMonitor.obj"), typeof(UCRObjGameMonitor), &RObjGameMonitor);
// 			RContainer_NET.Log(UCString("开始连接监控容器...\r\n"));
// 			while (FiberData->IsValid())
// 			{
// 				ucINT Code = RObjGameMonitor.Link(UCString("sm://monitor:0//UCRObjGameMonitor.obj"), DEFAULT_PASSWORD);
// 				if (Code == 0)
// 					break;
// 				//RContainer_NET.Log(UCString("监控容器连接失败") + ITOS(Code) + UCString("\r\n"));
// 				FiberData->Every(1000);
// 			}
// 			RContainer_NET.Log(UCString("监控连接succeed\r\n"));
// 
// 			RObjGameMonitor.Gate_Register(Index, Monitor_Key, Monitor_PCU_MaxTotal, Monitor_PCU_MaxChannel, Monitor_PCU_Current);
// 			if (Index < 0 || Monitor_PCU_MaxTotal < 0)
// 				RContainer_NET.Log(UCString("注册监控容器失败\r\n"));
// 			else
// 				RContainer_NET.Log(UCString("注册监控容器succeed\r\n"));
		}

		RContainer_NET.Log(UCString("开始连接中心容器...\r\n"));
		while (FiberData->IsValid())
		{
			ucINT Code = RObjGameCenter.Link(UCString("sm://center:0//UCRObjGameCenter.obj"));
			if (Code == 0)
				break;
			RContainer_NET.Log(UCString("中心容器连接失败") + ITOS(Code) + UCString("\r\n"));
			FiberData->Every(10000);
		}
		RContainer_NET.Log(UCString("中心容器连接succeed\r\n"));
		UCString Result = RObjGameCenter.GameGate_Update(GateInfo->Index, GateInfo->Password, AdminKey);
		if (Result != "succeed")
		{
			RContainer_NET.Log(UCString("中心容器验证失败，请检查配置后重启容器 : ") + Result + UCString("\r\n"));
			return;
		}
		RContainer_NET.Log(UCString("验证succeed\r\n"));

		RContainer_NET.SetAdminKey(AdminKey);

		//if (GateInfo->Mode == GATEMODE_ROUTE)
		{
			while (FiberData->IsValid())
			{
				ucINT Code = RObjGameAccount_Infos.Link(UCString("sm://center:0//UCRObjGameAccount_Infos.obj"), UCROBJECT_LINK_STABLE, AdminKey);
				if (Code == 0)
					break;
				RContainer_NET.Log(UCString("平台的Account列表连接失败:") + ITOS(Code) + UCString("\r\n"));
				FiberData->Every(1000);
			}
			RContainer_NET.Log(UCString("平台的Account列表连接succeed\r\n"));

			while (FiberData->IsValid())
			{
				ucINT Code = RObjGameHome_Infos.Link(UCString("sm://center:0//UCRObjGameHome_Infos.obj"), UCROBJECT_LINK_STABLE, AdminKey);
				if (Code == 0)
					break;
				RContainer_NET.Log(UCString("平台的Home列表连接失败:") + ITOS(Code) + UCString("\r\n"));
				FiberData->Every(1000);
			}
			RContainer_NET.Log(UCString("平台的Home列表连接succeed\r\n"));
		}

		RContainer_NET.Log(UCString("当前容器初始化完毕\r\n"));

// 		if (GateInfo->Mode == GATEMODE_ROUTE)
// 		{
// 			while (FiberData->IsValid())
// 			{
// 				//RObjGameMonitor.Gate_Upload(Index, Monitor_Key, RContainer_NET.GetNetDataCount(m_NetType), m_LimitPCU, m_LimitSinglePCU, m_CurrentPCU);
// 				//RContainer_NET.GetHostByNames(UCString("GameAccount"), m_aryGameAccountDom);
// 
// 				FiberData->Delay(10000);
// 			}
// 		}
	}
	ucVOID OnRContainer_End(UCObject* Object, UCEventArgs*)
	{
		//RObjGameMonitor.UnRegisterGate(m_MonitorIndex, m_MonitorKey);
	}
	ucINT Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("gate"));
		if (!GameGateCfg.LoadFromXML(UCString("gate/gate.xml")))
		{
			UCGameGateInfo* GateInfo = GameGateCfg.GetData();
			GateInfo->Index = 0;
			GateInfo->Password = "123456";
			GateInfo->Mode = GATEMODE_MIRROR;
			GateInfo->Type = GATETYPE_WS;
			GateInfo->Port = 6800;		//默认端口
			GateInfo->CerFile = "xxxx_magicx.com.pem";
			GateInfo->KeyFile = "xxxx_magicx.com.key";
			GameGateCfg.SaveToXML(UCString("gate/gate.xml"));
		}
		UCGameGateInfo* GateInfo = GameGateCfg.GetData();
		if (GateInfo->Type < GATETYPE_TCP || GateInfo->Type > GATETYPE_WSS)
		{
			WBox(UCString("网络类型错误：") + ITOS(GateInfo->Type));
			return 0;
		}

		App = UCGetApp();
		if (App->Name.IsEmpty())
			App->Name = UCString("gate");

		UCGetSystemExt()->SetWindowCaption(App->Name);

		SetExceptionMode(2);

		if (GateInfo->Mode == GATEMODE_ROUTE)
			RContainer_NET.SetAllocPermission(UCRCONTAINER_ALLOC_ROUTE);
		else
			RContainer_NET.SetAllocPermission(UCRCONTAINER_ALLOC_MIRROR);

		FiberInit.Start(0);
		return 1;
	}
	ucVOID Run()
	{
		if (!Init())
			return;
		RContainer_NET.Run(App->Name, App->Index, ucFALSE, ucTRUE, 10);
	}
};

#endif	//_gate_H_