/********************************************************************
生成时间：	2021/03/19 11:58:06
文 件 名：	uchome.h
所 有 者：	
			
*********************************************************************/
#ifndef _UCHome_H_
#define _UCHome_H_
#include "uchome.ui.h"
#include "../player/avatar.h"

#include "ucprofile.h"

#include "ucusermanagement.h"
#include "ucstoremanagement.h"
#include "ucshop.h"
#include "ucinventory.h"

class UCHome : public UCHome_UI
{
public:
	UCGameUserID			GameUserID;
	ucUINT64				Token;
	UCString				Gate;

	UCProfile*				m_pProfile;
	UCUserManagement*		m_pUserManagement;
	UCStoreManagement*		m_pStoreManagement;
	UCShop*					m_pShop;
	UCInventory*			m_pInventory;

	UCMaterial				MtlObject;
	UCAvatar				Avatar;

	UCFiber					FiberRotate;
	UCFiber					FiberKeepAlive;

	UCEvent					OnMatchSucceed;
private:
	UCRObjGameHome*			RObjGameHome;
	UCRObjGameMatch*		RObjGameMatch;
	UCRObjGameBattle*		RObjGameBattle;

	UCRObjGameUserData*			RObjGameUserData;
	UCRObjGameUserData_Ext*		RObjGameUserData_Ext;
public:
	UCHome(UCRObjGameHome* RObjGameHome,
		UCRObjGameMatch* RObjGameMatch,
		UCRObjGameBattle* RObjGameBattle,
		UCRObjGameUserData* RObjGameUserData,
		UCRObjGameUserData_Ext* RObjGameUserData_Ext)	//构造函数
	{
		//默认不可见
		Visible = ucFALSE;
		Enable = ucFALSE;

		Avatar.Parent = Scene;

		this->RObjGameHome = RObjGameHome;
		this->RObjGameMatch = RObjGameMatch;
		this->RObjGameBattle = RObjGameBattle;

		this->RObjGameUserData = RObjGameUserData;
		this->RObjGameUserData_Ext = RObjGameUserData_Ext;

		RObjGameUserData->VMoney.AppendEvent(UCEvent(this, VMoney_OnChange));

		SunDir = uc3dxVector3(0.0f, -1.0f, 1.0f);
		SunBase = uc3dxVector3(0.6f, 0.6f, 0.6f);
		SunBala = uc3dxVector3(0.6f, 0.6f, 0.6f);

		MtlObject.Name = UCString("res/shader/object.material");

		FiberRotate.FiberEvent = UCEvent(this, OnFiberRotate);
		FiberKeepAlive.FiberEvent = UCEvent(this, OnFiberKeepAlive);
		FiberKeepAlive.Run(0);

		m_btTrain.OnClick = UCEvent(this, OnTrainClick);
		m_btProfile.OnClick = UCEvent(this, OnProfileClick);
		m_btShop.OnClick = UCEvent(this, OnShopClick);
		m_btInventory.OnClick = UCEvent(this, OnInventoryClick);

		m_btUserManagement.OnClick = UCEvent(this, OnUserManagementClick);
		m_btStoreManagement.OnClick = UCEvent(this, OnStoreManagementClick);

		RObjGameUserData_Ext->BattleInfo.AppendEvent(UCEvent(this, OnBattleInfoSet));

		Size.OnChange += UCEvent(this, OnMySized);

		m_pProfile = new UCProfile;
		m_pProfile->Visible = ucFALSE;
		AddControl(m_pProfile, 10000);

		m_pUserManagement = new UCUserManagement(RObjGameHome);
		m_pUserManagement->Visible = ucFALSE;
		AddControl(m_pUserManagement, 10000);

		m_pStoreManagement = new UCStoreManagement(RObjGameHome);
		m_pStoreManagement->Visible = ucFALSE;
		AddControl(m_pStoreManagement, 10000);

		m_pShop = new UCShop(RObjGameHome, RObjGameUserData, RObjGameUserData_Ext);
		m_pShop->Visible = ucFALSE;
		AddControl(m_pShop, 10000);

		m_pInventory = new UCInventory(RObjGameHome, RObjGameUserData, RObjGameUserData_Ext);
		m_pInventory->Visible = ucFALSE;
		m_pInventory->Location.OnChange += UCEvent(this, Inventory_OnMySized);
		m_pInventory->Size.OnChange += UCEvent(this, Inventory_OnMySized);
		m_pInventory->OnAvatarChanged += UCEvent(this, Inventory_OnAvatarChanged);
		AddControl(m_pInventory, 10000);
	}
	~UCHome()	//析构函数
	{
		delete m_pUserManagement;
		delete m_pStoreManagement;
		delete m_pShop;
		delete m_pInventory;
		delete m_pProfile;
	}
	ucVOID OnMySized(UCObject* Object, UCEventArgs* Args)
	{
		SceneTex->Camera.fWidth = 1.0f * Size.cx.Value;
		SceneTex->Camera.fHeight = 1.0f * Size.cy.Value;

		ucINT SizeCX = Size.cx.Value / 2;
		if (SizeCX < 352)
			SizeCX = 352;
		m_pInventory->SetNewSize(SizeCX, m_pInventory->Size.cy.Value);
		if (m_pInventory->Visible.Value == ucFALSE)
			m_pInventory->Location.x = -m_pInventory->Size.cx.Value;
	}
	ucVOID Inventory_OnMySized(UCObject* Object, UCEventArgs* Args)
	{
		m_btInventory.Location.x = m_pInventory->Size.cx.Value + m_pInventory->Location.x.Value;
	}
	ucVOID Inventory_OnAvatarChanged(UCObject* Object, UCEventArgs* Args)
	{
		UCRProAvatarData AvatarData;
		AvatarData.FaceID = RObjGameUserData->AvatarData.FaceID;
		AvatarData.HandID = RObjGameUserData->AvatarData.HairID;

		AvatarData.HairID = RObjGameUserData->AvatarData.HairID;

		AvatarData.BodyID = RObjGameUserData->AvatarData.BodyID;
		AvatarData.PantID = RObjGameUserData->AvatarData.PantID;
		AvatarData.ShoeID = RObjGameUserData->AvatarData.ShoeID;

		AvatarData.CarID = RObjGameUserData->AvatarData.CarID;

		Avatar.ChangeModel(&MtlObject, 1, AvatarData);
	}
	ucVOID VMoney_OnChange(UCObject* Object, UCEventArgs* Args)
	{
		m_lbVMoney.Text = ITOS(RObjGameUserData->VMoney.Value) + UCString("$");
	}
	ucVOID OnBattleInfoSet(UCObject* Object, UCEventArgs* Args)
	{
	}
	ucVOID OnTrainClick(UCObject* Object, UCEventArgs* Args)
	{
		UCString strResult = RObjGameMatch->Match(GameUserID, Token);

		if (!RObjGameMatch->Linked)
		{
			ucINT Code = RObjGameMatch->Relink();
			if (Code != 0)
			{
				WBox(UCString("RObjGameMatch Link失败，code = ") + ITOS(Code));
				return;
			}
			strResult = RObjGameMatch->Match(GameUserID, Token);
		}
		if (strResult != "succeed")
		{
			WBox(strResult);
			return;
		}

		Visible = ucFALSE;

		UCLabel					Matching;
		Matching.Text = UCString("Matching");
		Matching.Size = UCSize(512, 64);
		Matching.Anchor = 0;
		Matching.Align = 0;
		Matching.Font.Size = UCSize(32, 64);
		Matching.Location.x = (Parent->Size.cx.Value - Matching.Size.cx.Value) / 2;
		Matching.Location.y = (Parent->Size.cy.Value - Matching.Size.cy.Value) / 2 - 192;
		Parent->AddControl(&Matching);

		UCTimeFiberData* FiberData = (UCTimeFiberData*)GetRunFiberData();
		for (ucINT i = 100; i > 0; i--)
		{
			Matching.Alpha = i * 25 / 20;
			FiberData->Every(100);
		}

		UCLabel					MatchTime;
		MatchTime.Text = UCString("10");
		MatchTime.Size = UCSize(256, 128);
		MatchTime.Anchor = 0;
		MatchTime.Align = 0;
		MatchTime.Font.Size = UCSize(64, 128);
		MatchTime.Location.x = (Parent->Size.cx.Value - MatchTime.Size.cx.Value) / 2;
		MatchTime.Location.y = (Parent->Size.cy.Value - MatchTime.Size.cy.Value) / 2;
		Parent->AddControl(&MatchTime);

		ucINT Time = 99;
		while (FiberData->IsValid())
		{
			UCString strResult = RObjGameUserData_Ext->Update();
			if (!RObjGameUserData_Ext->Linked)
			{
				ucINT Code = RObjGameUserData_Ext->Relink();
				if (Code != 0)
				{
					WBox(UCString("RObjGameUserData_Ext Link失败，code = ") + ITOS(Code));
					return;
				}
			}

			if (RObjGameUserData_Ext->BattleInfo.LogicID != -1 &&
				RObjGameUserData_Ext->BattleInfo.BattleID != -1)
				break;

			ucINT TotalTick = Time / 10;
			ucINT LastTime = Time % 10;

			if (TotalTick % 2)
				Matching.Alpha = (10 - LastTime - 1) * 10;
			else
				Matching.Alpha = LastTime * 10;
			MatchTime.Text = ITOS(Time / 10);

			if (Time > 0)
				Time--;

			FiberData->Every(1000);
		}

		OnMatchSucceed(this, 0);
	}
	ucVOID OnProfileClick(UCObject* Object, UCEventArgs* Args)
	{
		m_pProfile->ShowUI();
	}
	ucVOID OnUserManagementClick(UCObject* Object, UCEventArgs* Args)
	{
		m_pUserManagement->ShowUI(GameUserID, Token);
	}
	ucVOID OnStoreManagementClick(UCObject* Object, UCEventArgs* Args)
	{
		m_pStoreManagement->ShowUI(GameUserID, Token);
	}
	ucVOID OnShopClick(UCObject* Object, UCEventArgs* Args)
	{
		m_pShop->ShowUI(GameUserID, Token);
	}
	ucVOID OnInventoryClick(UCObject* Object, UCEventArgs* Args)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)GetRunFiberData();
		if (m_pInventory->Visible.Value == ucFALSE)
		{
			m_btInventory.Enable = ucFALSE;
			m_pInventory->Visible = ucTRUE;
			m_pInventory->Location.x = -m_pInventory->Size.cx.Value;

			for (ucINT CurrX = m_pInventory->Location.x.Value; CurrX <= 0; CurrX += 8)
			{
				m_pInventory->Location.x = CurrX;

				FiberData->Every(100);
			}

			m_btInventory.Enable = ucTRUE;
			m_pInventory->Enable = ucTRUE;
			m_pInventory->UpdateUI(GameUserID, Token);
		}
		else
		{
			m_btInventory.Enable = ucFALSE;
			m_pInventory->Enable = ucFALSE;

			for (ucINT CurrX = m_pInventory->Location.x.Value; CurrX >= -m_pInventory->Size.cx.Value; CurrX -= 8)
			{
				m_pInventory->Location.x = CurrX;

				FiberData->Every(100);
			}

			m_pInventory->UpdateUI(GameUserID, Token);
			m_pInventory->Visible = ucFALSE;
			m_btInventory.Enable = ucTRUE;
		}
	}
	ucVOID ShowUI(ucCONST UCGameUserID& GameUserID, UCString Gate, ucUINT64 Token)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)GetRunFiberData();

		Visible = ucTRUE;
		Alpha = 0;
		for (ucINT i = 0; i < 255; i += 5)
		{
			Alpha = i;
			FiberData->Every(100);
		}
		Enable = ucTRUE;
		Alpha = 255;

		this->GameUserID = GameUserID;
		this->Gate = Gate;
		this->Token = Token;

		if (RObjGameUserData->Access_Level.Value > 1)
		{
			m_btTrain.Visible = ucFALSE;
			m_btMatch.Visible = ucFALSE;
			Avatar.SetAllVisible(ucFALSE);
		}

		if (RObjGameUserData->Access_Level.Value >= 1)
			m_btStoreManagement.Visible = 1;

		if (RObjGameUserData->Access_Level.Value >= 2)
			m_btUserManagement.Visible = 1;

		m_lbNickname.Text = RObjGameUserData->Nickname.Value;
		m_lbVMoney.Text = ITOS(RObjGameUserData->VMoney.Value) + UCString("$");

		Enter();
	}
	ucVOID HideUI()
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)GetRunFiberData();

		Enable = ucFALSE;
		for (ucINT i = 255; i > 0; i -= 5)
		{
			Alpha = i;
			FiberData->Every(100);
		}
		Visible = ucFALSE;
		Alpha = 0;
	}
	ucVOID Enter()
	{
		SceneTex->Camera.Pos.y = 30.0f;
		SceneTex->Camera.fWidth = RealSize.cx.Value;
		SceneTex->Camera.fHeight = RealSize.cy.Value;
		SceneTex->Camera.fFOV = 3.14f / 2.0f;
		SceneTex->Camera.distance = -80.0f;

		UCRProAvatarData AvatarData;
		AvatarData.FaceID = RObjGameUserData->AvatarData.FaceID;
		AvatarData.HandID = RObjGameUserData->AvatarData.HairID;

		AvatarData.HairID = RObjGameUserData->AvatarData.HairID;

		AvatarData.BodyID = RObjGameUserData->AvatarData.BodyID;
		AvatarData.PantID = RObjGameUserData->AvatarData.PantID;
		AvatarData.ShoeID = RObjGameUserData->AvatarData.ShoeID;

		AvatarData.CarID = RObjGameUserData->AvatarData.CarID;

		Avatar.ChangeModel(&MtlObject, 1, AvatarData);

		if (RObjGameUserData->Nickname.Value.IsEmpty())
			m_lbNickname.Text = RObjGameUserData->Username.Value;
		else
			m_lbNickname.Text = RObjGameUserData->Nickname.Value;

		m_lbVMoney.Text = ITOS(RObjGameUserData->VMoney.Value) + UCString("$");

		FiberRotate.Run(0);
	}
	ucVOID OnFiberKeepAlive(UCObject* Sender, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)GetRunFiberData();

		while (FiberData->IsValid())
		{
			RObjGameUserData->Update();
			RObjGameUserData_Ext->Update();
			FiberData->Every(10000);
		}
	}
	ucVOID OnFiberRotate(UCObject* Sender, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)GetRunFiberData();

		while (FiberData->IsValid())
		{
			for (ucINT i = 0; i < 400; i++)
			{
				Avatar.Rot.y += 0.002f;
				FiberData->Every(100);
			}
			for (ucINT i = 0; i < 800; i++)
			{
				Avatar.Rot.y -= 0.002f;
				FiberData->Every(100);
			}
			for (ucINT i = 0; i < 400; i++)
			{
				Avatar.Rot.y += 0.002f;
				FiberData->Every(100);
			}
			FiberData->Every(100);
		}
	}
};

#endif //_UCHome_H_