#ifndef _accountdata_H_
#define _accountdata_H_

#include "../../common/game_accountdatabase.h"
#include "../../common/game_centerbase.h"

//本地配置存档
struct UCGameAccountDataCfg
{
	UCString		Key;						//密钥
	UCString		PrivateGate;
};

struct UCGameAccountDataLevelDBCfg
{
	UCString		Url;
};

//调试用快照
class UCRObjGameData_Debug : public UCRObject
{
public:
	UCRInt			Data_Total[DATA_MAX];			//当前注册量
	UCRInt			Data_DNU[DATA_MAX];				//当日数据库新增

	UCRInt			Channel_Total[CHANNEL_MAX];		//渠道注册量
	UCRInt			Channel_DNU[CHANNEL_MAX];		//当日渠道新增

	UCRInt			DNU;							//当日总计新增
};

struct UCGameData_Channel
{
	ucDWORD			Total;
	UCString		Name;
};

struct UCRObjGameData_InfosLocal : public UCRObjGameData_Infos
{
};

struct UCGameUser_AccountData
{
	UCGameUserID	GameUserID;
	UCGameUserExt	GameUserExt;
};

//账号数据服务器
class UCRObjGameAccountDataLocal : public UCRObjGameAccountData
{
public:
	UCString Login(ucINT ChannelID, ucCONST UCString& Username, ucCONST UCString& Password, UCGameUserID& GameUserID)
	{
		if (MapAccountTotalGameData.GetSize() == 0)
			return UCString("区域没有效的服务器");

		if (ChannelID < 0 || ChannelID >= CHANNEL_MAX)
			return UCString("渠道错误");

		UCGameData_Channel* GameData_Channel = &(GameData_Channels[ChannelID]);

		UCString FullName = Username + UCString("@") + GameData_Channel->Name;

		UCRMessage RMessage;
		ucINT Code = LDB_Account.Get(LDB_ReadOption, UCRMessage(FullName), RMessage);

		if (Code == 0)
		{
			ucCONST UCGameUser_AccountData& GameUser_AccountData = *((ucCONST UCGameUser_AccountData*)RMessage.RUnpacker.GetLastBuffer());
			if (GameUser_AccountData.GameUserExt.ChannelID != ChannelID)
				return UCString("渠道校验错误");

			UCString strPassword = GameUser_AccountData.GameUserExt.Password.ToString();
			if (Password != strPassword)
				return UCString("密码错误");

			GameUserID.Set(GameUser_AccountData.GameUserID);
			return UCString("成功");
		}
		else
		{
			if (Code != 1)
			{
				RContainer_NET.Log(UCString("获取RdbCF_Account数据错误:") + ITOS(Code) + UCString("\r\n"));
				return UCString("已注册") + ITOS(Code);
			}
		}

		ucINT Data_Index = MapAccountTotalGameData.GetValueAt(0);
		ucINT Data_Total = RObjGameData_Debug.Data_Total[Data_Index].Value;
		if (Data_Total >= DATA_USERMAX)
			return UCString("区域服务器已满");

		GameUserID.SetDataID(Data_Index);
		GameUserID.SetUserID(Data_Total);

		UCGameUser_AccountData GameUser_AccountData;
		GameUser_AccountData.GameUserID.Set(GameUserID);
		GameUser_AccountData.GameUserExt.ChannelID = ChannelID;
		GameUser_AccountData.GameUserExt.Password = Password;

		UCLDB_WriteGroup	LDB_WriteGroup(LDB_ReadOption, LDB_WriteOption);
		Code = LDB_WriteGroup.Put(UCRMessage(FullName), UCRMessage(&GameUser_AccountData, sizeof(GameUser_AccountData)), LDB_Account);
		if (Code)
			return UCString("Put用户名失败：") + ITOS(Code);

		Code = LDB_WriteGroup.Put(UCRMessage(&GameUserID, sizeof(UCGameUserID)), UCRMessage(FullName), LDB_AccountID);
		if (Code)
			return UCString("Put用户ID失败：") + ITOS(Code);

		RObjGameData_Debug.Data_Total[Data_Index] = RObjGameData_Debug.Data_Total[Data_Index].Value + 1;
		Code = LDB_WriteGroup.Put(UCRMessage(&Data_Index, sizeof(ucINT)), UCRMessage(&Data_Total, sizeof(ucDWORD)), LDB_GameData);
		if (Code)
			return UCString("Put用户数量错误：") + ITOS(Code);

		Total++;
		GameData_Channel->Total++;
		RObjGameData_Debug.Channel_Total[ChannelID] += 1;

		Code = LDB_WriteGroup.Put(UCRMessage(&ChannelID, sizeof(ucINT)), UCRMessage(&RObjGameData_Debug.Channel_Total[ChannelID].Value, sizeof(ucINT)), LDB_Channel);
		if (Code)
			return UCString("Put渠道数量错误：") + ITOS(Code);

		RObjGameData_Debug.Data_DNU[Data_Index] += 1;
		RObjGameData_Debug.Channel_DNU[ChannelID] += 1;
		RObjGameData_Debug.DNU += 1;

		return UCString("成功");
	}
private:
	UCLDB_ReadOptions				LDB_ReadOption;
	UCLDB_WriteOptions				LDB_WriteOption;
public:
	UCRObjGameData_Debug			RObjGameData_Debug;				//调试数据
public:
	ucDWORD							Total;

	UCGameData_Channel				GameData_Channels[CHANNEL_MAX];	//渠道统计

	UCIntIntMap						MapAccountTotalGameData;		//根据账号数量重新排序
	UCIntIntMap						MapGameData_Valid;				//有效
	UCIntIntMap						MapGameData_Unvalid;			//无效
public:
	UCLevelDB						LDB_Account;
	UCLevelDB						LDB_AccountID;
	UCLevelDB						LDB_Channel;
	UCLevelDB						LDB_GameData;
	UCRObjGameCenter				RObjGameCenter;					//渠道中央对象
	UCRObjGameData_InfosLocal		RObjGameData_Infos;				//渠道中心对象获得的GameData信息
private:
	UCTime							CurrDayTime;					//计算当天重置的时间

	ucINT64							AdminKey;						//管理员密钥
	ucDWORD							CenterKey;						//中心服的临时key
	UCFiber							FiberInit;						//连接初始化
	UCFiber							FiberSort;						//定时排序
private:
	UCEApp*							App;
	UCRContainer_NET				RContainer_NET;

	UCEConfig<UCGameAccountDataCfg>	GameAccountDataCfg;		//服务器配置
public:
	UCRObjGameAccountDataLocal()
	{
		App = UCGetApp();

		Total = 0;

		GameData_Channels[Channel_MagicX].Total = 0;
		GameData_Channels[Channel_MagicX].Name = UCString("MX_");

		CenterKey = 0;
		AdminKey = 0;
		FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		FiberSort.FiberEvent = UCEvent(this, OnFiber_Sort);
	}
	~UCRObjGameAccountDataLocal()
	{
	}
	ucVOID OnFiber_Sort(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;
		while (FiberData->IsValid())
		{
			MapAccountTotalGameData.RemoveAll();
			for (ucINT i = 0; i < MapGameData_Valid.GetSize(); i++)
			{
				ucINT Index = MapGameData_Valid.GetKeyAt(i);
				MapAccountTotalGameData.Add(RObjGameData_Debug.Data_Total[Index].Value, Index);
			}

			if (UCTime() > CurrDayTime)
			{
				for (ucINT i = 0; i < DATA_MAX; ++i)
				{
					if (RObjGameData_Debug.Data_DNU[i].Value != 0)
						RObjGameData_Debug.Data_DNU[i] = 0;
				}
				for (ucINT i = 0; i < CHANNEL_MAX; ++i)
				{
					if (RObjGameData_Debug.Channel_DNU[i].Value != 0)
						RObjGameData_Debug.Channel_DNU[i] = 0;
				}
				RObjGameData_Debug.DNU = 0;
				UCTime curTime;
				CurrDayTime = UCTime(curTime.GetYear(), curTime.GetMonth(), curTime.GetDay() + 1, 0, 0, 0);
			}
			FiberData->Every(10000);
		}
	}
	ucBOOL Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("accountdata"));
		if (!GameAccountDataCfg.LoadFromXML(UCString("accountdata/accountdata.xml")))
		{
			UCGameAccountDataCfg* LocalCfg = GameAccountDataCfg.GetData();

			LocalCfg->Key = "123456";
			LocalCfg->PrivateGate = "";
			GameAccountDataCfg.SaveToXML(UCString("accountdata/accountdata.xml"));
		}
		UCGameAccountDataCfg* LocalCfg = GameAccountDataCfg.GetData();
		if (!LocalCfg->PrivateGate.IsEmpty() && LocalCfg->PrivateGate.Right(2) != UCString("//"))
		{
			WBox(UCString("PrivateGate路径错误：") + LocalCfg->PrivateGate);
			return ucFALSE;
		}

		if (App->Name.IsEmpty())
			App->Name = UCString("accountdata");

		UCGetSystemExt()->SetWindowCaption(UCString("accountdata/") + App->Name);
		if (!InitData())
			return ucFALSE;

		//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
		SetExceptionMode(0);

		FiberInit.Start(0);
		return ucTRUE;
	}
	ucBOOL InitData()
	{
		UCEConfig<UCGameAccountDataLevelDBCfg>	GameAccountDataLevelDBCfg;
		if (!GameAccountDataLevelDBCfg.LoadFromXML(UCString("accountdata/xml/leveldb.xml")))
		{
			UCGameAccountDataLevelDBCfg* LevelDBCfg = GameAccountDataLevelDBCfg.GetData();
			LevelDBCfg->Url = UCString("accountdata/leveldb");
			GameAccountDataLevelDBCfg.SaveToXML(UCString("accountdata/xml/leveldb.xml"));
			MBox("读取leveldb.xml失败");
			return ucFALSE;
		}
		UCGameAccountDataLevelDBCfg* LevelDBCfg = GameAccountDataLevelDBCfg.GetData();

		//打开参数
		UCLDB_Options LDB_Options;
		LDB_Options.create_if_missing = 1;
		//LDB_Options.error_if_exists = 1;

		if (!LDB_Account.IsOpen())
		{
			UCString strPath_Account = LevelDBCfg->Url + UCString("/account");
			RContainer_NET.Log(UCString("Account LevelDB路径:") + strPath_Account + UCString("\r\n"));
			UCFileExt_GetStatic().CreateFullPath(strPath_Account);
			ucINT Code = LDB_Account.Open(LDB_Options, strPath_Account);
			if (Code)
			{
				WBox(UCString("Account LevelDB打开失败:") + ITOS(Code));
				return ucFALSE;
			}
		}

		if (!LDB_AccountID.IsOpen())
		{
			UCString strPath_AccountID = LevelDBCfg->Url + UCString("/accountid");
			RContainer_NET.Log(UCString("AccountID LevelDB路径:") + strPath_AccountID + UCString("\r\n"));
			UCFileExt_GetStatic().CreateFullPath(strPath_AccountID);
			ucINT Code = LDB_AccountID.Open(LDB_Options, strPath_AccountID);
			if (Code)
			{
				WBox(UCString("AccountID LevelDB打开失败:") + ITOS(Code));
				return ucFALSE;
			}
		}

		if (!LDB_Channel.IsOpen())
		{
			UCString strPath_Channel = LevelDBCfg->Url + UCString("/channel");
			RContainer_NET.Log(UCString("Channel LevelDB路径:") + strPath_Channel + UCString("\r\n"));
			UCFileExt_GetStatic().CreateFullPath(strPath_Channel);
			ucINT Code = LDB_Channel.Open(LDB_Options, strPath_Channel);
			if (Code)
			{
				WBox(UCString("Channel LevelDB打开失败:") + ITOS(Code));
				return ucFALSE;
			}
		}

		if (!LDB_GameData.IsOpen())
		{
			UCString strPath_GameData = LevelDBCfg->Url + UCString("/gamedata");
			RContainer_NET.Log(UCString("GameData LevelDB路径:") + strPath_GameData + UCString("\r\n"));
			UCFileExt_GetStatic().CreateFullPath(strPath_GameData);
			ucINT Code = LDB_GameData.Open(LDB_Options, strPath_GameData);
			if (Code)
			{
				WBox(UCString("GameData LevelDB打开失败:") + ITOS(Code));
				return ucFALSE;
			}

			UCString strInfo = "读取UserData注册量：\r\n\t\t";
			for (ucINT j = 0; j < DATA_MAX; j++)
			{
				//如果读取失败
				if (!ReadGameData(j))
				{
					RObjGameData_Debug.Data_Total[j] = 0;
					MBox("ReadGameData失败");
					return ucFALSE;
				}
				if (j % 64 == 63)
					strInfo += ITOS(RObjGameData_Debug.Data_Total[j].Value) + UCString("\r\n\t\t");
				else
					strInfo += ITOS(RObjGameData_Debug.Data_Total[j].Value, UCString("%d,"));
				RObjGameData_Debug.Data_DNU[j] = 0;
			}

			for (ucINT j = 0; j < CHANNEL_MAX; ++j)
			{
				if (GameData_Channels[j].Name.IsEmpty())
				{
					MBox("渠道名为空，请检查");
					return ucFALSE;
				}
				GameData_Channels[j].Total = 0;
				RObjGameData_Debug.Channel_DNU[j] = 0;
				RObjGameData_Debug.Channel_Total[j] = 0;
			}

			RObjGameData_Debug.DNU = 0;

			if (!ReadChannel())
			{
				MBox("读取渠道统计失败");
				return ucFALSE;
			}

			RContainer_NET.Log(strInfo);

			UCTime curTime;
			CurrDayTime = UCTime(curTime.GetYear(), curTime.GetMonth(), curTime.GetDay() + 1, 0, 0, 0);
		}
		return ucTRUE;
	}

	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		if (App->Index < 0)
			RContainer_NET.Log(UCString("start:") + App->Name + UCString("\r\n"));
		else
			RContainer_NET.Log(UCString("start:") + App->Name + ITOS(App->Index, UCString(":%d")) + UCString("\r\n"));

		UCGameAccountDataCfg* LocalCfg = GameAccountDataCfg.GetData();

		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));
		
		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameAccountData.obj"), typeof(UCRObjGameAccountData), typeof(UCRObjGameAccountDataLocal), this, ucFALSE);
		{
			RContainer_NET.AppendClient(LocalUrl + UCString("//UCRObjGameCenter.obj"), typeof(UCRObjGameCenter), &RObjGameCenter);
			RContainer_NET.AppendClient(LocalUrl + UCString("//UCRObjGameData_Infos.obj"), typeof(UCRObjGameData_Infos), &RObjGameData_Infos);
			RContainer_NET.AppendClient(LocalUrl + UCString("//UCRObjGameData_Debug.obj"), typeof(UCRObjGameData_Debug), &RObjGameData_Debug);

			for (ucINT j = 0; j < DATA_MAX; j++)
			{
				RObjGameData_Infos.GameData_Infos[j].AppendEvent(UCEvent(this, GameData_InfosOnChanged));
			}
			RObjGameData_Infos.OnReset = UCEvent(this, GameData_InfosOnReset);

			UCString strRemoteUrl = LocalCfg->PrivateGate + UCString("sm://center:0");

			RContainer_NET.Log(UCString("开始连接中心容器\r\n"));
			while (FiberData->IsValid())
			{
				ucINT Code = RObjGameCenter.Link(strRemoteUrl + UCString("//UCRObjGameCenter.obj"));
				if (Code == 0)
					break;
				FiberData->Every(1000);
			}
			RContainer_NET.Log(UCString("中心服连接成功!\r\n"));

			UCString Result = RObjGameCenter.GameAccountData_Update(App->Index, LocalCfg->Key, AdminKey, CenterKey);
			if (Result != "成功")
			{
				RContainer_NET.Log(UCString("游戏中心服验证失败，请检查配置后重启服务 : ") + Result + UCString("\r\n"));
				return;
			}
			RContainer_NET.Log(UCString("验证成功\r\n"));

			RContainer_NET.SetAdminKey(AdminKey);
			while (FiberData->IsValid())
			{
				ucINT Code = RObjGameData_Infos.Link(strRemoteUrl + UCString("//UCRObjGameData_Infos.obj"), UCROBJECT_LINK_STABLE, AdminKey);
				if (Code == 0)
				{
					RContainer_NET.Log(UCString("中心的GameData列表连接成功\r\n"));
					break;
				}
				RContainer_NET.Log(UCString("中心的GameData列表连接失败\r\n"));
				FiberData->Every(1000);
			}

			RContainer_NET.Log(UCString("当前容器初始化完毕\r\n"));
		}

		FiberSort.Run(0);
	}
	ucVOID GameData_InfosOnReset(UCObject* Object, UCEventArgs*)
	{
		MapGameData_Valid.RemoveAll();
		MapGameData_Unvalid.RemoveAll();
		for (ucINT i = 0; i < DATA_MAX; i++)
		{
			if (RObjGameData_Infos.GameData_Infos[i].Valid)
				MapGameData_Valid.Add(i, i);
			else
				MapGameData_Unvalid.Add(i, i);
		}
	}
	ucVOID GameData_InfosOnChanged(UCObject* Object, UCEventArgs* e)
	{
		UCRPropertyArgs* Args = (UCRPropertyArgs*)e;

		UCGameData_Info* GameData_Info = (UCGameData_Info*)Args->Data;
		ucINT GameDataIndex = RObjGameData_Infos.GameData_Infos[0].GetArrayIndex(GameData_Info);

		RContainer_NET.Log(UCString("收到服务消息 GameData[") + ITOS(GameDataIndex) + UCString("]\r\n"));

		if (GameData_Info->Valid)
		{
			MapGameData_Valid.Add(GameDataIndex, GameDataIndex);
			MapGameData_Unvalid.Remove(GameDataIndex);
		}
		else
		{
			MapGameData_Valid.Remove(GameDataIndex);
			MapGameData_Unvalid.Add(GameDataIndex, GameDataIndex);
		}
	}
	ucBOOL ReadGameData(ucINT DataIndex)
	{
		UCRMessage RMessage;
		ucINT Code = LDB_GameData.Get(LDB_ReadOption, UCRMessage(&DataIndex, sizeof(ucINT)), RMessage);
		if (Code != 0)
		{
			if (Code == 1)
			{
				RObjGameData_Debug.Data_Total[DataIndex] = 0;
				return WriteGameData(DataIndex);
			}
			RContainer_NET.Log(UCString("读取失败:Code=") + ITOS(Code) + UCString(",GameData[") + ITOS(DataIndex) + UCString("\r\n"));
			return ucFALSE;
		}

		ucUINT Value = 0;
		if (!RMessage.RUnpacker.PopUINT32(Value))
		{
			RContainer_NET.Log(UCString("数据解析出错:") + UCString("GameData[") + ITOS(DataIndex) + UCString("\r\n"));
			return ucFALSE;
		}

		RObjGameData_Debug.Data_Total[DataIndex] = Value;
		//RContainer_NET.Log(UCString("读取成功:GameData[") + ITOS(DataIndex) + UCString("].Tick=") + ITOS(RObjGameData_Debug.Data_Total[DataIndex].Value) + UCString("\r\n"));
		return ucTRUE;
	}
	ucBOOL WriteGameData(ucINT DataIndex)
	{
		ucINT Code = LDB_GameData.Put(LDB_WriteOption, UCRMessage(&DataIndex, sizeof(ucINT)), UCRMessage(&RObjGameData_Debug.Data_Total[DataIndex].Value, sizeof(ucDWORD)));
		if (Code)
		{
			RContainer_NET.Log(UCString("写入失败:Code=") + ITOS(Code) + UCString(",GameData[") + ITOS(DataIndex) + UCString("].Tick=") + ITOS(RObjGameData_Debug.Data_Total[DataIndex].Value) + UCString("\r\n"));
			return ucFALSE;
		}
		return ucTRUE;
	}
	ucBOOL ReadChannel()
	{
		UCLDB_Iterator LDB_Iterator(LDB_Channel, LDB_ReadOption);
		if (LDB_Iterator.Handle() == ucNULL)
			return ucFALSE;
		ucINT Code = LDB_Iterator.SeekToFirst();
		if (Code)
		{
			RContainer_NET.Log(UCString("读取Channel失败:Code=") + ITOS(Code) + UCString("\r\n"));
			return ucFALSE;
		}
		while (LDB_Iterator.Valid())
		{
			ucUINT ChannelIndex = 0;
			if (!LDB_Iterator.Key.PopUINT32(ChannelIndex))
			{
				RContainer_NET.Log(UCString("读取Channel数据解析失败") + UCString("\r\n"));
				return ucNULL;
			}
			if (ChannelIndex >= 0 && ChannelIndex < CHANNEL_MAX)
			{
				ucUINT Value = 0;
				if (!LDB_Iterator.Value.PopUINT32(Value))
				{
					RContainer_NET.Log(UCString("读取Channel数据解析失败") + UCString("\r\n"));
					return ucNULL;
				}
				GameData_Channels[ChannelIndex].Total = Value;
				RObjGameData_Debug.Channel_Total[ChannelIndex] = GameData_Channels[ChannelIndex].Total;
			}
			Code = LDB_Iterator.Next();
			if (Code)
			{
				RContainer_NET.Log(UCString("Channel迭代Next失败:") + ITOS(Code) + UCString("\r\n"));
				return ucFALSE;
			}
		}
		return ucTRUE;
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

#endif	//_accountdata_H_