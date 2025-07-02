#ifndef _moba_db_H_
#define _moba_db_H_

#include "../../common/game_account_db_robj.h"
#include "../../common/game_center_robj.h"
#include "../../common/log_robj.h"
#include "../../../private/common/game_db_robj.h"

#define USERID_MAX			0x7FFFFFFF					//最大是0x7FFFFFFF，后面的是保留ID，用作内部测试使用
#define USER_BASEID			0x2710						//定义用户基础ID，十进制10000

//本地配置存档
struct CGame_AccountDB_LocalCfg
{
	ucINT						EnableLog;					//是否开启日志
	UCString				LogUrl;						//日志系统地址

	UCString				UserName;					//连接中心服的账号
	UCString				Password;					//连接中心服的密码

	UCString				PrivateGate;				//内网Gate地址//结束（本机可以填空）
	UCString				DnsURL;						//DNS服路径//结束
};


//游戏账号数据情况
struct CGameDB
{
	ucINT			Open;				//是否对外开放
	ucINT			Index;				//ID

	ucDWORD		PlatID_Curr;		//UserGameID当前指针，最大是0x7FFFFFFF，后面的是保留ID，用作内部测试使用
};

//调试用快照
class CGameDB_Debug_RDObj : public UCVF_RemoteDataObj
{
public:
	UCVF_INT				UserID_Curr[GAMEDB_MAX];		//当前注册量
	UCVF_INT				ChannelNum[CANAL_MAX];			//渠道注册量
	UCVF_INT				DBDNU[GAMEDB_MAX];				//当日数据库新增
	UCVF_INT				ChannelDNU[CANAL_MAX];			//当日渠道新增
	UCVF_INT				DNU;							//当日总计新增
};

//账号数据服务器
class CGame_AccountDBService : public CGame_Account_DB_RCObj
{
	UCVF_Service			m_Service;					//服务模块
private:
	UCRocksDB				m_DB;						//数据库引擎
	UCRdbColumnFamily*		m_AccountCF;			//账号的列
	UCRdbColumnFamily*		m_AccountIDCF;			//账号ID的列，托管账户，可以通过m_AccountIDCF查到对应数据
	UCRdbColumnFamily*		m_GameDBCF;					//游戏数据的列
	UCRdbColumnFamily*		m_ChannelCF;				//用来统计渠道注册量
	UCRdbWriteOptions		m_WriteOptions;				//写参数
	UCRdbReadOptions		m_ReadOptions;				//读参数

	ucDWORD					m_ChannelNum[CANAL_MAX];			//渠道注册量
	UCString				m_ChannelName[CANAL_MAX];			//渠道前缀

	UCTime					m_DayTime;					//计算当天重置的时间
public:
	CGameDB					m_GameDB[GAMEDB_MAX];		//注册信息
	UCIntStringMap			m_MapGameDB_Open;			//对外
	UCIntIntMap				m_MapGameDB_UnOpen;			//对内
	UCIntIntMap				m_MapAccountNumGameDB;		//根据账号数量重新排序

	CGameDB_Info_RDObj		m_GameDB_Info_RDObj;		//从中心服务器获得的GameDB信息
private:
	CGame_Center_RCObj		m_Game_Center_RCObj;		//中央服务器的远程调用
	CGameDB_Debug_RDObj		m_GameDB_Debug_RDObj;		//调试数据

	UCEConfig<CGame_AccountDB_LocalCfg>	m_LocalCfg;		//服务器配置

	ucINT64					m_AdminKey;					//管理员密钥
	UCFiber					m_FiberInit;				//连接初始化
	UCFiber					m_FiberSort;				//定时排序
	UCLogCall				m_Log;
public:
	UCString LoadData(ucINT CanalID, const UCString& Username, UCString& ExtInfo, CUserGameID& UserGameID)
	{
		if (m_AccountCF == 0)
			return UCString("空列族");
		if (CanalID < 0 || CanalID >= CANAL_MAX || m_ChannelName[CanalID].IsEmpty())
			return UCString("渠道错误");

		UCString strData;
		ucINT ret = m_AccountCF->Get(m_ReadOptions, UCRdbSlicePin(m_ChannelName[CanalID] + Username), strData);
		if (ret)
			return UCString("Get失败：") + ITOS(ret);

		UCString strChannelID = strData.Left(3);
		if (STOI(strChannelID, UCString("%03x")) != CanalID)
			return UCString("渠道校验错误");

		UCString strDBIndex = strData.Mid(4, 8);
		UCString strUserID = strData.Mid(13, 8);
		UCString strExtInfo = strData.Mid(22);

		UserGameID.DBIndex = STOI(strDBIndex, UCString("%08x"));
		UserGameID.Index = STOI(strUserID, UCString("%08x"));
		ExtInfo = strExtInfo;

		return UCString("成功");
	}

	UCString SaveData(ucINT CanalID, const UCString& Username, const UCString& ExtInfo, CUserGameID& UserGameID)
	{
		if (m_AccountCF == 0 || m_AccountIDCF == 0 || m_GameDBCF == 0)
			return UCString("空列族");

		if (CanalID < 0 || CanalID >= CANAL_MAX || m_ChannelName[CanalID].IsEmpty())
			return UCString("渠道错误");

		if (m_MapAccountNumGameDB.GetSize() == 0)
			return UCString("没有效的服务器");

		UCString strData;
		ucINT ret = m_AccountCF->Get(m_ReadOptions, UCRdbSlicePin(m_ChannelName[CanalID] + Username), strData);
		if (ret == 0)
		{
			UCString strChannelID = strData.Left(3);
			if (STOI(strChannelID, UCString("%03x")) != CanalID)
				return UCString("渠道校验错误");

			UCString strDBIndex = strData.Mid(4, 8);
			UCString strUserID = strData.Mid(13, 8);
			UCString strExtInfo = strData.Mid(22);
			if (ExtInfo != strExtInfo)
			{
				//相当于修改密码
				//格式是3:8:8:string
				//strData = ITOS(CanalID, UCString("%03x:")) + ITOS(UserGameID.DBIndex, UCString("%08x:")) + ITOS(UserGameID.Index, UCString("%08x:")) + ExtInfo;
				strData = strData.Left(22) + ExtInfo;
				ret = m_AccountCF->Put(m_WriteOptions, UCRdbSlicePin(m_ChannelName[CanalID] + Username), UCRdbSlicePin(strData));
				if (ret)
					return UCString("存档失败");
			}
			UserGameID.DBIndex = STOI(strDBIndex, UCString("%08x"));
			UserGameID.Index = STOI(strUserID, UCString("%08x"));
			return UCString("成功");
		}
		else
		{
			if (ret != 1)
			{
				m_Service.Log(UCString("获取m_AccountCF数据错误:") + ITOS(ret) + UCString("\r\n"));
				return UCString("已注册") + ITOS(ret);
			}
		}

		//相当于注册
		ucINT iIndex = m_MapAccountNumGameDB.GetValueAt(0);
		CGameDB* pGameDB = &m_GameDB[iIndex];
		if (pGameDB->PlatID_Curr >= USERID_MAX)
			return UCString("服务器已满");

		//分配UserGameID
		UserGameID.DBIndex = pGameDB->Index;
		UserGameID.Index = pGameDB->PlatID_Curr + USER_BASEID;

		//格式是3:8:8:string
		strData = ITOS(CanalID, UCString("%03x:")) + ITOS(UserGameID.DBIndex, UCString("%08x:")) + ITOS(UserGameID.Index, UCString("%08x:")) + ExtInfo;

		UCRdbWriteBatch writebatch;	//批量处理，保证原子性
		ret = writebatch.Put(UCRdbSlicePin(m_ChannelName[CanalID] + Username), UCRdbSlicePin(strData), m_AccountCF);
		if (ret)
		{
			return UCString("Set失败5：") + ITOS(ret);
		}
		ret = writebatch.Put(UCRdbSlicePin(&UserGameID, sizeof(CUserGameID)), UCRdbSlicePin(m_ChannelName[CanalID] + Username), m_AccountIDCF);
		if (ret)
		{
			return UCString("Set失败6：") + ITOS(ret);
		}
		++pGameDB->PlatID_Curr;
		m_GameDB_Debug_RDObj.UserID_Curr[iIndex] = pGameDB->PlatID_Curr;
		ret = writebatch.Put(UCRdbSlicePin(&pGameDB->Index, sizeof(ucINT)), UCRdbSlicePin(&pGameDB->PlatID_Curr, sizeof(ucDWORD)), m_GameDBCF);
		if (ret)
		{
			return UCString("Set失败7：") + ITOS(ret);
		}
		++m_ChannelNum[CanalID];
		m_GameDB_Debug_RDObj.ChannelNum[CanalID] = m_ChannelNum[CanalID];
		ret = writebatch.Put(UCRdbSlicePin(&CanalID, sizeof(ucINT)), UCRdbSlicePin(&m_ChannelNum[CanalID], sizeof(ucDWORD)), m_ChannelCF);
		if (ret)
		{
			return UCString("Set失败8：") + ITOS(ret);
		}
		ret = m_DB.Write(m_WriteOptions, writebatch);
		if (ret)
		{
			return UCString("Set失败9：") + ITOS(ret);
		}
		m_GameDB_Debug_RDObj.DBDNU[iIndex] += 1;
		m_GameDB_Debug_RDObj.ChannelDNU[CanalID] += 1;
		m_GameDB_Debug_RDObj.DNU += 1;

		//OutputLog(UCString("GameDB_") + ITOS(GameDB->Index) + UCString("写入成功,UserName=") + Username + UCString(",UserID=") + ITOS(UserID.Index));
		return UCString("成功");
	}
	//修改玩家用户名（也可以用来把匿名用户修改成正式用户）
	UCString ChaneUserName(ucINT CanalID, const UCString& strOldUsername, const UCString& strNewUsername)
	{
		if (m_AccountCF == 0)
			return UCString("空列族");
		if (CanalID < 0 || CanalID >= CANAL_MAX || m_ChannelName[CanalID].IsEmpty())
			return UCString("渠道错误");

		UCString strData;
		ucINT ret = m_AccountCF->Get(m_ReadOptions, UCRdbSlicePin(m_ChannelName[CanalID] + strOldUsername), strData);
		if (ret)
			return UCString("读取失败:") + ITOS(ret);

		UCString strChannelID = strData.Left(3);
		ucINT readChannelID = STOI(strChannelID, UCString("%03x"));
		if (readChannelID != CanalID)
			return UCString("渠道校验错误");

		UCString strDataTemp;
		ret = m_AccountCF->Get(m_ReadOptions, UCRdbSlicePin(m_ChannelName[CanalID] + strNewUsername), strDataTemp);
		if (ret == 0)
		{
			if (strData.Left(22) == strDataTemp.Left(22))
				return UCString("成功");
			m_Service.Log(UCString("改名失败:old=") + m_ChannelName[CanalID] + strOldUsername + UCString(",new:") + m_ChannelName[CanalID] + strNewUsername + UCString(",oldValue:") + strData + UCString(",已存在:") + strDataTemp + UCString("\r\n"));
			return UCString("已被注册");
		}

		UCString strDBIndex = strData.Mid(4, 8);
		UCString strUserID = strData.Mid(13, 8);

		CUserGameID UserGameID;
		UserGameID.DBIndex = STOI(strDBIndex, UCString("%08x"));
		UserGameID.Index = STOI(strUserID, UCString("%08x"));
		UCString strValidName;
		ret = m_AccountIDCF->Get(m_ReadOptions, UCRdbSlicePin(&UserGameID, sizeof(CUserGameID)), strValidName);
		if (ret == 0)
		{
			if (strValidName != m_ChannelName[CanalID] + strOldUsername && strValidName != m_ChannelName[CanalID] + strNewUsername)
			{
				m_Service.Log(UCString("可能串号:") + m_ChannelName[CanalID] + strOldUsername + UCString(",valid:") + strValidName + UCString("\r\n"));
				return UCString("可能串号");
			}
		}

		//删除旧账号，增加新账号
		UCRdbWriteBatch writebatch;	//批量处理，保证原子性
		ret = writebatch.Put(UCRdbSlicePin(m_ChannelName[CanalID] + strNewUsername), UCRdbSlicePin(strData), m_AccountCF);
		if (ret)
			return UCString("改名失败1：") + ITOS(ret);
		ret = writebatch.Delete(UCRdbSlicePin(m_ChannelName[CanalID] + strOldUsername), m_AccountCF);
		if (ret)
			return UCString("改名失败2：") + ITOS(ret);

		ret = writebatch.Put(UCRdbSlicePin(&UserGameID, sizeof(CUserGameID)), UCRdbSlicePin(m_ChannelName[CanalID] + strNewUsername), m_AccountIDCF);
		if (ret)
			return UCString("改名失败3：") + ITOS(ret);
		ret = m_DB.Write(m_WriteOptions, writebatch);
		if (ret)
			return UCString("改名失败4：") + ITOS(ret);

		return UCString("成功");
	}
public:
	CGame_AccountDBService()
	{
		//定义每个平台的前缀
		m_ChannelName[CANAL_MAGICX] = UCString("MX_");

		m_AdminKey = 0;
		m_AccountCF = ucNULL;
		m_AccountIDCF = ucNULL;
		m_GameDBCF = ucNULL;
		m_ChannelCF = ucNULL;
		m_FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		m_FiberSort.FiberEvent = UCEvent(this, OnFiberSort);
	}
	~CGame_AccountDBService()
	{
	}
	ucVOID OnFiberSort(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		//每秒重新排序一次
		while (FiberData->IsValid())
		{
			m_MapAccountNumGameDB.RemoveAll();
			//只排序对外开放的
			for (ucINT i = 0; i < m_MapGameDB_Open.GetSize(); i++)
			{
				ucINT iIndex = m_MapGameDB_Open.GetKeyAt(i);
				m_MapAccountNumGameDB.Add(m_GameDB[iIndex].PlatID_Curr, iIndex);
			}

			if (UCTime() > m_DayTime)
			{
				for (ucINT i = 0; i < GAMEDB_MAX; ++i)
				{
					if (m_GameDB_Debug_RDObj.DBDNU[i].Value != 0)
						m_GameDB_Debug_RDObj.DBDNU[i] = 0;
				}
				for (ucINT i = 0; i < CANAL_MAX; ++i)
				{
					if (m_GameDB_Debug_RDObj.ChannelDNU[i].Value != 0)
						m_GameDB_Debug_RDObj.ChannelDNU[i] = 0;
				}
				m_GameDB_Debug_RDObj.DNU = 0;
				UCTime curTime;
				m_DayTime = UCTime(curTime.GetYear(), curTime.GetMonth(), curTime.GetDay() + 1, 0, 0, 0);
			}

			FiberData->Every(10000);
		}
	}
	ucINT Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("game_account_db"));
		if (!m_LocalCfg.LoadFromXML(UCString("game_account_db/game_account_db.xml")))
		{
			CGame_AccountDB_LocalCfg* LocalCfg = m_LocalCfg.GetData();
			LocalCfg->EnableLog = 1;
			LocalCfg->LogUrl = "GameLog//";

			LocalCfg->UserName = "GameAccountDB";
			LocalCfg->Password = "123456";
			LocalCfg->PrivateGate = "";
			LocalCfg->DnsURL = "Dns//";
			m_LocalCfg.SaveToXML(UCString("game_account_db/game_account_db.xml"));
		}
		CGame_AccountDB_LocalCfg* LocalCfg = m_LocalCfg.GetData();
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
		m_Service.Name = UCGetApp()->Name;
		UCGetSystemExt()->SetWindowCaption(UCString("GameAccountDB/") + m_Service.Name);
		if (!InitDB())
			return 0;
		m_FiberInit.Start(0);
		return 1;
	}
	ucINT InitDB()
	{
		if (m_DB.IsOpen())
			return 1;
		UCCsv csv;
		if (!csv.ReadFromFile("../../csv/rocksdbadress.csv"))
		{
			MBox("读取rocksdbadress.csv失败");
			return 0;
		}
		UCString dbaddress;
		if(!csv.GetString(2, 2, dbaddress)||dbaddress.IsEmpty())
		{
			MBox("读取rocksdbadress.csv文件内容失败");
			return 0;
		}

		//打开参数
		UCRdbOptions option;
		option.create_if_missing = 1;
		//option.error_if_exists = 1;
		option.create_missing_column_families = 1;

		UCRdbColumnFamilyOptions cfOptions;
		UCRdbColumnFamilyOptionsMap mapCF;
		mapCF.Add(UCString("Account"), cfOptions);
		mapCF.Add(UCString("AccountID"), cfOptions);
		mapCF.Add(UCString("GameDB"), cfOptions);
		mapCF.Add(UCString("Channel"), cfOptions);
		mapCF.Add(UCString("default"), cfOptions);
		UCString strDB = dbaddress + UCString("account");
		m_Service.Log(UCString("RocksDB路径:") + strDB + UCString("\r\n"));
		UCFileExt_GetStatic().CreateFullPath(strDB);
		ucINT openret = m_DB.Open(option, strDB, mapCF);
		if (openret)
		{
			WBox(UCString("数据库打开失败:") + ITOS(openret));
			return 0;
		}
		m_AccountCF = m_DB.GetColumnFamily(UCString("Account"));
		if (m_AccountCF == 0)
		{
			MBox("打开Account列族失败");
			return 0;
		}
		m_AccountIDCF = m_DB.GetColumnFamily(UCString("AccountID"));
		if (m_AccountIDCF == 0)
		{
			MBox("打开AccountID列族失败");
			return 0;
		}
		m_GameDBCF = m_DB.GetColumnFamily(UCString("GameDB"));
		if (m_GameDBCF == 0)
		{
			MBox("打开UserData列族失败");
			return 0;
		}
		m_ChannelCF = m_DB.GetColumnFamily(UCString("Channel"));
		if (m_ChannelCF == 0)
		{
			MBox("打开Channel列族失败");
			return 0;
		}
		UCString strInfo = "读取UserData注册量：\r\n\t\t";
		for (ucINT i = 0; i < GAMEDB_MAX; i++)
		{
			m_GameDB[i].Open = 0;
			m_GameDB[i].Index = i;
			m_GameDB[i].PlatID_Curr = 0;
			//如果读取失败
			if (!ReadGameDB(&m_GameDB[i]))
			{
				m_GameDB_Debug_RDObj.UserID_Curr[i] = 0;
				MBox("ReadGameDB失败");
				return 0;
			}
			if (i % 64 == 63)
				strInfo += ITOS(m_GameDB[i].PlatID_Curr) + UCString("\r\n\t\t");
			else
				strInfo += ITOS(m_GameDB[i].PlatID_Curr, UCString("%d,"));
			m_GameDB_Debug_RDObj.UserID_Curr[i] = m_GameDB[i].PlatID_Curr;
			m_GameDB_Debug_RDObj.DBDNU[i] = 0;
		}
		for (ucINT i = 0; i < CANAL_MAX; ++i)
		{
			if (m_ChannelName[i].IsEmpty())
			{
				MBox("渠道名为空，请检查");
				return 0;
			}
			m_ChannelNum[i] = 0;
			m_GameDB_Debug_RDObj.ChannelDNU[i] = 0;
			m_GameDB_Debug_RDObj.ChannelNum[i] = 0;
		}
		m_GameDB_Debug_RDObj.DNU = 0;
		if (!ReadChannel())
		{
			MBox("读取渠道统计失败");
			return 0;
		}

		UCTime curTime;
		m_DayTime = UCTime(curTime.GetYear(), curTime.GetMonth(), curTime.GetDay() + 1, 0, 0, 0);
		m_Service.Log(strInfo);
		return 1;
	}

	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		m_Service.Log(UCString("start GameAccountDB:") + m_Service.Name + UCString("\r\n"));
		m_Service.SetValue(1);		//先对内状态

		CGame_AccountDB_LocalCfg* LocalCfg = m_LocalCfg.GetData();
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
			UCString strRet = logRCObj.AddModule(UCString("GameAccountDB"), m_Service.Name, m_Service.StateObj->PID.Value, bEnablePosWindows);
			if (strRet == UCString("成功"))
			{
				//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
				if (bEnablePosWindows == 0)
					SetExceptionMode(2);
				m_Log.SetService(&m_Service);
				ucINT ret = m_Log.Init(LocalCfg->LogUrl, UCString("GameAccountDB"), m_Service.Name, ADMIN_PASSWORD);
				if (ret)
					m_Service.Log(UCString("平台日志服初始化失败") + ITOS(ret) + UCString("\r\n"));
			}
			else
			{
				m_Service.Log(UCString("注册平台日志服失败:") + strRet + UCString("\r\n"));
			}
		}

		//暂停一段时间，让GameCenter启动完成
		//FiberData->Delay(1000);

		//添加本地的account服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//CGame_Account_DB_RCObj.call"), typeof(CGame_Account_DB_RCObj), typeof(CGame_AccountDBService), this, UCVF_Guest_Permission_Disable);
		//连接中心服务
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGame_Center_RCObj.call"), typeof(CGame_Center_RCObj), &m_Game_Center_RCObj);
		//连接中心服务的GameDB列表
		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGameDB_Info_RDObj.data"), typeof(CGameDB_Info_RDObj), &m_GameDB_Info_RDObj);

		m_Service.AppendLocalFile(m_Service.Name + UCString("//Local/CGameDB_Debug_RDObj.data"), typeof(CGameDB_Debug_RDObj), &m_GameDB_Debug_RDObj);

		for (ucINT i = 0; i < GAMEDB_MAX; i++)
		{
			m_GameDB_Info_RDObj.GameDB_Info[i].BindOnSet(UCEvent(this, OnGameDB_InfoOnChanged));
		}
		m_GameDB_Info_RDObj.OnReset = UCEvent(this, OnMyGameDBInfoReset);

		m_Service.Log(UCString("开始连接中心服务器...\r\n"));
		while (FiberData->IsValid())
		{
			//连接中心服务器
			ucINT linkresult = m_Game_Center_RCObj.Link(LocalCfg->PrivateGate + UCString("dom://GameCenter//CGame_Center_RCObj.call"));
			if (linkresult == 0)
				break;
			//m_Service.Log(UCString("中心连接失败") + ITOS(linkresult) + UCString("\r\n"));
			FiberData->Every(1000);
		}
		m_Service.Log(UCString("中心服连接成功\r\n"));

		UCString Result = m_Game_Center_RCObj.RefeshService(LocalCfg->UserName, LocalCfg->Password, m_AdminKey);
		if (Result != "成功")
		{
			m_Service.Log(UCString("游戏中心服验证失败，请检查配置后重启服务 : ") + Result + UCString("\r\n"));
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
			m_Service.Log(UCString("中心的GameDB列表连接失败\r\n"));
			FiberData->Every(1000);
		}

		m_Service.Log(UCString("-----服务器启动完成-----\r\n\r\n"));
		m_Service.SetValue(2);		//可以对外了
		m_Service.SetTTL(DEF_DNS_TTL);
		m_FiberSort.Run(0);
	}
	ucVOID OnMyGameDBInfoReset(UCObject*, UCEventArgs*)
	{
		m_MapGameDB_Open.RemoveAll();
		m_MapGameDB_UnOpen.RemoveAll();
		for (ucINT i = 0; i < GAMEDB_MAX; i++)
		{
			if (m_GameDB_Info_RDObj.GameDB_Info[i].Url.IsEmpty())
				continue;
			if (m_GameDB_Info_RDObj.GameDB_Info[i].Open)
			{
				m_MapGameDB_Open.Add(i, m_GameDB_Info_RDObj.GameDB_Info[i].Url);
			}
			else
			{
				m_MapGameDB_UnOpen.Add(i, 0);
			}
		}
	}
	ucVOID OnGameDB_InfoOnChanged(UCObject*, UCEventArgs* e)
	{
		UCVF_PropertyBaseCmd_Args* Args = (UCVF_PropertyBaseCmd_Args*)e;
		
		CGameDB_Info* pInfo = (CGameDB_Info*)Args->Data;
		ucINT iIndex = m_GameDB_Info_RDObj.GameDB_Info[0].GetArrayIndex(pInfo);

		m_Service.Log(UCString("收到服务消息 GameDB[") + ITOS(iIndex) + UCString("].url=") + pInfo->Url + UCString("\r\n"));

		//如果服务地址为空，不认为是合法的地址
		if (pInfo->Url.IsEmpty())
		{
			m_MapGameDB_Open.Remove(iIndex);
			m_MapGameDB_UnOpen.Add(iIndex, 0);
			return;
		}

		if (pInfo->Open)
		{
			m_MapGameDB_Open.Add(iIndex, pInfo->Url);
			m_MapGameDB_UnOpen.Remove(iIndex);
		}
		else
		{
			m_MapGameDB_Open.Remove(iIndex);
			m_MapGameDB_UnOpen.Add(iIndex, 0);
		}
	}
	ucINT ReadGameDB(CGameDB* pGameDB)
	{
		UCRdbSlicePin pinValue;
		ucINT ret = m_GameDBCF->Get(m_ReadOptions, UCRdbSlicePin(&pGameDB->Index, sizeof(ucINT)), pinValue);
		if (ret != 0)
		{
			if (ret == 1)
			{
				pGameDB->PlatID_Curr = 0;
				return WriteGameDB(pGameDB);
			}
			m_Service.Log(UCString("读取失败:ret=") + ITOS(ret) + UCString(",GameDB[") + ITOS(pGameDB->Index) + UCString("\r\n"));
			return 0;
		}
		pGameDB->PlatID_Curr = pinValue.ToInt();
		//m_Service.Log(UCString("读取成功:GameDB[") + ITOS(pGameDB->Index) + UCString("].PlatID_Curr=") + ITOS(pGameDB->PlatID_Curr) + UCString("\r\n"));
		return 1;
	}
	ucINT WriteGameDB(CGameDB* pGameDB)
	{
		ucINT ret = m_GameDBCF->Put(m_WriteOptions, UCRdbSlicePin(&pGameDB->Index, sizeof(ucINT)), UCRdbSlicePin(&pGameDB->PlatID_Curr, sizeof(ucDWORD)));
		if (ret)
		{
			m_Service.Log(UCString("写入失败:ret=") + ITOS(ret) + UCString(",GameDB[") + ITOS(pGameDB->Index) + UCString("].PlatID_Curr=") + ITOS(pGameDB->PlatID_Curr) + UCString("\r\n"));
			return 0;
		}
		//m_Service.Log(UCString("写入成功:GameDB[") + ITOS(pGameDB->Index) + UCString("].PlatID_Curr=") + ITOS(pGameDB->PlatID_Curr) + UCString("\r\n"));
		return 1;
	}
	ucINT ReadChannel()
	{
		UCRdbIterator* pIt = m_ChannelCF->NewIterator(m_ReadOptions);
		if (pIt == 0)
			return 0;
		ucINT ret = pIt->SeekToFirst();
		if (ret)
		{
			m_Service.Log(UCString("读取Channel失败:ret=") + ITOS(ret) + UCString("\r\n"));
			return 0;
		}
		while (pIt->Valid())
		{
			ucINT iIndex = pIt->key().ToInt();
			if (iIndex >= 0 && iIndex < CANAL_MAX)
			{
				m_ChannelNum[iIndex] = pIt->value().ToInt();
				m_GameDB_Debug_RDObj.ChannelNum[iIndex] = m_ChannelNum[iIndex];
			}
			ret = pIt->Next();
			if (ret)
			{
				m_Service.Log(UCString("Channel迭代Next失败:") + ITOS(ret) + UCString("\r\n"));
				return 0;
			}
		}
		return 1;
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

#endif	//_moba_db_H_