/********************************************************************
修改时间：	2025/06/23 18:39:22
文 件 名：	ucprofile.ui.h
编辑警告：	这是由界面编辑器自动生成的代码文件
			手动修改不保证编辑器能正确识别和恢复
*********************************************************************/
#ifndef _UCProfile_UI_H_
#define _UCProfile_UI_H_

class UCProfile_UI : public UCPanel
{
public:
	UCButton m_btConfirm;
	UCButton m_btReturn;
	UCLabel m_lbEmail;
	UCLabel m_lbNickname;
	UCLabel m_lbPassword;
	UCLabel m_lbPassword1;
	UCLabel m_lbUsername;
	UCTextBox m_tbEmail;
	UCTextBox m_tbEmailTip;
	UCTextBox m_tbNickname;
	UCTextBox m_tbPassword;
	UCTextBox m_tbPassword1;
	UCTextBox m_tbPasswordTip;
	UCTextBox m_tbUsername;
public:
	UCProfile_UI()	//构造函数
	{
		//自己的初始化
		{
			UIName = "界面编辑器类：UCProfile";
			SetNewRealSize(540.000000,960.000000);
			Anchor = 15;
		}

		//UCButton m_btConfirm;
		{
			m_btConfirm.UIName = "UCProfile::m_btConfirm";
			m_btConfirm.SetNewRealSize(122.000000,72.000000);
			m_btConfirm.RealLocation = UCFPoint(-62.000000,-40.000000);
			m_btConfirm.Anchor = 10;
			m_btConfirm.Font.Weight = 900;
			m_btConfirm.Font.Decoration = 0x50;
			m_btConfirm.Font.Size = UCSize(12,24);
			m_btConfirm.Font.OutlineSize = 4;
			m_btConfirm.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff6d6d6d;
			m_btConfirm.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btConfirm.Text = "Confirm";
		}

		//UCButton m_btReturn;
		{
			m_btReturn.UIName = "UCProfile::m_btReturn";
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

		//UCLabel m_lbEmail;
		{
			m_lbEmail.UIName = "UCProfile::m_lbEmail";
			m_lbEmail.SetNewRealSize(142.000000,70.000000);
			m_lbEmail.RealLocation = UCFPoint(49.000000,570.000000);
			m_lbEmail.Font.Decoration = 0x10;
			m_lbEmail.Font.Size = UCSize(12,24);
			m_lbEmail.Font.OutlineSize = 4;
			m_lbEmail.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbEmail.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbEmail.Text = "Email:";
		}

		//UCLabel m_lbNickname;
		{
			m_lbNickname.UIName = "UCProfile::m_lbNickname";
			m_lbNickname.SetNewRealSize(142.000000,70.000000);
			m_lbNickname.RealLocation = UCFPoint(49.000000,480.000000);
			m_lbNickname.Font.Decoration = 0x10;
			m_lbNickname.Font.Size = UCSize(12,24);
			m_lbNickname.Font.OutlineSize = 4;
			m_lbNickname.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbNickname.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbNickname.Text = "Nickname:";
		}

		//UCLabel m_lbPassword;
		{
			m_lbPassword.UIName = "UCProfile::m_lbPassword";
			m_lbPassword.SetNewRealSize(142.000000,70.000000);
			m_lbPassword.RealLocation = UCFPoint(49.000000,120.000000);
			m_lbPassword.Font.Decoration = 0x10;
			m_lbPassword.Font.Size = UCSize(12,24);
			m_lbPassword.Font.OutlineSize = 4;
			m_lbPassword.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Text = 0xffffffff;
			m_lbPassword.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Outline = 0xffc0c0c0;
			m_lbPassword.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xffc0c0c0;
			m_lbPassword.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbPassword.Text = "Password:";
		}

		//UCLabel m_lbPassword1;
		{
			m_lbPassword1.UIName = "UCProfile::m_lbPassword1";
			m_lbPassword1.SetNewRealSize(142.000000,70.000000);
			m_lbPassword1.RealLocation = UCFPoint(49.000000,210.000000);
			m_lbPassword1.Font.Decoration = 0x10;
			m_lbPassword1.Font.Size = UCSize(12,24);
			m_lbPassword1.Font.OutlineSize = 4;
			m_lbPassword1.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbPassword1.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbPassword1.Text = "Comfirm:";
		}

		//UCLabel m_lbUsername;
		{
			m_lbUsername.UIName = "UCProfile::m_lbUsername";
			m_lbUsername.SetNewRealSize(142.000000,70.000000);
			m_lbUsername.RealLocation = UCFPoint(49.000000,30.000000);
			m_lbUsername.Font.Decoration = 0x10;
			m_lbUsername.Font.Size = UCSize(12,24);
			m_lbUsername.Font.OutlineSize = 4;
			m_lbUsername.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbUsername.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbUsername.Text = "Username:";
		}

		//UCTextBox m_tbEmail;
		{
			m_tbEmail.UIName = "UCProfile::m_tbEmail";
			m_tbEmail.SetNewRealSize(276.000000,70.000000);
			m_tbEmail.RealLocation = UCFPoint(204.000000,570.000000);
			m_tbEmail.Anchor = 7;
			m_tbEmail.Font.Decoration = 0x60;
			m_tbEmail.Font.Size = UCSize(19,38);
			m_tbEmail.Font.OutlineSize = 4;
			m_tbEmail.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
		}

		//UCTextBox m_tbEmailTip;
		{
			m_tbEmailTip.UIName = "UCProfile::m_tbEmailTip";
			m_tbEmailTip.SetNewRealSize(430.000000,140.000000);
			m_tbEmailTip.RealLocation = UCFPoint(49.000000,660.000000);
			m_tbEmailTip.Anchor = 15;
			m_tbEmailTip.ReadOnly = 1;
			m_tbEmailTip.MultiColumn = 1;
			m_tbEmailTip.Font.Weight = 900;
			m_tbEmailTip.Font.Decoration = 0x50;
			m_tbEmailTip.Font.Size = UCSize(19,38);
			m_tbEmailTip.Font.OutlineSize = 4;
			m_tbEmailTip.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_tbEmailTip.Align = 1;
		}

		//UCTextBox m_tbNickname;
		{
			m_tbNickname.UIName = "UCProfile::m_tbNickname";
			m_tbNickname.SetNewRealSize(276.000000,70.000000);
			m_tbNickname.RealLocation = UCFPoint(204.000000,480.000000);
			m_tbNickname.Anchor = 7;
			m_tbNickname.Font.Decoration = 0x60;
			m_tbNickname.Font.Size = UCSize(19,38);
			m_tbNickname.Font.OutlineSize = 4;
			m_tbNickname.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
		}

		//UCTextBox m_tbPassword;
		{
			m_tbPassword.UIName = "UCProfile::m_tbPassword";
			m_tbPassword.SetNewRealSize(276.000000,70.000000);
			m_tbPassword.RealLocation = UCFPoint(204.000000,120.000000);
			m_tbPassword.Anchor = 7;
			m_tbPassword.CharKey = '*';
			m_tbPassword.Font.Decoration = 0x60;
			m_tbPassword.Font.Size = UCSize(19,38);
			m_tbPassword.Font.OutlineSize = 4;
			m_tbPassword.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_tbPassword.Align = 1;
		}

		//UCTextBox m_tbPassword1;
		{
			m_tbPassword1.UIName = "UCProfile::m_tbPassword1";
			m_tbPassword1.SetNewRealSize(276.000000,70.000000);
			m_tbPassword1.RealLocation = UCFPoint(204.000000,210.000000);
			m_tbPassword1.Anchor = 7;
			m_tbPassword1.CharKey = '*';
			m_tbPassword1.Font.Decoration = 0x60;
			m_tbPassword1.Font.Size = UCSize(19,38);
			m_tbPassword1.Font.OutlineSize = 4;
			m_tbPassword1.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_tbPassword1.Align = 1;
		}

		//UCTextBox m_tbPasswordTip;
		{
			m_tbPasswordTip.UIName = "UCProfile::m_tbPasswordTip";
			m_tbPasswordTip.SetNewRealSize(430.000000,140.000000);
			m_tbPasswordTip.RealLocation = UCFPoint(49.666687,300.000000);
			m_tbPasswordTip.Anchor = 7;
			m_tbPasswordTip.ReadOnly = 1;
			m_tbPasswordTip.MultiColumn = 1;
			m_tbPasswordTip.Font.Weight = 900;
			m_tbPasswordTip.Font.Decoration = 0x50;
			m_tbPasswordTip.Font.Size = UCSize(16,32);
			m_tbPasswordTip.Font.OutlineSize = 4;
			m_tbPasswordTip.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_tbPasswordTip.Text = "If you do not wish to change your password, please leave it blank";
		}

		//UCTextBox m_tbUsername;
		{
			m_tbUsername.UIName = "UCProfile::m_tbUsername";
			m_tbUsername.SetNewRealSize(276.000000,70.000000);
			m_tbUsername.RealLocation = UCFPoint(204.000000,30.000000);
			m_tbUsername.Anchor = 7;
			m_tbUsername.ReadOnly = 1;
			m_tbUsername.Font.Weight = 900;
			m_tbUsername.Font.Decoration = 0x50;
			m_tbUsername.Font.Size = UCSize(19,38);
			m_tbUsername.Font.OutlineSize = 4;
			m_tbUsername.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_tbUsername.Align = 1;
		}

		//父子关系结构
		{
			this->AddControl(&m_btConfirm);
			this->AddControl(&m_btReturn);
			this->AddControl(&m_lbEmail);
			this->AddControl(&m_lbNickname);
			this->AddControl(&m_lbPassword);
			this->AddControl(&m_lbPassword1);
			this->AddControl(&m_lbUsername);
			this->AddControl(&m_tbEmail);
			this->AddControl(&m_tbEmailTip);
			this->AddControl(&m_tbNickname);
			this->AddControl(&m_tbPassword);
			this->AddControl(&m_tbPassword1);
			this->AddControl(&m_tbPasswordTip);
			this->AddControl(&m_tbUsername);
		}
	}
	~UCProfile_UI()	//析构函数
	{
	}
};

#endif //_UCProfile_UI_H_