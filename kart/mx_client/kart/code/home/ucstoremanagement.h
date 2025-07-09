/********************************************************************
生成时间：	2025/06/24 10:00:00
文 件 名：	ucstoremanagement.h
所 有 者：	
			
*********************************************************************/
#ifndef _UCStoreManagement_H_
#define _UCStoreManagement_H_
#include "ucstoremanagement.ui.h"
// 包含服务器端的商品数据结构定义
#include "../../../../mx_server/container/private/common/game_homebase.h"

class UCStoreTool : public UCControl
{
public:
	UCButton			Apply;
	UCButton			Cancel;

	UCEventManager		OnConfirm;
	UCEventManager		OnCancel;

	UCGameStoreData*	GameStoreData;
	UCGameStoreData*	GameStoreDataBackup;
public:
	UCStoreTool()
	{
		GameStoreData = ucNULL;
		GameStoreDataBackup = ucNULL;

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
	~UCStoreTool()
	{
		if (GameStoreData)
			delete GameStoreData;
		if (GameStoreDataBackup)
			delete GameStoreDataBackup;
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
		UpdateUIFromData(ucFALSE);
		OnCancel(this, 0);
	}
	ucVOID SetData(UCGameStoreData& GameStoreData)
	{
		if (this->GameStoreData != ucNULL)
			delete this->GameStoreData;
		this->GameStoreData = new UCGameStoreData(GameStoreData);
		if (GameStoreDataBackup != ucNULL)
			delete GameStoreDataBackup;
		GameStoreDataBackup = new UCGameStoreData(GameStoreData);
	}
	ucVOID UpdateDataFromUI()
	{
		UCTextBox* lbName = (UCTextBox*)Parent->AryControl[1];
		GameStoreData->Name = lbName->Text.Value;

		UCTextBox* tbDescription = (UCTextBox*)Parent->AryControl[2];
		GameStoreData->Description = tbDescription->Text.Value;

		UCTextBox* tbType = (UCTextBox*)Parent->AryControl[3];
		GameStoreData->Type = STOI(tbType->Text.Value);

		UCTextBox* tbPrice = (UCTextBox*)Parent->AryControl[4];
		GameStoreData->Price = STOI(tbPrice->Text.Value);

		UCTextBox* tbResID = (UCTextBox*)Parent->AryControl[5];
		GameStoreData->ResID = STOI(tbResID->Text.Value);

		Apply.Visible = ucFALSE;
		Cancel.Visible = ucFALSE;
	}
	ucVOID UpdateUIFromData(ucBOOL ChangeName)
	{
		UCTextBox* lbName = (UCTextBox*)Parent->AryControl[1];
		lbName->Enable = ChangeName;
		lbName->Text = GameStoreDataBackup->Name;
		lbName->Font.Size = UCSize(16, 32);

		UCTextBox* tbDescription = (UCTextBox*)Parent->AryControl[2];
		tbDescription->Text = GameStoreDataBackup->Description;
		tbDescription->Font.Size = UCSize(16, 32);

		UCTextBox* tbType = (UCTextBox*)Parent->AryControl[3];
		tbType->Text = ITOS(GameStoreDataBackup->Type);
		tbType->Font.Size = UCSize(16, 32);

		UCTextBox* tbPrice = (UCTextBox*)Parent->AryControl[4];
		tbPrice->Text = ITOS(GameStoreDataBackup->Price);
		tbPrice->Font.Size = UCSize(16, 32);

		UCTextBox* tbResID = (UCTextBox*)Parent->AryControl[5];
		tbResID->Text = ITOS(GameStoreDataBackup->ResID);
		tbResID->Font.Size = UCSize(16, 32);

		Apply.Visible = ucFALSE;
		Cancel.Visible = ucFALSE;
	}
};

class UCStoreTextBox : public UCTextBox
{
public:
	UCStoreTextBox()
	{
		Text.OnSet += UCEvent(this, Text_OnSet);
	}
	ucVOID Text_OnSet(UCObject* Object, UCEventArgs* e)
	{
		UCStoreTool* StoreTool = (UCStoreTool*)Parent->AryControl[0];

		StoreTool->Apply.Visible = ucTRUE;
		StoreTool->Cancel.Visible = ucTRUE;
	}
};

class UCStoreManagement : public UCStoreManagement_UI
{
public:
	UCGameUserID		GameUserID;
	ucUINT64			Token;
	UCRObjGameHome*		RObjGameHome;
public:
	UCStoreManagement(UCRObjGameHome* RObjGameHome)	//Constructor
	{
		//Default invisible
		Visible = ucFALSE;
		Enable = ucFALSE;

		this->RObjGameHome = RObjGameHome;
		this->GameUserID.Set(0);
		this->Token = 0;

		m_lbStores.InsertCol(UCString("Tool"), -1, 120, typeof(UCStoreTool));

		m_lbStores.InsertCol(UCString("Name"), -1, 200, typeof(UCStoreTextBox));
		m_lbStores.InsertCol(UCString("Description"), -1, 800, typeof(UCStoreTextBox));

		m_lbStores.InsertCol(UCString("Type"), -1, 150, typeof(UCStoreTextBox));
		m_lbStores.InsertCol(UCString("Price"), -1, 120, typeof(UCStoreTextBox));
		m_lbStores.InsertCol(UCString("ResID"), -1, 100, typeof(UCStoreTextBox));


		UCControl* Head = m_lbStores.GetControl(0, -1);
		Head->SetNewSize(Head->Size.cx.Value, 48);

		for (ucINT i = 0; i < m_lbStores.GetColSize(); i++)
		{
			UCLabel* Label = (UCLabel*)m_lbStores.GetControl(0, i);
			Label->Font.Size = UCSize(16, 32);
			Label->Size.cy = 48;
		}

		m_lbStores.DockView();

		m_btNew.OnClick += UCEvent(this, OnNewClick);
		m_btDelete.OnClick += UCEvent(this, OnDeleteClick);
		m_btReturn.OnClick += UCEvent(this, OnReturnClick);
		
		// Add list selection event
		m_lbStores.OnSelectChange += UCEvent(this, OnListSelectionChanged);
	}
	~UCStoreManagement()	//Destructor
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

		// Load store data
		LoadStoreData();
	}
	ucVOID LoadStoreData()
	{
		// Get store data from server
		UCEArray<UCGameStoreData> AryStore;
		UCString Result = RObjGameHome->GetAllStoreItems(GameUserID, Token, 0, 20, AryStore);
		
		if (Result == UCString("succeed"))
		{
			m_lbStores.EmptyRow();
			for (ucINT i = 0; i < AryStore.GetSize(); i++)
			{
				UCGameStoreData& GameStoreData = AryStore[i];

				m_lbStores.InsertRow(48);

				UCStoreTool* StoreTool = (UCStoreTool*)m_lbStores.GetControl(i + 1, 0);
				StoreTool->SetData(GameStoreData);
				StoreTool->OnConfirm += UCEvent(this, StoreTool_OnConfirm);
				StoreTool->OnCancel += UCEvent(this, StoreTool_OnCancel);

				StoreTool->UpdateUIFromData(ucFALSE);
			}

			m_tbTip.Text = UCString("Store Management - Total ") + ITOS(AryStore.GetSize()) + UCString(" items");
		}
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
	ucVOID OnNewClick(UCObject*, UCEventArgs*)
	{
		// Create new store item
		UCGameStoreData NewItem;
		NewItem.ID = 0; // 0 indicates new item
		NewItem.Name = UCString("New Item");
		NewItem.Description = UCString("New item description");
		NewItem.Type = 0;
		NewItem.Price = 100;
		NewItem.ResID = 0;

		// Add to list
		m_lbStores.InsertRow(48);
		ucINT RowIndex = m_lbStores.GetRowSize() - 1;

		UCStoreTool* StoreTool = (UCStoreTool*)m_lbStores.GetControl(RowIndex, 0);
		StoreTool->SetData(NewItem);
		StoreTool->OnConfirm += UCEvent(this, StoreTool_OnConfirm);
		StoreTool->OnCancel += UCEvent(this, StoreTool_OnCancel);

		StoreTool->UpdateUIFromData(ucTRUE);

		// Show apply/cancel buttons for editing
		StoreTool->Apply.Visible = ucTRUE;
		StoreTool->Cancel.Visible = ucTRUE;

		m_tbTip.Text = UCString("New item added - please edit and save");
	}
	ucVOID OnDeleteClick(UCObject*, UCEventArgs*)
	{
		// Get selected row
		if (m_lbStores.GetSelectRowSize() == 0)
		{
			m_tbTip.Text = UCString("Please select an item to delete");
			return;
		}
		ucINT SelectedRow = m_lbStores.GetSelectRowAt(0);

		UCStoreTool* StoreTool = (UCStoreTool*)m_lbStores.GetControl(SelectedRow, 0);
		if (StoreTool != ucNULL && StoreTool->GameStoreData != ucNULL)
		{
			UCString ItemName = StoreTool->GameStoreData->Name;
			
			// Call server to delete item
			UCString Result = RObjGameHome->DeleteStoreItem(GameUserID, Token, ItemName);
			
			if (Result == UCString("succeed"))
			{
				// Remove from list
				m_lbStores.DeleteRow(SelectedRow);
				m_btDelete.Enable = ucFALSE;
				m_tbTip.Text = UCString("Item ") + ItemName + UCString(" deleted successfully");
			}
			else
			{
				m_tbTip.Text = UCString("Failed to delete item ") + ItemName + UCString(": ") + Result;
			}
		}
	}
	ucVOID OnListSelectionChanged(UCObject*, UCEventArgs*)
	{
		// Enable delete button when an item is selected
		if (m_lbStores.GetSelectRowSize() > 0)
			m_btDelete.Enable = ucTRUE;
		else
			m_btDelete.Enable = ucFALSE;
	}
	ucVOID OnReturnClick(UCObject*, UCEventArgs*)
	{
		HideUI();
	}
	ucVOID StoreTool_OnConfirm(UCObject* Object, UCEventArgs*)
	{
		UCStoreTool* StoreTool = (UCStoreTool*)Object;
		
		UCString Result = RObjGameHome->UpdateStoreItem(GameUserID, Token, *StoreTool->GameStoreData);

		if (Result == UCString("succeed"))
		{
			m_tbTip.Text = UCString("Item ") + StoreTool->GameStoreData->Name + UCString(" operation successful");

			StoreTool->Parent->AryControl[1]->Enable = ucFALSE;
		}
		else
		{
			m_tbTip.Text = UCString("Failed to operate item ") + StoreTool->GameStoreData->Name + UCString(": ") + Result;
		}
	}
	ucVOID StoreTool_OnCancel(UCObject* Object, UCEventArgs*)
	{
		UCStoreTool* StoreTool = (UCStoreTool*)Object;

		for (ucINT i = 0; i < m_lbStores.GetRowSize(); i++)
		{
			UCStoreTool* StoreToolSelect = (UCStoreTool*)m_lbStores.GetControl(i, 0);

			if (StoreTool == StoreToolSelect)
			{
				m_lbStores.DeleteRow(i);
				return;
			}
		}
	}
};

#endif //_UCStoreManagement_H_ 