/********************************************************************
修改时间：	2025/06/24 10:00:00
文 件 名：	ucstoremanagement.ui.h
编辑警告：	这是由界面编辑器自动生成的代码文件
			手动修改不保证编辑器能正确识别和恢复
*********************************************************************/
#ifndef _UCStoreManagement_UI_H_
#define _UCStoreManagement_UI_H_

class UCStoreManagement_UI : public UCPanel
{
public:
	UCButton m_btNew;
	UCButton m_btDelete;
	UCButton m_btNext;
	UCButton m_btPrev;
	UCButton m_btReturn;
	UCListBox m_lbStores;
	UCTextBox m_tbTip;
public:
	UCStoreManagement_UI()	//构造函数
	{
		//自己的初始化
		{
			UIName = "Interface Editor Class: UCStoreManagement";
			SetNewRealSize(540.000000,960.000000);
			Anchor = 15;
		}

		//UCButton m_btNew;
		{
			m_btNew.UIName = "UCStoreManagement::m_btNew";
			m_btNew.SetNewRealSize(122.000000,72.000000);
			m_btNew.RealLocation = UCFPoint(-186.000000, 48.000000);
			m_btNew.Anchor = 6;
			m_btNew.Font.Weight = 900;
			m_btNew.Font.Decoration = 0x50;
			m_btNew.Font.Size = UCSize(12,24);
			m_btNew.Font.OutlineSize = 4;
			m_btNew.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff6d6d6d;
			m_btNew.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btNew.Text = "New";
		}

		//UCButton m_btDelete;
		{
			m_btDelete.UIName = "UCStoreManagement::m_btDelete";
			m_btDelete.SetNewRealSize(122.000000,72.000000);
			m_btDelete.RealLocation = UCFPoint(-48.000000,48.000000);
			m_btDelete.Anchor = 6;
			m_btDelete.Font.Weight = 900;
			m_btDelete.Font.Decoration = 0x50;
			m_btDelete.Font.Size = UCSize(12,24);
			m_btDelete.Font.OutlineSize = 4;
			m_btDelete.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff6d6d6d;
			m_btDelete.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btDelete.Text = "Delete";
			m_btDelete.Enable = ucFALSE;
		}

		//UCButton m_btNext;
		{
			m_btNext.UIName = "UCStoreManagement::m_btNext";
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
			m_btPrev.UIName = "UCStoreManagement::m_btPrev";
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
			m_btReturn.UIName = "UCStoreManagement::m_btReturn";
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

		//UCListBox m_lbStores;
		{
			m_lbStores.UIName = "UCStoreManagement::m_lbStores";
			m_lbStores.SetNewRealSize(430.000000,460.000000);
			m_lbStores.RealLocation = UCFPoint(48.000000,128.000000);
			m_lbStores.Anchor = 15;
			//UCHScroll m_lbStores.HScroll;
			{
				//UCButton m_lbStores.HScroll.Left;
				{
					m_lbStores.HScroll.Left.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbStores.HScroll.Left.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbStores.HScroll.Right;
				{
					m_lbStores.HScroll.Right.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbStores.HScroll.Right.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbStores.HScroll.Move;
				{
					m_lbStores.HScroll.Move.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbStores.HScroll.Move.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
			}
			//UCVScroll m_lbStores.VScroll;
			{
				//UCButton m_lbStores.VScroll.Up;
				{
					m_lbStores.VScroll.Up.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbStores.VScroll.Up.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbStores.VScroll.Down;
				{
					m_lbStores.VScroll.Down.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbStores.VScroll.Down.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				//UCButton m_lbStores.VScroll.Move;
				{
					m_lbStores.VScroll.Move.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbStores.VScroll.Move.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
			}
		}

		//UCTextBox m_tbTip;
		{
			m_tbTip.UIName = "UCStoreManagement::m_tbTip";
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
			this->AddControl(&m_btNew);
			this->AddControl(&m_btDelete);
			this->AddControl(&m_btNext);
			this->AddControl(&m_btPrev);
			this->AddControl(&m_btReturn);
			this->AddControl(&m_lbStores);
			{
				//m_lbStores.AddControl(&m_lbStores.HScroll);
				{
					//m_lbStores.HScroll.AddControl(&m_lbStores.HScroll.Back);
					//m_lbStores.HScroll.AddControl(&m_lbStores.HScroll.Left);
					//m_lbStores.HScroll.AddControl(&m_lbStores.HScroll.Move);
					//m_lbStores.HScroll.AddControl(&m_lbStores.HScroll.Right);
				}
				//m_lbStores.AddControl(&m_lbStores.VScroll);
				{
					//m_lbStores.VScroll.AddControl(&m_lbStores.VScroll.Back);
					//m_lbStores.VScroll.AddControl(&m_lbStores.VScroll.Down);
					//m_lbStores.VScroll.AddControl(&m_lbStores.VScroll.Move);
					//m_lbStores.VScroll.AddControl(&m_lbStores.VScroll.Up);
				}
				//m_lbStores.AddControl(&m_lbStores.View);
				{
					//m_lbStores.View.AddControl(&m_lbStores.Client);
				}
			}
			this->AddControl(&m_tbTip);
		}
	}
	~UCStoreManagement_UI()	//析构函数
	{
	}
};

#endif //_UCStoreManagement_UI_H_ 