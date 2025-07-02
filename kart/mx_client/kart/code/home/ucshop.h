/********************************************************************
生成时间：	2025/06/24 11:00:00
文 件 名：	ucshop.h
所 有 者：	
			
*********************************************************************/
#ifndef _UCShop_H_
#define _UCShop_H_
#include "ucshop.ui.h"
// 包含服务器端的商品数据结构定义
#include "../../../../mx_server/container/private/common/game_homebase.h"

class UCShop : public UCShop_UI
{
public:
	UCGameUserID		GameUserID;
	ucUINT64			Token;

	UCRObjGameHome*		RObjGameHome;
	UCRObjGameUserData* RObjGameUserData;
	UCRObjGameUserData_Ext* RObjGameUserData_Ext;

	UCEArray<UCGameStoreData>	AryStore;
public:
	UCShop(UCRObjGameHome* RObjGameHome,
		UCRObjGameUserData* RObjGameUserData,
		UCRObjGameUserData_Ext* RObjGameUserData_Ext)	//构造函数
	{
		//默认不可见
		Visible = ucFALSE;
		Enable = ucFALSE;

		this->RObjGameHome = RObjGameHome;
		this->RObjGameUserData = RObjGameUserData;
		this->RObjGameUserData_Ext = RObjGameUserData_Ext;

		this->GameUserID.Set(0);
		this->Token = 0;

		m_lbShopItems.InsertCol(UCString("Name"), -1, 200, typeof(UCLabel));
		m_lbShopItems.InsertCol(UCString("Description"), -1, 300, typeof(UCLabel));
		m_lbShopItems.InsertCol(UCString("Type"), -1, 150, typeof(UCLabel));
		m_lbShopItems.InsertCol(UCString("Price"), -1, 120, typeof(UCLabel));

		UCControl* Head = m_lbShopItems.GetControl(0, -1);
		Head->SetNewSize(Head->Size.cx.Value, 48);

		for (ucINT i = 0; i < m_lbShopItems.GetColSize(); i++)
		{
			UCLabel* Label = (UCLabel*)m_lbShopItems.GetControl(0, i);
			Label->Font.Size = UCSize(16, 32);
			Label->Size.cy = 48;
		}

		m_lbShopItems.DockView();

		m_btReturn.OnClick += UCEvent(this, OnReturnClick);
		m_btBuy.OnClick += UCEvent(this, OnBuyClick);
		
		// Add list selection event
		m_lbShopItems.OnSelectChange += UCEvent(this, OnListSelectionChanged);
	}
	~UCShop()	//析构函数
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

		// 更新用户金钱显示
		m_tbUserMoney.Text = UCString("Money: ") + ITOS(RObjGameUserData->VMoney.Value) + UCString("$");
		
		// Load shop data from server
		LoadShopData();
	}
	ucVOID LoadShopData()
	{
		// Get store data from server
		UCString Result = RObjGameHome->GetAllStoreItems(GameUserID, Token, 0, 20, AryStore);
		
		if (Result == UCString("succeed"))
		{
			m_lbShopItems.EmptyRow();
			for (ucINT i = 0; i < AryStore.GetSize(); i++)
			{
				UCGameStoreData& GameStoreData = AryStore[i];

				m_lbShopItems.InsertRow(48);

				// Set item data
				UCLabel* NameLabel = (UCLabel*)m_lbShopItems.GetControl(i + 1, 0);
				NameLabel->Text = GameStoreData.Name;
				NameLabel->Font.Size = UCSize(16, 32);

				UCLabel* DescLabel = (UCLabel*)m_lbShopItems.GetControl(i + 1, 1);
				DescLabel->Text = GameStoreData.Description;
				DescLabel->Font.Size = UCSize(16, 32);

				UCLabel* TypeLabel = (UCLabel*)m_lbShopItems.GetControl(i + 1, 2);
				TypeLabel->Text = ITOS(GameStoreData.Type);
				TypeLabel->Font.Size = UCSize(16, 32);

				UCLabel* PriceLabel = (UCLabel*)m_lbShopItems.GetControl(i + 1, 3);
				PriceLabel->Text = ITOS(GameStoreData.Price) + UCString("$");
				PriceLabel->Font.Size = UCSize(16, 32);

				// Add ResID information to description or create a new column
				DescLabel->Text = DescLabel->Text.Value + UCString(" (ResID:") + ITOS(GameStoreData.ResID) + UCString(")");
			}

			m_tbItemInfo.Text = UCString("Shop loaded - Total ") + ITOS(AryStore.GetSize()) + UCString(" items available");
		}
		else
		{
			m_tbItemInfo.Text = UCString("Failed to load shop data: ") + Result;
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
	ucVOID OnReturnClick(UCObject*, UCEventArgs*)
	{
		HideUI();
	}
	ucVOID OnBuyClick(UCObject*, UCEventArgs*)
	{
		if (m_lbShopItems.GetSelectRowSize() == 0)
		{
			m_tbItemInfo.Text = UCString("Please select an item to buy");
			return;
		}
		
		ucINT SelectedRow = m_lbShopItems.GetSelectRowAt(0);
		if (SelectedRow > 0 && SelectedRow <= AryStore.GetSize())
		{
			UCGameStoreData& GameStoreData = AryStore[SelectedRow - 1];
			ucINT ItemID = GameStoreData.ID;

			if (RObjGameUserData->VMoney.Value < GameStoreData.Price)
			{
				m_tbItemInfo.Text = UCString("Not enough money! You need ") + ITOS(GameStoreData.Price - RObjGameUserData->VMoney.Value) + UCString(" more.");
				return;
			}
			
			UCString Result = RObjGameHome->BuyStoreItem(GameUserID, Token, ItemID);
			
			if (Result == UCString("succeed"))
			{
				m_tbUserMoney.Text = UCString("Money: ") + ITOS(RObjGameUserData->VMoney.Value) + UCString("$");
				
				m_tbItemInfo.Text = UCString("Successfully purchased: ") + GameStoreData.Name + UCString("!");

				OnListSelectionChanged(ucNULL, ucNULL);
			}
			else if (Result == UCString("not enough money"))
				m_tbItemInfo.Text = UCString("Not enough money! You need ") + ITOS(GameStoreData.Price - RObjGameUserData->VMoney.Value) + UCString(" more.");
			else if (Result == UCString("item not found"))
				m_tbItemInfo.Text = UCString("Item not found in store.");
			else
				m_tbItemInfo.Text = UCString("Purchase failed: ") + Result;
		}
	}
	ucVOID OnListSelectionChanged(UCObject*, UCEventArgs*)
	{
		// Enable buy button when an item is selected
		if (m_lbShopItems.GetSelectRowSize() > 0)
		{
			ucINT SelectedRow = m_lbShopItems.GetSelectRowAt(0);
			if (SelectedRow > 0 && SelectedRow <= AryStore.GetSize())
			{
				UCGameStoreData& GameStoreData = AryStore[SelectedRow - 1];

				m_btBuy.Enable = RObjGameUserData->VMoney.Value >= GameStoreData.Price;
			}
		}
		else
			m_btBuy.Enable = ucFALSE;
	}
};

#endif //_UCShop_H_ 