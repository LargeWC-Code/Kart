/********************************************************************
修改时间：	2025/06/19 16:43:30
文 件 名：	ucregister.ui.h
编辑警告：	这是由界面编辑器自动生成的代码文件
			手动修改不保证编辑器能正确识别和恢复
*********************************************************************/
#ifndef _UCRegister_UI_H_
#define _UCRegister_UI_H_

class UCRegister_UI : public UCImage
{
public:
	UCImage m_imgTip;
	UCLabel m_lbEmail;
	UCLabel m_lbPassword;
	UCLabel m_lbPassword1;
	UCLabel m_lbTip;
	UCLabel m_lbUsername;
	UCTextBox m_tbEmail;
	UCTextBox m_tbPassword;
	UCTextBox m_tbPassword1;
	UCTextBox m_tbUsername;
public:
	UCRegister_UI()	//构造函数
	{
		//自己的初始化
		{
			UIName = "界面编辑器类：UCRegister";
			SetNewRealSize(540.000000,960.000000);
			Anchor = 15;
		}

		//UCImage m_imgTip;
		{
			m_imgTip.UIName = "UCRegister::m_imgTip";
			m_imgTip.Enable = 0;
			m_imgTip.SetNewRealSize(540.000000,30.000000);
			m_imgTip.RealLocation = UCFPoint(0.000000,-47.000000);
			m_imgTip.Anchor = 11;
			m_imgTip.Picture.Name = "res/ui/common/title_tip.tga";
			m_imgTip.Picture.Style = 4;
		}

		//UCLabel m_lbEmail;
		{
			m_lbEmail.UIName = "UCRegister::m_lbEmail";
			m_lbEmail.SetNewRealSize(142.000000,70.000000);
			m_lbEmail.RealLocation = UCFPoint(49.000000,456.000000);
			m_lbEmail.Anchor = 0;
			m_lbEmail.Font.Decoration = 0x10;
			m_lbEmail.Font.Size = UCSize(12,24);
			m_lbEmail.Font.OutlineSize = 4;
			m_lbEmail.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbEmail.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbEmail.Text = "Email:";
		}

		//UCLabel m_lbPassword;
		{
			m_lbPassword.UIName = "UCRegister::m_lbPassword";
			m_lbPassword.SetNewRealSize(142.000000,70.000000);
			m_lbPassword.RealLocation = UCFPoint(49.000000,594.000000);
			m_lbPassword.Anchor = 0;
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
			m_lbPassword1.UIName = "UCRegister::m_lbPassword1";
			m_lbPassword1.SetNewRealSize(142.000000,70.000000);
			m_lbPassword1.RealLocation = UCFPoint(49.000000,735.000000);
			m_lbPassword1.Anchor = 0;
			m_lbPassword1.Font.Decoration = 0x10;
			m_lbPassword1.Font.Size = UCSize(12,24);
			m_lbPassword1.Font.OutlineSize = 4;
			m_lbPassword1.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbPassword1.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbPassword1.Text = "Comfirm:";
		}

		//UCLabel m_lbTip;
		{
			m_lbTip.UIName = "UCRegister::m_lbTip";
			m_lbTip.SetNewRealSize(228.000000,25.000000);
			m_lbTip.RealLocation = UCFPoint(150.000000,4.000000);
			m_lbTip.Anchor = 0;
			m_lbTip.Font.Decoration = 0x10;
			m_lbTip.Font.Size = UCSize(8,16);
			m_lbTip.Font.OutlineSize = 4;
			m_lbTip.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Text = 0xffffffff;
			m_lbTip.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Outline = 0xffc0c0c0;
			m_lbTip.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xffc0c0c0;
			m_lbTip.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbTip.Align = 0;
			m_lbTip.Text = "Click any place to continue";
		}

		//UCLabel m_lbUsername;
		{
			m_lbUsername.UIName = "UCRegister::m_lbUsername";
			m_lbUsername.SetNewRealSize(142.000000,70.000000);
			m_lbUsername.RealLocation = UCFPoint(49.000000,323.000000);
			m_lbUsername.Anchor = 0;
			m_lbUsername.Font.Decoration = 0x10;
			m_lbUsername.Font.Size = UCSize(12,24);
			m_lbUsername.Font.OutlineSize = 4;
			m_lbUsername.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbUsername.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbUsername.Text = "Username:";
		}

		//UCTextBox m_tbEmail;
		{
			m_tbEmail.UIName = "UCRegister::m_tbEmail";
			m_tbEmail.SetNewRealSize(276.000000,70.000000);
			m_tbEmail.RealLocation = UCFPoint(204.000000,456.000000);
			m_tbEmail.Anchor = 0;
			m_tbEmail.Font.Weight = 900;
			m_tbEmail.Font.Decoration = 0x50;
			m_tbEmail.Font.Size = UCSize(19,38);
			m_tbEmail.Font.OutlineSize = 4;
			m_tbEmail.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
		}

		//UCTextBox m_tbPassword;
		{
			m_tbPassword.UIName = "UCRegister::m_tbPassword";
			m_tbPassword.SetNewRealSize(276.000000,70.000000);
			m_tbPassword.RealLocation = UCFPoint(204.000000,594.000000);
			m_tbPassword.Anchor = 0;
			m_tbPassword.CharKey = '*';
			m_tbPassword.Font.Decoration = 0x60;
			m_tbPassword.Font.Size = UCSize(19,38);
			m_tbPassword.Font.OutlineSize = 4;
			m_tbPassword.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_tbPassword.Align = 1;
		}

		//UCTextBox m_tbPassword1;
		{
			m_tbPassword1.UIName = "UCRegister::m_tbPassword1";
			m_tbPassword1.SetNewRealSize(276.000000,70.000000);
			m_tbPassword1.RealLocation = UCFPoint(204.000000,735.000000);
			m_tbPassword1.Anchor = 0;
			m_tbPassword1.CharKey = '*';
			m_tbPassword1.Font.Decoration = 0x60;
			m_tbPassword1.Font.Size = UCSize(19,38);
			m_tbPassword1.Font.OutlineSize = 4;
			m_tbPassword1.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_tbPassword1.Align = 1;
		}

		//UCTextBox m_tbUsername;
		{
			m_tbUsername.UIName = "UCRegister::m_tbUsername";
			m_tbUsername.SetNewRealSize(276.000000,70.000000);
			m_tbUsername.RealLocation = UCFPoint(204.000000,323.000000);
			m_tbUsername.Anchor = 0;
			m_tbUsername.Font.Weight = 900;
			m_tbUsername.Font.Decoration = 0x50;
			m_tbUsername.Font.Size = UCSize(19,38);
			m_tbUsername.Font.OutlineSize = 4;
			m_tbUsername.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
		}

		//父子关系结构
		{
			this->AddControl(&m_imgTip);
			{
				m_imgTip.AddControl(&m_lbTip);
			}
			this->AddControl(&m_lbEmail);
			this->AddControl(&m_lbPassword);
			this->AddControl(&m_lbPassword1);
			this->AddControl(&m_lbUsername);
			this->AddControl(&m_tbEmail);
			this->AddControl(&m_tbPassword);
			this->AddControl(&m_tbPassword1);
			this->AddControl(&m_tbUsername);
		}
	}
	~UCRegister_UI()	//析构函数
	{
	}
};

#endif //_UCRegister_UI_H_