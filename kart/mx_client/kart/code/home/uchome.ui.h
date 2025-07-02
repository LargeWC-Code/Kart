/********************************************************************
修改时间：	2025/06/23 18:41:58
文 件 名：	uchome.ui.h
编辑警告：	这是由界面编辑器自动生成的代码文件
			手动修改不保证编辑器能正确识别和恢复
*********************************************************************/
#ifndef _UCHome_UI_H_
#define _UCHome_UI_H_

class UCHome_UI : public UCSceneCtl
{
public:
	UCButton m_btMatch;
	UCButton m_btProfile;
	UCButton m_btShop;
	UCButton m_btInventory;
	UCButton m_btStoreManagement;
	UCButton m_btTrain;
	UCButton m_btUserManagement;
	UCLabel m_lbNickname;
	UCLabel m_lbVMoney;
public:
	UCHome_UI()	//构造函数
	{
		//自己的初始化
		{
			UIName = "界面编辑器类：UCHome";
			SetNewRealSize(540.000000,960.000000);
			Anchor = 15;
		}

		//UCButton m_btMatch;
		{
			m_btMatch.UIName = "UCHome::m_btMatch";
			m_btMatch.SetNewRealSize(200.000000,96.000000);
			m_btMatch.RealLocation = UCFPoint(-170.000000,-60.000000);
			m_btMatch.Anchor = 10;
			m_btMatch.Font.Decoration = 0x50;
			m_btMatch.Font.Size = UCSize(16,32);
			m_btMatch.Font.OutlineSize = 8;
			m_btMatch.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff484848;
			m_btMatch.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btMatch.Text = "Multiplayer";
		}

		//UCButton m_btProfile;
		{
			m_btProfile.UIName = "UCHome::m_btProfile";
			m_btProfile.SetNewRealSize(220.000000,48.000000);
			m_btProfile.RealLocation = UCFPoint(-24.000000,24.000000);
			m_btProfile.Anchor = 6;
			m_btProfile.Font.Weight = 900;
			m_btProfile.Font.Decoration = 0x50;
			m_btProfile.Font.Size = UCSize(16,32);
			m_btProfile.Font.OutlineSize = 8;
			m_btProfile.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff484848;
			m_btProfile.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btProfile.Text = "Edit Profile";
		}

		//UCButton m_btShop;
		{
			m_btShop.UIName = "UCHome::m_btShop";
			m_btShop.SetNewRealSize(120.000000,96.000000);
			m_btShop.RealLocation = UCFPoint(-24.000000,-60.000000);
			m_btShop.Anchor = 10;
			m_btShop.Font.Weight = 900;
			m_btShop.Font.Decoration = 0x50;
			m_btShop.Font.Size = UCSize(16,32);
			m_btShop.Font.OutlineSize = 8;
			m_btShop.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff484848;
			m_btShop.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btShop.Text = "Shop";
		}

		//UCButton m_btInventory;
		{
			m_btInventory.UIName = "UCHome::m_btInventory";
			m_btInventory.SetNewRealSize(120.000000,96.000000);
			m_btInventory.RealLocation = UCFPoint(24.000000, 440.000000);
			m_btInventory.Anchor = 1;
			m_btInventory.Font.Weight = 900;
			m_btInventory.Font.Decoration = 0x50;
			m_btInventory.Font.Size = UCSize(16,32);
			m_btInventory.Font.OutlineSize = 8;
			m_btInventory.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff484848;
			m_btInventory.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btInventory.Text = "Inventory";
		}

		//UCButton m_btStoreManagement;
		{
			m_btStoreManagement.UIName = "UCHome::m_btStoreManagement";
			m_btStoreManagement.Visible = 0;
			m_btStoreManagement.SetNewRealSize(396.000000,120.000000);
			m_btStoreManagement.RealLocation = UCFPoint(78.000000,354.000000);
			m_btStoreManagement.Anchor = 7;
			m_btStoreManagement.Font.Weight = 900;
			m_btStoreManagement.Font.Decoration = 0x50;
			m_btStoreManagement.Font.Size = UCSize(16,32);
			m_btStoreManagement.Font.OutlineSize = 8;
			m_btStoreManagement.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff484848;
			m_btStoreManagement.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btStoreManagement.Text = "Store Management";
		}

		//UCButton m_btTrain;
		{
			m_btTrain.UIName = "UCHome::m_btTrain";
			m_btTrain.SetNewRealSize(120.000000,96.000000);
			m_btTrain.RealLocation = UCFPoint(72.000000,-60.000000);
			m_btTrain.Anchor = 9;
			m_btTrain.Font.Decoration = 0x50;
			m_btTrain.Font.Size = UCSize(16,32);
			m_btTrain.Font.OutlineSize = 8;
			m_btTrain.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff484848;
			m_btTrain.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btTrain.Text = "Single Player";
		}

		//UCButton m_btUserManagement;
		{
			m_btUserManagement.UIName = "UCHome::m_btUserManagement";
			m_btUserManagement.Visible = 0;
			m_btUserManagement.SetNewRealSize(396.000000,120.000000);
			m_btUserManagement.RealLocation = UCFPoint(78.666687,158.000000);
			m_btUserManagement.Anchor = 7;
			m_btUserManagement.Font.Weight = 900;
			m_btUserManagement.Font.Decoration = 0x50;
			m_btUserManagement.Font.Size = UCSize(16,32);
			m_btUserManagement.Font.OutlineSize = 8;
			m_btUserManagement.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff484848;
			m_btUserManagement.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btUserManagement.Text = "User Management";
		}

		//UCLabel m_lbNickname;
		{
			m_lbNickname.UIName = "UCHome::m_lbNickname";
			m_lbNickname.SetNewRealSize(128.000000,48.000000);
			m_lbNickname.RealLocation = UCFPoint(24.000000,24.000000);
			m_lbNickname.Font.Decoration = 0x50;
			m_lbNickname.Font.Size = UCSize(16,32);
			m_lbNickname.Font.OutlineSize = 6;
			m_lbNickname.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbNickname.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Text = 0xffb6b6fe;
			m_lbNickname.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_lbNickname.Align = 0;
		}

		//UCLabel m_lbVMoney;
		{
			m_lbVMoney.UIName = "UCHome::m_lbVMoney";
			m_lbVMoney.SetNewRealSize(128.000000,48.000000);
			m_lbVMoney.RealLocation = UCFPoint(160.000000,24.000000);
			m_lbVMoney.Font.Weight = 900;
			m_lbVMoney.Font.Decoration = 0x50;
			m_lbVMoney.Font.Size = UCSize(16,32);
			m_lbVMoney.Font.OutlineSize = 6;
			m_lbVMoney.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbVMoney.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Text = 0xffffff00;
			m_lbVMoney.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_lbVMoney.Align = 2;
		}

		//父子关系结构
		{
			this->AddControl(&m_btMatch);
			this->AddControl(&m_btProfile);
			this->AddControl(&m_btShop);
			this->AddControl(&m_btInventory);
			this->AddControl(&m_btStoreManagement);
			this->AddControl(&m_btTrain);
			this->AddControl(&m_btUserManagement);
			this->AddControl(&m_lbNickname);
			this->AddControl(&m_lbVMoney);
		}
	}
	~UCHome_UI()	//析构函数
	{
	}
};

#endif //_UCHome_UI_H_