#ifndef _account_
#define _account_

#include "../../common/game_accountbase.h"
#include "../../common/game_accountdatabase.h"
#include "../../common/game_centerbase.h"

//本地配置存档
struct UCGameAccountCfg
{
	UCString				Key;						//密钥
	UCString				PrivateGate;				//内网Gate
};

struct UCGameUser_ActivateChannelInfo
{
	UCString				Username;					//渠道账号
	UCString				Password;					//密钥
	UCString				Extend;
	ucVOID Set(ucCONST UCGameUser_ActivateChannelInfo& in)
	{
		Username = in.Username;
		Password = in.Password;
		Extend = in.Extend;
	}
};
//用户激活信息
struct UCGameUser_ActivateInfo
{
	ucINT					Ref;						//引用计数

	ucDWORD					Key;						//临时密钥
	UCGameUserID			GameUserID;					//游戏用户ID
	UCGameUser_ActivateChannelInfo		ChannelInfo;	//渠道信息
	UCGameUser_ActivateInfo() { Ref = 0; }
	UCGameUser_ActivateInfo(ucCONST UCGameUser_ActivateInfo& in)
	{
		Ref = in.Ref;

		Key = in.Key;
		GameUserID.Set(in.GameUserID);
		ChannelInfo.Set(in.ChannelInfo);
	}
};

struct UCGameUser_ActivateInfoPtr
{
	ucDWORD								Tick;			//时间戳
	UCGameUser_ActivateInfo*			Data;
	UCGameUser_ActivateInfoPtr()
	{
	}
	UCGameUser_ActivateInfoPtr(ucCONST UCGameUser_ActivateInfoPtr& in)
	{
		Tick = in.Tick;
		Data = in.Data;
	}
};

struct UCGameAccountData_Users
{
	UCESimpleMap<ucDWORD, UCGameUser_ActivateInfo>		MapActivateInfo;
	UCGameAccountData_Users() {}
	UCGameAccountData_Users(ucCONST UCGameAccountData_Users& in)
	{
		MapActivateInfo = in.MapActivateInfo;
	}
};

//活跃用户管理器
struct UCGameAccount_ActivateManage
{
	UCESimpleMap<ucDWORD, UCGameAccountData_Users>		MapAccountData_Users;		//根据
	ucINT											Total;						//统计在线人数
public:
	UCGameAccount_ActivateManage() { Total = 0; }
	~UCGameAccount_ActivateManage() {}
	UCGameUser_ActivateInfo* Get(ucCONST UCGameUserID& GameUserID)
	{
		ucINT Pos = MapAccountData_Users.FindKey(GameUserID.GetDataID());
		if (Pos < 0)
			return ucNULL;
		UCGameAccountData_Users& GameAccountData_Users = MapAccountData_Users.GetValueAt(Pos);

		Pos = GameAccountData_Users.MapActivateInfo.FindKey(GameUserID.GetUserID());
		if (Pos < 0)
			return ucNULL;
		return &GameAccountData_Users.MapActivateInfo.GetValueAt(Pos);
	}
	UCGameUser_ActivateInfo* Add(ucCONST UCGameUserID& GameUserID)
	{
		ucINT Pos = MapAccountData_Users.FindKey(GameUserID.GetDataID());
		if (Pos < 0)
			Pos = MapAccountData_Users.Add(GameUserID.GetDataID(), UCGameAccountData_Users());
		UCGameAccountData_Users& GameAccountData_Users = MapAccountData_Users.GetValueAt(Pos);

		Pos = GameAccountData_Users.MapActivateInfo.FindKey(GameUserID.GetUserID());
		if (Pos < 0)
		{
			UCGameUser_ActivateInfo GameUser_ActivateInfo;
			GameUser_ActivateInfo.GameUserID.Set(GameUserID);
			Pos = GameAccountData_Users.MapActivateInfo.Add(GameUserID.GetUserID(), GameUser_ActivateInfo);
			Total++;
		}

		UCGameUser_ActivateInfo& GameUser_ActivateInfo = GameAccountData_Users.MapActivateInfo.GetValueAt(Pos);
		GameUser_ActivateInfo.Ref++;
		return &GameUser_ActivateInfo;
	}
	ucVOID Remove(ucCONST UCGameUserID& GameUserID)
	{
		ucINT Pos1 = MapAccountData_Users.FindKey(GameUserID.GetDataID());
		if (Pos1 < 0)
			return ;
		UCGameAccountData_Users& GameAccountData_Users = MapAccountData_Users.GetValueAt(Pos1);

		ucINT Pos2 = GameAccountData_Users.MapActivateInfo.FindKey(GameUserID.GetUserID());
		if (Pos2 < 0)
			return ;

		UCGameUser_ActivateInfo& GameUser_ActivateInfo = GameAccountData_Users.MapActivateInfo.GetValueAt(Pos2);
		GameUser_ActivateInfo.Ref--;
		if (GameUser_ActivateInfo.Ref == 0)
		{
			Total--;
			GameAccountData_Users.MapActivateInfo.RemoveAt(Pos2);
			if (GameAccountData_Users.MapActivateInfo.GetSize() == 0)
				MapAccountData_Users.RemoveAt(Pos1);
		}
	}
};

//用户账号服务器
class UCRObjGameAccountLocal : public UCRObjGameAccount
{
public:
	UCRObjGameAccountData						RObjGameAccountData;			//账号数据服务器
	UCRObjGameCenter							RObjGameCenter;					//中央服务器

	UCGameAccount_ActivateManage				GameAccount_ActivateManage;
	UCEArray<UCGameUser_ActivateInfoPtr>		AryGameUser_ActivateInfoPtr;

	//服务相关信息，用于压力均衡
	UCIntIntMap									MapGameData_Valid;				//所有开放Data
	UCIntIntMap									MapGameData_Info_Unvalid;		//所有未开放Data
	UCRObjGameData_Infos						RObjGameData_Infos;				//所有的Data信息
private:
	UCEConfig<UCGameAccountCfg>					GameAccountCfg;
	UCRContainer_NET							RContainer_NET;

	//通用Key
	ucINT64										AdminKey;
	ucDWORD										CenterKey;					//中心服的临时key

	UCEApp*										App;
	UCFiber										FiberInit;					//连接初始化
	UCFiber										m_FiberUpdateActivate;
public:
	UCString Login(ucCONST UCString& Username, ucCONST UCString& Password, UCGameUserID& GameUserID, ucDWORD& AccountKey)
	{
		if (!RObjGameAccountData.Linked)
			return UCString("AccountData未连接");

		if (Username.IsEmpty())
			return UCString("用户名为空");

		UCString strRet = RObjGameAccountData.Login(Channel_MagicX, Username, Password, GameUserID);
		if (strRet != "成功")
			return strRet;

		ucINT Pos = MapGameData_Valid.FindKey(GameUserID.GetDataID());
		if (Pos < 0)
			return UCString("维护中");

		UCGameUser_ActivateInfo* GameUser_ActivateInfo = GeneralActivateInfo(GameUserID, Username, Password);
		AccountKey = GameUser_ActivateInfo->Key;
		return UCString("成功");
	}
	UCString GetInfo(ucCONST UCGameUserID& GameUserID, ucDWORD AccountKey, UCString& Username, UCString& Password)
	{
		UCGameUser_ActivateInfo* Data = GameAccount_ActivateManage.Get(GameUserID);
		if (Data == 0)
			return UCString("无效用户");
		if (Data->Key != AccountKey)
			return UCString("密钥错误");
		Username = Data->ChannelInfo.Username;
		Password = Data->ChannelInfo.Password;
		return UCString("成功");
	}
public:
	UCRObjGameAccountLocal()
	{
		App = UCGetApp();

		CenterKey = 0;
		AdminKey = 0;
		FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		m_FiberUpdateActivate.FiberEvent = UCEvent(this, OnFiberUpdateActivate);
		RContainer_NET.OnEnd = UCEvent(this, OnRContainer_End);
	}
	~UCRObjGameAccountLocal()
	{
	}
	UCGameUser_ActivateInfo* GeneralActivateInfo(ucCONST UCGameUserID& GameUserID, ucCONST UCString& Username, ucCONST UCString& Password)
	{
		UCGameUser_ActivateInfo* Data = GameAccount_ActivateManage.Add(GameUserID);
		Data->ChannelInfo.Username = Username;
		Data->ChannelInfo.Password = Password;

		Data->Key = randint(0, 65535) << 16 | randint(1, 65535);

		UCGameUser_ActivateInfoPtr Game_Account_ActiveInfoPtr;
		Game_Account_ActiveInfoPtr.Data = Data;
		Game_Account_ActiveInfoPtr.Tick = UCGetTickCount();
		AryGameUser_ActivateInfoPtr.Add(Game_Account_ActiveInfoPtr);
		return Data;
	}
	ucVOID OnFiberUpdateActivate(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		while (FiberData->IsValid())
		{
			ucDWORD CurrentTick = UCGetTickCount();
			while (AryGameUser_ActivateInfoPtr.GetSize() && FiberData->IsValid())
			{
				ucCONST UCGameUser_ActivateInfoPtr* Account_ActiveInfoPtr = &AryGameUser_ActivateInfoPtr.GetAt(0);
				if (CurrentTick - Account_ActiveInfoPtr->Tick > 30 * 1000)
				{
					GameAccount_ActivateManage.Remove(Account_ActiveInfoPtr->Data->GameUserID);
					AryGameUser_ActivateInfoPtr.RemoveAt(0);
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
		UCFileExt_GetStatic().CreateFullPath(UCString("account"));
		if (!GameAccountCfg.LoadFromXML(UCString("account/account.xml")))
		{
			UCGameAccountCfg* LocalCfg = GameAccountCfg.GetData();
			LocalCfg->Key = "123456";
			GameAccountCfg.SaveToXML(UCString("account/account.xml"));
		}
		UCGameAccountCfg* LocalCfg = GameAccountCfg.GetData();

		UCGetSystemExt()->SetWindowCaption(App->Name + ITOS(App->Index, UCString(":%d")));
		m_FiberUpdateActivate.Start(0);
		FiberInit.Start(0);
		return 1;
	}
	ucVOID OnRContainer_End(UCObject*, UCEventArgs*)
	{
		if (RObjGameCenter.Linked)
			RObjGameCenter.GameAccount_Empty(App->Index, CenterKey);
	}
	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		if (App->Index < 0 && App->Index >= ACCOUNT_MAX)
		{
			RContainer_NET.Log(UCString("无法启动，错误的序号 : ") + ITOS(App->Index) + UCString("\r\n"));
			return;
		}

		if (App->Index < 0)
			RContainer_NET.Log(UCString("start:") + App->Name + UCString("\r\n"));
		else
			RContainer_NET.Log(UCString("start:") + App->Name + ITOS(App->Index, UCString(":%d")) + UCString("\r\n"));

		UCGameAccountCfg* LocalCfg = GameAccountCfg.GetData();
	
		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));

		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameAccount.obj"), typeof(UCRObjGameAccount), typeof(UCRObjGameAccountLocal), this, ucTRUE);
		RContainer_NET.AppendClient(LocalUrl + UCString("//local/UCRObjGameCenter.obj"), typeof(UCRObjGameCenter), &RObjGameCenter);
		RContainer_NET.AppendClient(LocalUrl + UCString("//local/UCRObjGameAccountData.obj"), typeof(UCRObjGameAccountData), &RObjGameAccountData);
		RContainer_NET.AppendClient(LocalUrl + UCString("//local/UCRObjGameData_Infos.obj"), typeof(UCRObjGameData_Infos), &RObjGameData_Infos);
		for (ucINT i = 0; i < DATA_MAX; i++)
			RObjGameData_Infos.GameData_Infos[i].AppendEvent(UCEvent(this, RObjGameData_Infos_OnChanged));
		RObjGameData_Infos.OnReset = UCEvent(this, RObjGameData_Infos_OnReset);

		RContainer_NET.Log(UCString("开始连接GameCenter...\r\n"));
		while (FiberData->IsValid())
		{
			//连接中心服务器
			if (RObjGameCenter.Link(LocalCfg->PrivateGate + UCString("sm://center:0//UCRObjGameCenter.obj")) == 0)
				break;
			//RContainer_NET.Log(UCString("GameCenter连接失败") + ITOS(Code) + UCString("\r\n"));
			FiberData->Every(10000);
		}
		RContainer_NET.Log(UCString("GameCenter连接成功\r\n"));

		UCString Result = RObjGameCenter.GameAccount_Update(App->Index, LocalCfg->Key, AdminKey, CenterKey);
		if (Result != "成功")
		{
			RContainer_NET.Log(UCString("GameCenter验证失败: ") + Result + UCString("\r\n"));
			return;
		}
		RContainer_NET.Log(UCString("验证成功\r\n"));
		RContainer_NET.SetAdminKey(AdminKey);

		while (FiberData->IsValid())
		{
			ucINT Code = RObjGameData_Infos.Link(LocalCfg->PrivateGate + UCString("sm://center:0//UCRObjGameData_Infos.obj"), UCROBJECT_LINK_STABLE, AdminKey);
			if (Code == 0)
			{
				RContainer_NET.Log(UCString("GameCenter的GameData_Infos连接成功\r\n"));
				break;
			}
			RContainer_NET.Log(UCString("GameCenter的GameData_Infos连接失败 : Code =") + ITOS(Code) + UCString("\r\n"));
			FiberData->Every(10000);
		}

		RContainer_NET.Log(UCString("连接GameAccountData...\r\n"));
		while (FiberData->IsValid())
		{
			//连接AccountDB
			ucINT Code = RObjGameAccountData.Link(UCString("sm://accountdata:0//UCRObjGameAccountData.obj"), UCROBJECT_LINK_STABLE, AdminKey);
			if (Code == 0)
			{
				RContainer_NET.Log(UCString("GameAccountData连接成功\r\n"));
				break;
			}
			//RContainer_NET.Log(UCString("Account DB连接失败 : dom://GameAccountDB//CGame_Account_DB_RCObj.call,ret=") + ITOS(Code) + UCString("\r\n"));
			FiberData->Every(10000);
		}

		RContainer_NET.Log(UCString("当前容器初始化完毕\r\n"));
	}
	ucVOID RObjGameData_Infos_OnReset(UCObject*, UCEventArgs*)
	{
		MapGameData_Valid.RemoveAll();
		MapGameData_Info_Unvalid.RemoveAll();
		for (ucINT i = 0; i < DATA_MAX; i++)
		{
			if (RObjGameData_Infos.GameData_Infos[i].Valid)
				MapGameData_Valid.Add(i, i);
			else
				MapGameData_Info_Unvalid.Add(i, 0);
		}
	}
	ucVOID RObjGameData_Infos_OnChanged(UCObject*, UCEventArgs* e)
	{
		UCRPropertyArgs* Args = (UCRPropertyArgs*)e;

		UCGameData_Info* GameData_Info = (UCGameData_Info*)Args->Data;
		ucINT Index = RObjGameData_Infos.GameData_Infos[0].GetArrayIndex(GameData_Info);

		RContainer_NET.Log(UCString("收到服务消息 RObjGameData_Infos.GameData_Infos[") + ITOS(Index) + UCString("]\r\n"));

		if (RObjGameData_Infos.GameData_Infos[Index].Valid)
		{
			MapGameData_Valid.Add(Index, Index);
			MapGameData_Info_Unvalid.Remove(Index);
		}
		else
		{
			MapGameData_Valid.Remove(Index);
			MapGameData_Info_Unvalid.Add(Index, Index);
		}
	}
public:
	ucVOID Run()
	{
		//初始化
		if (!Init())
			return;
		RContainer_NET.Run(App->Name, App->Index, ucFALSE, ucTRUE, 100);
	}
};

#endif	//_account_