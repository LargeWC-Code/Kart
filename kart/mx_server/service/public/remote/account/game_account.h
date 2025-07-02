#ifndef _moba_account_H_
#define _moba_account_H_
#include "../../common/game_account_db_robj.h"
#include "../../common/game_account_robj.h"
#include "../../common/game_center_robj.h"
#include "../../common/log_robj.h"
#include "../../../private/common/game_secret.h"

//本地配置存档
struct CGame_Account_LocalCfg
{
	ucINT						EnableLog;					//是否开启日志
	UCString				LogUrl;						//日志系统地址

	UCString				Key;						//密钥，防止冲突

	UCString				PrivateGate;				//内网Gate地址//结束（本机可以填空）
	UCString				DnsURL;						//DNS服路径//结束
};

//用户激活信息
struct CGame_Account_ActivateInfo
{
	CUserGameID					UserGameID;					//用户ID
	ucDWORD						Key;						//临时密钥
	ucINT							ChannelID;					//渠道ID
	UCString					ExtInfo;					//扩展数据，针对不同渠道，数据含义不一样
	UCString					UnionID;					//平台唯一ID

	ucINT							Life;						//生命个数，被激活一次，生命+1
};

struct CGame_Account_ActivateInfoPtr
{
	ucDWORD								Tick;		//时间戳
	CGame_Account_ActivateInfo*		Data;
	CGame_Account_ActivateInfoPtr()
	{
	}
	CGame_Account_ActivateInfoPtr(const CGame_Account_ActivateInfoPtr& in)
	{
		Tick = in.Tick;
		Data = in.Data;
	}
};

//活跃用户管理器
struct CGame_Account_ActivateManage
{
	UCDWORDVoidMap				MapGameDBAccountID;		//根据
	ucINT							m_TotleSize;			//统计在线人数
public:
	CGame_Account_ActivateManage()
	{
		m_TotleSize = 0;
	}
	~CGame_Account_ActivateManage()
	{
		for (ucINT i = MapGameDBAccountID.GetSize() - 1; i >= 0; --i)
		{
			UCDWORDVoidMap* MapAccountID = (UCDWORDVoidMap*)MapGameDBAccountID.GetValueAt(i);
			for (ucINT j = MapAccountID->GetSize() - 1; j >= 0; --j)
			{
				CGame_Account_ActivateInfo* Account_ActivateInfo = (CGame_Account_ActivateInfo*)MapAccountID->GetValueAt(j);
				delete Account_ActivateInfo;
			}
			delete MapAccountID;
		}
	}
	CGame_Account_ActivateInfo*		Get(const CUserGameID& UserGameID)
	{
		ucINT Pos = MapGameDBAccountID.FindKey(UserGameID.DBIndex);
		if (Pos < 0)
			return 0;

		UCDWORDVoidMap* MapAccountID = (UCDWORDVoidMap*)MapGameDBAccountID.GetValueAt(Pos);

		Pos = MapAccountID->FindKey(UserGameID.Index);
		if (Pos < 0)
			return 0;

		CGame_Account_ActivateInfo* Account_ActivateInfo = (CGame_Account_ActivateInfo*)MapAccountID->GetValueAt(Pos);
		//Account_ActivateInfo->Life++;
		return Account_ActivateInfo;
	}
	CGame_Account_ActivateInfo*		Add(const CUserGameID& UserGameID)
	{
		ucINT Pos = MapGameDBAccountID.FindKey(UserGameID.DBIndex);
		if (Pos < 0)
			Pos = MapGameDBAccountID.Add(UserGameID.DBIndex, new UCDWORDVoidMap);

		UCDWORDVoidMap* MapAccountID = (UCDWORDVoidMap*)MapGameDBAccountID.GetValueAt(Pos);

		Pos = MapAccountID->FindKey(UserGameID.Index);
		if (Pos < 0)
		{
			CGame_Account_ActivateInfo* Account_ActivateInfo = new CGame_Account_ActivateInfo;
			Account_ActivateInfo->Life = 0;
			Account_ActivateInfo->UserGameID.DBIndex = UserGameID.DBIndex;
			Account_ActivateInfo->UserGameID.Index = UserGameID.Index;
			Pos = MapAccountID->Add(UserGameID.Index, Account_ActivateInfo);
			++m_TotleSize;
		}

		CGame_Account_ActivateInfo* Account_ActivateInfo = (CGame_Account_ActivateInfo*)MapAccountID->GetValueAt(Pos);
		Account_ActivateInfo->Life++;
		return Account_ActivateInfo;
	}
	ucINT GetSize()
	{
		return m_TotleSize;
	}
	ucVOID		Remove(const CUserGameID& UserGameID)
	{
		ucINT Pos = MapGameDBAccountID.FindKey(UserGameID.DBIndex);
		if (Pos < 0)
			return;

		UCDWORDVoidMap* MapAccountID = (UCDWORDVoidMap*)MapGameDBAccountID.GetValueAt(Pos);
		ucINT Pos1 = MapAccountID->FindKey(UserGameID.Index);
		if (Pos1 < 0)
			return;

		CGame_Account_ActivateInfo* Account_ActivateInfo = (CGame_Account_ActivateInfo*)MapAccountID->GetValueAt(Pos1);
		Account_ActivateInfo->Life--;
		if (Account_ActivateInfo->Life == 0)
		{
			--m_TotleSize;
			MapAccountID->RemoveAt(Pos1);
			delete Account_ActivateInfo;
			if (MapAccountID->GetSize() == 0)
			{
				MapGameDBAccountID.RemoveAt(Pos);
				delete MapAccountID;
			}
		}
	}
};

//用户账号服务器
class CGame_AccountService : public CGame_Account_RCObj
{
public:
	CGame_Account_DB_RCObj						m_Game_Account_DB_RCObj;		//账号服务器的远程调用
	CGame_Center_RCObj							m_Game_Center_RCObj;			//中央服务器的远程调用

	CGame_Account_ActivateManage				m_Game_Account_ActivateManage;
	UCEArray<CGame_Account_ActivateInfoPtr>		m_Game_Account_ActiveInfoPtrArray;

	//服务相关信息，用于压力均衡
	UCIntStringMap								m_MapGameDB_Info_Open;			//所有对外GameDB
	UCIntIntMap									m_MapGameDB_Info_UnOpen;		//所有对内GameDB
	CGameDB_Info_RDObj							m_GameDB_Info_RDObj;			//所有的GameDB信息
private:
	UCEConfig<CGame_Account_LocalCfg>			m_LocalCfg;
	UCVF_Service								m_Service;

	ucINT											m_Index;
	ucDWORD										m_CenterKey;					//中心服的临时key
	ucINT64										m_AdminKey;						//管理员密钥
	UCFiber										m_FiberInit;					//连接初始化
	UCFiber										m_FiberUpdateActivate;

	UCCUrl										m_CUrl;							//HTTP网络服务
	UCLogCall									m_Log;
public:
	//普通用户名密码登录，返回玩家UserID和随机key
	UCString Login(const UCString& Username, const UCString& Password, CUserGameID& UserGameID, ucINT& AccountID, ucDWORD& AccountKey, UCString& GameDBUrl)
	{
		if (!m_Game_Account_DB_RCObj.Linked)
			return UCString("DB未连接");

		if (Username.IsEmpty())
			return UCString("Empty String");

		UCString Password_DB;
		CUserGameID PlatID_DB;
		PlatID_DB.DBIndex = -1;
		PlatID_DB.Index = -1;

		UCString strRet = m_Game_Account_DB_RCObj.LoadData(CANAL_MAGICX, Username, Password_DB, PlatID_DB);
		if (strRet != "成功")
		{
			return UCString("账号未注册");
// 			if (strRet != UCString("Get失败：1"))
// 				return strRet;
// 			//下面自动注册
// 			strRet = m_Game_Account_DB_RCObj.SaveData(CANAL_MAGICX, Username, Password, PlatID_DB);
// 			if (strRet != "成功")
// 				return strRet;
		}
		else
		{
			if (Password_DB != Password)
				return UCString("密码错误");
		}

		ucINT iFind = m_MapGameDB_Info_Open.FindKey(PlatID_DB.DBIndex);
		if (iFind < 0)
			return UCString("维护中");
		UCString strValidDBAddress;
		if (!m_Service.GetHostByName(UCString("GameDB_") + ITOS(PlatID_DB.DBIndex), strValidDBAddress))
			return UCString("维护中1");

		UserGameID.DBIndex = PlatID_DB.DBIndex;
		UserGameID.Index = PlatID_DB.Index;

		CGame_Account_ActivateInfo* Game_Account_ActivateInfo = GeneralActivateInfo(UserGameID);
		Game_Account_ActivateInfo->ChannelID = CANAL_MAGICX;
		Game_Account_ActivateInfo->ExtInfo = Password;
		Game_Account_ActivateInfo->UnionID = Username;
		AccountKey = Game_Account_ActivateInfo->Key;
		AccountID = m_Index;
		GameDBUrl = UCString("dom://GameDB_") + ITOS(PlatID_DB.DBIndex);
		return UCString("成功");
	}
	//注册用户（用不到了，都走自动注册流程）
	UCString Register(const UCString& Username, const UCString& Password, CUserGameID& UserGameID, ucINT& AccountID, ucDWORD& AccountKey, UCString& GameDBUrl)
	{
		if (!m_Game_Account_DB_RCObj.Linked)
			return UCString("DB未连接");

		if (Username.IsEmpty())
			return UCString("Empty String");

		UCString Password_DB;
		CUserGameID PlatID_DB;
		PlatID_DB.DBIndex = -1;
		PlatID_DB.Index = -1;
		//m_Service.Log(UCString("Register Username:") + Username + UCString("\r\n"));
		UCString strRet = m_Game_Account_DB_RCObj.LoadData(CANAL_MAGICX, Username, Password_DB, PlatID_DB);
		if (strRet == "成功")
			return UCString("已创建用户");
		if (strRet != UCString("Get失败：1"))
			return strRet;

		strRet = m_Game_Account_DB_RCObj.SaveData(CANAL_MAGICX, Username, Password, PlatID_DB);
		if (strRet != "成功")
			return strRet;

		if (m_MapGameDB_Info_Open.GetSize() <= 0)
			return UCString("维护中");
		ucINT iFind = m_MapGameDB_Info_Open.FindKey(PlatID_DB.DBIndex);
		if (iFind < 0)
			return UCString("维护中");
		UCString strValidDBAddress;
		if (!m_Service.GetHostByName(UCString("GameDB_") + ITOS(PlatID_DB.DBIndex), strValidDBAddress))
			return UCString("维护中");

		UserGameID.DBIndex = PlatID_DB.DBIndex;
		UserGameID.Index = PlatID_DB.Index;

		CGame_Account_ActivateInfo* Game_Account_ActivateInfo = GeneralActivateInfo(UserGameID);
		Game_Account_ActivateInfo->ChannelID = CANAL_MAGICX;
		Game_Account_ActivateInfo->ExtInfo = Password;
		AccountKey = Game_Account_ActivateInfo->Key;
		AccountID = m_Index;
		GameDBUrl = UCString("dom://GameDB_") + ITOS(PlatID_DB.DBIndex);
		return UCString("成功");
	}
	//验证key的合法性，给GameDB校验用户合法性，返回渠道ID和渠道扩展数据
	UCString CheckValid(const CUserGameID& UserGameID, ucDWORD AccountKey, ucINT& iChannelID, UCString& strExtInfo, UCString& strUnionID)
	{
		CGame_Account_ActivateInfo* Data = m_Game_Account_ActivateManage.Get(UserGameID);
		if (Data == 0)
			return UCString("无效用户");
		if (Data->Key != AccountKey)
			return UCString("密钥错误");
		iChannelID = Data->ChannelID;
		strExtInfo = Data->ExtInfo;
		strUnionID = Data->UnionID;
		return UCString("成功");
	}
public:
	CGame_AccountService()
	{
		m_CenterKey = 0;
		m_AdminKey = 0;
		m_FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		m_FiberUpdateActivate.FiberEvent = UCEvent(this, OnFiberUpdateActivate);
		m_Service.OnClose = UCEvent(this, OnMyServiceClose);
	}
	~CGame_AccountService()
	{
	}
	CGame_Account_ActivateInfo* GeneralActivateInfo(const CUserGameID& UserGameID)
	{
		CGame_Account_ActivateInfo* Data = m_Game_Account_ActivateManage.Add(UserGameID);
		Data->Key = randint(0, 65535) << 16 | randint(1, 65535);

		CGame_Account_ActivateInfoPtr Game_Account_ActiveInfoPtr;
		Game_Account_ActiveInfoPtr.Data = Data;
		Game_Account_ActiveInfoPtr.Tick = UCGetTickCount();
		m_Game_Account_ActiveInfoPtrArray.Add(Game_Account_ActiveInfoPtr);
		m_Service.SetValue(2 + m_Game_Account_ActivateManage.GetSize());
		return Data;
	}
	ucVOID OnFiberUpdateActivate(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		while (FiberData->IsValid())
		{
			ucDWORD CurrentTick = UCGetTickCount();
			while (m_Game_Account_ActiveInfoPtrArray.GetSize() && FiberData->IsValid())
			{
				const CGame_Account_ActivateInfoPtr* Account_ActiveInfoPtr = &m_Game_Account_ActiveInfoPtrArray.GetAt(0);
				if (CurrentTick - Account_ActiveInfoPtr->Tick > 30 * 1000)
				{
					m_Game_Account_ActivateManage.Remove(Account_ActiveInfoPtr->Data->UserGameID);
					m_Game_Account_ActiveInfoPtrArray.RemoveAt(0);
					m_Service.SetValue(2 + m_Game_Account_ActivateManage.GetSize());
					FiberData->Every(10);
				}
				else
					break;
			}
			FiberData->Every(10000);
		}
	}
	ucINT Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("game_account"));
		if (!m_LocalCfg.LoadFromXML(UCString("game_account/game_account.xml")))
		{
			CGame_Account_LocalCfg* LocalCfg = m_LocalCfg.GetData();
			LocalCfg->EnableLog = 1;
			LocalCfg->LogUrl = "GameLog//";

			LocalCfg->Key = "123456";
			LocalCfg->PrivateGate = "";
			LocalCfg->DnsURL = "Dns//";
			m_LocalCfg.SaveToXML(UCString("game_account/game_account.xml"));
		}
		CGame_Account_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->EnableLog && LocalCfg->LogUrl.Right(2) != UCString("//"))
		{
			WBox(UCString("LogUrl路径错误：") + LocalCfg->LogUrl);
			return 0;
		}
		if (!LocalCfg->PrivateGate.IsEmpty() && LocalCfg->PrivateGate.Right(2) != UCString("//"))
		{
			WBox(UCString("PrivateGate路径错误：") + LocalCfg->PrivateGate);
			return 0;
		}
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
		//也可以添加本机域名
		//m_Service.AddHost(UCString("a"), UCString("GameHall_0"));

		m_Index = UCGetApp()->Index;
		m_Service.Name = UCGetApp()->Name + ITOS(m_Index, UCString("_%d"));
		UCGetSystemExt()->SetWindowCaption(UCString("GameAccount/") + m_Service.Name);
		m_FiberUpdateActivate.Start(0);
		m_FiberInit.Start(0);
		return 1;
	}
	ucVOID OnMyServiceClose(UCObject*, UCEventArgs*)
	{
		if (m_Game_Center_RCObj.Linked)
			m_Game_Center_RCObj.UnRefeshAccount(m_Index, m_CenterKey);
	}
	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		m_Service.Log(UCString("start GameAccount:") + m_Service.Name + UCString("\r\n"));
		m_Service.SetValue(1);		//先对内状态

		CGame_Account_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		//优先连接日志系统
		if (LocalCfg->EnableLog)
		{
			CLog_RCObj logRCObj;
			m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CLog_RCObj.call"), typeof(CLog_RCObj), &logRCObj);
			m_Service.Log(UCString("开始连接游戏日志服务器...\r\n"));
			UCString linkadress = LocalCfg->LogUrl + UCString("CLog_RCObj.call");
			while (FiberData->IsValid())
			{
				ucINT linkresult = logRCObj.Link(linkadress, ADMIN_PASSWORD);
				if (linkresult == 0)
					break;
				m_Service.Log(UCString("游戏日志服连接失败") + ITOS(linkresult) + UCString("\r\n"));
				FiberData->Every(10000);
			}
			m_Service.Log(UCString("游戏日志服连接成功\r\n"));
			ucINT bEnablePosWindows = 1;
			UCString strRet = logRCObj.AddModule(UCString("GameAccount"), m_Service.Name, m_Service.StateObj->PID.Value, bEnablePosWindows);
			if (strRet == UCString("成功"))
			{
				if (bEnablePosWindows == 0)
					SetExceptionMode(2);
				m_Log.SetService(&m_Service);
				ucINT ret = m_Log.Init(LocalCfg->LogUrl, UCString("GameAccount"), m_Service.Name, ADMIN_PASSWORD);
				if (ret)
					m_Service.Log(UCString("游戏日志服初始化失败") + ITOS(ret) + UCString("\r\n"));
			}
			else
			{
				m_Service.Log(UCString("注册游戏日志服失败:") + strRet + UCString("\r\n"));
			}
		}

		//暂停一段时间，让GameCenter和AccoudnDB启动完成
		//FiberData->Delay(2000);

		//添加本地的account服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CGame_Account_RCObj.call"), typeof(CGame_Account_RCObj), typeof(CGame_AccountService), this, UCVF_Guest_Permission_ReadWrite);
		//连接中心服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CGame_Center_RCObj.call"), typeof(CGame_Center_RCObj), &m_Game_Center_RCObj);
		//连接账号DB
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CGame_Account_DB_RCObj.call"), typeof(CGame_Account_DB_RCObj), &m_Game_Account_DB_RCObj);
		//连接中心服务的GameDB列表
		m_Service.AppendLocalFile(m_Service.Name + UCString("//local/CGameDB_Info_RDObj.data"), typeof(CGameDB_Info_RDObj), &m_GameDB_Info_RDObj);
		for (ucINT i = 0; i < GAMEDB_MAX; i++)
		{
			m_GameDB_Info_RDObj.GameDB_Info[i].BindOnSet(UCEvent(this, OnGameDB_InfoOnChanged));
		}
		m_GameDB_Info_RDObj.OnReset = UCEvent(this, OnMyGameDBInfoReset);

		if (m_Index < 0 && m_Index >= ACCOUNT_MAX)
		{
			m_Service.Log(UCString("无法连接Game中心服，错误的序号 : ") + ITOS(m_Index) + UCString("\r\n"));
			return;
		}

		m_Service.Log(UCString("开始连接中心服务器...\r\n"));
		while (FiberData->IsValid())
		{
			//连接中心服务器
			if (m_Game_Center_RCObj.Link(LocalCfg->PrivateGate + UCString("dom://GameCenter//CGame_Center_RCObj.call")) == 0)
				break;
			//m_Service.Log(UCString("中心服连接失败") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("中心连接成功\r\n"));

		UCString Result = m_Game_Center_RCObj.RefeshAccount(m_Service.Name, m_Index, LocalCfg->Key, m_AdminKey, m_CenterKey);
		if (Result != "成功")
		{
			m_Service.Log(UCString("中心服验证失败，请检查配置后重启服务 : ") + Result + UCString("\r\n"));
			return;
		}
		m_Service.Log(UCString("验证成功\r\n"));
		m_Service.SetAdminKey(m_AdminKey);

		while (FiberData->IsValid())
		{
			//连接中心服务器，获得GameDB的注册情况
			ucINT linkresult = m_GameDB_Info_RDObj.StableLink(LocalCfg->PrivateGate + UCString("dom://GameCenter//CGameDB_Info_RDObj.data"), m_AdminKey);
			if (linkresult == 0)
			{
				m_Service.Log(UCString("中心的GameDB列表连接成功\r\n"));
				break;
			}
			m_Service.Log(UCString("中心的GameDB列表连接失败 : dom://GameCenter//CGameDB_Info_RDObj.data,ret=") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}

		m_Service.Log(UCString("开始连接GameAccountDB服...\r\n"));
		while (FiberData->IsValid())
		{
			//连接AccountDB
			ucINT linkresult = m_Game_Account_DB_RCObj.Link(UCString("GameAccountDB//CGame_Account_DB_RCObj.call"), m_AdminKey);
			//ucINT linkresult = m_Game_Account_DB_RCObj.Link(LocalCfg->PrivateGate + UCString("dom://GameAccountDB//CGame_Account_DB_RCObj.call"), m_AdminKey);
			if (linkresult == 0)
			{
				m_Service.Log(UCString("GameAccountDB服连接成功\r\n"));
				break;
			}
			//m_Service.Log(UCString("Account DB连接失败 : dom://GameAccountDB//CGame_Account_DB_RCObj.call,ret=") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}

		m_Service.Log(UCString("-----服务器启动完成-----\r\n\r\n"));
		m_Service.SetValue(2);		//可以对外了
		m_Service.SetTTL(DEF_DNS_TTL);
	}
	ucVOID OnMyGameDBInfoReset(UCObject*, UCEventArgs*)
	{
		m_MapGameDB_Info_Open.RemoveAll();
		m_MapGameDB_Info_UnOpen.RemoveAll();
		for (ucINT i = 0; i < GAMEDB_MAX; i++)
		{
			if (m_GameDB_Info_RDObj.GameDB_Info[i].Url.IsEmpty())
				continue;
			if (m_GameDB_Info_RDObj.GameDB_Info[i].Open)
			{
				m_MapGameDB_Info_Open.Add(i, m_GameDB_Info_RDObj.GameDB_Info[i].Url);
			}
			else
			{
				m_MapGameDB_Info_UnOpen.Add(i, 0);
			}
		}
	}
	ucVOID OnGameDB_InfoOnChanged(UCObject*, UCEventArgs* e)
	{
		UCVF_PropertyBaseCmd_Args* Args = (UCVF_PropertyBaseCmd_Args*)e;

		CGameDB_Info* pInfo = (CGameDB_Info*)Args->Data;
		ucINT Index = m_GameDB_Info_RDObj.GameDB_Info[0].GetArrayIndex(pInfo);

		m_Service.Log(UCString("收到服务消息 GameDB[") + ITOS(Index) + UCString("].url=") + pInfo->Url + UCString("\r\n"));

		//如果服务地址为空，不认为是合法的地址
		if (pInfo->Url.IsEmpty())
		{
			m_MapGameDB_Info_Open.Remove(Index);
			m_MapGameDB_Info_UnOpen.Add(Index, 0);
			return;
		}

		if (pInfo->Open)
		{
			m_MapGameDB_Info_Open.Add(Index, pInfo->Url);
			m_MapGameDB_Info_UnOpen.Remove(Index);
		}
		else
		{
			m_MapGameDB_Info_Open.Remove(Index);
			m_MapGameDB_Info_UnOpen.Add(Index, 0);
		}
	}
public:
	ucVOID Run()
	{
		//初始化
		if (!Init())
			return;
		m_Service.Run(100);
	}
};

#endif	//_moba_account_H_