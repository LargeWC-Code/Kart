/********************************************************************
修改时间：	2025/06/23 09:36:48
文 件 名：	ucusermanagement.ui.h
编辑警告：	这是由界面编辑器自动生成的代码文件
			手动修改不保证编辑器能正确识别和恢复
*********************************************************************/
#ifndef _UCUserManagement_UI_H_
#define _UCUserManagement_UI_H_

class UCUserManagement_UI : public UCPanel
{
public:
	UCButton m_btNext;
	UCButton m_btPrev;
	UCButton m_btReturn;
	UCListBox m_lbUsers;
	UCTextBox m_tbTip;
public:
	UCUserManagement_UI()	//构造函数
	{
		//自己的初始化
		{
			UIName = "界面编辑器类：UCUserManagement";
			SetNewRealSize(540.000000,960.000000);
			Anchor = 15;
		}

		//UCButton m_btNext;
		{
			m_btNext.UIName = "UCUserManagement::m_btNext";
			m_btNext.SetNewRealSize(122.000000,72.000000);
			m_btNext.RealLocation = UCFPoint(-60.000000,-40.000000);
			m_btNext.Anchor = 10;
			m_btNext.Font.Weight = 900;
			m_btNext.Font.Decoration = 0x50;
			m_btNext.Font.Size = UCSize(12,24);
			m_btNext.Font.OutlineSize = 4;
			m_btNext.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff6d6d6d;
			m_btNext.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btNext.Text = "Next";
		}

		//UCButton m_btPrev;
		{
			m_btPrev.UIName = "UCUserManagement::m_btPrev";
			m_btPrev.SetNewRealSize(122.000000,72.000000);
			m_btPrev.RealLocation = UCFPoint(-198.000000,-40.000000);
			m_btPrev.Anchor = 10;
			m_btPrev.Font.Weight = 900;
			m_btPrev.Font.Decoration = 0x50;
			m_btPrev.Font.Size = UCSize(12,24);
			m_btPrev.Font.OutlineSize = 4;
			m_btPrev.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff6d6d6d;
			m_btPrev.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btPrev.Text = "Prev";
		}

		//UCButton m_btReturn;
		{
			m_btReturn.UIName = "UCUserManagement::m_btReturn";
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

		//UCListBox m_lbUsers;
		{
			m_lbUsers.UIName = "UCUserManagement::m_lbUsers";
			m_lbUsers.SetNewRealSize(430.000000,540.000000);
			m_lbUsers.RealLocation = UCFPoint(48.000000,48.000000);
			m_lbUsers.Anchor = 15;
			//UCHScroll m_lbUsers.HScroll;
			{
				//UCButton m_lbUsers.HScroll.Left;
				{
					m_lbUsers.HScroll.Left.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbUsers.HScroll.Left.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbUsers.HScroll.Right;
				{
					m_lbUsers.HScroll.Right.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbUsers.HScroll.Right.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbUsers.HScroll.Move;
				{
					m_lbUsers.HScroll.Move.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbUsers.HScroll.Move.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
			}
			//UCVScroll m_lbUsers.VScroll;
			{
				//UCButton m_lbUsers.VScroll.Up;
				{
					m_lbUsers.VScroll.Up.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbUsers.VScroll.Up.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbUsers.VScroll.Down;
				{
					m_lbUsers.VScroll.Down.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbUsers.VScroll.Down.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbUsers.VScroll.Move;
				{
					m_lbUsers.VScroll.Move.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbUsers.VScroll.Move.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
			}
		}

		//UCTextBox m_tbTip;
		{
			m_tbTip.UIName = "UCUserManagement::m_tbTip";
			m_tbTip.SetNewRealSize(430.000000,200.000000);
			m_tbTip.RealLocation = UCFPoint(48.000000,-159.333374);
			m_tbTip.Anchor = 11;
			m_tbTip.Font.Decoration = 0x10;
			m_tbTip.Font.Size = UCSize(16,32);
			m_tbTip.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Outline = 0xffb2b2b2;
			m_tbTip.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
		}

		//父子关系结构
		{
			this->AddControl(&m_btNext);
			this->AddControl(&m_btPrev);
			this->AddControl(&m_btReturn);
			this->AddControl(&m_lbUsers);
			{
				//m_lbUsers.AddControl(&m_lbUsers.HScroll);
				{
					//m_lbUsers.HScroll.AddControl(&m_lbUsers.HScroll.Back);
					//m_lbUsers.HScroll.AddControl(&m_lbUsers.HScroll.Left);
					//m_lbUsers.HScroll.AddControl(&m_lbUsers.HScroll.Move);
					//m_lbUsers.HScroll.AddControl(&m_lbUsers.HScroll.Right);
				}
				//m_lbUsers.AddControl(&m_lbUsers.VScroll);
				{
					//m_lbUsers.VScroll.AddControl(&m_lbUsers.VScroll.Back);
					//m_lbUsers.VScroll.AddControl(&m_lbUsers.VScroll.Down);
					//m_lbUsers.VScroll.AddControl(&m_lbUsers.VScroll.Move);
					//m_lbUsers.VScroll.AddControl(&m_lbUsers.VScroll.Up);
				}
				//m_lbUsers.AddControl(&m_lbUsers.View);
				{
					//m_lbUsers.View.AddControl(&m_lbUsers.Client);
				}
			}
			this->AddControl(&m_tbTip);
		}
	}
	~UCUserManagement_UI()	//析构函数
	{
	}
};

#endif //_UCUserManagement_UI_H_