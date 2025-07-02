/********************************************************************
生成时间：	2021/03/19 11:17:03
文 件 名：	ucregister.h
所 有 者：	
			
*********************************************************************/
#ifndef _UCUserManagement_H_
#define _UCUserManagement_H_
#include "ucusermanagement.ui.h"

class UCUserTool : public UCControl
{
public:
	UCButton			Apply;
	UCButton			Cancel;

	UCEventManager		OnConfirm;

	UCGameUserData*		GameUserData;
public:
	UCUserTool()
	{
		GameUserData = ucNULL;

		Apply.Font.Size = UCSize(16, 32);
		Apply.Text = UCString("√");
		Apply.Size = UCSize(Size.cx.Value / 2, Size.cy.Value);
		Apply.Anchor = 1 | 4 | 8;
		Apply.Visible = ucFALSE;
		Apply.OnClick += UCEvent(this, Apply_OnClick);
		AddControl(&Apply);

		Cancel.Font.Size = UCSize(16, 32);
		Cancel.Text = UCString("×");
		Cancel.Size = UCSize(Size.cx.Value / 2, Size.cy.Value);
		Cancel.Anchor = 2 | 4 | 8;
		Cancel.Visible = ucFALSE;
		Cancel.OnClick += UCEvent(this, Cancel_OnClick);
		AddControl(&Cancel);

		OnAddedToParent += UCEvent(this, This_OnAddedToParent);
	}
	~UCUserTool()
	{
		if (GameUserData)
			delete GameUserData;
	}
	ucVOID This_OnAddedToParent(UCObject* Object, UCEventArgs* e)
	{
	}
	ucVOID Apply_OnClick(UCObject* Object, UCEventArgs* e)
	{
		UpdateDataFromUI();
		OnConfirm(this, 0);
	}
	ucVOID Cancel_OnClick(UCObject* Object, UCEventArgs* e)
	{
		UpdateUIFromData();
	}
	
	// Helper function to convert Avatar data to string
	UCString AvatarToString(ucCONST UCAvatarData& AvatarData)
	{
		return ITOS(AvatarData.FaceID) + UCString(",") +
			   ITOS(AvatarData.HairID) + UCString(",") +
			   ITOS(AvatarData.BodyID) + UCString(",") +
			   ITOS(AvatarData.HandID) + UCString(",") +
			   ITOS(AvatarData.PantID) + UCString(",") +
			   ITOS(AvatarData.ShoeID) + UCString(",") +
			   ITOS(AvatarData.CarID);
	}
	
	// Helper function to convert string to Avatar data
	ucVOID StringToAvatar(ucCONST UCString& AvatarString, UCAvatarData& AvatarData)
	{
		UCEArray<ucINT> AryAvatar;
		StringToIntArray(AvatarString, AryAvatar);
		if (AryAvatar.GetSize() >= 7)
		{
			AvatarData.FaceID = AryAvatar[0];
			AvatarData.HairID = AryAvatar[1];
			AvatarData.BodyID = AryAvatar[2];
			AvatarData.HandID = AryAvatar[3];
			AvatarData.PantID = AryAvatar[4];
			AvatarData.ShoeID = AryAvatar[5];
			AvatarData.CarID = AryAvatar[6];
		}
		else
		{
			// Set default values if string is invalid
			AvatarData.FaceID = -1;
			AvatarData.HairID = -1;
			AvatarData.BodyID = -1;
			AvatarData.HandID = -1;
			AvatarData.PantID = -1;
			AvatarData.ShoeID = -1;
			AvatarData.CarID = -1;
		}
	}
	
	ucVOID UpdateDataFromUI()
	{
		UCLabel* lbUsername = (UCLabel*)Parent->AryControl[1];
		GameUserData->Username = lbUsername->Text.Value;

		UCTextBox* tbAccess = (UCTextBox*)Parent->AryControl[2];
		GameUserData->Access_Level = STOI(tbAccess->Text.Value);

		UCTextBox* tbVMoney = (UCTextBox*)Parent->AryControl[3];
		GameUserData->VMoney = STOI(tbVMoney->Text.Value);

		UCTextBox* tbNickname = (UCTextBox*)Parent->AryControl[4];
		GameUserData->Nickname = tbNickname->Text.Value;

		UCTextBox* tbEmail = (UCTextBox*)Parent->AryControl[5];
		GameUserData->Email = tbEmail->Text.Value;

		UCTextBox* tbItems = (UCTextBox*)Parent->AryControl[6];
		StringToIntArray(tbItems->Text.Value, GameUserData->AryItemData);

		UCTextBox* tbAvatar = (UCTextBox*)Parent->AryControl[7];
		StringToAvatar(tbAvatar->Text.Value, GameUserData->AvatarData);

		Apply.Visible = ucFALSE;
		Cancel.Visible = ucFALSE;
	}
	ucVOID UpdateUIFromData()
	{
		UCLabel* lbUsername = (UCLabel*)Parent->AryControl[1];
		lbUsername->Text = GameUserData->Username;
		lbUsername->Font.Size = UCSize(16, 32);

		UCTextBox* tbAccess = (UCTextBox*)Parent->AryControl[2];
		tbAccess->Text = ITOS(GameUserData->Access_Level);
		tbAccess->Font.Size = UCSize(16, 32);

		UCTextBox* tbVMoney = (UCTextBox*)Parent->AryControl[3];
		tbVMoney->Text = ITOS(GameUserData->VMoney);
		tbVMoney->Font.Size = UCSize(16, 32);

		UCTextBox* tbNickname = (UCTextBox*)Parent->AryControl[4];
		tbNickname->Text = GameUserData->Nickname;
		tbNickname->Font.Size = UCSize(16, 32);

		UCTextBox* tbEmail = (UCTextBox*)Parent->AryControl[5];
		tbEmail->Text = GameUserData->Email;
		tbEmail->Font.Size = UCSize(16, 32);

		UCTextBox* tbItems = (UCTextBox*)Parent->AryControl[6];
		tbItems->Text = IntArrayToString(GameUserData->AryItemData);
		tbItems->Font.Size = UCSize(16, 32);

		UCTextBox* tbAvatar = (UCTextBox*)Parent->AryControl[7];
		tbAvatar->Text = AvatarToString(GameUserData->AvatarData);
		tbAvatar->Font.Size = UCSize(16, 32);

		Apply.Visible = ucFALSE;
		Cancel.Visible = ucFALSE;
	}
};

class UCUserTextBox : public UCTextBox
{
public:
	UCUserTextBox()
	{
		Text.OnSet += UCEvent(this, Text_OnSet);
	}
	ucVOID Text_OnSet(UCObject* Object, UCEventArgs* e)
	{
		UCUserTool* UserTool = (UCUserTool*)Parent->AryControl[0];

		UserTool->Apply.Visible = ucTRUE;
		UserTool->Cancel.Visible = ucTRUE;
	}
};

class UCUserManagement : public UCUserManagement_UI
{
public:
	UCGameUserID		GameUserID;
	ucUINT64			Token;
	UCRObjGameHome*		RObjGameHome;
public:
	UCUserManagement(UCRObjGameHome* RObjGameHome)	//构造函数
	{
		//默认不可见
		Visible = ucFALSE;
		Enable = ucFALSE;

		this->RObjGameHome = RObjGameHome;
		this->GameUserID.Set(0);
		this->Token = 0;

		m_lbUsers.InsertCol(UCString("Tool"), -1, 120, typeof(UCUserTool));

		m_lbUsers.InsertCol(UCString("Username"), -1, 400, typeof(UCLabel));
		m_lbUsers.InsertCol(UCString("Access"), -1, 120, typeof(UCUserTextBox));

		m_lbUsers.InsertCol(UCString("VMoney"), -1, 120, typeof(UCUserTextBox));

		m_lbUsers.InsertCol(UCString("Nickname"), -1, 400, typeof(UCUserTextBox));
		m_lbUsers.InsertCol(UCString("Email"), -1, 500, typeof(UCUserTextBox));

		m_lbUsers.InsertCol(UCString("Items"), -1, 500, typeof(UCUserTextBox));

		m_lbUsers.InsertCol(UCString("Avatar"), -1, 500, typeof(UCUserTextBox));

		UCControl* Head = m_lbUsers.GetControl(0, -1);
		Head->SetNewSize(Head->Size.cx.Value, 48);

		for (ucINT i = 0; i < m_lbUsers.GetColSize(); i++)
		{
			UCLabel* Label = (UCLabel*)m_lbUsers.GetControl(0, i);
			Label->Font.Size = UCSize(16, 32);
			Label->Size.cy = 48;
		}

		m_lbUsers.DockView();

		m_btReturn.OnClick += UCEvent(this, OnReturnClick);
	}
	~UCUserManagement()	//析构函数
	{
	}
	ucVOID ShowUI(ucCONST UCGameUserID& GameUserID, ucUINT64 Token)
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
		this->Token = Token;

		UCEArray<UCGameUserData> AryUser;
		RObjGameHome->GetAllUsers(GameUserID, Token, 0, 20, AryUser);

		m_lbUsers.EmptyRow();
		for (ucINT i = 0; i < AryUser.GetSize(); i++)
		{
			UCGameUserData& GameUserData = AryUser[i];

			m_lbUsers.InsertRow(48);

			UCUserTool* UserTool = (UCUserTool*)m_lbUsers.GetControl(i + 1, 0);
			UserTool->GameUserData = new UCGameUserData(GameUserData);
			UserTool->OnConfirm += UCEvent(this, UserTool_OnConfirm);

			UserTool->UpdateUIFromData();
		}
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
	}

	ucVOID OnReturnClick(UCObject*, UCEventArgs*)
	{
		HideUI();
	}

	ucVOID UserTool_OnConfirm(UCObject* Object, UCEventArgs*)
	{
		UCUserTool* UserTool = (UCUserTool*)Object;

		UCString Result = RObjGameHome->ChangeProfile(GameUserID, Token, *UserTool->GameUserData);
		m_tbTip.Text = Result;
	}
};

#endif //_UCUserManagement_H_