#ifndef _data_
#define _data_

#include "../../common/game_database.h"
#include "../../common/game_centerbase.h"
#include "../../common/game_monitorbase.h"

//本地配置存档
struct UCGameDataInfo
{
	ucINT						Index;						//编号
	UCString					Password;					//连接中心服的密码
};

#define ROBJ_TIMEOUT	1
class UCRObjGameDataLocal : public UCRObjGameData
{
public:
	UCEApp*						App;
	UCString					LocalUrl;
	UCEConfig<UCGameDataInfo>	GameDataCfg;
	UCRContainer_NET			RContainer_NET;

	UCRObjGameCenter			RObjGameCenter;
	UCRObjGameMonitor			RObjGameMonitor;
public:
	//通用Key
	ucINT64						AdminKey;
	ucDWORD						RandKey;
	UCFiber						FiberInit;
public:
	UCString Active(ucCONST UCString& RObjectUrl, ucINT64& PassKey)
	{
		UCString strMirrorUrl = LocalUrl + UCString("//") + RObjectUrl;

		//查找
		UCRStruct* RMirror = RContainer_NET.FindRStruct(strMirrorUrl);
		if (RMirror)
		{
			PassKey = RMirror->LocalInfo.PassKey;
			return UCString("成功");
		}

		//加载
		UCRMessage RMessage;
		ucINT Code = LDB_Object.Get(LDB_ReadOptions, UCRMessage(strMirrorUrl), RMessage);
		if (Code != 0)
			return UCString("路径无数据");

		//创建随机钥匙
		ucINT64 NewPassKey = randint64();

		//ROBJ_TIMEOUT时间超时，是数据源
		RMirror = RContainer_NET.AppendMirror(strMirrorUrl, ROBJ_TIMEOUT, ucTRUE, ucFALSE, ucTRUE, NewPassKey);
		if (RMirror == ucNULL)
			return UCString("路径非法");

		//从数据序列化加载到对象
		if (!RMirror->From(RMessage))
		{
			RContainer_NET.RemoveRStruct(RMirror);
			return UCString("加载失败");
		}
		//设置钥匙
		PassKey = NewPassKey;
		//超时处理
		RMirror->OnTimeOut = UCEvent(this, OnRStructTimeOut);
		return UCString("成功");
	}
	UCString Mirror(ucCONST UCString& RObjectUrl, ucCONST UCRMessage& RMessage, ucINT64& PassKey)
	{
		UCString strMirrorUrl = LocalUrl + UCString("//") + RObjectUrl;

		//查找
		UCRStruct* RMirror = RContainer_NET.FindRStruct(strMirrorUrl);
		if (RMirror)
		{
			PassKey = RMirror->LocalInfo.PassKey;
			return UCString("成功");
		}

		//创建随机钥匙
		ucINT64 NewPassKey = randint64();

		//ROBJ_TIMEOUT时间超时，是数据源
		RMirror = RContainer_NET.AppendMirror(strMirrorUrl, ROBJ_TIMEOUT, ucTRUE, ucFALSE, ucTRUE, NewPassKey);
		if (RMirror == ucNULL)
			return UCString("添加失败");

		//从数据序列化加载到对象
		if (!RMirror->From(RMessage))
		{
			RContainer_NET.RemoveRStruct(RMirror);
			return UCString("加载失败");
		}
		//设置钥匙
		PassKey = NewPassKey;
		//超时处理
		RMirror->OnTimeOut = UCEvent(this, OnRStructTimeOut);
		return UCString("成功");
	}
public:
	UCRObjGameDataLocal()
	{
		App = ucNULL;
		FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		RContainer_NET.OnEnd = UCEvent(this, OnRContainer_End);
	}
	~UCRObjGameDataLocal()
	{
	}
	ucVOID OnRContainer_End(UCObject* Object, UCEventArgs*)
	{
	}
	ucVOID OnRStructTimeOut(UCObject* Object, UCEventArgs*)
	{
		UCRStruct* RStruct = (UCRStruct*)Object;

		ucINT Code = LDB_Object.Put(LDB_WriteOptions, UCRMessage(RStruct->LocalInfo.Url), UCRMessage(RStruct));
		if (Code != 0)
			RContainer_NET.Log(UCString("写入数据错误:") + ITOS(Code, UCString("%d\r\n")));
		else
		{
			RContainer_NET.Log(UCString("写入数据成功:") + RStruct->LocalInfo.Url + UCString("\r\n"));
			RContainer_NET.RemoveRStruct(RStruct);
		}
	}
	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		if (App->Index < 0)
			RContainer_NET.Log(UCString("start:") + App->Name + UCString("\r\n"));
		else
			RContainer_NET.Log(UCString("start:") + App->Name + ITOS(App->Index, UCString(":%d")) + UCString("\r\n"));

		UCGameDataInfo* DataInfo = GameDataCfg.GetData();
		ucINT AppIndex = App->Index;
		if (AppIndex < 0)
			AppIndex = 0;

		LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));
		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameData.obj"), typeof(UCRObjGameData), typeof(UCRObjGameDataLocal), this, ucTRUE);

		RContainer_NET.AppendClient(LocalUrl + UCString("//Local/UCRObjGameCenter.obj"), typeof(UCRObjGameCenter), &RObjGameCenter);

		RContainer_NET.AppendClient(LocalUrl + UCString("//Local/UCRObjGameMonitor.obj"), typeof(UCRObjGameMonitor), &RObjGameMonitor);
// 		RContainer_NET.Log(UCString("开始连接监控容器...\r\n"));
// 		while (FiberData->IsValid())
// 		{
// 			ucINT Code = RObjGameMonitor.Link(UCString("sm://monitor:0//UCRObjGameMonitor.obj"), DEFAULT_PASSWORD);
// 			if (Code == 0)
// 				break;
// 			//RContainer_NET.Log(UCString("监控容器连接失败") + ITOS(Code) + UCString("\r\n"));
// 			FiberData->Every(1000);
// 		}
// 		RContainer_NET.Log(UCString("监控连接成功\r\n"));
// 
// 		RObjGameMonitor.Gate_Register(Index, Monitor_Key, Monitor_PCU_MaxTotal, Monitor_PCU_MaxChannel, Monitor_PCU_Current);
// 		if (Index < 0 || Monitor_PCU_MaxTotal < 0)
// 			RContainer_NET.Log(UCString("注册监控容器失败\r\n"));
// 		else
// 			RContainer_NET.Log(UCString("注册监控容器成功\r\n"));

		RContainer_NET.Log(UCString("开始连接中心容器...\r\n"));
		while (FiberData->IsValid())
		{
			ucINT Code = RObjGameCenter.Link(UCString("sm://center:0//UCRObjGameCenter.obj"));
			if (Code == 0)
				break;
			RContainer_NET.Log(UCString("中心容器连接失败") + ITOS(Code) + UCString("\r\n"));
			FiberData->Every(10000);
		}
		RContainer_NET.Log(UCString("中心容器连接成功\r\n"));
		UCString Result = RObjGameCenter.GameData_Update(DataInfo->Index, DataInfo->Password, AdminKey, RandKey);
		if (Result != "成功")
		{
			RContainer_NET.Log(UCString("中心容器验证失败，请检查配置后重启容器 : ") + Result + UCString("\r\n"));
			return;
		}
		RContainer_NET.Log(UCString("验证成功\r\n"));

		RContainer_NET.SetAdminKey(AdminKey);

		RContainer_NET.Log(UCString("当前容器初始化完毕\r\n"));

		RContainer_NET.Log(UCString("初始化数据库\r\n"));
		InitLDB_Object();
		RContainer_NET.Log(UCString("初始化数据库完毕\r\n"));
	}

	ucINT Init()
	{
		App = UCGetApp();
		if (App->Name.IsEmpty())
			App->Name = UCString("data");

		UCFileExt_GetStatic().CreateFullPath(UCString("data"));
		if (!GameDataCfg.LoadFromXML(UCString("data/data.xml")))
		{
			UCGameDataInfo* DataInfo = GameDataCfg.GetData();
			DataInfo->Index = 0;
			DataInfo->Password = "123456";
			GameDataCfg.SaveToXML(UCString("data/data.xml"));
		}
		UCGameDataInfo* DataInfo = GameDataCfg.GetData();

		UCGetSystemExt()->SetWindowCaption(App->Name);
		//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
		SetExceptionMode(2);

		FiberInit.Start(0);
		return 1;
	}
	ucVOID Run()
	{
		if (!Init())
			return;
		RContainer_NET.Run(App->Name, App->Index, ucFALSE, ucTRUE, 10);
	}
	ucVOID InitLDB_Object()
	{
		UCString strDBUrl = UCString("data/leveldb/object");
		RContainer_NET.Log(UCString("LDB_Object路径:") + strDBUrl + UCString("\r\n"));
		UCFileExt_GetStatic().CreateFullPath(strDBUrl);

		UCLDB_Options Options;
		Options.create_if_missing = 1;
		//Options.error_if_exists = 1;

		ucINT Ret = LDB_Object.Open(Options, strDBUrl);
		if (Ret)
		{
			WBox(UCString("数据库打开失败:") + ITOS(Ret));
			return;
		}
	}
private:
	UCLevelDB						LDB_Object;
	UCLDB_ReadOptions				LDB_ReadOptions;					//读设置
	UCLDB_WriteOptions				LDB_WriteOptions;					//写设置
};

#endif	//_database_H_