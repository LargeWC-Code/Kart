/********************************************************************
Last Modified: 2025/06/24 11:00:00
File Name: ucinventory.ui.h
Edit Warning: This is an auto-generated code file by the UI editor.
Manual modifications may prevent the editor from correctly recognizing and restoring.
*********************************************************************/
#ifndef _UCInventory_UI_H_
#define _UCInventory_UI_H_

class UCInventory_UI : public UCPanel
{
public:
	UCButton	m_btUse;
	UCListBox	m_lbInventoryItems;
public:
	UCInventory_UI() // Constructor
	{
		// Custom initialization
		{
			UIName = "UI Editor Class: UCInventory";
			SetNewRealSize(400.000000,360.000000);
			RealLocation = UCFPoint(0.0f, 320.0f);
			Anchor = 1;
		}

		// UCButton m_btUse;
		{
			m_btUse.UIName = "UCInventory::m_btUse";
			m_btUse.SetNewRealSize(128.000000,48.000000);
			m_btUse.RealLocation = UCFPoint(-30.000000,-20.000000);
			m_btUse.Anchor = 10;
			m_btUse.Font.Weight = 900;
			m_btUse.Font.Decoration = 0x50;
			m_btUse.Font.Size = UCSize(12,24);
			m_btUse.Font.OutlineSize = 4;
			m_btUse.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff6d6d6d;
			m_btUse.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
			m_btUse.Text = "Equip";
		}

		// UCListBox m_lbInventoryItems;
		{
			m_lbInventoryItems.UIName = "UCInventory::m_lbInventoryItems";
			m_lbInventoryItems.SetNewRealSize(352.000000,260.000000);
			m_lbInventoryItems.RealLocation = UCFPoint(24.000000,24.000000);
			m_lbInventoryItems.Anchor = 15;
			// UCHScroll m_lbInventoryItems.HScroll;
			{
				// UCButton m_lbInventoryItems.HScroll.Left;
				{
					m_lbInventoryItems.HScroll.Left.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbInventoryItems.HScroll.Left.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				// UCButton m_lbInventoryItems.HScroll.Right;
				{
					m_lbInventoryItems.HScroll.Right.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbInventoryItems.HScroll.Right.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				// UCButton m_lbInventoryItems.HScroll.Move;
				{
					m_lbInventoryItems.HScroll.Move.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbInventoryItems.HScroll.Move.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
			}
			// UCVScroll m_lbInventoryItems.VScroll;
			{
				// UCButton m_lbInventoryItems.VScroll.Up;
				{
					m_lbInventoryItems.VScroll.Up.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbInventoryItems.VScroll.Up.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				// UCButton m_lbInventoryItems.VScroll.Down;
				{
					m_lbInventoryItems.VScroll.Down.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbInventoryItems.VScroll.Down.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
				// UCButton m_lbInventoryItems.VScroll.Move;
				{
					m_lbInventoryItems.VScroll.Move.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff000000;
					m_lbInventoryItems.VScroll.Move.DisableFontColor.TextColor[UCDISPLAYMODE_DARK].Outline = 0xff404040;
				}
			}
		}

		// Parent-child relationship structure
		{
			this->AddControl(&m_btUse);
			this->AddControl(&m_lbInventoryItems);
			{
				// m_lbInventoryItems.AddControl(&m_lbInventoryItems.HScroll);
				{
					// m_lbInventoryItems.HScroll.AddControl(&m_lbInventoryItems.HScroll.Back);
					// m_lbInventoryItems.HScroll.AddControl(&m_lbInventoryItems.HScroll.Left);
					// m_lbInventoryItems.HScroll.AddControl(&m_lbInventoryItems.HScroll.Move);
					// m_lbInventoryItems.HScroll.AddControl(&m_lbInventoryItems.HScroll.Right);
				}
				// m_lbInventoryItems.AddControl(&m_lbInventoryItems.VScroll);
				{
					// m_lbInventoryItems.VScroll.AddControl(&m_lbInventoryItems.VScroll.Back);
					// m_lbInventoryItems.VScroll.AddControl(&m_lbInventoryItems.VScroll.Down);
					// m_lbInventoryItems.VScroll.AddControl(&m_lbInventoryItems.VScroll.Move);
					// m_lbInventoryItems.VScroll.AddControl(&m_lbInventoryItems.VScroll.Up);
				}
				// m_lbInventoryItems.AddControl(&m_lbInventoryItems.View);
				{
					// m_lbInventoryItems.View.AddControl(&m_lbInventoryItems.Client);
				}
			}
		}
	}
	~UCInventory_UI() // Destructor
	{
	}
};

#endif //_UCInventory_UI_H_ 