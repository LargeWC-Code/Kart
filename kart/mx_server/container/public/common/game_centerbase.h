#ifndef _game_centerbase_
#define _game_centerbase_

#define DEFAULT_PASSWORD		123456789

struct UCGameBase_Info : public UCRProData
{
	ucINT				Valid;		//是否有效
	UCGameBase_Info()
	{
		Valid = 0;
	}
};

struct UCGameGate_Info : public UCGameBase_Info
{
};
#define GATE_MAX			0x100
struct UCRObjGameGate_Infos : public UCRObject
{
	UCGameGate_Info			GameGate_Infos[GATE_MAX];
};

struct UCGameAccount_Info : public UCGameBase_Info
{
};
#define ACCOUNT_MAX			0x10
struct UCRObjGameAccount_Infos : public UCRObject
{
	UCGameAccount_Info		GameAccount_Infos[ACCOUNT_MAX];
};

struct UCGameAccountData_Info : public UCGameBase_Info
{
};
#define ACCOUNTDATA_MAX		0x100
struct UCRObjGameAccountData_Infos : public UCRObject
{
	UCGameAccountData_Info	GameAccountData_Infos[ACCOUNTDATA_MAX];
};
struct UCGameData_Info : public UCGameBase_Info
{
};

#define DATA_MAX			0x100
struct UCRObjGameData_Infos : public UCRObject
{
	UCGameData_Info			GameData_Infos[DATA_MAX];
};
struct UCGameHome_Info : public UCGameBase_Info
{
};

#define HOME_MAX			0x400
struct UCRObjGameHome_Infos : public UCRObject
{
	UCGameHome_Info			GameHome_Infos[HOME_MAX];
};

struct UCGameMatch_Info : public UCGameBase_Info
{
};
#define MATCH_MAX			0x10
struct UCRObjGameMatch_Infos : public UCRObject
{
	UCGameMatch_Info		GameMatch_Infos[MATCH_MAX];
};

struct UCGameLogic_Info : public UCGameBase_Info
{
};
#define LOGIC_MAX			0x400
struct UCRObjGameLogic_Infos : public UCRObject
{
	UCGameLogic_Info		GameLogic_Infos[LOGIC_MAX];
};
#define BATTLE_MAX			0x200

enum GAMECOMMON_TYPE
{
	GAMECOMMON_TYPE_ACCOUNT = 0,
	GAMECOMMON_TYPE_DATA,
	GAMECOMMON_TYPE_HALL,
	GAMECOMMON_TYPE_MATCH,
	GAMECOMMON_TYPE_LOGIC
};
//游戏中心账号
class UCRObjGameCenter_Account : public UCRObject
{
	UCString	ChangePassword(ucCONST UCString& strOldPassword, ucCONST UCString& strNewPassword)
	{
		return UCString("local");
	}
	UCString	GameCommon_Register(ucINT GameCommon_Type, ucCONST UCString& strPassword, ucINT Index, UCString& strKey)
	{
		return UCString("local");
	}
};

class UCRObjGameCenter : public UCRObject
{
public:
	UCString	GameBase_GetAdminKey(ucCONST UCString& Username, ucCONST UCString& Password, ucINT64& AdminKey)
	{
		return UCString("local");
	}
	UCString	GameGate_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey)
	{
		return UCString("local");
	}
	UCString	GameGate_Empty(ucINT Index, ucDWORD RandKey)
	{
		return UCString("local");
	}

	UCString	GameAccount_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		return UCString("local");
	}
	UCString	GameAccount_Empty(ucINT Index, ucDWORD RandKey)
	{
		return UCString("local");
	}
	UCString	GameAccountData_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		return UCString("local");
	}
	UCString	GameAccountData_Empty(ucINT Index, ucDWORD RandKey)
	{
		return UCString("local");
	}

	UCString	GameData_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		return UCString("local");
	}
	UCString	GameData_Empty(ucINT Index, ucDWORD RandKey)
	{
		return UCString("local");
	}

	UCString	GameHome_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		return UCString("local");
	}
	UCString	GameHome_Empty(ucINT Index, ucDWORD RandKey)
	{
		return UCString("local");
	}
	UCString	GameMatch_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		return UCString("local");
	}
	UCString	GameMatch_Empty(ucINT Index, ucDWORD RandKey)
	{
		return UCString("local");
	}
	UCString	GameLogic_Update(ucINT Index, ucCONST UCString& strKey, ucINT64& AdminKey, ucDWORD& RandKey)
	{
		return UCString("local");
	}
	UCString	GameLogic_Empty(ucINT Index, ucDWORD RandKey)
	{
		return UCString("local");
	}
};

#endif