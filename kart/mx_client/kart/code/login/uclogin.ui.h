/********************************************************************
修改时间：	2025/06/23 09:43:16
文 件 名：	uclogin.ui.h
编辑警告：	这是由界面编辑器自动生成的代码文件
			手动修改不保证编辑器能正确识别和恢复
*********************************************************************/
#ifndef _UCLogin_UI_H_
#define _UCLogin_UI_H_

class UCLogin_UI : public UCImage
{
public:
	UCButton m_btRegister;
	UCImage m_imgLogo;
	UCImage m_imgTip;
	UCLabel m_lbKart;
	UCLabel m_lbMini;
	UCLabel m_lbPassword;
	UCLabel m_lbTip;
	UCLabel m_lbUsername;
	UCTextBox m_tbPassword;
	UCLabel m_tbTip;
	UCTextBox m_tbUsername;
public:
	UCLogin_UI()	//构造函数
	{
		//自己的初始化
		{
			UIName = "界面编辑器类：UCLogin";
			SetNewRealSize(540.000000,960.000000);
			Anchor = 15;
			Picture.Style = 4;
		}

		//UCButton m_btRegister;
		{
			m_btRegister.UIName = "UCLogin::m_btRegister";
			m_btRegister.Visible = 0;
			m_btRegister.SetNewRealSize(162.000000,66.000000);
			m_btRegister.RealLocation = UCFPoint(185.000000,791.000000);
			m_btRegister.Anchor = 0;
			m_btRegister.Font.Weight = 900;
			m_btRegister.Font.Decoration = 0x50;
			m_btRegister.Font.Size = UCSize(16,32);
			m_btRegister.Font.OutlineSize = 6;
			m_btRegister.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Text = 0xff3a4100;
			m_btRegister.FontColor.TextColor[UCDISPLAYMODE_DARK].Text = 0xff3a4100;
			m_btRegister.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xffffffff;
			m_btRegister.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btRegister.Text = "Register";
		}

		//UCImage m_imgLogo;
		{
			m_imgLogo.UIName = "UCLogin::m_imgLogo";
			m_imgLogo.Enable = 0;
			m_imgLogo.SetNewRealSize(279.000000,355.000000);
			m_imgLogo.RealLocation = UCFPoint(240.000000,31.000000);
			m_imgLogo.Anchor = 0;
			m_imgLogo.Picture.Name = "../../../../../../MIT/assignment/kart/runtime/client/res/ui/movie/head.png";
			m_imgLogo.Picture.Style = 2;
		}

		//UCImage m_imgTip;
		{
			m_imgTip.UIName = "UCLogin::m_imgTip";
			m_imgTip.Enable = 0;
			m_imgTip.SetNewRealSize(540.000000,40.000000);
			m_imgTip.RealLocation = UCFPoint(0.000000,-47.000000);
			m_imgTip.Anchor = 11;
			m_imgTip.Picture.Name = "res/ui/common/title_tip.tga";
			m_imgTip.Picture.Style = 4;
		}

		//UCLabel m_lbKart;
		{
			m_lbKart.UIName = "UCLogin::m_lbKart";
			m_lbKart.SetNewRealSize(540.000000,235.000000);
			m_lbKart.RealLocation = UCFPoint(0.000000,350.000000);
			m_lbKart.Anchor = 0;
			m_lbKart.Font.Weight = 900;
			m_lbKart.Font.Decoration = 0x50;
			m_lbKart.Font.Size = UCSize(64,128);
			m_lbKart.Font.OutlineSize = 8;
			m_lbKart.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Text = 0xff010101;
			m_lbKart.FontColor.TextColor[UCDISPLAYMODE_DARK].Text = 0xff010101;
			m_lbKart.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xffffffff;
			m_lbKart.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbKart.Align = 0;
			m_lbKart.Text = "Kart";
		}

		//UCLabel m_lbMini;
		{
			m_lbMini.UIName = "UCLogin::m_lbMini";
			m_lbMini.SetNewRealSize(289.000000,235.000000);
			m_lbMini.RealLocation = UCFPoint(0.000000,82.000000);
			m_lbMini.Anchor = 0;
			m_lbMini.Font.Weight = 900;
			m_lbMini.Font.Decoration = 0x50;
			m_lbMini.Font.Size = UCSize(48,96);
			m_lbMini.Font.OutlineSize = 8;
			m_lbMini.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Text = 0xffffc801;
			m_lbMini.FontColor.TextColor[UCDISPLAYMODE_DARK].Text = 0xffffc801;
			m_lbMini.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xffffffff;
			m_lbMini.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbMini.Align = 0;
			m_lbMini.Text = "Mini";
		}

		//UCLabel m_lbPassword;
		{
			m_lbPassword.UIName = "UCLogin::m_lbPassword";
			m_lbPassword.SetNewRealSize(142.000000,70.000000);
			m_lbPassword.RealLocation = UCFPoint(49.000000,694.000000);
			m_lbPassword.Anchor = 0;
			m_lbPassword.Font.Decoration = 0x10;
			m_lbPassword.Font.Size = UCSize(12,24);
			m_lbPassword.Font.OutlineSize = 4;
			m_lbPassword.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Text = 0xffffffff;
			m_lbPassword.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Outline = 0xffc0c0c0;
			m_lbPassword.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xffc0c0c0;
			m_lbPassword.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbPassword.Text = "Password： ";
		}

		//UCLabel m_lbTip;
		{
			m_lbTip.UIName = "UCLogin::m_lbTip";
			m_lbTip.SetNewRealSize(360.000000,36.000000);
			m_lbTip.RealLocation = UCFPoint(90.000000,2.000000);
			m_lbTip.Anchor = 0;
			m_lbTip.Font.Decoration = 0x10;
			m_lbTip.Font.Size = UCSize(12,24);
			m_lbTip.Font.OutlineSize = 4;
			m_lbTip.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Text = 0xff010101;
			m_lbTip.FontColor.TextColor[UCDISPLAYMODE_DARK].Text = 0xff010101;
			m_lbTip.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xffffffff;
			m_lbTip.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbTip.Align = 0;
			m_lbTip.Text = "Click any place to continue";
		}

		//UCLabel m_lbUsername;
		{
			m_lbUsername.UIName = "UCLogin::m_lbUsername";
			m_lbUsername.SetNewRealSize(142.000000,70.000000);
			m_lbUsername.RealLocation = UCFPoint(49.000000,593.000000);
			m_lbUsername.Anchor = 0;
			m_lbUsername.Font.Decoration = 0x10;
			m_lbUsername.Font.Size = UCSize(12,24);
			m_lbUsername.Font.OutlineSize = 4;
			m_lbUsername.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Text = 0xff010101;
			m_lbUsername.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Outline = 0xffc0c0c0;
			m_lbUsername.FontColor.TextColor[UCDISPLAYMODE_DARK].Text = 0xff010101;
			m_lbUsername.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xffc0c0c0;
			m_lbUsername.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_lbUsername.Text = "Username: ";
		}

		//UCTextBox m_tbPassword;
		{
			m_tbPassword.UIName = "UCLogin::m_tbPassword";
			m_tbPassword.SetNewRealSize(276.000000,70.000000);
			m_tbPassword.RealLocation = UCFPoint(204.000000,694.000000);
			m_tbPassword.Anchor = 0;
			m_tbPassword.CharKey = '*';
			m_tbPassword.Font.Decoration = 0x60;
			m_tbPassword.Font.Size = UCSize(19,38);
			m_tbPassword.Font.OutlineSize = 4;
			m_tbPassword.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_tbPassword.Align = 1;
		}

		//UCTextBox m_tbTip;
		{
			m_tbTip.UIName = "UCLogin::m_tbTip";
			m_tbTip.SetNewRealSize(431.000000,70.000000);
			m_tbTip.RealLocation = UCFPoint(49.000000,784.000000);
			m_tbTip.Anchor = 0;
			m_tbTip.Font.Size = UCSize(16,32);
			m_tbTip.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
			m_tbTip.Align = 0;
		}

		//UCTextBox m_tbUsername;
		{
			m_tbUsername.UIName = "UCLogin::m_tbUsername";
			m_tbUsername.SetNewRealSize(276.000000,70.000000);
			m_tbUsername.RealLocation = UCFPoint(204.000000,593.000000);
			m_tbUsername.Anchor = 0;
			m_tbUsername.Font.Weight = 900;
			m_tbUsername.Font.Decoration = 0x50;
			m_tbUsername.Font.Size = UCSize(19,38);
			m_tbUsername.Font.OutlineSize = 4;
			m_tbUsername.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
		}

		//父子关系结构
		{
			this->AddControl(&m_btRegister);
			this->AddControl(&m_imgLogo);
			this->AddControl(&m_imgTip);
			{
				m_imgTip.AddControl(&m_lbTip);
			}
			this->AddControl(&m_lbKart);
			this->AddControl(&m_lbMini);
			this->AddControl(&m_lbPassword);
			this->AddControl(&m_lbUsername);
			this->AddControl(&m_tbPassword);
			this->AddControl(&m_tbTip);
			this->AddControl(&m_tbUsername);
		}
	}
	~UCLogin_UI()	//析构函数
	{
	}
};

#endif //_UCLogin_UI_H_