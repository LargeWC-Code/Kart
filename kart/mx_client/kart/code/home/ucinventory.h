/********************************************************************
生成时间：	2025/06/24 11:00:00
文 件 名：	ucinventory.h
所 有 者：	
			
*********************************************************************/
#ifndef _UCInventory_H_
#define _UCInventory_H_
#include "ucinventory.ui.h"
// Contains server-side product data structure definitions
#include "../../../../mx_server/container/private/common/game_homebase.h"

class UCInventory : public UCInventory_UI
{
public:
	UCGameUserID		GameUserID;
	ucUINT64			Token;

	UCRObjGameHome*			RObjGameHome;
	UCRObjGameUserData*		RObjGameUserData;
	UCRObjGameUserData_Ext* RObjGameUserData_Ext;

	UCEArray<UCGameStoreData>	AryStore;	// Stores all shop item info, used to look up user item details
	UCEArray<ucINT>				AryUserItems;  // List of item IDs owned by the user

	// Callback function to update avatar display
	UCEventManager				OnAvatarChanged;
public:
	UCInventory(UCRObjGameHome* RObjGameHome,
		UCRObjGameUserData* RObjGameUserData,
		UCRObjGameUserData_Ext* RObjGameUserData_Ext)	// Constructor
	{
		// Default: not visible
		Visible = ucFALSE;
		Enable = ucFALSE;

		this->RObjGameHome = RObjGameHome;
		this->RObjGameUserData = RObjGameUserData;
		this->RObjGameUserData_Ext = RObjGameUserData_Ext;

		this->GameUserID.Set(0);
		this->Token = 0;

		m_lbInventoryItems.InsertCol(UCString("Name"), -1, 200, typeof(UCLabel));
		m_lbInventoryItems.InsertCol(UCString("Description"), -1, 300, typeof(UCLabel));
		m_lbInventoryItems.InsertCol(UCString("Type"), -1, 150, typeof(UCLabel));
		m_lbInventoryItems.InsertCol(UCString("Quantity"), -1, 120, typeof(UCLabel));

		UCControl* Head = m_lbInventoryItems.GetControl(0, -1);
		Head->SetNewSize(Head->Size.cx.Value, 48);

		for (ucINT i = 0; i < m_lbInventoryItems.GetColSize(); i++)
		{
			UCLabel* Label = (UCLabel*)m_lbInventoryItems.GetControl(0, i);
			Label->Font.Size = UCSize(16, 32);
			Label->Size.cy = 48;
		}

		m_lbInventoryItems.DockView();

		m_btUse.OnClick += UCEvent(this, OnUseClick);
		m_btUse.Text = UCString("Use");  // Default button text
		
		// Add list selection event
		m_lbInventoryItems.OnSelectChange += UCEvent(this, OnListSelectionChanged);
	}
	~UCInventory()	// Destructor
	{
	}
	ucVOID UpdateUI(ucCONST UCGameUserID& GameUserID, ucUINT64 Token)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)GetRunFiberData();

		this->GameUserID = GameUserID;
		this->Token = Token;
		
		// Load inventory data
		LoadInventoryData();
	}
	ucVOID LoadInventoryData()
	{
		// First, get all store items to have reference data
		UCString Result = RObjGameHome->GetAllStoreItems(GameUserID, Token, 0, 100, AryStore);
		
		if (Result == UCString("succeed"))
		{
			// Get user's item list
			AryUserItems.RemoveAll();
			for (ucINT i = 0; i < RObjGameUserData->AryItemData.GetSize(); i++)
			{
				AryUserItems.Add(RObjGameUserData->AryItemData[i]);
			}

			// Count item quantities and create display list
			UCEArray<ucINT> ItemQuantities;
			UCEArray<ucINT> UniqueItemIDs;
			
			for (ucINT i = 0; i < AryUserItems.GetSize(); i++)
			{
				ucINT ItemID = AryUserItems[i];
				ucINT FoundIndex = -1;
				
				// Check if we already have this item
				for (ucINT j = 0; j < UniqueItemIDs.GetSize(); j++)
				{
					if (UniqueItemIDs[j] == ItemID)
					{
						FoundIndex = j;
						break;
					}
				}
				
				if (FoundIndex >= 0)
				{
					// Increment quantity
					ItemQuantities[FoundIndex]++;
				}
				else
				{
					// Add new item
					UniqueItemIDs.Add(ItemID);
					ItemQuantities.Add(1);
				}
			}

			// Store the unique items for later use
			AryUserItems.RemoveAll();
			for (ucINT i = 0; i < UniqueItemIDs.GetSize(); i++)
			{
				AryUserItems.Add(UniqueItemIDs[i]);
			}

			// Display inventory items
			m_lbInventoryItems.EmptyRow();
			for (ucINT i = 0; i < UniqueItemIDs.GetSize(); i++)
			{
				ucINT ItemID = UniqueItemIDs[i];
				ucINT Quantity = ItemQuantities[i];
				
				// Find item details from store data
				UCGameStoreData* ItemData = ucNULL;
				for (ucINT j = 0; j < AryStore.GetSize(); j++)
				{
					if (AryStore[j].ID == ItemID)
					{
						ItemData = &AryStore[j];
						break;
					}
				}

				m_lbInventoryItems.InsertRow(48);

				// Set item data
				UCLabel* NameLabel = (UCLabel*)m_lbInventoryItems.GetControl(i + 1, 0);
				if (ItemData != ucNULL)
				{
					NameLabel->Text = ItemData->Name;
				}
				else
				{
					NameLabel->Text = UCString("Unknown Item (ID:") + ITOS(ItemID) + UCString(")");
				}
				NameLabel->Font.Size = UCSize(16, 32);

				UCLabel* DescLabel = (UCLabel*)m_lbInventoryItems.GetControl(i + 1, 1);
				if (ItemData != ucNULL)
				{
					DescLabel->Text = ItemData->Description + UCString(" (ResID:") + ITOS(ItemData->ResID) + UCString(")");
				}
				else
				{
					DescLabel->Text = UCString("Item not found in store database");
				}
				DescLabel->Font.Size = UCSize(16, 32);

				UCLabel* TypeLabel = (UCLabel*)m_lbInventoryItems.GetControl(i + 1, 2);
				if (ItemData != ucNULL)
				{
					TypeLabel->Text = ITOS(ItemData->Type);
				}
				else
				{
					TypeLabel->Text = UCString("Unknown");
				}
				TypeLabel->Font.Size = UCSize(16, 32);

				UCLabel* QuantityLabel = (UCLabel*)m_lbInventoryItems.GetControl(i + 1, 3);
				QuantityLabel->Text = ITOS(Quantity);
				QuantityLabel->Font.Size = UCSize(16, 32);
			}
		}
	}
	ucVOID OnUseClick(UCObject*, UCEventArgs*)
	{
		if (m_lbInventoryItems.GetSelectRowSize() == 0)
			return;
		
		ucINT SelectedRow = m_lbInventoryItems.GetSelectRowAt(0);

		if (SelectedRow > 0 && SelectedRow <= AryUserItems.GetSize())
		{
			ucINT ItemID = AryUserItems[SelectedRow - 1];
			
			// Find item details
			UCGameStoreData* ItemData = ucNULL;
			for (ucINT i = 0; i < AryStore.GetSize(); i++)
			{
				if (AryStore[i].ID == ItemID)
				{
					ItemData = &AryStore[i];
					break;
				}
			}

			if (ItemData)
			{
				// Check if this is an avatar item and if it's currently equipped
				ucBOOL IsEquipped = ucFALSE;
				if (ItemData->Type >= ITEM_TYPE_FACE && ItemData->Type <= ITEM_TYPE_CAR)
				{
					if (ItemData->Type == ITEM_TYPE_FACE && RObjGameUserData->AvatarData.FaceID == ItemData->ResID)
						IsEquipped = ucTRUE;
					else if (ItemData->Type == ITEM_TYPE_HAIR && RObjGameUserData->AvatarData.HairID == ItemData->ResID)
						IsEquipped = ucTRUE;
					else if (ItemData->Type == ITEM_TYPE_BODY && RObjGameUserData->AvatarData.BodyID == ItemData->ResID)
						IsEquipped = ucTRUE;
					else if (ItemData->Type == ITEM_TYPE_HAND && RObjGameUserData->AvatarData.HandID == ItemData->ResID)
						IsEquipped = ucTRUE;
					else if (ItemData->Type == ITEM_TYPE_PANT && RObjGameUserData->AvatarData.PantID == ItemData->ResID)
						IsEquipped = ucTRUE;
					else if (ItemData->Type == ITEM_TYPE_SHOE && RObjGameUserData->AvatarData.ShoeID == ItemData->ResID)
						IsEquipped = ucTRUE;
					else if (ItemData->Type == ITEM_TYPE_CAR && RObjGameUserData->AvatarData.CarID == ItemData->ResID)
						IsEquipped = ucTRUE;
				}
				
				// Create a copy of current avatar data
				UCRProAvatarData NewAvatarData;
				NewAvatarData.FaceID = RObjGameUserData->AvatarData.FaceID;
				NewAvatarData.HairID = RObjGameUserData->AvatarData.HairID;
				NewAvatarData.BodyID = RObjGameUserData->AvatarData.BodyID;
				NewAvatarData.HandID = RObjGameUserData->AvatarData.HandID;
				NewAvatarData.PantID = RObjGameUserData->AvatarData.PantID;
				NewAvatarData.ShoeID = RObjGameUserData->AvatarData.ShoeID;
				NewAvatarData.CarID = RObjGameUserData->AvatarData.CarID;

				if (IsEquipped)
				{
					// Unequip the item (set to -1)
					if (ItemData->Type == ITEM_TYPE_FACE)
					{
						NewAvatarData.FaceID = -1;
					}
					else if (ItemData->Type == ITEM_TYPE_HAIR)
					{
						NewAvatarData.HairID = -1;
					}
					else if (ItemData->Type == ITEM_TYPE_BODY)
					{
						NewAvatarData.BodyID = -1;
					}
					else if (ItemData->Type == ITEM_TYPE_HAND)
					{
						NewAvatarData.HandID = -1;
					}
					else if (ItemData->Type == ITEM_TYPE_PANT)
					{
						NewAvatarData.PantID = -1;
					}
					else if (ItemData->Type == ITEM_TYPE_SHOE)
					{
						NewAvatarData.ShoeID = -1;
					}
					else if (ItemData->Type == ITEM_TYPE_CAR)
					{
						NewAvatarData.CarID = -1;
					}
				}
				else
				{
					// Equip the item
					if (ItemData->Type == ITEM_TYPE_FACE)
					{
						NewAvatarData.FaceID = ItemData->ResID;
					}
					else if (ItemData->Type == ITEM_TYPE_HAIR)
					{
						NewAvatarData.HairID = ItemData->ResID;
					}
					else if (ItemData->Type == ITEM_TYPE_BODY)
					{
						NewAvatarData.BodyID = ItemData->ResID;
					}
					else if (ItemData->Type == ITEM_TYPE_HAND)
					{
						NewAvatarData.HandID = ItemData->ResID;
					}
					else if (ItemData->Type == ITEM_TYPE_PANT)
					{
						NewAvatarData.PantID = ItemData->ResID;
					}
					else if (ItemData->Type == ITEM_TYPE_SHOE)
					{
						NewAvatarData.ShoeID = ItemData->ResID;
					}
					else if (ItemData->Type == ITEM_TYPE_CAR)
					{
						NewAvatarData.CarID = ItemData->ResID;
					}
				}
				
				// Call server interface to update avatar
				UCString Result = RObjGameHome->ChangeAvatar(GameUserID, Token, NewAvatarData);
				
				// Update local avatar data if server call succeeds
				if (Result == UCString("succeed"))
				{
					// Assign avatar data members individually
					RObjGameUserData->AvatarData.FaceID = NewAvatarData.FaceID;
					RObjGameUserData->AvatarData.HairID = NewAvatarData.HairID;
					RObjGameUserData->AvatarData.BodyID = NewAvatarData.BodyID;
					RObjGameUserData->AvatarData.HandID = NewAvatarData.HandID;
					RObjGameUserData->AvatarData.PantID = NewAvatarData.PantID;
					RObjGameUserData->AvatarData.ShoeID = NewAvatarData.ShoeID;
					RObjGameUserData->AvatarData.CarID = NewAvatarData.CarID;
					
					// Trigger avatar change callback to update display
					OnAvatarChanged(this, ucNULL);
					
					// Update button text after the change
					OnListSelectionChanged(this, ucNULL);
				}
			}
		}
	}
	ucVOID OnListSelectionChanged(UCObject*, UCEventArgs*)
	{
		m_btUse.Enable = m_lbInventoryItems.GetSelectRowSize() > 0;
		
		if (m_lbInventoryItems.GetSelectRowSize() > 0)
		{
			ucINT SelectedRow = m_lbInventoryItems.GetSelectRowAt(0);
			
			if (SelectedRow > 0 && SelectedRow <= AryUserItems.GetSize())
			{
				ucINT ItemID = AryUserItems[SelectedRow - 1];
				
				// Find item details
				UCGameStoreData* ItemData = ucNULL;
				for (ucINT i = 0; i < AryStore.GetSize(); i++)
				{
					if (AryStore[i].ID == ItemID)
					{
						ItemData = &AryStore[i];
						break;
					}
				}
				
				if (ItemData)
				{
					// Check if it's an avatar item (between ITEM_TYPE_FACE and ITEM_TYPE_CAR)
					if (ItemData->Type >= ITEM_TYPE_FACE && ItemData->Type <= ITEM_TYPE_CAR)
					{
						// Check if this item is currently equipped
						ucBOOL IsEquipped = ucFALSE;
						
						if (ItemData->Type == ITEM_TYPE_FACE && RObjGameUserData->AvatarData.FaceID == ItemData->ResID)
							IsEquipped = ucTRUE;
						else if (ItemData->Type == ITEM_TYPE_HAIR && RObjGameUserData->AvatarData.HairID == ItemData->ResID)
							IsEquipped = ucTRUE;
						else if (ItemData->Type == ITEM_TYPE_BODY && RObjGameUserData->AvatarData.BodyID == ItemData->ResID)
							IsEquipped = ucTRUE;
						else if (ItemData->Type == ITEM_TYPE_HAND && RObjGameUserData->AvatarData.HandID == ItemData->ResID)
							IsEquipped = ucTRUE;
						else if (ItemData->Type == ITEM_TYPE_PANT && RObjGameUserData->AvatarData.PantID == ItemData->ResID)
							IsEquipped = ucTRUE;
						else if (ItemData->Type == ITEM_TYPE_SHOE && RObjGameUserData->AvatarData.ShoeID == ItemData->ResID)
							IsEquipped = ucTRUE;
						else if (ItemData->Type == ITEM_TYPE_CAR && RObjGameUserData->AvatarData.CarID == ItemData->ResID)
							IsEquipped = ucTRUE;
						
						if (IsEquipped)
						{
							m_btUse.Text = UCString("Unequip");
						}
						else
						{
							m_btUse.Text = UCString("Equip");
						}
					}
					else
					{
						// Non-avatar items
						m_btUse.Text = UCString("Use");
					}
				}
				else
				{
					m_btUse.Text = UCString("Use");
				}
			}
		}
	}
};

#endif //_UCInventory_H_ 