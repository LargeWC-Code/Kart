/********************************************************************
修改时间：	2025/06/24 11:00:00
文 件 名：	ucshop.ui.h
编辑警告：	这是由界面编辑器自动生成的代码文件
			手动修改不保证编辑器能正确识别和恢复
*********************************************************************/
#ifndef _UCShop_UI_H_
#define _UCShop_UI_H_

class UCShop_UI : public UCPanel
{
public:
	UCButton m_btReturn;
	UCButton m_btBuy;
	UCListBox m_lbShopItems;
	UCTextBox m_tbUserMoney;
	UCTextBox m_tbItemInfo;
	UCLabel m_lbTitle;
public:
	UCShop_UI()	//构造函数
	{
		//自己的初始化
		{
			UIName = "界面编辑器类：UCShop";
			SetNewRealSize(540.000000,960.000000);
			Anchor = 15;
		}

		//UCButton m_btReturn;
		{
			m_btReturn.UIName = "UCShop::m_btReturn";
			m_btReturn.SetNewRealSize(122.000000,72.000000);
			m_btReturn.RealLocation = UCFPoint(49.000000,-40.000000);
			m_btReturn.Anchor = 9;
			m_btReturn.Font.Weight = 900;
			m_btReturn.Font.Decoration = 0x50;
			m_btReturn.Font.Size = UCSize(12,24);
			m_btReturn.Font.OutlineSize = 4;
			m_btReturn.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff6d6d6d;
			m_btReturn.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btReturn.Text = "Return";
		}

		//UCButton m_btBuy;
		{
			m_btBuy.UIName = "UCShop::m_btBuy";
			m_btBuy.SetNewRealSize(122.000000,72.000000);
			m_btBuy.RealLocation = UCFPoint(-60.000000,-40.000000);
			m_btBuy.Anchor = 10;
			m_btBuy.Font.Weight = 900;
			m_btBuy.Font.Decoration = 0x50;
			m_btBuy.Font.Size = UCSize(12,24);
			m_btBuy.Font.OutlineSize = 4;
			m_btBuy.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff6d6d6d;
			m_btBuy.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btBuy.Text = "Buy";
		}

		//UCListBox m_lbShopItems;
		{
			m_lbShopItems.UIName = "UCShop::m_lbShopItems";
			m_lbShopItems.SetNewRealSize(430.000000,460.000000);
			m_lbShopItems.RealLocation = UCFPoint(48.000000,128.000000);
			m_lbShopItems.Anchor = 15;
			//UCHScroll m_lbShopItems.HScroll;
			{
				//UCButton m_lbShopItems.HScroll.Left;
				{
					m_lbShopItems.HScroll.Left.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbShopItems.HScroll.Left.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbShopItems.HScroll.Right;
				{
					m_lbShopItems.HScroll.Right.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbShopItems.HScroll.Right.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbShopItems.HScroll.Move;
				{
					m_lbShopItems.HScroll.Move.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbShopItems.HScroll.Move.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
			}
			//UCVScroll m_lbShopItems.VScroll;
			{
				//UCButton m_lbShopItems.VScroll.Up;
				{
					m_lbShopItems.VScroll.Up.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbShopItems.VScroll.Up.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbShopItems.VScroll.Down;
				{
					m_lbShopItems.VScroll.Down.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbShopItems.VScroll.Down.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbShopItems.VScroll.Move;
				{
					m_lbShopItems.VScroll.Move.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbShopItems.VScroll.Move.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
			}
		}

		//UCTextBox m_tbUserMoney;
		{
			m_tbUserMoney.UIName = "UCShop::m_tbUserMoney";
			m_tbUserMoney.SetNewRealSize(200.000000,48.000000);
			m_tbUserMoney.RealLocation = UCFPoint(48.000000,48.000000);
			m_tbUserMoney.Anchor = 5;
			m_tbUserMoney.Font.Weight = 900;
			m_tbUserMoney.Font.Decoration = 0x50;
			m_tbUserMoney.Font.Size = UCSize(16,32);
			m_tbUserMoney.Font.OutlineSize = 4;
			m_tbUserMoney.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff00ff00;
			//m_tbUserMoney.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_tbUserMoney.Text = "Money: 1000";
		}

		//UCTextBox m_tbItemInfo;
		{
			m_tbItemInfo.UIName = "UCShop::m_tbItemInfo";
			m_tbItemInfo.SetNewRealSize(430.000000,148.000000);
			m_tbItemInfo.RealLocation = UCFPoint(48.000000,-168.000000);
			m_tbItemInfo.Anchor = 11;
			m_tbItemInfo.Font.Decoration = 0x10;
			m_tbItemInfo.Font.Size = UCSize(14,28);
			m_tbItemInfo.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Outline = 0xffb2b2b2;
			m_tbItemInfo.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_tbItemInfo.Text = "Select an item to view details";
		}

		//UCLabel m_lbTitle;
		{
			m_lbTitle.UIName = "UCShop::m_lbTitle";
			m_lbTitle.SetNewRealSize(300.000000,64.000000);
			m_lbTitle.RealLocation = UCFPoint(-48.000000,48.000000);
			m_lbTitle.Anchor = 6;
			m_lbTitle.Font.Weight = 900;
			m_lbTitle.Font.Decoration = 0x50;
			m_lbTitle.Font.Size = UCSize(24,48);
			m_lbTitle.Font.OutlineSize = 6;
			m_lbTitle.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbTitle.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Text = 0xffb6b6fe;
			m_lbTitle.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_lbTitle.Align = 0;
			m_lbTitle.Text = "Game Shop";
		}

		//父子关系结构
		{
			this->AddControl(&m_btReturn);
			this->AddControl(&m_btBuy);
			this->AddControl(&m_lbShopItems);
			{
				//m_lbShopItems.AddControl(&m_lbShopItems.HScroll);
				{
					//m_lbShopItems.HScroll.AddControl(&m_lbShopItems.HScroll.Back);
					//m_lbShopItems.HScroll.AddControl(&m_lbShopItems.HScroll.Left);
					//m_lbShopItems.HScroll.AddControl(&m_lbShopItems.HScroll.Move);
					//m_lbShopItems.HScroll.AddControl(&m_lbShopItems.HScroll.Right);
				}
				//m_lbShopItems.AddControl(&m_lbShopItems.VScroll);
				{
					//m_lbShopItems.VScroll.AddControl(&m_lbShopItems.VScroll.Back);
					//m_lbShopItems.VScroll.AddControl(&m_lbShopItems.VScroll.Down);
					//m_lbShopItems.VScroll.AddControl(&m_lbShopItems.VScroll.Move);
					//m_lbShopItems.VScroll.AddControl(&m_lbShopItems.VScroll.Up);
				}
				//m_lbShopItems.AddControl(&m_lbShopItems.View);
				{
					//m_lbShopItems.View.AddControl(&m_lbShopItems.Client);
				}
			}
			this->AddControl(&m_tbUserMoney);
			this->AddControl(&m_tbItemInfo);
			this->AddControl(&m_lbTitle);
		}
	}
	~UCShop_UI()	//析构函数
	{
	}
};

#endif //_UCShop_UI_H_ 