#ifndef _match_
#define _match_

#include "../../../public/common/game_userbase.h"
#include "../../../public/common/game_database.h"
#include "../../../public/common/game_centerbase.h"
#include "../../../public/common/game_monitorbase.h"

#include "../../common/game_matchbase.h"

//本地配置存档
struct UCGameMatchInfo
{
	UCString					Password;					//连接中心服的密码
};

//单局需要匹配的玩家数量
class UCRObjGameBattle_MatchInfoExt : public UCRObjGameBattle_MatchInfo
{
	ucINT64					AdminKey;
public:
	ucINT					LogicIndex;				//逻辑服务ID
	ucINT					BattleIndex;			//战场ID
public:
	UCProINT				TypeWait;				//房间等待状态：0-无效列表，1-公开列表，2-有效房间列表
	UCEListPtr*				ListPtr;				//给外部管理器UCEList的索引

	UCInt64IntMap			MapGameUserIDReady;		//已经等待的用户
	UCInt64IntMap			MapGameUserIDBlock;		//阻塞的用户
public:
	UCRObjGameBattle_MatchInfoExt()
	{
		LogicIndex = -1;
		BattleIndex = -1;

		TypeWait = 0;
		ListPtr = ucNULL;
	}
	ucBOOL	Init(UCRContainer_NET* RContainer_NET, ucCONST UCString& LocalUrl, ucINT LogicIndex, ucINT BattleIndex, ucINT64 AdminKey)
	{
		this->LogicIndex = LogicIndex;
		this->BattleIndex = BattleIndex;

		this->AdminKey = AdminKey;

		UCString WholeUrl = LocalUrl + UCString("//UCRObjGameLogic/") + ITOS(LogicIndex) + UCString("/UCRObjGameBattle_MatchInfo/") + ITOS(BattleIndex) + UCString(".obj");

		if (!RContainer_NET->AppendClient(WholeUrl, typeof(UCRObjGameBattle_MatchInfo), this))
		{
			RContainer_NET->Log(UCString("AppendObject房间出错：") + LocalUrl + UCString("\r\n"));
			return 0;
		}
		for (ucINT i = 0; i < BATTLE_USER_MAX; ++i)
		{
			GameBattle_MatchInfo.GameUserID[i] = 0xFFFFFFFFFFFFFFFFU;
			GameBattle_MatchInfo.GameUserID[i].AppendEvent(UCEvent(this, GameUserID_OnMySet));
		}
		OnReset = UCEvent(this, Battle_OnMyReset);
		GameBattle_MatchInfo.Type.AppendEvent(UCEvent(this, BattleType_OnMySet));
		GameBattle_MatchInfo.State.AppendEvent(UCEvent(this, BattleState_OnMySet));
		return ucTRUE;
	}
	ucINT	CheckLink()
	{
		if (Linked)
			return 0;

		UCString WholeUrl = UCString("sm://logic:") + ITOS(LogicIndex) + UCString("//UCRObjGameBattle_MatchInfo/") + ITOS(BattleIndex) + UCString(".obj");
		ucINT Code = Link(WholeUrl, UCROBJECT_LINK_STABLE, AdminKey);
		if (Code)
			RContainer->Log(WholeUrl + ITOS(Code, UCString(":%d\r\n")));
		return Code;
	}
private:
	ucVOID	Refresh_TypeWait()
	{
		ucINT BattleType = BATTLE_TYPE_UNVALID;
		if (GameBattle_MatchInfo.Type.Value != BATTLE_TYPE_UNVALID)
		{
			BattleType = BATTLE_TYPE_PRIVATE;
			if (GameBattle_MatchInfo.Type.Value == BATTLE_TYPE_PUBLIC)
			{
				if (GameBattle_MatchInfo.State.Value <= BATTLE_STATE_READY && GameBattle_MatchInfo.MasterID.Value != 0xFFFFFFFFFFFFFFFFU)
				{
					if (MapGameUserIDReady.GetSize() < GameBattle_MatchInfo.MaxCount.Value)
						BattleType = BATTLE_TYPE_PUBLIC;
				}
			}
		}
		if (TypeWait.Value != BattleType)
		{
			ucINT TypeWaitBackup = TypeWait.Value;
			TypeWait = BattleType;
		}
	}
	ucVOID	Battle_OnMyReset(UCObject*, UCEventArgs*)
	{
		BattleType_OnMySet(0, 0);
	}
	ucVOID	BattleType_OnMySet(UCObject*, UCEventArgs*)
	{
		MapGameUserIDReady.RemoveAll();
		if (GameBattle_MatchInfo.Type.Value)
		{
			for (ucINT i = 0; i < BATTLE_USER_MAX; ++i)
			{
				if (GameBattle_MatchInfo.GameUserID[i].Value != 0xFFFFFFFFFFFFFFFFU)
				{
					MapGameUserIDReady.Add(GameBattle_MatchInfo.GameUserID[i].Value, 0xFFFFFFFFFFFFFFFFU);
					MapGameUserIDBlock.Remove(GameBattle_MatchInfo.GameUserID[i].Value);
				}
			}
		}
		Refresh_TypeWait();
	}
	ucVOID	BattleState_OnMySet(UCObject*, UCEventArgs*)
	{
		Refresh_TypeWait();
	}
	ucVOID	GameUserID_OnMySet(UCObject*, UCEventArgs* e)
	{
		if (GameBattle_MatchInfo.Type.Value == BATTLE_TYPE_UNVALID)
			return;
		UCRPropertyArgs* Args = (UCRPropertyArgs*)e;
		UCRInt64* GameUserIDPtr = (UCRInt64*)Args->Data;
		if (GameUserIDPtr->Value != 0xFFFFFFFFFFFFFFFFU)
			MapGameUserIDReady.Add(GameUserIDPtr->Value, 0);
		else
		{
			MapGameUserIDReady.RemoveAll();
			for (ucINT i = 0; i < BATTLE_USER_MAX; ++i)
			{
				if (GameBattle_MatchInfo.GameUserID[i].Value != 0xFFFFFFFFFFFFFFFFU)
				{
					MapGameUserIDReady.Add(GameBattle_MatchInfo.GameUserID[i].Value, 0xFFFFFFFFFFFFFFFFU);
					MapGameUserIDBlock.Remove(GameBattle_MatchInfo.GameUserID[i].Value);
				}
			}
		}
		Refresh_TypeWait();
	}
};

// 逻辑对象，匹配扩展
class UCRObjGameLogic_MatchExt : public UCRObjGameLogic
{
public:
	ucINT64							AdminKey;

	ucINT							LogicIndex;
	ucBOOL							CheckLinkSucceed;

	UCIntIntMap						MapBattleValid;
	UCIntIntMap						MapBattleTemp;

	UCRObjGameBattle_MatchInfoExt	RObjGameBattle_MatchInfoExt[BATTLE_MAX];
	UCEvent							BattleState_OnChange;
public:
	UCRObjGameLogic_MatchExt()
	{
		LogicIndex = -1;
		CheckLinkSucceed = ucFALSE;
	}
	ucBOOL	Init(UCRContainer_NET* RContainer_NET, ucCONST UCString& LocalUrl, ucINT LogicIndex, ucINT64 AdminKey)
	{
		this->LogicIndex = LogicIndex;
		this->AdminKey = AdminKey;

		UCString WholeUrl = LocalUrl + UCString("//UCRObjGameLogic/") + ITOS(LogicIndex) + UCString(".obj");
		if (!RContainer_NET->AppendClient(WholeUrl, typeof(UCRObjGameLogic), this))
			return ucFALSE;

		for (ucINT i = 0; i < BATTLE_MAX; ++i)
		{
			RObjGameBattle_MatchInfoExt[i].Init(RContainer_NET, LocalUrl, LogicIndex, i, AdminKey);
			RObjGameBattle_MatchInfoExt[i].GameBattle_MatchInfo.State.AppendEvent(UCEvent(this, GameBattle_MatchInfo_StateOnSet));
		}
		return ucTRUE;
	}
	ucINT	CheckLink(ucINT MatchIndex, ucCONST UCString& LocalUrl)
	{
		if (Linked)
			return 0;

		UCString WholeUrl = UCString("sm://logic:") + ITOS(LogicIndex) + UCString("//UCRObjGameLogic.obj");
		ucINT Code = Link(WholeUrl, UCROBJECT_LINK_STABLE, AdminKey);
		if (Code)
		{
			RContainer->Log(UCString("连接") + WholeUrl + UCString("失败:") + ITOS(Code) + UCString("\r\n"));
			return Code;
		}
		CheckLinkSucceed = ucTRUE;
		return 0;
	}
	ucVOID GameBattle_MatchInfo_StateOnSet(UCObject* Object, UCEventArgs* e)
	{
		UCRObjGameBattle_MatchInfoExt* RObjGameBattle_MatchInfoExt = (UCRObjGameBattle_MatchInfoExt*)Object;

		BattleState_OnChange.Run(RObjGameBattle_MatchInfoExt, ucNULL);
	}
};

struct UCGameUserID_MatchExt : public UCGameUserID
{
	UCEListPtr*			ListPtr;			//给外部管理器UCEList的索引
	ucDWORD				MatchTime;			//匹配时间
public:
	UCGameUserID_MatchExt()
	{
		ListPtr = ucNULL;
		MatchTime = UCGetTime();
	}
	UCGameUserID_MatchExt(ucCONST UCGameUserID_MatchExt& in)
	{
		ID = in.ID;

		ListPtr = in.ListPtr;
		MatchTime = in.MatchTime;
	}
};

class UCRObjGameMatchLocal : public UCRObjGameMatch
{
private:
	UCEApp*							App;
	UCEConfig<UCGameMatchInfo>		GameMatchCfg;
	UCRContainer_NET				RContainer_NET;

	UCRObjGameCenter				RObjGameCenter;
	UCRObjGameMonitor				RObjGameMonitor;
private:
	UCIntIntMap						MapGameLogic_Info_Open;								//所有对外游戏服务器
	UCIntIntMap						MapGameLogic_Info_UnOpen;							//所有对内游戏服务器
	UCRObjGameLogic_Infos			RObjGameLogic_Infos;								//中央存储的所有的大厅信息

	UCRObjGameLogic_MatchExt*		RObjGameLogic_MatchExt[LOGIC_MAX];					//逻辑服务
private:
	UCEList<UCRObjGameBattle_MatchInfoExt*>			ListRObjGameBattle_MatchInfoExt_Wait;		//可加入的战场

	UCEAVLMap<ucINT64, UCGameUserID_MatchExt>		MapGameUserID_MatchExt;						//正在匹配的玩家
	UCEList<UCGameUserID_MatchExt*>					ListMatchInfo;								//匹配队列

	UCGroupFiber							FiberBattle;
	UCFiber									FiberMatch;
private:
	//通用Key
	UCString						LocalUrl;
	ucINT64							AdminKey;
	ucDWORD							RandKey;
	UCFiber							FiberInit;
public:
	UCString	Match(ucCONST UCGameUserID& GameUserID, ucUINT64 Token)
	{
		UCENode Pos = MapGameUserID_MatchExt.Find(GameUserID.ID);
		if (!Pos.IsNull())
			return UCString("succeed");

		UCGameUserID_MatchExt* MatchExt = MapGameUserID_MatchExt.At(GameUserID.ID);

		MatchExt->ID = GameUserID.ID;
		MatchExt->ListPtr = ListMatchInfo.AddTail(MatchExt);

		TotalTick += 1;
		Count = ListMatchInfo.GetCount();

		return UCString("succeed");
	} 
	//取消匹配
	UCString	StopMatch(ucCONST UCGameUserID& GameUserID, ucUINT64 Token)
	{
		UCENode Pos = MapGameUserID_MatchExt.Find(GameUserID.ID);
		if (Pos.IsNull())
			return UCString("succeed");

		UCGameUserID_MatchExt* MatchExt = &MapGameUserID_MatchExt.GetValueAt(Pos);

		ListMatchInfo.RemoveAt(MatchExt->ListPtr);
		MapGameUserID_MatchExt.RemoveAt(Pos);

		CancelTick += 1;
		Count = ListMatchInfo.GetCount();

		return UCString("succeed");
	}
private:
	//战场用户相关
	ucINT	Battle_RemoveUser(ucINT64 GameUserID)
	{
		if (GameUserID == 0xFFFFFFFFFFFFFFFFU)
			return ucFALSE;
		UCENode Pos = MapGameUserID_MatchExt.Find(GameUserID);
		if (Pos.IsNull())
			return ucFALSE;

		UCGameUserID_MatchExt* MatchExt = &MapGameUserID_MatchExt.GetValueAt(Pos);
		ListMatchInfo.RemoveAt(MatchExt->ListPtr);

		Count = ListMatchInfo.GetCount();
		MapGameUserID_MatchExt.RemoveAt(Pos);
		return ucTRUE;
	}
	ucVOID	Battle_ClearUser(UCGameMatch_BattleBase* Match_BattleBase)
	{
		for (ucINT i = 0; i < BATTLE_USER_MAX; ++i)
			Battle_RemoveUser(Match_BattleBase->GameUserID[i].ID);
	}
	ucVOID	BattleState_OnMyChange(UCObject* o, UCEventArgs* e)
	{
		UCRObjGameBattle_MatchInfoExt* RObjGameBattle_MatchInfoExt = (UCRObjGameBattle_MatchInfoExt*)o;

		//房间等待状态：0-无效列表，1-公开列表，2-有效房间列表
		if (RObjGameBattle_MatchInfoExt->TypeWait.Value != BATTLE_TYPE_UNVALID)
		{
			// 			if (TypeOld == 0)
			// 				m_Debug_RDObj.ValidRoomNum += 1;
		}
		else
		{
			// 			if (TypeOld)
			// 				m_Debug_RDObj.ValidRoomNum -= 1;
		}
		if (RObjGameBattle_MatchInfoExt->TypeWait.Value == BATTLE_TYPE_PUBLIC)
		{
			if (RObjGameBattle_MatchInfoExt->ListPtr == ucNULL)
			{
				RObjGameBattle_MatchInfoExt->ListPtr = ListRObjGameBattle_MatchInfoExt_Wait.AddTail(RObjGameBattle_MatchInfoExt);

				//m_Debug_RDObj.PublicRoomNum = ListRObjGameBattle_MatchInfoExt_Wait.GetCount();
			}
		}
		else
		{
			if (RObjGameBattle_MatchInfoExt->ListPtr != ucNULL)
			{
				ListRObjGameBattle_MatchInfoExt_Wait.RemoveAt(RObjGameBattle_MatchInfoExt->ListPtr);
				RObjGameBattle_MatchInfoExt->ListPtr = ucNULL;

				//m_Debug_RDObj.PublicRoomNum = ListRObjGameBattle_MatchInfoExt_Wait.GetCount();
			}
		}
	}
private:
	//逻辑服务器相关
	ucINT	RandLogicIndex()
	{
		if (MapGameLogic_Info_Open.GetSize() > 0)
		{
			ucINT Pos = randint(0, MapGameLogic_Info_Open.GetSize() - 1);
			return MapGameLogic_Info_Open.GetKeyAt(Pos);
		}
		return -1;
	}
	UCRObjGameLogic_MatchExt* RefreshRObjLogic(ucINT LogicIndex, ucINT MatchIndex)
	{
		if (LogicIndex < 0 || LogicIndex >= LOGIC_MAX)
			return 0;
		if (RObjGameLogic_Infos.GameLogic_Infos[LogicIndex].Valid == 0)
			return 0;
		if (RObjGameLogic_MatchExt[LogicIndex] == 0)
		{
			RObjGameLogic_MatchExt[LogicIndex] = new UCRObjGameLogic_MatchExt;
			RObjGameLogic_MatchExt[LogicIndex]->BattleState_OnChange = UCEvent(this, BattleState_OnMyChange);
			if (!RObjGameLogic_MatchExt[LogicIndex]->Init(&RContainer_NET, LocalUrl, LogicIndex, AdminKey))
				return ucNULL;
		}
		ucINT Code = RObjGameLogic_MatchExt[LogicIndex]->CheckLink(MatchIndex, LocalUrl);
		if (Code)
			return ucNULL;
		return RObjGameLogic_MatchExt[LogicIndex];
	}
	ucBOOL	Battle_OnStart(UCGameMatch_BattleBase* Match_BattleBase)
	{
		ucINT LogicIndex = RandLogicIndex();
		UCRObjGameLogic_MatchExt* RObjGameLogic = RefreshRObjLogic(LogicIndex, App->Index);
		if (RObjGameLogic == ucNULL)
		{
			RContainer_NET.Log(UCString("无效服1：") + ITOS(LogicIndex) + UCString("\r\n"));
			return ucFALSE;
		}

		ucINT BattleIndex = -1;
		//5秒 倒计时
		ucINT LeastTime = 5;
		UCString strResult = RObjGameLogic->Direct_CreateBattle(*Match_BattleBase, App->Index, 0, LeastTime, BattleIndex);
		if (strResult == UCString("succeed"))
		{
			ucINT Code = RObjGameLogic->RObjGameBattle_MatchInfoExt[BattleIndex].CheckLink();
			if (Code)
				RContainer_NET.Log(UCString("连接房间失败：") + ITOS(BattleIndex) + UCString("\r\n"));
			//m_Debug_RDObj.CreateRoomNum += 1;
			return ucTRUE;
		}
		RContainer_NET.Log(UCString("创建失败1：") + strResult + UCString("\r\n"));
		return ucFALSE;
	}

	ucVOID	FiberBattle_OnRun(UCObject*, UCEventArgs* e)
	{
		UCGameMatch_BattleBase* Match_BattleBase = (UCGameMatch_BattleBase*)e;
		Battle_ClearUser(Match_BattleBase);

		if (!Battle_OnStart(Match_BattleBase))
		{
			//失败的用户是否需要加回来
			;
		}
		delete Match_BattleBase;
	}
private:
	//匹配相关
	ucBOOL	Match_Update()
	{
		ucDWORD dwMaxTime = 0;

		UCEArray<UCGameUserID_MatchExt*> AryMatch;
		UCEListPtr* ListPtr = ListMatchInfo.GetHeadPosition();

		//捡取最大数量的用户
		for (ucINT i = BATTLE_USER_MAX; i > 0 && ListPtr != ucNULL; --i)
		{
			UCGameUserID_MatchExt* MatchExt = (UCGameUserID_MatchExt*)ListMatchInfo.GetAt(ListPtr);
			ucDWORD dwDistTime = UCGetTime() - MatchExt->MatchTime;
			if (dwMaxTime < dwDistTime)
				dwMaxTime = dwDistTime;
			AryMatch.Add(MatchExt);
			ListPtr = ListMatchInfo.GetNext(ListPtr);
		}

		ucINT AryMatchSize = AryMatch.GetSize();
		//少于ROOM_USER_MIN不能开始
		if (AryMatchSize < BATTLE_USER_MIN)
			return ucFALSE;

		//人数没满，且没过MATCH_TIME_MIN秒
		if (AryMatchSize < BATTLE_USER_MAX && dwMaxTime < MATCH_TIME_MIN * 10000)
			return ucFALSE;

		UCGameMatch_BattleBase* Match_BattleBase = new UCGameMatch_BattleBase;

		//随机分配作为
		for (ucINT CurrSeat = 0; AryMatch.GetSize() > 0; CurrSeat++)
		{
			ucINT RandIndex = randint(0, AryMatch.GetSize() - 1);
			UCGameUserID_MatchExt* MatchExt = AryMatch.GetAt(RandIndex);
			Match_BattleBase->GameUserID[CurrSeat].ID = MatchExt->ID;

			AryMatch.RemoveAt(RandIndex);
		}
		FiberBattle.RunNew((UCEventArgs*)Match_BattleBase);
		return ucTRUE;
	}
	ucVOID	FiberMatch_OnStart(UCObject* o, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)o;
		while (FiberData->IsValid())
		{
			while (Match_Update() && FiberData->IsValid())	{}
			FiberData->Every(1000);
		}
	}
private:
	ucVOID	RObjGameLogic_Infos_OnReset(UCObject*, UCEventArgs*)
	{
		MapGameLogic_Info_Open.RemoveAll();
		MapGameLogic_Info_UnOpen.RemoveAll();
		for (ucINT i = 0; i < DATA_MAX; i++)
		{
			if (RObjGameLogic_Infos.GameLogic_Infos[i].Valid)
				MapGameLogic_Info_Open.Add(i, i);
			else
				MapGameLogic_Info_UnOpen.Add(i, 0);
		}
	}
	ucVOID	RObjGameLogic_Infos_OnChanged(UCObject*, UCEventArgs* e)
	{
		UCRPropertyArgs* Args = (UCRPropertyArgs*)e;
		ucINT Index = RObjGameLogic_Infos.GameLogic_Infos[0].GetArrayIndex(Args->Data);

		RContainer_NET.Log(UCString("收到服务消息 RObjGameLogic_Infos.GameLogic_Infos[") + ITOS(Index) + UCString("]\r\n"));

		if (RObjGameLogic_Infos.GameLogic_Infos[Index].Valid)
		{
			MapGameLogic_Info_Open.Add(Index, Index);
			MapGameLogic_Info_UnOpen.Remove(Index);
		}
		else
		{
			MapGameLogic_Info_Open.Remove(Index);
			MapGameLogic_Info_UnOpen.Add(Index, Index);
		}
	}
public:
	UCRObjGameMatchLocal()
	{
		App = UCGetApp();
		if (App->Name.IsEmpty())
			App->Name = UCString("match");

		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));

		for (ucINT i = 0; i < LOGIC_MAX; i++)
			RObjGameLogic_MatchExt[i] = ucNULL;

		LocalUrl = LocalUrl;

		FiberInit.FiberEvent = UCEvent(this, FiberInit_OnStart);
		FiberBattle.FiberEvent = UCEvent(this, FiberBattle_OnRun);
		FiberMatch.FiberEvent = UCEvent(this, FiberMatch_OnStart);

		RContainer_NET.OnEnd = UCEvent(this, OnRContainer_Exit);
	}
	~UCRObjGameMatchLocal()
	{
		for (ucINT i = 0; i < LOGIC_MAX; i++)
		{
			if (RObjGameLogic_MatchExt[i] != ucNULL)
				delete RObjGameLogic_MatchExt[i];
		}
	}
	ucVOID	OnRContainer_Exit(UCObject* Object, UCEventArgs*)
	{
		// 解决退出事件
	}
	ucVOID	FiberInit_OnStart(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		if (App->Index < 0)
			RContainer_NET.Log(UCString("start:") + App->Name + UCString("\r\n"));
		else
			RContainer_NET.Log(UCString("start:") + App->Name + ITOS(App->Index, UCString(":%d")) + UCString("\r\n"));

		UCGameMatchInfo* MatchInfo = GameMatchCfg.GetData();
		ucINT AppIndex = App->Index;
		if (AppIndex < 0)
			AppIndex = 0;

		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));
		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameMatch.obj"), typeof(UCRObjGameMatch), typeof(UCRObjGameMatchLocal), this, ucTRUE);

		RContainer_NET.AppendClient(LocalUrl + UCString("//Local/UCRObjGameCenter.obj"), typeof(UCRObjGameCenter), &RObjGameCenter);

		RContainer_NET.AppendClient(LocalUrl + UCString("//local/UCRObjGameLogic_Infos.obj"), typeof(UCRObjGameLogic_Infos), &RObjGameLogic_Infos);
		for (ucINT i = 0; i < DATA_MAX; i++)
			RObjGameLogic_Infos.GameLogic_Infos[i].AppendEvent(UCEvent(this, RObjGameLogic_Infos_OnChanged));
		RObjGameLogic_Infos.OnReset = UCEvent(this, RObjGameLogic_Infos_OnReset);

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
		// 		RContainer_NET.Log(UCString("监控连接succeed\r\n"));
		// 
		// 		RObjGameMonitor.Gate_Register(Index, Monitor_Key, Monitor_PCU_MaxTotal, Monitor_PCU_MaxChannel, Monitor_PCU_Current);
		// 		if (Index < 0 || Monitor_PCU_MaxTotal < 0)
		// 			RContainer_NET.Log(UCString("注册监控容器失败\r\n"));
		// 		else
		// 			RContainer_NET.Log(UCString("注册监控容器succeed\r\n"));

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
		UCString Result = RObjGameCenter.GameMatch_Update(App->Index, MatchInfo->Password, AdminKey, RandKey);
		if (Result != "succeed")
		{
			RContainer_NET.Log(UCString("中心容器验证失败，请检查配置后重启容器 : ") + Result + UCString("\r\n"));
			return;
		}
		RContainer_NET.Log(UCString("验证succeed\r\n"));

		RContainer_NET.SetAdminKey(AdminKey);


		while (FiberData->IsValid())
		{
			ucINT Code = RObjGameLogic_Infos.Link(UCString("sm://center:0//UCRObjGameLogic_Infos.obj"), UCROBJECT_LINK_STABLE, AdminKey);
			if (Code == 0)
			{
				RContainer_NET.Log(UCString("GameCenter的GameLogic_Infos连接succeed\r\n"));
				break;
			}
			RContainer_NET.Log(UCString("GameCenter的GameLogic_Infos连接失败 : Code =") + ITOS(Code) + UCString("\r\n"));
			FiberData->Every(1000);
		}


		RContainer_NET.Log(UCString("当前容器初始化完毕\r\n"));

		FiberMatch.Start(ucNULL);
	}
	ucINT	Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("home"));
		if (!GameMatchCfg.LoadFromXML(UCString("home/home.xml")))
		{
			UCGameMatchInfo* MatchInfo = GameMatchCfg.GetData();
			MatchInfo->Password = "123456";
			GameMatchCfg.SaveToXML(UCString("home/home.xml"));
		}
		UCGameMatchInfo* MatchInfo = GameMatchCfg.GetData();

		UCGetSystemExt()->SetWindowCaption(App->Name);
		//设置异常处理方法：0-默认弹窗提醒，1-忽略继续（可能会死循环或闪退），2-临时本次返回，3-本脚本以后始终返回（防止问题扩散）
		SetExceptionMode(0);

		FiberInit.Start(0);
		return 1;
	}
	ucVOID	Run()
	{
		if (!Init())
			return;
		RContainer_NET.Run(App->Name, App->Index, ucFALSE, ucTRUE, 10);
	}
};

#endif	//_match_