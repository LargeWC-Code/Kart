#ifndef _logic_
#define _logic_

#include "../../../public/common/game_userbase.h"
#include "../../../public/common/game_database.h"
#include "../../../public/common/game_centerbase.h"
#include "../../../public/common/game_monitorbase.h"

#include "../../common/game_homebase.h"
#include "../../common/game_logicbase.h"

//本地配置存档
struct UCGameLogicInfo
{
	UCString					Password;					//连接中心服的密码
};

class UCRObjGameUserData_Logic : public UCRObjGameUserData
{
	ucDWORD							DataLife;
public:
	UCRObjGameUserData_Ext			RObjGameUserData_Ext;

	//本地公开数据
	UCRGameUserPubInfo				RGameUserPubInfo;
	//本地物理数据
	UCRObjGameUserPhyInfo			RObjGameUserPhyInfo;
public:
	UCRObjGameUserData_Logic(ucCONST UCGameUserID& GameUserID, ucINT SeatID)
	{
		DataLife = UCGetTime();
		RGameUserPubInfo.Clear();
		RGameUserPubInfo.GameUserID = GameUserID.ID;
		RGameUserPubInfo.SeatID = SeatID;

		//RGameUser_PhyInfo.GameUserID = GameUserID.ID;
	}
	UCString Init(ucCONST UCString& strRoot, ucINT64 AdminKey, UCRContainer* RContainer)
	{
		UCEApp* App = UCGetApp();
		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));

		UCGameUserID GameUserID(RGameUserPubInfo.GameUserID.Value);
		UCString strGameUserID = GameUserID.ToString();
		UCString strHomeUrl = UCString("sm://home:") + ITOS(GameUserID.GetDataID());
		{
			//先连接GameKey
			if (!RContainer->AppendClient(LocalUrl + UCString("//UCRObjGameUserData_Ext/") + strGameUserID + UCString(".obj"), typeof(UCRObjGameUserData_Ext), &RObjGameUserData_Ext, true))
				return UCString("AppendObject UCRObjGameUserData_Ext错误") + strGameUserID;

			UCString strUrl_GameUserData_Ext = strHomeUrl + UCString("//UCRObjGameUserData/UCRObjGameUserData_Ext/") + strGameUserID + UCString(".obj");
			ucINT Code = RObjGameUserData_Ext.Link(strUrl_GameUserData_Ext, UCROBJECT_LINK_STABLE, AdminKey);
			if (Code != 0)
				return UCString("Link UCRObjGameUserData_Ext错误:") + ITOS(Code);
		}

		{
			if (!RContainer->AppendClient(LocalUrl + UCString("//UCRObjGameUserData/") + strGameUserID + UCString(".obj"), typeof(UCRObjGameUserData), this, ucTRUE))
				return UCString("AppendObject UCRObjGameUserData错误");

			UCString strUrl_GameUserData = strHomeUrl + UCString("//UCRObjGameUserData/") + strGameUserID + UCString(".obj");
			ucINT Code = Link(strUrl_GameUserData, UCROBJECT_LINK_STABLE, AdminKey);

			if (Code != 0)
				return UCString("Link UCRObjGameUserData错误:") + ITOS(Code);
		}

		RGameUserPubInfo.Nickname = Nickname.Value;

		RGameUserPubInfo.AvatarData.FaceID = AvatarData.FaceID;
		RGameUserPubInfo.AvatarData.HairID = AvatarData.HairID;
		RGameUserPubInfo.AvatarData.BodyID = AvatarData.BodyID;
		RGameUserPubInfo.AvatarData.HandID = AvatarData.HandID;
		RGameUserPubInfo.AvatarData.PantID = AvatarData.PantID;
		RGameUserPubInfo.AvatarData.ShoeID = AvatarData.ShoeID;

		return UCString("succeed");
	}
	ucVOID CheckKeepAlive()
	{
		ucDWORD CurrentTime = UCGetTime();
		//每20秒刷新一次
		if (CurrentTime - DataLife > 200000)
		{
			//RContainer->Log(UCString("KeepValid\r\n"));
			KeepValid();
			DataLife = CurrentTime;
		}
	}
};

#define GAMEUSERDATA_TIMEOUT			60000

class UCRObjGameBattleLocal : public UCRObjGameBattle
{
private:
	UCRObjGameBattle_MatchInfo					RObjGameBattle_MatchInfo;

	UCESimpleMap<ucINT64, UCRObjGameUserData_Logic*>	MapRObjGameUserData_Logic;			//房间内的玩家
	UCIntIntMap									MapEmptySeatID;						//空闲位置

	UCFiber										FiberGame;

	ucINT										MatchIndex;
	ucINT										LogicIndex;
	ucINT										BattleIndex;

	ucINT64										AdminKey;

	UCString									LocalUrl;
public:
	UCString				StartGame(ucCONST UCGameUserID& GameUserID, ucUINT64 Token)
	{
		FiberGame.Run(0);
		return UCString("succeed");
	}

	UCString				Sync(ucCONST UCGameUserID& GameUserID, ucUINT64 Token,
		ucCONST uc3dxVector3& Pos0, ucFLOAT RotY0,
		ucCONST uc3dxVector3& Pos1, ucFLOAT RotY1)
	{
		ucINT Pos = MapRObjGameUserData_Logic.FindKey(GameUserID.ID);
		if (Pos < 0)
			return UCString("无效用户");

		UCRObjGameUserData_Logic* RObjGameUserData = MapRObjGameUserData_Logic.GetValueAt(Pos);
	
		UserPhyInfo[RObjGameUserData->RGameUserPubInfo.SeatID.Value].Pos = Pos0;
		UserPhyInfo[RObjGameUserData->RGameUserPubInfo.SeatID.Value].RotY = RotY0;
		UserPhyInfo[RObjGameUserData->RGameUserPubInfo.SeatID.Value].Commit();

		UserPhyInfo[RObjGameUserData->RGameUserPubInfo.SeatID.Value].Pos = Pos1;
		UserPhyInfo[RObjGameUserData->RGameUserPubInfo.SeatID.Value].RotY = RotY1;
		UserPhyInfo[RObjGameUserData->RGameUserPubInfo.SeatID.Value].Commit();

		RContainer->Log(ITOS(RObjGameUserData->RGameUserPubInfo.SeatID.Value) + UCString(":") + VTOS(Pos0) + UCString(":") + VTOS(Pos1) + UCString("\r\n"));
		return UCString("succeed");
	}

	UCString				ExitGame(ucCONST UCGameUserID& GameUserID, ucUINT64 Token)
	{
		ucINT64 n64GameUserID = GameUserID.ID;
		ucINT Pos = MapRObjGameUserData_Logic.FindKey(n64GameUserID);
		if (Pos < 0)
			return UCString("ID错误");

		UCRObjGameUserData_Logic* RObjGameUserData_Logic = MapRObjGameUserData_Logic.GetValueAt(Pos);

		Leave(RObjGameUserData_Logic);
		return UCString("succeed");
	}
private:
	ucVOID OnFiberGame(UCObject* Sender, UCEventArgs* e)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Sender;

		FiberData->Delay(LeastTime.Value * 10000);
		LeastTime = 0;

		GameBattle_MatchInfo.State = BATTLE_STATE_GAMING;
		RObjGameBattle_MatchInfo.GameBattle_MatchInfo.State = BATTLE_STATE_GAMING;

		while (FiberData->IsValid())
		{
			FiberData->Every(10000);
		}
	}

public:
	UCString Join(ucCONST UCGameUserID& GameUserID, UCRObjGameUserData_Logic** ppRObjGameUserData_Logic)
	{
		UCEApp* App = UCGetApp();
		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));

		*ppRObjGameUserData_Logic = ucNULL;

		ucINT64 n64GameUserID = GameUserID.ID;
		ucINT Pos = MapRObjGameUserData_Logic.FindKey(n64GameUserID);
		if (Pos >= 0)
		{
			*ppRObjGameUserData_Logic = MapRObjGameUserData_Logic.GetValueAt(Pos);
			return UCString("succeed");
		}

		if (RObjGameBattle_MatchInfo.GameBattle_MatchInfo.State.Value >= BATTLE_STATE_GAMING)
			return UCString("已经开始");
		if (MapEmptySeatID.GetSize() <= 0)
			return UCString("没空位");

		ucINT SeatID = MapEmptySeatID.GetKeyAt(0);
		MapEmptySeatID.RemoveAt(0);

		UCString strRoot = LocalUrl + UCString("//Battle_Room/") + ITOS(BattleIndex);
		UCRObjGameUserData_Logic* pRObjGameUserData_Logic = new UCRObjGameUserData_Logic(GameUserID, SeatID);
		UCString strResult = pRObjGameUserData_Logic->Init(strRoot, AdminKey, RContainer);
		if (strResult != "succeed")
		{
			delete pRObjGameUserData_Logic;
			//位置还回去
			MapEmptySeatID.Add(SeatID, 0);
			return strResult;
		}
		if (RObjGameBattle_MatchInfo.GameBattle_MatchInfo.Type.Value == BATTLE_TYPE_UNVALID)
		{
			delete pRObjGameUserData_Logic;
			//位置还回去
			MapEmptySeatID.Add(SeatID, 0);
			return UCString("房间失效");
		}
		Pos = MapRObjGameUserData_Logic.FindKey(n64GameUserID);
		if (Pos >= 0)
		{
			UCRObjGameUserData_Logic* RObjGameUserData_Logic_Old = MapRObjGameUserData_Logic.GetValueAt(Pos);
			delete pRObjGameUserData_Logic;
			*ppRObjGameUserData_Logic = RObjGameUserData_Logic_Old;
			//位置还回去
			MapEmptySeatID.Add(SeatID, 0);
			return UCString("succeed");
		}
		ucINT IsMaster = 0;

		RObjGameBattle_MatchInfo.GameBattle_MatchInfo.GameUserID[SeatID] = n64GameUserID;
		//UserPubInfo[SeatID].EnterTime = UCGetTime();

		UserPubInfo[SeatID].GameUserID = n64GameUserID;
		UserPubInfo[SeatID].Nickname = pRObjGameUserData_Logic->Nickname;

		// Assign avatar data members individually
		UserPubInfo[SeatID].AvatarData.FaceID = pRObjGameUserData_Logic->AvatarData.FaceID;
		UserPubInfo[SeatID].AvatarData.HairID = pRObjGameUserData_Logic->AvatarData.HairID;
		UserPubInfo[SeatID].AvatarData.BodyID = pRObjGameUserData_Logic->AvatarData.BodyID;
		UserPubInfo[SeatID].AvatarData.HandID = pRObjGameUserData_Logic->AvatarData.HandID;
		UserPubInfo[SeatID].AvatarData.PantID = pRObjGameUserData_Logic->AvatarData.PantID;
		UserPubInfo[SeatID].AvatarData.ShoeID = pRObjGameUserData_Logic->AvatarData.ShoeID;
		UserPubInfo[SeatID].AvatarData.CarID = pRObjGameUserData_Logic->AvatarData.CarID;

		MapRObjGameUserData_Logic.Add(n64GameUserID, pRObjGameUserData_Logic);

		pRObjGameUserData_Logic->RObjGameUserPhyInfo.GameUserID = n64GameUserID;
		pRObjGameUserData_Logic->RObjGameUserPhyInfo.KeepValid();

		pRObjGameUserData_Logic->RObjGameUserPhyInfo.TimeOut = GAMEUSERDATA_TIMEOUT;
		pRObjGameUserData_Logic->RObjGameUserPhyInfo.OnTimeOut = UCEvent(this, RObjGameUser_PhyInfo_OnTimeOut);
		pRObjGameUserData_Logic->RObjGameUserPhyInfo.OnRemove = UCEvent(this, RObjGameUser_PhyInfo_OnRemove);

		UCRLocker RLocker;
		ucINT Code = RLocker.CheckOut(&pRObjGameUserData_Logic->RObjGameUserData_Ext);
		if (Code == 0)
		{
			pRObjGameUserData_Logic->RObjGameUserData_Ext.BattleInfo.MatchID = MatchIndex;
			pRObjGameUserData_Logic->RObjGameUserData_Ext.BattleInfo.LogicID = LogicIndex;
			pRObjGameUserData_Logic->RObjGameUserData_Ext.BattleInfo.BattleID = BattleIndex;
			pRObjGameUserData_Logic->RObjGameUserData_Ext.BattleInfo.Commit();
			RLocker.CheckIn();
		}
		else
			RContainer->Log(UCString("Join => GameExt.LockHost进入失败:") + ITOS(Code) + UCString("\r\n"));

		//多少人能开始游戏
		if (MapRObjGameUserData_Logic.GetSize() >= BATTLE_USER_MIN)
		{
			GameBattle_MatchInfo.State = BATTLE_STATE_READY;
			RObjGameBattle_MatchInfo.GameBattle_MatchInfo.State = BATTLE_STATE_READY;
		}
		else
		{
			GameBattle_MatchInfo.State = BATTLE_STATE_WAITING;
			RObjGameBattle_MatchInfo.GameBattle_MatchInfo.State = BATTLE_STATE_WAITING;
		}
		*ppRObjGameUserData_Logic = pRObjGameUserData_Logic;
		return UCString("succeed");
	}
	//玩家离开游戏
	ucVOID Leave(UCRObjGameUserData_Logic* pRObjGameUserData_Logic)
	{
		ucINT64 deleteID = pRObjGameUserData_Logic->RGameUserPubInfo.GameUserID.Value;
		if (GameBattle_MatchInfo.State.Value == BATTLE_STATE_GAMING)
		{
// 			if (Battle_RDObj.Result.Value == 0)
// 				SetGameOver(0);
		}

		ucINT SeatID = pRObjGameUserData_Logic->RGameUserPubInfo.SeatID.Value;
		if (SeatID >= 0 && SeatID < BATTLE_USER_MAX)
		{
			RObjGameBattle_MatchInfo.GameBattle_MatchInfo.GameUserID[SeatID] = 0xFFFFFFFFFFFFFFFFU;
			UserPubInfo[SeatID].GameUserID = 0xFFFFFFFFFFFFFFFFU;
			MapEmptySeatID.Add(SeatID, 0xFFFFFFFFFFFFFFFFU);
		}

		UCRLocker RLocker;
		ucINT Code = RLocker.CheckOut(&pRObjGameUserData_Logic->RObjGameUserData_Ext);
		if (Code == 0)
		{
			pRObjGameUserData_Logic->RObjGameUserData_Ext.BattleInfo.MatchID = -1;
			pRObjGameUserData_Logic->RObjGameUserData_Ext.BattleInfo.LogicID = -1;
			pRObjGameUserData_Logic->RObjGameUserData_Ext.BattleInfo.BattleID = -1;
			pRObjGameUserData_Logic->RObjGameUserData_Ext.BattleInfo.Commit();
			RLocker.CheckIn();
		}
		else
		{
			RContainer->Log(UCString("Leave = > RObjGameUserData_Ext.Lock失败:") + UCString("\r\n"));
			return;
		}

		RContainer->Log(UCString("玩家离开:") + pRObjGameUserData_Logic->Nickname.Value + UCString("\r\n"));

		MapRObjGameUserData_Logic.Remove(deleteID);
		delete pRObjGameUserData_Logic;

		if (MapRObjGameUserData_Logic.GetSize() <= 0)
		{
			//m_MainFiber.SetUnValid();
			//让整个房间无效
			RObjGameBattle_MatchInfo.GameBattle_MatchInfo.State = BATTLE_STATE_WAITING;
			GameBattle_MatchInfo.State = BATTLE_STATE_WAITING;

			//Reset();
			//RContainer->Log(UCString("房间销毁：") + ITOS(RoomIndex) + UCString("\r\n"));
			
			//OnUnValid.Run(this, 0);
			return;
		}

		//如果自己是Master
		if (RObjGameBattle_MatchInfo.GameBattle_MatchInfo.MasterID.Value == deleteID)
		{
			if (MapRObjGameUserData_Logic.GetSize() > 0)
			{
				RObjGameBattle_MatchInfo.GameBattle_MatchInfo.MasterID = MapRObjGameUserData_Logic.GetKeyAt(randint(0, MapRObjGameUserData_Logic.GetSize() - 1));
				GameBattle_MatchInfo.MasterID = RObjGameBattle_MatchInfo.GameBattle_MatchInfo.MasterID.Value;
			}
			else
			{
				RObjGameBattle_MatchInfo.GameBattle_MatchInfo.MasterID = 0;
				GameBattle_MatchInfo.MasterID = 0;
			}
		}
		if (GameBattle_MatchInfo.State.Value == BATTLE_STATE_GAMING || GameBattle_MatchInfo.MasterID.Value == 0xFFFFFFFFFFFFFFFFU)
			return;

		//多少人能开始游戏
		if (MapRObjGameUserData_Logic.GetSize() >= BATTLE_USER_MIN)
		{
			RObjGameBattle_MatchInfo.GameBattle_MatchInfo.State = BATTLE_STATE_READY;
			GameBattle_MatchInfo.State = BATTLE_STATE_READY;
		}
		else
		{
			RObjGameBattle_MatchInfo.GameBattle_MatchInfo.State = BATTLE_STATE_WAITING;
			GameBattle_MatchInfo.State = BATTLE_STATE_WAITING;
		}
	}
	ucVOID RObjGameUser_PhyInfo_OnTimeOut(UCObject* Sender, UCEventArgs*)
	{
		RContainer->Log(UCString("注意：不应该出现的日志，home超时离开\r\n"));

		UCRObjGameUserPhyInfo* RObjGameUserPhyInfo = (UCRObjGameUserPhyInfo*)Sender;
		ucINT Pos = MapRObjGameUserData_Logic.FindKey(RObjGameUserPhyInfo->GameUserID.Value);
		if (Pos >= 0)
		{
			UCRObjGameUserData_Logic* pRObjGameUserData_Logic = MapRObjGameUserData_Logic.GetValueAt(Pos);
			//如果游戏中。暂时屏蔽
			if (0) //(State.Value == BATTLE_STATE_GAMING)
			{
				pRObjGameUserData_Logic->RObjGameUserPhyInfo.KeepValid();
// 				if (pRObjGameUserData_Logic->m_Offline == 0)
// 					pRObjGameUserData_Logic->m_Offline = 1;
			}
			else
			{
				MapRObjGameUserData_Logic.RemoveAt(Pos);
				//RContainer->Log(UCString("玩家超时离开:") + pRObjGameUserData_Logic->Nickname.Value + UCString("\r\n"));
				Leave(pRObjGameUserData_Logic);
			}
		}
	}
	ucVOID RObjGameUser_PhyInfo_OnRemove(UCObject* Object, UCEventArgs*)
	{
		UCRObjGameUserPhyInfo* RObjGameUserPhyInfo = (UCRObjGameUserPhyInfo*)Object;

		ucINT Pos = MapRObjGameUserData_Logic.FindKey(RObjGameUserPhyInfo->GameUserID.Value);

		if (Pos >= 0)
		{
			UCRObjGameUserData_Logic* pRObjGameUserData_Logic = MapRObjGameUserData_Logic.GetValueAt(Pos);
			if (RObjGameUserPhyInfo == &(pRObjGameUserData_Logic->RObjGameUserPhyInfo))
			{
				MapRObjGameUserData_Logic.RemoveAt(Pos);
				Leave(pRObjGameUserData_Logic);
			}
			else
			{
				RContainer->Log(UCString("GameDB超时离开错误:") + pRObjGameUserData_Logic->Nickname.Value + UCString("\r\n"));
			}
		}
		else
		{
			RContainer->Log(UCString("GameDB超时离开错误ID:") + ITOS64(RObjGameUserPhyInfo->GameUserID.Value) + UCString("\r\n"));
		}
	}
	ucVOID Clear()
	{
		GameBattle_MatchInfo.Clear();

		LeastTime = 0;

		for (ucINT i = 0; i < BATTLE_USER_MAX; i++)
		{
			UserPubInfo[i].Clear();
			UserPhyInfo[i].Clear();
		}
	}
public:
	UCRObjGameBattleLocal()
	{
		LeastTime = 0;
		FiberGame.FiberEvent = UCEvent(this, OnFiberGame);
	}
	~UCRObjGameBattleLocal()
	{
		for (ucINT i = 0; i < MapRObjGameUserData_Logic.GetSize(); i++)
			delete MapRObjGameUserData_Logic.GetValueAt(i);
	}
	ucBOOL Init(ucINT LogicIndex, ucINT BattleIndex, ucINT64 AdminKey, ucCONST UCString& LocalUrl, UCRContainer* RContainer)
	{
		this->LogicIndex = LogicIndex;
		this->BattleIndex = BattleIndex;
		
		this->AdminKey = AdminKey;

		this->LocalUrl = LocalUrl;
		this->RContainer = RContainer;

		RContainer->AppendSource(LocalUrl + UCString("//UCRObjGameBattle/") + ITOS(BattleIndex) + UCString(".obj"), 
			typeof(UCRObjGameBattle), typeof(UCRObjGameBattleLocal), this, true);
		RContainer->AppendSource(LocalUrl + UCString("//UCRObjGameBattle_MatchInfo/") + ITOS(BattleIndex) + UCString(".obj"),
			typeof(UCRObjGameBattle_MatchInfo), &RObjGameBattle_MatchInfo, true);
		return ucTRUE;
	}

	UCString CreateMatchGame(ucCONST UCGameMatch_BattleBase& Match_BattleBase, ucDWORD MatchIndex, ucINT MapID, ucINT LeastTime)
	{
		if (RObjGameBattle_MatchInfo.GameBattle_MatchInfo.Type.Value != BATTLE_TYPE_UNVALID)
			return UCString("房间已存在");

		this->MatchIndex = MatchIndex;
		RObjGameBattle_MatchInfo.GameBattle_MatchInfo.MapID = MapID;
		Reset();

		RObjGameBattle_MatchInfo.GameBattle_MatchInfo.Type = BATTLE_TYPE_PRIVATE;
		ucINT TotleUser = 0;
		for (ucINT i = 0; i < BATTLE_USER_MAX; i++)
		{
			if (Match_BattleBase.GameUserID[i].ID == 0xFFFFFFFFFFFFFFFFU)
				continue;
			UCRObjGameUserData_Logic* pRObjGameUserData_Logic = 0;
			UCString strRet = Join(UCGameUserID(Match_BattleBase.GameUserID[i].ID), &pRObjGameUserData_Logic);
			if (strRet == UCString("succeed"))
				TotleUser++;
			else
			{
				RContainer->Log(UCString("CreateMatchGame::Join失败:") + strRet + UCString("\r\n"));
				continue;
			}
		}
		//设置房主
		GameBattle_MatchInfo.MasterID = 0;
		RObjGameBattle_MatchInfo.GameBattle_MatchInfo.MasterID = 0;
		RObjGameBattle_MatchInfo.GameBattle_MatchInfo.State = RObjGameBattle_MatchInfo.GameBattle_MatchInfo.State.Value;

		//倒计时
		if (LeastTime < 1)
			LeastTime = 1;
		this->LeastTime = LeastTime;
		
		//m_MainFiber.Start(0);
		return UCString("succeed");
	}
	ucVOID Reset()
	{
		UCESimpleMap<ucINT64, UCRObjGameUserData_Logic*> MapPlatIDPlayer = MapRObjGameUserData_Logic;
		MapRObjGameUserData_Logic.RemoveAll();
		MapEmptySeatID.RemoveAll();
		for (ucINT i = 0; i < BATTLE_USER_MAX; i++)
			MapEmptySeatID.Add(i, i);
		Clear();
		RObjGameBattle_MatchInfo.GameBattle_MatchInfo.Clear();
		for (ucINT i = 0; i < MapPlatIDPlayer.GetSize(); i++)
			delete MapPlatIDPlayer.GetValueAt(i);
	}
};

class UCRObjGameLogicLocal : public UCRObjGameLogic
{
private:
	UCEApp*							App;
	UCEConfig<UCGameLogicInfo>		GameLogicCfg;
	UCRContainer_NET				RContainer_NET;

	UCRObjGameCenter				RObjGameCenter;
	UCRObjGameMonitor				RObjGameMonitor;
private:
	UCRObjGameBattleLocal					RObjGameBattleLocal[BATTLE_MAX];

	UCESimpleMap<ucINT, UCRObjGameBattleLocal*>	MapRObjGameBattleLocal_Play;
	UCESimpleMap<ucINT, UCRObjGameBattleLocal*>	MapRObjGameBattleLocal_Wait;
private:
	//通用Key
	UCString						LocalUrl;
	ucINT64							AdminKey;
	ucDWORD							RandKey;
	UCFiber							FiberInit;
public:
	UCString	CreateBattle(ucCONST UCGameUserID& GameUserID, ucDWORD GameKey, ucINT MatchIndex, ucINT MapID, ucINT& BattleIndex)
	{
		return UCString("local");
	}
	UCString	JoinBattle(ucCONST UCGameUserID& GameUserID, ucDWORD GameKey, ucINT MatchIndex, ucINT MapID, ucINT BattleIndex)
	{
		return UCString("local");
	}
	UCString	Direct_CreateBattle(ucCONST UCGameMatch_BattleBase& Match_BattleBase, ucINT MatchIndex, ucINT MapID, ucINT LeastTime, ucINT& BattleIndex)
	{
		if (MapRObjGameBattleLocal_Wait.GetSize() <= 0)
			return UCString("服务满");

		BattleIndex = MapRObjGameBattleLocal_Wait.GetKeyAt(0);
		MapRObjGameBattleLocal_Wait.RemoveAt(0);

		UCRObjGameBattleLocal* RObjGameBattleLocal = &this->RObjGameBattleLocal[BattleIndex];
		UCString strResult = RObjGameBattleLocal->CreateMatchGame(Match_BattleBase, MatchIndex, MapID, LeastTime);
		if (strResult != "succeed")
		{
			MapRObjGameBattleLocal_Wait.Add(BattleIndex, 0);
			return strResult;
		}

		//RObjGameBattleLocal->m_Log = &m_Log;

		MapRObjGameBattleLocal_Play.Add(BattleIndex, 0);

		//RContainer_NET.SetValue(2 + MapRObjGameBattleLocal_Play.GetSize());
		return UCString("succeed");
	}
public:
	UCRObjGameLogicLocal()
	{
		App = UCGetApp();
		if (App->Name.IsEmpty())
			App->Name = UCString("logic");

		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));

		for (ucINT i = 0; i < BATTLE_MAX; i++)
			MapRObjGameBattleLocal_Wait.Add(i, &(RObjGameBattleLocal[i]));

		LocalUrl = LocalUrl;

		FiberInit.FiberEvent = UCEvent(this, FiberInit_OnStart);
		RContainer_NET.OnEnd = UCEvent(this, OnRContainer_Exit);
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

		UCGameLogicInfo* LogicInfo = GameLogicCfg.GetData();
		ucINT AppIndex = App->Index;
		if (AppIndex < 0)
			AppIndex = 0;

		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));
		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameLogic.obj"), typeof(UCRObjGameLogic), typeof(UCRObjGameLogicLocal), this, ucTRUE);

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
		UCString Result = RObjGameCenter.GameLogic_Update(App->Index, LogicInfo->Password, AdminKey, RandKey);
		if (Result != "succeed")
		{
			RContainer_NET.Log(UCString("中心容器验证失败，请检查配置后重启容器 : ") + Result + UCString("\r\n"));
			return;
		}
		RContainer_NET.Log(UCString("验证succeed\r\n"));

		RContainer_NET.SetAdminKey(AdminKey);

		for (ucINT i = 0; i < BATTLE_MAX; i++)
		{
			RObjGameBattleLocal[i].Init(App->Index, i, AdminKey, LocalUrl, &RContainer_NET);
// 			RObjGameBattleLocal[i].OnUnValid = UCEvent(this, OnMyGameRoomUnValid);
// 			RObjGameBattleLocal[i].OnRepairUser = UCEvent(this, OnMyGameRoomRepairUser);
			MapRObjGameBattleLocal_Wait.Add(i, &(RObjGameBattleLocal[i]));
		}

		RContainer_NET.Log(UCString("当前容器初始化完毕\r\n"));
	}
	ucINT	Init()
	{
		UCFileExt_GetStatic().CreateFullPath(UCString("home"));
		if (!GameLogicCfg.LoadFromXML(UCString("home/home.xml")))
		{
			UCGameLogicInfo* LogicInfo = GameLogicCfg.GetData();
			LogicInfo->Password = "123456";
			GameLogicCfg.SaveToXML(UCString("home/home.xml"));
		}
		UCGameLogicInfo* LogicInfo = GameLogicCfg.GetData();

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

#endif	//_logic_H_