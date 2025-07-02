/*
	file : service
	author : LargeWC
	created : 2020/04/14
*/

#ifndef _game_center_
#define _game_center_

#include "public_base.h"

//定义DNS更新频率，单位秒
#define DEF_DNS_TTL  1

//服务器状态
struct CServiceBase_Info : public UCVF_ProData
{
	int					Open;		//是否对外开放
	UCString			Url;		//路径
	CServiceBase_Info()
	{
		Open = 0;
	}
};

//登录服务器信息
struct CAccount_Info : public CServiceBase_Info
{
};
//游戏数据服务信息
struct CGameDB_Info : public CServiceBase_Info
{
};
//游戏大厅信息
struct CHall_Info : public CServiceBase_Info
{
};
//匹配服务信息
struct CMatch_Info : public CServiceBase_Info
{
};
//游戏服务信息
struct CGame_Info : public CServiceBase_Info
{
};

//暂定账号服务器最多是16个
#define ACCOUNT_MAX			0x10
struct CAccount_Info_RDObj : public UCVF_RemoteDataObj
{
	CAccount_Info		Account_Info[ACCOUNT_MAX];
};

//暂定游戏数据服务器最多是256个
#define GAMEDB_MAX			0x100
struct CGameDB_Info_RDObj : public UCVF_RemoteDataObj
{
	CGameDB_Info		GameDB_Info[GAMEDB_MAX];
};
//暂定大厅服务器最多是1024个
#define HALL_MAX			0x400
struct CHall_Info_RDObj : public UCVF_RemoteDataObj
{
	CHall_Info			Hall_Info[HALL_MAX];
};
//暂定匹配服务器最多是16个
#define MATCH_MAX			0x10
struct CMatch_Info_RDObj : public UCVF_RemoteDataObj
{
	CMatch_Info			Match_Info[MATCH_MAX];
};
//暂定游戏服务器最多是1024个
#define GAME_MAX			0x400
struct CGame_Info_RDObj : public UCVF_RemoteDataObj
{
	CGame_Info			Game_Info[GAME_MAX];
};

//暂定游戏服最多开512个房间
#define BATTLE_MAX			0x200

//给工具使用，获取密钥
class CCenterKey_RCObj : public UCVF_RemoteCallObj
{
public:
	//工具使用，由密码获取管理员密钥
	ucINT64		GetAdminKey(ucCONST UCString& strPassword)
	{
		return 0;
	}
};

struct CToken_RDObj : public UCVF_RemoteDataObj
{
	UCVF_String		m_strToken;
};

//给GM工具使用的
class CCenterAdmin_RCObj : public UCVF_RemoteCallObj
{
public:
	//修改服务器密码
	UCString	ChangePassword(const UCString& strOldPassword, const UCString& strNewPassword)
	{
		return UCString("本地");
	}
	//注册Account服务
	UCString	RegisterAccount(const UCString& strPassword, int Index, UCString& strKey)
	{
		return UCString("本地");
	}
	//注册GameDB服务
	UCString	RegisterGameDB(const UCString& strPassword, int Index, UCString& strKey)
	{
		return UCString("本地");
	}
	//注册Hall服务
	UCString	RegisterHall(const UCString& strPassword, int Index, UCString& strKey)
	{
		return UCString("本地");
	}
	//注册Match服务
	UCString	RegisterMatch(const UCString& strPassword, int Index, UCString& strKey)
	{
		return UCString("本地");
	}
	//注册Game服务
	UCString	RegisterGame(const UCString& strPassword, int Index, UCString& strKey)
	{
		return UCString("本地");
	}
	//注册其它服务
	UCString	RegisterService(const UCString& strPassword, const UCString& strName, UCString& strKey)
	{
		return UCString("本地");
	}
};

//给普通服务器使用
class CGame_Center_RCObj : public UCVF_RemoteCallObj
{
public:
	//刷新Account信息
	UCString	RefeshAccount(const UCString& Url, int Index, const UCString& strKey, ucINT64& AdminKey, ucDWORD& randKey)
	{
		return UCString("本地");
	}
	//刷新GameDB信息，也用来检测是否有效 Index是ID Key对应密钥
	UCString	RefeshGameDB(const UCString& Url, int Index, const UCString& strKey, ucINT64& AdminKey, ucDWORD& randKey)
	{
		return UCString("本地");
	}
	//刷新Hall信息，也用来检测是否有效 Index是ID Key对应密钥
	UCString	RefeshHall(const UCString& Url, int Index, const UCString& strKey, ucINT64& AdminKey, ucDWORD& randKey)
	{
		return UCString("本地");
	}
	//刷新Match信息，也用来检测是否有效 Index是ID Key对应密钥
	UCString	RefeshMatch(const UCString& Url, int Index, const UCString& strKey, ucINT64& AdminKey, ucDWORD& randKey)
	{
		return UCString("本地");
	}
	//刷新Game信息，也用来检测是否有效 Index是ID Key对应密钥
	UCString	RefeshGame(const UCString& Url, int Index, const UCString& strKey, ucINT64& AdminKey, ucDWORD& randKey)
	{
		return UCString("本地");
	}
	//刷新其它服务
	UCString	RefeshService(const UCString& strName, const UCString& strKey, ucINT64& AdminKey)
	{
		return UCString("本地");
	}

	//注销一个Account
	UCString	UnRefeshAccount(int Index, ucDWORD randKey)
	{
		return UCString("本地");
	}
	//注销一个GameDB
	UCString	UnRefeshGameDB(int Index, ucDWORD randKey)
	{
		return UCString("本地");
	}
	//注销一个Hall
	UCString	UnRefeshHall(int Index, ucDWORD randKey)
	{
		return UCString("本地");
	}
	//注销一个Match
	UCString	UnRefeshMatch(int Index, ucDWORD randKey)
	{
		return UCString("本地");
	}
	//注销一个Game
	UCString	UnRefeshGame(int Index, ucDWORD randKey)
	{
		return UCString("本地");
	}
};


#endif