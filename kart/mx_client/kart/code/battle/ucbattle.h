/********************************************************************
生成时间：	2021/04/08 21:55:07
文 件 名：	ucbattle.h
所 有 者：	
			
*********************************************************************/
#ifndef _UCBattle_H_
#define _UCBattle_H_
#include "ucbattle.ui.h"
#include "map/map01.h"

struct UCSyncData
{
	uc3dxVector3	Pos;
	ucFLOAT			RotY;
	UCSyncData()
	{
	}
	UCSyncData(ucCONST UCSyncData& in)
	{
		Pos = in.Pos;
		RotY = in.RotY;
	}
};

class UCBattle : public UCBattle_UI
{
public:
	UCEventManager	OnExit;
private:
	UCGameUserID	GameUserID;
	ucUINT64		Token;

	ucINT			MapID;
	UCKartMap*		Map;

	ucINT			SeatID;

	ucBOOL			MouseDown;
	uc3dxVector3	CameraRot;

	UCFiber			FiberSync;
	UCFiber			FiberSyncSend;
	UCFiber			FiberGame;

	UCEList<UCSyncData>	ListSyncData;

	UCButton		BT_Exit;
	UCTextBox		TB_Position;

	UCLabel			LB_Time;
private:
	UCRObjGameBattle*	RObjGameBattle;
public:
	UCBattle(UCRObjGameBattle* RObjGameBattle)	//构造函数
	{
		MapID = 1;
		Map = ucNULL;

		SeatID = -1;

		MouseDown = ucFALSE;

		this->RObjGameBattle = RObjGameBattle;
		GameUserID.Set(0);
		Token = 0;

		Visible = 0;

		m_imgWheel.Picture.Center = UCPoint(512 / 2, 256);

		World->GravityY = -20.0f;

		FiberGame.FiberEvent = UCEvent(this, OnFiberGame);
		FiberSync.FiberEvent = UCEvent(this, OnFiberSync);
		FiberSyncSend.FiberEvent = UCEvent(this, OnFiberSyncSend);

		FiberGame.AddChild(&FiberSync);
		FiberGame.AddChild(&FiberSyncSend);

		this->OnMouseDownEx += UCEvent(this, OnMyMouseDown);
		this->OnMouseUpEx += UCEvent(this, OnMyMouseUp);
		this->OnMouseMoveEx += UCEvent(this, OnMyMouseMove);

		this->Size.OnChange += UCEvent(this, OnMySized);

		TB_Position.Size = UCSize(120, 20);
		AddControl(&TB_Position);

		BT_Exit.Size = UCSize(120, 48);
		BT_Exit.Location = UCPoint(0, 20);
		BT_Exit.Text = UCString("退出");
		BT_Exit.OnClick += UCEvent(this, OnBT_ExitClick);
		AddControl(&BT_Exit);

		LB_Time.Anchor = 0;
		LB_Time.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Text = 0xFFFF0000;
		LB_Time.FontColor.TextColor[UCDISPLAYMODE_DARK].Text = 0xFFFF0000;
		LB_Time.Font.Size = UCSize(72, 144);
		LB_Time.SetNewSize(144, 144);
		LB_Time.Location = UCPoint((Size.cx.Value - LB_Time.Size.cx.Value) / 2, (Size.cy.Value - LB_Time.Size.cy.Value) / 2);
		AddControl(&LB_Time);
	}
	~UCBattle()	//析构函数
	{
		ExitMap();
	}
	ucVOID UserPubInfoGameUserID_OnSet(UCObject*, UCEventArgs* e)
	{
		UCRPropertyArgs* Args = (UCRPropertyArgs*)e;

		ucINT Index = RObjGameBattle->UserPubInfo[0].GetArrayIndex(Args->Data);

		if (RObjGameBattle->UserPubInfo[Index].GameUserID.Value == -1 && Index == SeatID)
			OnExit.RunNew(this, ucNULL);
	}
	ucVOID OnBT_ExitClick(UCObject*, UCEventArgs*)
	{
		RObjGameBattle->ExitGame(GameUserID, Token);
	}
	ucVOID InitUser(UCKartMap* KartMap)
	{
		for (ucINT i = 0; i < BATTLE_USER_MAX && i < MAX_USER; i++)
		{
			UCRGameUserPubInfo& UserPubInfo = RObjGameBattle->UserPubInfo[i];
			if (UserPubInfo.GameUserID.Value == -1)
				continue;

			UCHuman* Human = new UCHuman;

			UCRProAvatarData AvatarData = UserPubInfo.AvatarData;

			if (AvatarData.FaceID < 0)
				AvatarData.FaceID = 0;
			if (AvatarData.HairID < 0)
				AvatarData.HairID = 23;

			if (AvatarData.BodyID < 0)
				AvatarData.BodyID = 23;
			if (AvatarData.HandID < 0)
				AvatarData.HandID = 23;

			if (AvatarData.PantID < 0)
				AvatarData.PantID = 23;
			if (AvatarData.ShoeID < 0)
				AvatarData.ShoeID = 23;

			if (AvatarData.CarID < 0)
				AvatarData.CarID = 7020;

			Human->CarID = AvatarData.CarID;
			Human->BallID = -1;
			Human->VentID = 8002;
			Human->HairID = AvatarData.HairID;
			Human->FaceID = AvatarData.FaceID;
			Human->BodyID = AvatarData.BodyID;
			Human->ShoeID = AvatarData.ShoeID;
			Human->HandID = AvatarData.HandID;
			Human->PantID = AvatarData.PantID;
			Human->SuitID = -1;
			Human->ID = 1 + i;
			Human->SexKind = 0;
			Human->GroupID = randint(11, 12);
			Human->LicenceID = -1;

			Map->Users[i] = Human;

			Human->Init(this, &Map->Manage);

			if (GameUserID.ID == UserPubInfo.GameUserID.Value)
			{
				SeatID = i;
				Map->Self = Map->Users[i];
				Map->Self->Physics.IsSelf = 1;
				Human->Physics.Center.Enable = 1;
			}
			else
				Human->Physics.Center.Enable = 0;
			Human->Physics.State.SetParentFiber(&FiberGame);
		}

		//启动飘逸Fiber
		FiberGame.AddChild(&Map->Self->Physics.FiberDrift);
		Map->Self->Physics.FiberDrift.Start(0);
	}
	ucVOID InitMap()
	{
		ShadowTex->Material.Name = UCString("res/shader/shadow_map.material");
		ShadowTex->Sun = 1;
		ShadowTex->Size = UCSize(1024, 1024);
		ShadowTex->Camera.fFOV = 0.0f;
		ShadowTex->Camera.distance = -2400.0f;
		ShadowTex->Camera.fWidth = 1024.0f;
		ShadowTex->Camera.fHeight = 1024.0f;
		ShadowTex->AppendFilter(0);
		ShadowTex->ResetOutPins(1);

		if (MapID == 1)
		{
			Map = new UCKartMap01;

			UCKartMap01* Map01 = (UCKartMap01*)Map;
			Map01->Init(this);

			InitUser(Map);
			Map01->Posite();
		}

		Map->Camera.fWidth = Size.cx.Value;
		Map->Camera.fHeight = Size.cy.Value;
		Map->Camera.Rot.x = 0.3f;
		Map->Camera.fFOV = UC3D_PI / 2.0f;
		Map->Camera.distance = -200.0f;
		Map->Camera.BindMesh(&Map->Self->Physics.CenterBody);
		SceneTex->Output = &Map->Camera;

		OnBeforeRender3D = UCEvent(this, OnShadowUpdate);
	}
	ucVOID ExitMap()
	{
		if (Map)
		{
			if (MapID == 1)
				delete (UCKartMap01*)Map;
			Map = ucNULL;
		}
	}
	ucVOID ShowUI(ucCONST UCGameUserID& GameUserID, ucUINT64 Token)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)GetRunFiberData();

		this->GameUserID.Set(GameUserID);
		this->Token = Token;

		InitMap();

		Visible = ucTRUE;
		Alpha = 0;
		for (ucINT i = 0; i < 255; i += 5)
		{
			Alpha = i;
			FiberData->Every(100);
		}
		Enable = ucTRUE;
		Alpha = 255;

		FiberGame.Run(0);
	}
	ucVOID HideUI()
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)GetRunFiberData();

		Enable = ucFALSE;
		Alpha = 255;
		for (ucINT i = 0; i < 255; i += 5)
		{
			Alpha = 255 - i;
			FiberData->Every(100);
		}
		Alpha = 0;
		Visible = ucFALSE;

		FiberGame.Stop();
		ExitMap();
	}
	ucVOID OnShadowUpdate(UCObject* o, UCEventArgs* e)
	{
		uc3dxMatrix4	matViewProj = (*ShadowTex->Camera.GetView()) * (*ShadowTex->Camera.GetProj());
		Map->Material.SetEffectValue(UCString("g_matLightViewProj"), &matViewProj, typeof(uc3dxMatrix4));
	}
	ucVOID OnMySized(UCObject* Sender, UCEventArgs* e)
	{
		if (Map)
		{
			Map->Camera.fWidth = RealSize.cx.Value;
			Map->Camera.fHeight = RealSize.cy.Value;
		}
	}
	ucVOID OnMyMouseDown(UCObject* Sender, UCEventArgs* e)
	{
		UCMouseEventArgs* Args = (UCMouseEventArgs*)e;
		MouseDown = ucTRUE;
		CameraRot = SceneTex->Camera.Rot.Value();

		KeyState.IsUP = ucTRUE;
	}
	ucVOID OnMyMouseMove(UCObject* Sender, UCEventArgs* e)
	{
		UCMouseEventArgs* Args = (UCMouseEventArgs*)e;
		if (MouseDown)
		{
			UCPoint Dis = Args->Position - UCPoint(Size.cx.Value / 2, Size.cy.Value);
			m_imgWheel.Picture.Angle = UC3D_PI * Dis.x / RealSize.cx.Value;

			ucFLOAT fAngle = m_imgWheel.Picture.Angle.Value;

			if (fAngle < 0.0f)
				KeyState.IsLEFT = ucTRUE;
			else
				KeyState.IsLEFT = ucFALSE;

			if (fAngle > 0.0f)
				KeyState.IsRIGHT = ucTRUE;
			else
				KeyState.IsRIGHT = ucFALSE;
			//Map->Camera.Rot.y = CameraRot.y + fAngle;
		}
	}
	ucVOID OnMyMouseUp(UCObject* Sender, UCEventArgs* e)
	{
		MouseDown = ucFALSE;
		KeyState.IsUP = ucFALSE;

		KeyState.IsLEFT = ucFALSE;
		KeyState.IsRIGHT = ucFALSE;

		m_imgWheel.Picture.Angle = 0.0f;
	}
	ucVOID OnPlayer_PhyInfo(UCObject* Sender, UCEventArgs* e)
	{
		UCRPropertyArgs* Args = (UCRPropertyArgs*)e;

		ucINT ArrayIndex = RObjGameBattle->UserPhyInfo[0].GetArrayIndex(Args->Data);

		UCRGameUserPubInfo& UserPubInfo = RObjGameBattle->UserPubInfo[ArrayIndex];
		if (UserPubInfo.GameUserID.Value == -1)
			return;

		UCHuman* Human = Map->Users[ArrayIndex];
		if (Human == ucNULL || Human->Physics.IsSelf)
			return;

		UCRGameUserPhyInfo* UserPhyInfo = &(RObjGameBattle->UserPhyInfo[ArrayIndex]);

		UCCarPhyInfo CarPhyInfo;
		CarPhyInfo.Pos = UserPhyInfo->Pos;
		CarPhyInfo.RotY = UserPhyInfo->RotY;
		CarPhyInfo.FPS = UserPhyInfo->FPS;
		Human->Physics.AryPhyInfo.Add(CarPhyInfo);
	}
	ucVOID OnFiberSyncSend(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* FiberData = (UCTickFiberData*)Sender;

		while (FiberData->IsValid())
		{
			if (ListSyncData.GetCount() >= 2)
			{
				UCSyncData SyncData1 = ListSyncData.GetHead();	ListSyncData.RemoveAt(ListSyncData.GetHeadPosition());
				UCSyncData SyncData2 = ListSyncData.GetHead();	ListSyncData.RemoveAt(ListSyncData.GetHeadPosition());

				UCString strRet = RObjGameBattle->Sync(GameUserID, Token, SyncData1.Pos, SyncData1.RotY,
					SyncData2.Pos, SyncData2.RotY);
			}
			FiberData->SyncTick(2);
		}
	}
	ucVOID OnFiberSync(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* FiberData = (UCTickFiberData*)Sender;

		for (ucINT i = 0; i < BATTLE_USER_MAX && i < MAX_USER; i++)
		{
			UCRGameUserPhyInfo& UserPhyInfo = RObjGameBattle->UserPhyInfo[i];

			UserPhyInfo.AppendEvent(UCEvent(this, OnPlayer_PhyInfo));
		}

		ucINT			FPS = 0;
		ucINT			PART = 4;
		while (FiberData->IsValid())
		{
			UCSyncData& SyncData = ListSyncData.NewHead();
			SyncData.Pos = Map->Self->Physics.Center.Pos.Value();
			SyncData.RotY = Map->Self->Physics.CenterBody.Rot.y.Value;
			FPS++;

			//RObjGameBattle->RContainer->Log(VTOS(SyncData.Pos) + UCString("\r\n"));
			for (ucINT i = 0; i < BATTLE_USER_MAX && i < MAX_USER; i++)
			{
				UCRGameUserPhyInfo& UserPhyInfo = RObjGameBattle->UserPhyInfo[i];
				UCHuman* Human = Map->Users[i];

				if (Human == ucNULL || Human->Physics.IsSelf)
					continue;

				if (Human->Physics.AryPhyInfo.GetSize() > 2)
				{
					ucINT CacheMax = 10000;
					if (Human->Physics.AryPhyInfo.GetSize() > CacheMax)
					{
						UCCarPhyInfo& CarPhyInfo = Human->Physics.AryPhyInfo.GetAt(Human->Physics.AryPhyInfo.GetSize() - 1);
						Human->Physics.Center.Pos = CarPhyInfo.Pos;
						Human->Physics.CenterBody.Rot.y = CarPhyInfo.RotY;
						Human->Physics.AryPhyInfo.RemoveAll();
					}
					else
					{
						UCCarPhyInfo& CarPhyInfo = Human->Physics.AryPhyInfo.GetAt(0);

						ucINT FPSPart = (FPS % PART);
						ucFLOAT Tick = 4.0f - FPSPart;
						if (FPSPart < (PART - 1))
						{
							Human->Physics.Center.Pos = Human->Physics.Center.Pos.Value() + (CarPhyInfo.Pos - Human->Physics.Center.Pos.Value()) / Tick;
							Human->Physics.CenterBody.Rot.y = Human->Physics.CenterBody.Rot.y.Value + (CarPhyInfo.RotY - Human->Physics.CenterBody.Rot.y.Value) / Tick;
						}
						else
						{
							Human->Physics.Center.Pos = CarPhyInfo.Pos;
							Human->Physics.CenterBody.Rot.y = CarPhyInfo.RotY;
							Human->Physics.AryPhyInfo.RemoveAt(0);
						}
					}

					TB_Position.Text = ITOS(Human->Physics.AryPhyInfo.GetSize()) + UCString(" = ") + VTOS(Human->Physics.Center.Pos.Value());
				}
			}

			//ShadowTex->Center.Pos = Map->Self->Physics.Center.Pos.Value();

			//每秒10个包
			FiberData->SyncTick(8);
		}
	}
	ucVOID OnFiberGame(UCObject* Sender, UCEventArgs* e)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Sender;

		for (ucINT i = 0; i < BATTLE_USER_MAX; i++)
			RObjGameBattle->UserPubInfo[i].GameUserID.AppendEvent(UCEvent(this, UserPubInfoGameUserID_OnSet));

		if (FiberData)
		{
			//FiberData->PerMinEvery = 0.0f;		//最小补帧时间百分比（0-1有效，<=0相当于最小Every(0)，>=1相当于Delay���
			FiberData->MaxError = 20000;
			//m_BlackPic.Visible = 0;
		}
		Map->Self->Physics.PortalObject.WayKind = 1;

		UCDevice3D* pDevice = UIGetDevice3D();

		RObjGameBattle->StartGame(GameUserID, Token);

		if (RObjGameBattle->GameBattle_MatchInfo.State.Value == BATTLE_STATE_GAMING)
		{
			Map->Self->Physics.Center.Pos = RObjGameBattle->UserPhyInfo[SeatID].Pos;
			Map->Self->Physics.Center.Rot.y = RObjGameBattle->UserPhyInfo[SeatID].RotY;
		}

		FiberSync.Run(0);
		FiberSyncSend.Run(0);

		LB_Time.Visible = ucTRUE;

		if (RObjGameBattle->GameBattle_MatchInfo.State.Value == BATTLE_STATE_READY)
		{
			for (ucINT i = RObjGameBattle->LeastTime.Value - 1; i >= 0; i--)
			{
				LB_Time.Text = ITOS(i);
				FiberData->Every(10000);
			}
		}
		LB_Time.Visible = ucFALSE;

		Map->Self->Physics.Center.AngularVel = uc3dxVector3(0.0f, 0.0f, 0.0f);
		Map->Self->Physics.Center.LinearVel = uc3dxVector3(0.0f, 0.0f, 0.0f);
		Map->Self->Physics.Center.GravityMode = 1;

		if (!KeyState.KEYIsUp())
			Map->Self->Physics.DriftUpTime = 20;

		while (FiberData->IsValid()) 
		{
			//Map->RefreshPane();
			//Map->HumanStateShow();

			Map->Self->Physics.MoveCheck();
			Map->Step();

			//80 FPS
			FiberData->Every(125);
		}

		for (ucINT i = 0; i < BATTLE_USER_MAX; i++)
			RObjGameBattle->UserPubInfo[i].GameUserID.RemoveEvent(UCEvent(this, UserPubInfoGameUserID_OnSet));

		FiberSync.Stop();
		FiberSyncSend.Stop();
	}
};

#endif //_UCBattle_H_