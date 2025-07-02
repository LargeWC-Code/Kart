#ifndef _center_
#define _center_

#include "../../common/game_userbase.h"
#include "../../common/game_centerbase.h"

struct UCGameCenter_LocalCfg
{
	UCString				Password;					//管理员密码
};

class UCGameCenter_AccountInfo
{
public:
	UCString				Name;
	UCString				Password;
	UCGameCenter_AccountInfo() {}
	UCGameCenter_AccountInfo(ucCONST UCGameCenter_AccountInfo& in)
	{
		Name = in.Name;
		Password = in.Password;
	}
};

class UCGameCenter_AccountInfoCfg
{
public:
	UCEArray<UCGameCenter_AccountInfo>		AryAccountInfo;
};

class UCRObjGameCenter_AccountLocal : public UCRObjGameCenter_Account
{
public:
	UCEConfig<UCGameCenter_LocalCfg>			GameCenter_LocalCfg;

	//Gate
	UCEConfig<UCGameCenter_AccountInfoCfg>		GameCenter_AccountInfoCfg_Gate;
	//账号
	UCEConfig<UCGameCenter_AccountInfoCfg>		GameCenter_AccountInfoCfg_Account;
	//账号数据
	UCEConfig<UCGameCenter_AccountInfoCfg>		GameCenter_AccountInfoCfg_AccountData;
	//数据
	UCEConfig<UCGameCenter_AccountInfoCfg>		GameCenter_AccountInfoCfg_Data;
	//家园
	UCEConfig<UCGameCenter_AccountInfoCfg>		GameCenter_AccountInfoCfg_Home;
	//匹配
	UCEConfig<UCGameCenter_AccountInfoCfg>		GameCenter_AccountInfoCfg_Match;
	//逻辑
	UCEConfig<UCGameCenter_AccountInfoCfg>		GameCenter_AccountInfoCfg_Logic;
public:
	UCString	ChangePassword(ucCONST UCString& strOldPassword, ucCONST UCString& strNewPassword)
	{
		if (strNewPassword.IsEmpty())
			return UCString("非法的密码");
		if (strOldPassword.IsEmpty())
			return UCString("密码错误");
		UCGameCenter_LocalCfg* LocalCfg = GameCenter_LocalCfg.GetData();
		if (LocalCfg->Password != strOldPassword)
			return UCString("密码错误");
		LocalCfg->Password = strNewPassword;
		//存盘本地配置
		if (!GameCenter_LocalCfg.SaveToXML(UCString("center/center.xml")))
		{
			if (RContainer != ucNULL)
				RContainer->Log(UCString("center/center.xml修改密码存盘失败\r\n"));
			LocalCfg->Password = strOldPassword;
			return UCString("失败");
		}
		return UCString("succeed");
	}
	UCString	GameCommon_Register(ucINT GameCommon_Type, ucCONST UCString& strPassword, ucINT Index, UCString& strKey)
	{
// 		if (Index < 0 || Index >= ACCOUNT_MAX)
// 			return UCString("Index越界");
// 		if (strPassword.IsEmpty())
// 			return UCString("密码错误");
// 		CGame_Center_LocalCfg* LocalCfg = m_LocalCfg.GetData();
// 		if (LocalCfg->Password != strPassword)
// 			return UCString("密码错误");
// 
// 		GetService()->Log(ITOS(Index) + UCString("注册Account\r\n"));
// 
// 		UCString strValue;
// 		if (m_CsvAccount.GetString(Index + 2, 2, strValue) && !strValue.IsEmpty())
// 			return UCString("已被注册");
// 
// 		//生成密钥
// 		UCString Key;
// 		AllocKey(Key);
// 
// 		//存盘本地配置
// 		strValue = Key;
// 		if (!m_CsvAccount.SetString(Index + 2, 2, strValue) || m_CsvAccount.SaveToFile(UCString("center/account.csv")))
// 		{
// 			GetService()->Log(UCString("Account_") + ITOS(Index) + UCString("注册失败\r\n"));
// 			return UCString("注册失败");
// 		}
// 		strKey = strValue;
// 
// 		GetService()->Log(UCString("Account_") + ITOS(Index) + UCString("注册succeed\r\n"));
		return UCString("succeed");
	}
public:
	UCRObjGameCenter_AccountLocal()
	{
	}
	//初始化配置
	ucVOID	Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("center"));
		if (!GameCenter_LocalCfg.LoadFromXML(UCString("center/center.xml")))
		{
			UCGameCenter_LocalCfg* LocalCfg = GameCenter_LocalCfg.GetData();
			LocalCfg->Password = "123456";
			GameCenter_LocalCfg.SaveToXML(UCString("center/center.xml"));
		}

		if (!GameCenter_AccountInfoCfg_Account.LoadFromCSV(UCString("center/gate.csv")))
		{
			UCGameCenter_AccountInfoCfg* AccountInfoCfg = GameCenter_AccountInfoCfg_Gate.GetData();
			UCGameCenter_AccountInfo	AccountInfo;
			AccountInfo.Name = "Gate0";
			AccountInfo.Password = "123456";
			AccountInfoCfg->AryAccountInfo.Add(AccountInfo);

			GameCenter_AccountInfoCfg_Gate.SaveToCSV(UCString("center/gate.csv"));
		}
		if (!GameCenter_AccountInfoCfg_Account.LoadFromCSV(UCString("center/account.csv")))
		{
			UCGameCenter_AccountInfoCfg* AccountInfoCfg = GameCenter_AccountInfoCfg_Account.GetData();
			UCGameCenter_AccountInfo	AccountInfo;
			AccountInfo.Name = "Account0";
			AccountInfo.Password = "123456";
			AccountInfoCfg->AryAccountInfo.Add(AccountInfo);

			GameCenter_AccountInfoCfg_Account.SaveToCSV(UCString("center/account.csv"));
		}
		if (!GameCenter_AccountInfoCfg_AccountData.LoadFromCSV(UCString("center/accountdata.csv")))
		{
			UCGameCenter_AccountInfoCfg* AccountInfoCfg = GameCenter_AccountInfoCfg_AccountData.GetData();
			UCGameCenter_AccountInfo	AccountInfo;
			AccountInfo.Name = "AccountData0";
			AccountInfo.Password = "123456";
			AccountInfoCfg->AryAccountInfo.Add(AccountInfo);

			GameCenter_AccountInfoCfg_Account.SaveToCSV(UCString("center/account.csv"));
		}
		if (!GameCenter_AccountInfoCfg_Data.LoadFromCSV(UCString("center/data.csv")))
		{
			UCGameCenter_AccountInfoCfg* AccountInfoCfg = GameCenter_AccountInfoCfg_Data.GetData();
			UCGameCenter_AccountInfo	AccountInfo;
			AccountInfo.Name = "Data0";
			AccountInfo.Password = "123456";
			AccountInfoCfg->AryAccountInfo.Add(AccountInfo);

			GameCenter_AccountInfoCfg_Data.SaveToCSV(UCString("center/data.csv"));
		}
		if (!GameCenter_AccountInfoCfg_Home.LoadFromCSV(UCString("center/home.csv")))
		{
			UCGameCenter_AccountInfoCfg* AccountInfoCfg = GameCenter_AccountInfoCfg_Home.GetData();
			UCGameCenter_AccountInfo	AccountInfo;
			AccountInfo.Name = "Home0";
			AccountInfo.Password = "123456";
			AccountInfoCfg->AryAccountInfo.Add(AccountInfo);

			GameCenter_AccountInfoCfg_Home.SaveToCSV(UCString("center/home.csv"));
		}
		if (!GameCenter_AccountInfoCfg_Match.LoadFromCSV(UCString("center/match.csv")))
		{
			UCGameCenter_AccountInfoCfg* AccountInfoCfg = GameCenter_AccountInfoCfg_Match.GetData();
			UCGameCenter_AccountInfo	AccountInfo;
			AccountInfo.Name = "Match0";
			AccountInfo.Password = "123456";
			AccountInfoCfg->AryAccountInfo.Add(AccountInfo);

			GameCenter_AccountInfoCfg_Match.SaveToCSV(UCString("center/match.csv"));
		}
		if (!GameCenter_AccountInfoCfg_Logic.LoadFromCSV(UCString("center/logic.csv")))
		{
			UCGameCenter_AccountInfoCfg* AccountInfoCfg = GameCenter_AccountInfoCfg_Logic.GetData();
			UCGameCenter_AccountInfo	AccountInfo;
			AccountInfo.Name = "Logic0";
			AccountInfo.Password = "123456";
			AccountInfoCfg->AryAccountInfo.Add(AccountInfo);

			GameCenter_AccountInfoCfg_Logic.SaveToCSV(UCString("center/logic.csv"));
		}
	}
};

class UCRObjGameCenterLocal : public UCRObjGameCenter
{
public:
	UCRContainer_NET			RContainer_NET;			//服务模块

	UCRObjGameGate_Infos			RObjGameGate_Infos;			//所有的Gate信息

	UCRObjGameAccount_Infos			RObjGameAccount_Infos;		//所有的Account信息
	UCRObjGameAccountData_Infos		RObjGameAccountData_Infos;	//所有的AccountData信息
	
	UCRObjGameData_Infos			RObjGameData_Infos;			//所有的Data信息
	
	UCRObjGameHome_Infos			RObjGameHome_Infos;			//所有的Home信息
	UCRObjGameMatch_Infos			RObjGameMatch_Infos;		//所有的Match信息
	UCRObjGameLogic_Infos			RObjGameLogic_Infos;		//所有的Logic信息

	//容器临时key
	UCIntIntMap					MapAccountKey;
	UCIntIntMap					MapDataKey;
	UCIntIntMap					MapHomeKey;
	UCIntIntMap					MapMatchKey;
	UCIntIntMap					MapLogicKey;
private:
	UCEApp*							App;
	ucDWORD							AdminKey;
	UCFiber							FiberInit;
	UCRObjGameCenter_AccountLocal	RObjGameCenter_AccountLocal;
public:
	UCString	GameBase_GetAdminKey(ucCONST UCString& Username, ucCONST UCString& strPassword, ucINT64& AdminKey)
	{
		if (strPassword.IsEmpty())
			return UCString("密码为空");
		
		UCGameCenter_LocalCfg* GameCenter_LocalCfg  = (UCGameCenter_LocalCfg*)RObjGameCenter_AccountLocal.GameCenter_LocalCfg.GetData();
		if (strPassword != GameCenter_LocalCfg->Password)
			return UCString("密码错误");

		AdminKey = this->AdminKey;
		return UCString("succeed");
	}
	UCString	GameGate_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey)
	{
		UCGameCenter_AccountInfoCfg* AccountInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_Gate.GetData();
		if (Index < 0 || Index >= AccountInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		if (strKey.IsEmpty() || AccountInfoCfg->AryAccountInfo.GetAt(Index).Password != strKey)
			return UCString("密钥错误");

		UCGameBase_Info* GameGate_Info = &(RObjGameGate_Infos.GameGate_Infos[Index]);
		GameGate_Info->Valid = 1;
		GameGate_Info->Commit();

		AdminKey = this->AdminKey;

		RContainer_NET.Log(UCString("Gate_") + ITOS(Index) + UCString("刷新succeed\r\n"));
		return UCString("succeed");
	}
	UCString	GameGate_Empty(ucINT Index, ucDWORD RandKey)
	{
		return UCString("local");
	}

	UCString	GameAccount_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		UCGameCenter_AccountInfoCfg* AccountInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_Account.GetData();
		if (Index < 0 || Index >= AccountInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		if (strKey.IsEmpty() || AccountInfoCfg->AryAccountInfo.GetAt(Index).Password != strKey)
			return UCString("密钥错误");

		UCGameAccount_Info* GameAccount_Info = &(RObjGameAccount_Infos.GameAccount_Infos[Index]);
		GameAccount_Info->Valid = ucTRUE;
		GameAccount_Info->Commit();

		AdminKey = this->AdminKey;
		RandKey = randuint(0, 0xFFFFFFFF);

		MapAccountKey.Add(Index, RandKey);
		RContainer_NET.Log(UCString("Account_") + ITOS(Index) + UCString("刷新succeed\r\n"));
		return UCString("succeed");
	}
	UCString	GameAccount_Empty(ucINT Index, ucDWORD RandKey)
	{
		UCGameCenter_AccountInfoCfg* AccountInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_Account.GetData();
		if (Index < 0 || Index >= AccountInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		ucINT Pos = MapAccountKey.FindKey(Index);
		if (Pos < 0 || MapAccountKey.GetValueAt(Pos) != RandKey)
			return UCString("密钥错误");

		UCGameAccount_Info* GameAccount_Info = &(RObjGameAccount_Infos.GameAccount_Infos[Index]);
		GameAccount_Info->Valid = ucFALSE;
		GameAccount_Info->Commit();

		MapAccountKey.Remove(Index);
		return UCString("succeed");
	}

	UCString	GameAccountData_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		UCGameCenter_AccountInfoCfg* AccountInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_AccountData.GetData();
		if (Index < 0 || Index >= AccountInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		if (strKey.IsEmpty() || AccountInfoCfg->AryAccountInfo.GetAt(Index).Password != strKey)
			return UCString("密钥错误");

		UCGameAccountData_Info* GameAccountData_Info = &(RObjGameAccountData_Infos.GameAccountData_Infos[Index]);
		GameAccountData_Info->Valid = ucTRUE;
		GameAccountData_Info->Commit();

		AdminKey = this->AdminKey;
		RandKey = randuint(0, 0xFFFFFFFF);

		MapAccountKey.Add(Index, RandKey);
		RContainer_NET.Log(UCString("AccountData_") + ITOS(Index) + UCString("刷新succeed\r\n"));
		return UCString("succeed");
	}
	UCString	GameAccountData_Empty(ucINT Index, ucDWORD RandKey)
	{
		UCGameCenter_AccountInfoCfg* AccountInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_AccountData.GetData();
		if (Index < 0 || Index >= AccountInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		ucINT Pos = MapAccountKey.FindKey(Index);
		if (Pos < 0 || MapAccountKey.GetValueAt(Pos) != RandKey)
			return UCString("密钥错误");

		UCGameAccountData_Info* GameAccountData_Info = &(RObjGameAccountData_Infos.GameAccountData_Infos[Index]);
		GameAccountData_Info->Valid = ucFALSE;
		GameAccountData_Info->Commit();

		MapAccountKey.Remove(Index);
		return UCString("succeed");
	}

	UCString	GameData_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		UCGameCenter_AccountInfoCfg* DataInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_Data.GetData();
		if (Index < 0 || Index >= DataInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		if (strKey.IsEmpty() || DataInfoCfg->AryAccountInfo.GetAt(Index).Password != strKey)
			return UCString("密钥错误");

		UCGameData_Info* GameData_Info = &(RObjGameData_Infos.GameData_Infos[Index]);
		GameData_Info->Valid = 1;
		GameData_Info->Commit();

		AdminKey = this->AdminKey;
		RandKey = randuint(0, 0xFFFFFFFF);

		MapDataKey.Add(Index, RandKey);
		RContainer_NET.Log(UCString("Data_") + ITOS(Index) + UCString("刷新succeed\r\n"));
		return UCString("succeed");
	}
	UCString	GameData_Empty(ucINT Index, ucDWORD RandKey)
	{
		UCGameCenter_AccountInfoCfg* DataInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_Data.GetData();
		if (Index < 0 || Index >= DataInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		ucINT Pos = MapDataKey.FindKey(Index);
		if (Pos < 0 || MapDataKey.GetValueAt(Pos) != RandKey)
			return UCString("密钥错误");

		UCGameData_Info* GameData_Info = &(RObjGameData_Infos.GameData_Infos[Index]);
		GameData_Info->Valid = ucFALSE;
		GameData_Info->Commit();

		MapDataKey.Remove(Index);
		return UCString("succeed");
	}

	UCString	GameHome_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		UCGameCenter_AccountInfoCfg* AccountInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_Home.GetData();
		if (Index < 0 || Index >= AccountInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		if (strKey.IsEmpty() || AccountInfoCfg->AryAccountInfo.GetAt(Index).Password != strKey)
			return UCString("密钥错误");

		UCGameHome_Info* GameHome_Info = &(RObjGameHome_Infos.GameHome_Infos[Index]);
		GameHome_Info->Valid = 1;
		GameHome_Info->Commit();

		AdminKey = this->AdminKey;
		RandKey = randuint(0, 0xFFFFFFFF);

		MapHomeKey.Add(Index, RandKey);
		RContainer_NET.Log(UCString("Home_") + ITOS(Index) + UCString("刷新succeed\r\n"));
		return UCString("succeed");
	}
	UCString	GameHome_Empty(ucINT Index, ucDWORD RandKey)
	{
		UCGameCenter_AccountInfoCfg* AccountInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_Home.GetData();
		if (Index < 0 || Index >= AccountInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		ucINT Pos = MapHomeKey.FindKey(Index);
		if (Pos < 0 || MapHomeKey.GetValueAt(Pos) != RandKey)
			return UCString("密钥错误");

		UCGameHome_Info* GameHome_Info = &(RObjGameHome_Infos.GameHome_Infos[Index]);
		GameHome_Info->Valid = ucFALSE;
		GameHome_Info->Commit();

		MapHomeKey.Remove(Index);
		return UCString("succeed");
	}

	UCString	GameMatch_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		UCGameCenter_AccountInfoCfg* AccountInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_Match.GetData();
		if (Index < 0 || Index >= AccountInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		if (strKey.IsEmpty() || AccountInfoCfg->AryAccountInfo.GetAt(Index).Password != strKey)
			return UCString("密钥错误");

		UCGameMatch_Info* GameMatch_Info = &(RObjGameMatch_Infos.GameMatch_Infos[Index]);
		GameMatch_Info->Valid = 1;
		GameMatch_Info->Commit();

		AdminKey = this->AdminKey;
		RandKey = randuint(0, 0xFFFFFFFF);

		MapMatchKey.Add(Index, RandKey);
		RContainer_NET.Log(UCString("Match_") + ITOS(Index) + UCString("刷新succeed\r\n"));
		return UCString("succeed");
	}
	UCString	GameMatch_Empty(ucINT Index, ucDWORD RandKey)
	{
		UCGameCenter_AccountInfoCfg* AccountInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_Match.GetData();
		if (Index < 0 || Index >= AccountInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		ucINT Pos = MapMatchKey.FindKey(Index);
		if (Pos < 0 || MapMatchKey.GetValueAt(Pos) != RandKey)
			return UCString("密钥错误");

		UCGameMatch_Info* GameMatch_Info = &(RObjGameMatch_Infos.GameMatch_Infos[Index]);
		GameMatch_Info->Valid = ucFALSE;
		GameMatch_Info->Commit();

		MapMatchKey.Remove(Index);
		return UCString("succeed");
	}
	UCString	GameLogic_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		UCGameCenter_AccountInfoCfg* AccountInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_Logic.GetData();
		if (Index < 0 || Index >= AccountInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		if (strKey.IsEmpty() || AccountInfoCfg->AryAccountInfo.GetAt(Index).Password != strKey)
			return UCString("密钥错误");

		UCGameLogic_Info* GameLogic_Info = &(RObjGameLogic_Infos.GameLogic_Infos[Index]);
		GameLogic_Info->Valid = 1;
		GameLogic_Info->Commit();

		AdminKey = this->AdminKey;
		RandKey = randuint(0, 0xFFFFFFFF);

		MapLogicKey.Add(Index, RandKey);
		RContainer_NET.Log(UCString("Logic_") + ITOS(Index) + UCString("刷新succeed\r\n"));
		return UCString("succeed");
	}
	UCString	GameLogic_Empty(ucINT Index, ucDWORD RandKey)
	{
		UCGameCenter_AccountInfoCfg* AccountInfoCfg = (UCGameCenter_AccountInfoCfg*)RObjGameCenter_AccountLocal.GameCenter_AccountInfoCfg_Logic.GetData();
		if (Index < 0 || Index >= AccountInfoCfg->AryAccountInfo.GetSize())
			return UCString("越界");
		ucINT Pos = MapLogicKey.FindKey(Index);
		if (Pos < 0 || MapLogicKey.GetValueAt(Pos) != RandKey)
			return UCString("密钥错误");

		UCGameLogic_Info* GameLogic_Info = &(RObjGameLogic_Infos.GameLogic_Infos[Index]);
		GameLogic_Info->Valid = ucFALSE;
		GameLogic_Info->Commit();

		MapLogicKey.Remove(Index);
		return UCString("succeed");
	}
public:
	UCRObjGameCenterLocal()
	{
		App = UCGetApp();
		FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
	}
	~UCRObjGameCenterLocal()
	{
	}
	ucINT	Init()
	{
		AdminKey = 0;
		for (ucINT i = 0; i < 8; i++)
			AdminKey |= randuint(48, 122) << (i * 8);
		RContainer_NET.SetAdminKey(AdminKey);

		RObjGameCenter_AccountLocal.Init();
		UCGameCenter_LocalCfg* GameCenter_LocalCfg = (UCGameCenter_LocalCfg*)RObjGameCenter_AccountLocal.GameCenter_LocalCfg.GetData();

		if (App->Name.IsEmpty())
			App->Name = UCString("center");
		UCGetSystemExt()->SetWindowCaption(App->Name);
		FiberInit.Start(0);
		return 1;
	}
	ucVOID	OnFiber_Init(UCObject* Object, UCEventArgs* e)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		if (App->Index < 0)
			RContainer_NET.Log(UCString("start:") + App->Name + UCString("\r\n"));
		else
			RContainer_NET.Log(UCString("start:") + App->Name + ITOS(App->Index, UCString(":%d")) + UCString("\r\n"));

		UCGameCenter_LocalCfg* LocalCfg = RObjGameCenter_AccountLocal.GameCenter_LocalCfg.GetData();

		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));
		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameCenter.obj"), typeof(UCRObjGameCenter), typeof(UCRObjGameCenterLocal), this, ucTRUE, ucFALSE, DEFAULT_PASSWORD);

		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameCenter_Account.obj"), typeof(UCRObjGameCenter_Account), typeof(UCRObjGameCenter_AccountLocal), &RObjGameCenter_AccountLocal, ucFALSE, ucFALSE, AdminKey);

		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameGate_Infos.obj"), typeof(UCRObjGameGate_Infos), &RObjGameGate_Infos, ucFALSE, ucFALSE, AdminKey);

		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameAccount_Infos.obj"), typeof(UCRObjGameAccount_Infos), &RObjGameAccount_Infos, ucFALSE, ucFALSE, AdminKey);
		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameAccountData_Infos.obj"), typeof(UCRObjGameAccountData_Infos), &RObjGameAccountData_Infos, ucFALSE, ucFALSE, AdminKey);

		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameData_Infos.obj"), typeof(UCRObjGameData_Infos), &RObjGameData_Infos, ucFALSE, ucFALSE, AdminKey);

		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameHome_Infos.obj"), typeof(UCRObjGameHome_Infos), &RObjGameHome_Infos, ucFALSE, ucFALSE, AdminKey);
		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameMatch_Infos.obj"), typeof(UCRObjGameMatch_Infos), &RObjGameMatch_Infos, ucFALSE, ucFALSE, AdminKey);
		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameLogic_Infos.obj"), typeof(UCRObjGameLogic_Infos), &RObjGameLogic_Infos, ucFALSE, ucFALSE, AdminKey);

		RContainer_NET.Log(UCString("当前容器初始化完毕\r\n"));
	}
public:
	ucVOID	Run()
	{
		//初始化
		if (!Init())
			return;
		RContainer_NET.Run(App->Name, App->Index, ucFALSE, ucTRUE, 100);
	}
};

#endif