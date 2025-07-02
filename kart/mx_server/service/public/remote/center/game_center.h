#ifndef _center_H_
#define _center_H_
#include "../../common/game_center_robj.h"
#include "../../common/log_robj.h"
#include "../../../private/common/game_secret.h"
//本地配置存档
struct CGame_Center_LocalCfg
{
	UCString				Password;					//管理员密码

	int						EnableLog;					//是否开启日志
	UCString				LogUrl;						//日志系统地址
	int						EnbaleTokenSwitch;			//是否开启屏蔽字符
};

//中心服务器管理
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

//中心服务器管理
class CCenterAdminRCObj : public CCenterAdmin_RCObj
{
public:
	UCEConfig<CGame_Center_LocalCfg>	m_LocalCfg;		//配置

	UCCsv					m_CsvAccount;				//Account服密码
	UCCsv					m_CsvGameDB;				//GameDB服密码
	UCCsv					m_CsvHall;					//Hall服密码
	UCCsv					m_CsvMatch;					//Match服密码
	UCCsv					m_CsvGame;					//Game服密码
	UCStringMap				m_mapService;				//通用服务账号密码
public:
	CCenterAdminRCObj()
	{
	}
	//初始化配置
	void Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("center"));
		if (!m_LocalCfg.LoadFromXML(UCString("center/center.xml")))
		{
			CGame_Center_LocalCfg* LocalCfg = m_LocalCfg.GetData();
			LocalCfg->Password = "Jump123456";		//默认一个管理员密码
			LocalCfg->EnableLog = 1;
			LocalCfg->LogUrl = "GameLog//";
			LocalCfg->EnbaleTokenSwitch = 0;
			m_LocalCfg.SaveToXML(UCString("center/center.xml"));
		}

		if (!m_CsvAccount.ReadFromFile(UCString("center/account.csv")))
		{
			m_CsvAccount.SetString(1, 1, UCString("Account服序号"));
			m_CsvAccount.SetString(1, 2, UCString("密码"));
			for (int i = 0; i < ACCOUNT_MAX; ++i)
			{
				m_CsvAccount.SetInt(2 + i, 1, i);
				m_CsvAccount.SetString(2 + i, 2, UCString("123456"));
			}
			m_CsvAccount.SaveToFile(UCString("center/account.csv"));
		}
		if (!m_CsvGameDB.ReadFromFile(UCString("center/gamedb.csv")))
		{
			m_CsvGameDB.SetString(1, 1, UCString("GameDB服序号"));
			m_CsvGameDB.SetString(1, 2, UCString("密码"));
			for (int i = 0; i < GAMEDB_MAX; ++i)
			{
				m_CsvGameDB.SetInt(2 + i, 1, i);
				m_CsvGameDB.SetString(2 + i, 2, UCString("123456"));
			}
			m_CsvGameDB.SaveToFile(UCString("center/gamedb.csv"));
		}
		if (!m_CsvHall.ReadFromFile(UCString("center/gamehall.csv")))
		{
			m_CsvHall.SetString(1, 1, UCString("Hall服序号"));
			m_CsvHall.SetString(1, 2, UCString("密码"));
			for (int i = 0; i < HALL_MAX; ++i)
			{
				m_CsvHall.SetInt(2 + i, 1, i);
				m_CsvHall.SetString(2 + i, 2, UCString("123456"));
			}
			m_CsvHall.SaveToFile(UCString("center/gamehall.csv"));
		}
		if (!m_CsvMatch.ReadFromFile(UCString("center/gamematch.csv")))
		{
			m_CsvMatch.SetString(1, 1, UCString("Match服序号"));
			m_CsvMatch.SetString(1, 2, UCString("密码"));
			for (int i = 0; i < HALL_MAX; ++i)
			{
				m_CsvMatch.SetInt(2 + i, 1, i);
				m_CsvMatch.SetString(2 + i, 2, UCString("123456"));
			}
			m_CsvMatch.SaveToFile(UCString("center/gamematch.csv"));
		}
		if (!m_CsvGame.ReadFromFile(UCString("center/game.csv")))
		{
			m_CsvGame.SetString(1, 1, UCString("Game服序号"));
			m_CsvGame.SetString(1, 2, UCString("密码"));
			for (int i = 0; i < HALL_MAX; ++i)
			{
				m_CsvGame.SetInt(2 + i, 1, i);
				m_CsvGame.SetString(2 + i, 2, UCString("123456"));
			}
			m_CsvGame.SaveToFile(UCString("center/game.csv"));
		}
		m_mapService.RemoveAll();
		UCCsv csv;
		if (!csv.ReadFromFile(UCString("center/other.csv")))
		{
			//内置一些默认账号
			m_mapService.Add(UCString("GameAccountDB"), UCString("123456"));
			m_mapService.Add(UCString("GameAccount"), UCString("123456"));
			m_mapService.Add(UCString("GameGate"), UCString("123456"));
			m_mapService.Add(UCString("GameBI"), UCString("123456"));
			SaveCsvService();
		}
		else
		{
			UCString strKey, strValue;
			int iSize = csv.GetRowSize();
			for (int i = 2; i <= iSize; ++i)
			{
				if (!csv.GetString(i, 1, strKey))
					continue;
				if (!csv.GetString(i, 2, strValue))
					continue;
				m_mapService.Add(strKey, strValue);
			}
		}
	}
public:
	UCString	ChangePassword(const UCString& strOldPassword, const UCString& strNewPassword)
	{
		if (strNewPassword.IsEmpty())
			return UCString("非法的密码");
		if (strOldPassword.IsEmpty())
			return UCString("密码错误");
		CGame_Center_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->Password != strOldPassword)
			return UCString("密码错误");
		LocalCfg->Password = strNewPassword;
		//存盘本地配置
		if (!m_LocalCfg.SaveToXML(UCString("center/center.xml")))
		{
			GetService()->Log(UCString("center/center.xml修改密码存盘失败\r\n"));
			LocalCfg->Password = strOldPassword;
			return UCString("失败");
		}
		return UCString("成功");
	}
	UCString	RegisterAccount(const UCString& strPassword, int Index, UCString& strKey)
	{
		if (Index < 0 || Index >= ACCOUNT_MAX)
			return UCString("Index越界");
		if (strPassword.IsEmpty())
			return UCString("密码错误");
		CGame_Center_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->Password != strPassword)
			return UCString("密码错误");

		GetService()->Log(ITOS(Index) + UCString("注册Account\r\n"));

		UCString strValue;
		if (m_CsvAccount.GetString(Index + 2, 2, strValue) && !strValue.IsEmpty())
			return UCString("已被注册");

		//生成密钥
		UCString Key;
		AllocKey(Key);

		//存盘本地配置
		strValue = Key;
		if (!m_CsvAccount.SetString(Index + 2, 2, strValue) || m_CsvAccount.SaveToFile(UCString("center/account.csv")))
		{
			GetService()->Log(UCString("Account_") + ITOS(Index) + UCString("注册失败\r\n"));
			return UCString("注册失败");
		}
		strKey = strValue;

		GetService()->Log(UCString("Account_") + ITOS(Index) + UCString("注册成功\r\n"));
		return UCString("成功");
	}
	UCString	RegisterGameDB(const UCString& strPassword, int Index, UCString& strKey)
	{
		if (Index < 0 || Index >= GAMEDB_MAX)
			return UCString("Index越界");
		if (strPassword.IsEmpty())
			return UCString("密码错误");
		CGame_Center_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->Password != strPassword)
			return UCString("密码错误");

		GetService()->Log(ITOS(Index) + UCString("开始注册GameDB\r\n"));

		UCString strValue;
		if (m_CsvGameDB.GetString(Index + 2, 2, strValue) && !strValue.IsEmpty())
			return UCString("已被注册");

		//生成密钥
		UCString Key;
		AllocKey(Key);

		//存盘本地配置
		strValue = Key;
		if (!m_CsvGameDB.SetString(Index + 2, 2, strValue) || m_CsvGameDB.SaveToFile(UCString("center/gamedb.csv")))
		{
			GetService()->Log(UCString("GameDB_") + ITOS(Index) + UCString("注册失败\r\n"));
			return UCString("注册失败");
		}
		strKey = strValue;

		GetService()->Log(UCString("GameDB_") + ITOS(Index) + UCString("注册成功\r\n"));
		return UCString("成功");
	}
	UCString	RegisterHall(const UCString& strPassword, int Index, UCString& strKey)
	{
		if (Index < 0 || Index >= HALL_MAX)
			return UCString("Index越界");
		if (strPassword.IsEmpty())
			return UCString("密码错误");
		CGame_Center_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->Password != strPassword)
			return UCString("密码错误");

		GetService()->Log(ITOS(Index) + UCString("开始注册Hall\r\n"));

		UCString strValue;
		if (m_CsvHall.GetString(Index + 2, 2, strValue) && !strValue.IsEmpty())
			return UCString("已被注册");

		//生成密钥
		UCString Key;
		AllocKey(Key);

		//存盘本地配置
		strValue = Key;
		if (!m_CsvHall.SetString(Index + 2, 2, strValue) || m_CsvHall.SaveToFile(UCString("center/gamehall.csv")))
		{
			GetService()->Log(UCString("Hall_") + ITOS(Index) + UCString("注册失败\r\n"));
			return UCString("注册失败");
		}
		strKey = strValue;

		GetService()->Log(UCString("Hall_") + ITOS(Index) + UCString("注册成功\r\n"));
		return UCString("成功");
	}
	UCString	RegisterMatch(const UCString& strPassword, int Index, UCString& strKey)
	{
		if (Index < 0 || Index >= MATCH_MAX)
			return UCString("Index越界");
		if (strPassword.IsEmpty())
			return UCString("密码错误");
		CGame_Center_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->Password != strPassword)
			return UCString("密码错误");

		GetService()->Log(ITOS(Index) + UCString("开始注册Match\r\n"));

		UCString strValue;
		if (m_CsvMatch.GetString(Index + 2, 2, strValue) && !strValue.IsEmpty())
			return UCString("已被注册");

		//生成密钥
		UCString Key;
		AllocKey(Key);

		//存盘本地配置
		strValue = Key;
		if (!m_CsvMatch.SetString(Index + 2, 2, strValue) || m_CsvMatch.SaveToFile(UCString("center/gamematch.csv")))
		{
			GetService()->Log(UCString("Match_") + ITOS(Index) + UCString("注册失败\r\n"));
			return UCString("注册失败");
		}
		strKey = strValue;

		GetService()->Log(UCString("Match_") + ITOS(Index) + UCString("注册成功\r\n"));
		return UCString("成功");
	}
	UCString	RegisterGame(const UCString& strPassword, int Index, UCString& strKey)
	{
		if (Index < 0 || Index >= GAME_MAX)
			return UCString("Index越界");
		if (strPassword.IsEmpty())
			return UCString("密码错误");
		CGame_Center_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->Password != strPassword)
			return UCString("密码错误");

		GetService()->Log(ITOS(Index) + UCString("开始注册Game\r\n"));

		UCString strValue;
		if (m_CsvGame.GetString(Index + 2, 2, strValue) && !strValue.IsEmpty())
			return UCString("已被注册");

		//生成密钥
		UCString Key;
		AllocKey(Key);

		//存盘本地配置
		strValue = Key;
		if (!m_CsvGame.SetString(Index + 2, 2, strValue) || m_CsvGame.SaveToFile(UCString("center/gamehall.csv")))
		{
			GetService()->Log(UCString("Game_") + ITOS(Index) + UCString("注册失败\r\n"));
			return UCString("注册失败");
		}
		strKey = strValue;

		GetService()->Log(UCString("Game_") + ITOS(Index) + UCString("注册成功\r\n"));
		return UCString("成功");
	}
	UCString	RegisterService(const UCString& strPassword, const UCString& strName, UCString& strKey)
	{
		if (strPassword.IsEmpty())
			return UCString("密码错误");
		if (strName.IsEmpty())
			return UCString("账号错误");
		CGame_Center_LocalCfg* LocalCfg = m_LocalCfg.GetData();
		if (LocalCfg->Password != strPassword)
			return UCString("密码错误");

		GetService()->Log(strName + UCString("开始注册Service\r\n"));

		int iPos = m_mapService.FindKey(strName);
		if (iPos >= 0)
			return UCString("已被注册");

		//生成密钥
		UCString Key;
		AllocKey(Key);
		strKey = Key;
		m_mapService.Add(strName, strKey);

		//存盘本地配置
		if (!SaveCsvService())
		{
			GetService()->Log(UCString("center/other.csv存盘失败\r\n"));
		}

		GetService()->Log(UCString("账号:") + strName + UCString("注册成功\r\n"));
		return UCString("成功");
	}
private:
	int SaveCsvService()
	{
		UCCsv csv;
		int iSize = m_mapService.GetSize();
		csv.SetString(1, 1, UCString("通用服务账号"));
		csv.SetString(1, 2, UCString("密码"));
		for (int i = 0; i < iSize; ++i)
		{
			csv.SetString(2 + i, 1, m_mapService.GetKeyAt(i));
			csv.SetString(2 + i, 2, m_mapService.GetValueAt(i));
		}
		return csv.SaveToFile(UCString("center/other.csv"));
	}
	//分配一个随机Key
	void AllocKey(UCString& Key)
	{
		Key = "AAAAAA";
		for (int i = 0; i < 6; i++)
			Key.SetAt(i, randint('A', 'Z'));
	}
};

class CCenterService : public CGame_Center_RCObj
{
	UCVF_Service			m_Service;					//服务模块
	CCenterKeyRCObj			m_CenterKeyRCObj;			//给工具用，获取密钥
	CCenterAdminRCObj		m_Admin_RCObj;				//给管理员使用

	CAccount_Info_RDObj		m_Account_Info_RDObj;		//所有的Account信息
	CGameDB_Info_RDObj		m_GameDB_Info_RDObj;		//所有的GameDB信息
	CHall_Info_RDObj		m_Hall_Info_RDObj;			//所有的Hall信息
	CMatch_Info_RDObj		m_Match_Info_RDObj;			//所有的Match信息
	CGame_Info_RDObj		m_Game_Info_RDObj;			//所有的Game信息
	CToken_RDObj			m_Token_RDObj;

	//服务器临时key
	ucDWORD					m_AccountKey[ACCOUNT_MAX];
	ucDWORD					m_GameDBKey[GAMEDB_MAX];
	ucDWORD					m_HallKey[HALL_MAX];
	ucDWORD					m_MatchKey[MATCH_MAX];
	ucDWORD					m_GameKey[GAME_MAX];
private:
	UCCUrl m_CUrl;						//HTTP网络服务
	UCFiber					m_FiberInit;				//连接初始化

	UCFiber					m_FiberToken;
	UCLogCall				m_Log;
public:
	//刷新Account信息
	UCString	RefeshAccount(const UCString& Url, int Index, const UCString& strKey, ucINT64& AdminKey, ucDWORD& randKey)
	{
		if (Index < 0 || Index >= ACCOUNT_MAX)
			return UCString("Index越界");

		UCString strValue;
		if (strKey.IsEmpty() || !m_Admin_RCObj.m_CsvAccount.GetString(2 + Index, 2, strValue) || strKey != strValue)
			return UCString("密钥错误");

		//if (!CheckURLValid(Url))
		//	return UCString("RefeshAccount Url不通");

		CAccount_Info* Account_Info = &(m_Account_Info_RDObj.Account_Info[Index]);
		//设置为有效状态
		Account_Info->Open = 1;
		Account_Info->Url = Url;
		Account_Info->UpdateData();

		AdminKey = m_CenterKeyRCObj.m_AdminKey;
		randKey = randint(0, 65535) << 16 | randint(1, 65535);
		m_AccountKey[Index] = randKey;
		m_Service.Log(UCString("Account_") + ITOS(Index) + UCString(":") + Url + UCString("刷新成功\r\n"));
		return UCString("成功");
	}
	UCString	RefeshGameDB(const UCString& Url, int Index, const UCString& strKey, ucINT64& AdminKey, ucDWORD& randKey)
	{
		if (Index < 0 || Index >= GAMEDB_MAX)
			return UCString("Index越界");

		UCString strValue;
		if (strKey.IsEmpty() || !m_Admin_RCObj.m_CsvGameDB.GetString(2 + Index, 2, strValue) || strKey != strValue)
			return UCString("密钥错误");

		//if (!CheckURLValid(Url))
		//	return UCString("RefeshGameDB Url不通");

		CGameDB_Info* GameDB_Info = &(m_GameDB_Info_RDObj.GameDB_Info[Index]);
		//设置为有效状态
		GameDB_Info->Open = 1;
		GameDB_Info->Url = Url;
		GameDB_Info->UpdateData();

		AdminKey = m_CenterKeyRCObj.m_AdminKey;
		randKey = randint(0, 65535) << 16 | randint(1, 65535);
		m_GameDBKey[Index] = randKey;
		m_Service.Log(UCString("GameDB_") + ITOS(Index) + UCString(":") + Url + UCString("刷新成功\r\n"));
		return UCString("成功");
	}
	UCString	RefeshHall(const UCString& Url, int Index, const UCString& strKey, ucINT64& AdminKey, ucDWORD& randKey)
	{
		if (Index < 0 || Index >= HALL_MAX)
			return UCString("Index越界");

		UCString strValue;
		if (strKey.IsEmpty() || !m_Admin_RCObj.m_CsvHall.GetString(2 + Index, 2, strValue) || strKey != strValue)
			return UCString("密钥错误");

		//if (!CheckURLValid(Url))
		//	return UCString("RefeshHall Url不通");

		CHall_Info* Hall_Info = &(m_Hall_Info_RDObj.Hall_Info[Index]);
		//设置为有效状态
		Hall_Info->Open = 1;
		Hall_Info->Url = Url;
		Hall_Info->UpdateData();

		AdminKey = m_CenterKeyRCObj.m_AdminKey;
		randKey = randint(0, 65535) << 16 | randint(1, 65535);
		m_HallKey[Index] = randKey;
		m_Service.Log(UCString("Hall_") + ITOS(Index) + UCString(":") + Url + UCString("刷新成功\r\n"));
		return UCString("成功");
	}
	UCString	RefeshMatch(const UCString& Url, int Index, const UCString& strKey, ucINT64& AdminKey, ucDWORD& randKey)
	{
		if (Index < 0 || Index >= MATCH_MAX)
			return UCString("Index越界");

		UCString strValue;
		if (strKey.IsEmpty() || !m_Admin_RCObj.m_CsvMatch.GetString(2 + Index, 2, strValue) || strKey != strValue)
			return UCString("密钥错误");

		//if (!CheckURLValid(Url))
		//	return UCString("RefeshMatch Url不通");

		CMatch_Info* Match_Info = &(m_Match_Info_RDObj.Match_Info[Index]);
		//设置为有效状态
		Match_Info->Open = 1;
		Match_Info->Url = Url;
		Match_Info->UpdateData();

		AdminKey = m_CenterKeyRCObj.m_AdminKey;
		randKey = randint(0, 65535) << 16 | randint(1, 65535);
		m_MatchKey[Index] = randKey;
		m_Service.Log(UCString("Match_") + ITOS(Index) + UCString(":") + Url + UCString("刷新成功\r\n"));
		return UCString("成功");
	}
	UCString	RefeshGame(const UCString& Url, int Index, const UCString& strKey, ucINT64& AdminKey, ucDWORD& randKey)
	{
		if (Index < 0 || Index >= GAME_MAX)
			return UCString("Index越界");

		UCString strValue;
		if (strKey.IsEmpty() || !m_Admin_RCObj.m_CsvGame.GetString(2 + Index, 2, strValue) || strKey != strValue)
			return UCString("密钥错误");

		//if (!CheckURLValid(Url))
		//	return UCString("RefeshGame Url不通");

		CGame_Info* Game_Info = &(m_Game_Info_RDObj.Game_Info[Index]);
		//设置为有效状态
		Game_Info->Open = 1;
		Game_Info->Url = Url;
		Game_Info->UpdateData();

		AdminKey = m_CenterKeyRCObj.m_AdminKey;
		randKey = randint(0, 65535) << 16 | randint(1, 65535);
		m_GameKey[Index] = randKey;
		m_Service.Log(UCString("Game_") + ITOS(Index) + UCString(":") + Url + UCString("刷新成功\r\n"));
		return UCString("成功");
	}
	UCString	RefeshService(const UCString& strName, const UCString& strKey, ucINT64& AdminKey)
	{
		int iPos = m_Admin_RCObj.m_mapService.FindKey(strName);
		if (iPos < 0 || m_Admin_RCObj.m_mapService.GetValueAt(iPos) != strKey)
			return UCString("密钥错误");

		AdminKey = m_CenterKeyRCObj.m_AdminKey;
		m_Service.Log(UCString("账号:") + strName + UCString("刷新成功\r\n"));
		return UCString("成功");
	}

	//注销一个Account
	UCString	UnRefeshAccount(int Index, ucDWORD randKey)
	{
		if (Index < 0 || Index >= ACCOUNT_MAX)
			return UCString("Index越界");
		if (m_AccountKey[Index] != randKey)
			return UCString("Key错误");
		m_AccountKey[Index] = 0;
		CAccount_Info* Account_Info = &(m_Account_Info_RDObj.Account_Info[Index]);
		//设置为无效状态
		Account_Info->Open = 0;
		Account_Info->Url = "";
		Account_Info->UpdateData();
		return UCString("成功");
	}
	//注销一个GameDB
	UCString	UnRefeshGameDB(int Index, ucDWORD randKey)
	{
		if (Index < 0 || Index >= GAMEDB_MAX)
			return UCString("Index越界");
		if (m_GameDBKey[Index] != randKey)
			return UCString("Key错误");
		m_GameDBKey[Index] = 0;
		CGameDB_Info* GameDB_Info = &(m_GameDB_Info_RDObj.GameDB_Info[Index]);
		//设置为无效状态
		GameDB_Info->Open = 0;
		GameDB_Info->Url = "";
		GameDB_Info->UpdateData();
		return UCString("成功");
	}
	//注销一个Hall
	UCString	UnRefeshHall(int Index, ucDWORD randKey)
	{
		if (Index < 0 || Index >= HALL_MAX)
			return UCString("Index越界");
		if (m_HallKey[Index] != randKey)
			return UCString("Key错误");
		m_HallKey[Index] = 0;
		CHall_Info* Hall_Info = &(m_Hall_Info_RDObj.Hall_Info[Index]);
		//设置为无效状态
		Hall_Info->Open = 0;
		Hall_Info->Url = "";
		Hall_Info->UpdateData();
		return UCString("成功");
	}
	//注销一个Match
	UCString	UnRefeshMatch(int Index, ucDWORD randKey)
	{
		if (Index < 0 || Index >= MATCH_MAX)
			return UCString("Index越界");
		if (m_MatchKey[Index] != randKey)
			return UCString("Key错误");
		m_MatchKey[Index] = 0;
		CMatch_Info* Match_Info = &(m_Match_Info_RDObj.Match_Info[Index]);
		//设置为无效状态
		Match_Info->Open = 0;
		Match_Info->Url = "";
		Match_Info->UpdateData();
		return UCString("成功");
	}
	//注销一个Game
	UCString	UnRefeshGame(int Index, ucDWORD randKey)
	{
		if (Index < 0 || Index >= GAME_MAX)
			return UCString("Index越界");
		if (m_GameKey[Index] != randKey)
			return UCString("Key错误");
		m_GameKey[Index] = 0;
		CGame_Info* Game_Info = &(m_Game_Info_RDObj.Game_Info[Index]);
		//设置为无效状态
		Game_Info->Open = 0;
		Game_Info->Url = "";
		Game_Info->UpdateData();
		return UCString("成功");
	}
public:
	CCenterService()
	{
		for (int i = 0; i < ACCOUNT_MAX; ++i)
		{
			m_AccountKey[i] = 0;
		}
		for (int i = 0; i < GAMEDB_MAX; ++i)
		{
			m_GameDBKey[i] = 0;
		}
		for (int i = 0; i < HALL_MAX; ++i)
		{
			m_HallKey[i] = 0;
		}
		for (int i = 0; i < MATCH_MAX; ++i)
		{
			m_MatchKey[i] = 0;
		}
		for (int i = 0; i < GAME_MAX; ++i)
		{
			m_GameKey[i] = 0;
		}
		m_FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		m_FiberToken.FiberEvent = UCEvent(this, OnFiberToken);
	}
	~CCenterService()
	{
	}
	//检查一个服务是否有效
	//int CheckURLValid(const UCString& Url)
	//{
	//	return 1;
	//	UCVF_ControlObj		ControlObj;
	//	m_Tick++;
	//	if (!m_Service.AppendLocalFile(m_Service.Name + UCString("//CheckURLValid/control") + ITOS(m_Tick) + UCString(".call"), typeof(UCVF_ControlObj), &ControlObj))
	//		return 0;
	//	CGame_Center_LocalCfg* localflg = m_Admin_RCObj.m_LocalCfg.GetData();
	//	int linkresult = ControlObj.Link(Url + UCString("//control.call"));
	//	m_Service.Log(UCString("连接:") + Url + UCString("//control.call,ret=") + ITOS(linkresult) + UCString("\r\n"));
	//	if (linkresult != 0)
	//		return 0;
	//	return 1;
	//}
	int	Init()
	{
		m_Admin_RCObj.Init();
		CGame_Center_LocalCfg* LocalCfg = m_Admin_RCObj.m_LocalCfg.GetData();
		if (LocalCfg->EnableLog && LocalCfg->LogUrl.Right(2) != UCString("//"))
		{
			MBox("LogUrl路径错误");
			return 0;
		}
		m_Service.Name = UCGetApp()->Name;
		m_CenterKeyRCObj.m_Password = LocalCfg->Password;
		UCGetSystemExt()->SetWindowCaption(UCString("GameCenter/") + m_Service.Name);

		//生成密钥
		for (ucINT i = 0; i < 8; i++)
		{
			ucINT64	c = randint(48, 122);
			m_CenterKeyRCObj.m_AdminKey |= c << (i * 8);
		}
		//设置密钥
		m_Service.SetAdminKey(m_CenterKeyRCObj.m_AdminKey);

		m_FiberInit.Start(0);
		if (LocalCfg->EnbaleTokenSwitch)
			m_FiberToken.Start(0);
		return 1;
	}
	void OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		m_Service.Log(UCString("start GameCenter:") + m_Service.Name + UCString("\r\n"));
		m_Service.SetValue(1);		//先对内状态

		CGame_Center_LocalCfg* LocalCfg = m_Admin_RCObj.m_LocalCfg.GetData();
		//优先连接日志系统
		if (LocalCfg->EnableLog)
		{
			CLog_RCObj logRCObj;
			m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CLog_RCObj.call"), typeof(CLog_RCObj), &logRCObj);
			m_Service.Log(UCString("开始连接游戏日志服务器...\r\n"));
			UCString linkadress = LocalCfg->LogUrl + UCString("CLog_RCObj.call");
			while (FiberData->IsValid())
			{
				int linkresult = logRCObj.Link(linkadress, ADMIN_PASSWORD);
				if (linkresult == 0)
					break;
				m_Service.Log(UCString("游戏日志服连接失败") + ITOS(linkresult) + UCString("\r\n"));
				FiberData->Every(10000);
			}
			m_Service.Log(UCString("游戏日志服连接成功\r\n"));
			int bEnablePosWindows = 1;
			UCString strRet = logRCObj.AddModule(UCString("GameCenter"), m_Service.Name, m_Service.StateObj->PID.Value, bEnablePosWindows);
			if (strRet == UCString("成功"))
			{
				//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
				if (bEnablePosWindows == 0)
					SetExceptionMode(2);
				m_Log.SetService(&m_Service);
				int ret = m_Log.Init(LocalCfg->LogUrl, UCString("GameCenter"), m_Service.Name, ADMIN_PASSWORD);
				if (ret)
					m_Service.Log(UCString("游戏日志服初始化失败") + ITOS(ret) + UCString("\r\n"));
			}
			else
			{
				m_Service.Log(UCString("注册游戏日志服失败:") + strRet + UCString("\r\n"));
			}
		}

		//添加本地服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CenterKeyRCObj.call"), typeof(CCenterKey_RCObj), typeof(CCenterKeyRCObj), &m_CenterKeyRCObj, UCVF_Guest_Permission_ReadWrite);
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CGame_Center_RCObj.call"), typeof(CGame_Center_RCObj), typeof(CCenterService), this, UCVF_Guest_Permission_ReadWrite);
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CCenterAdmin_RCObj.call"), typeof(CCenterAdmin_RCObj), typeof(CCenterAdminRCObj), &m_Admin_RCObj);
		//连接中心服务的Account列表
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CAccount_Info_RDObj.data"), typeof(CAccount_Info_RDObj), &m_Account_Info_RDObj);
		//连接中心服务的GameDB列表
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CGameDB_Info_RDObj.data"), typeof(CGameDB_Info_RDObj), &m_GameDB_Info_RDObj);
		//连接中心服务的Hall列表
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CHall_Info_RDObj.data"), typeof(CHall_Info_RDObj), &m_Hall_Info_RDObj);
		//连接中心服务的Match列表
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CMatch_Info_RDObj.data"), typeof(CMatch_Info_RDObj), &m_Match_Info_RDObj);
		//连接中心服务的Game列表
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CGame_Info_RDObj.data"), typeof(CGame_Info_RDObj), &m_Game_Info_RDObj);

		m_Service.Log(UCString("-----服务器启动完成-----\r\n\r\n"));
		m_Service.SetValue(2);		//可以对外了
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CToken_RDObj.data"), typeof(CToken_RDObj), &m_Token_RDObj);
	}
	void OnFiberToken(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;
		FiberData->Delay(10000 * 3);

		while (FiberData->IsValid())
		{
			UCString msgcode = UCString("https://developer.toutiao.com/api/apps/token?appid=") + BYTEDANCE_APPID + UCString("&secret=") + BYTEDANCE_SECRET + UCString("&grant_type=client_credential");
			UCString str;
			int result = m_CUrl.HttpGet(msgcode, str);
			UCJson json;
			if (json.Parse(str))
			{
				FiberData->Every(10000);
				continue;
			}
			if (json.isMember("errcode"))
			{
				if (json.GetMember("errcode").isInt())
				{
					FiberData->Every(10000);
					continue;
				}
			}
			if (!json.isMember("access_token"))
			{
				FiberData->Every(10000);
				continue;
			}
			UCJson tokenjson = json.GetMember("access_token");
			if (!tokenjson.isString())
			{
				FiberData->Every(10000);
				continue;
			}
			UCString strToken = tokenjson.asString();
			m_Service.Log(UCString("toutiao token:") + strToken + UCString("\r\n"));
			if (!strToken.IsEmpty() && m_Token_RDObj.m_strToken.Value != strToken)
				m_Token_RDObj.m_strToken = strToken;

			FiberData->Every(3600 * 10000);
		}
	}
public:
	void Run()
	{
		//初始化
		if (!Init())
			return;
		m_Service.Run(100);
	}
};

#endif	//_center_H_