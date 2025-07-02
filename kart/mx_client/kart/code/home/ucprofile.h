/********************************************************************
生成时间：	2021/03/19 11:17:03
文 件 名：	ucregister.h
所 有 者：	
			
*********************************************************************/
#ifndef _UCProfile_H_
#define _UCProfile_H_
#include "ucprofile.ui.h"

class UCProfile : public UCProfile_UI
{
public:
	ucBOOL Confirmed;
	ucBOOL CheckPassword;
	ucBOOL CheckEmail;
public:
	UCProfile()	//构造函数
	{
		//默认不可见
		Visible = ucFALSE;
		Enable = ucFALSE;

		CheckPassword = ucTRUE;
		CheckEmail = ucFALSE;

		m_tbPassword.OnModify += UCEvent(this, OnPasswordCheck);
		m_tbPassword1.OnModify += UCEvent(this, OnPasswordCheck);

		m_tbEmail.OnModify += UCEvent(this, OnEmailModify);

		Confirmed = ucFALSE;
		m_btReturn.OnClick += UCEvent(this, OnReturnClick);
		m_btConfirm.OnClick += UCEvent(this, OnConfirmClick);
	}
	~UCProfile()	//析构函数
	{
	}
	ucVOID ShowUI()
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
	}
	ucVOID HideUI()
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)GetRunFiberData();

		Enable = ucFALSE;
		Alpha = 255;
		for (ucINT i = 0; i < 255; i += 5)
		{
			Alpha = 255 - i;
			FiberData->Every(100);
		}
		Alpha = 0;
		Visible = ucFALSE;
	}
	ucVOID OnPasswordCheck(UCObject*, UCEventArgs*)
	{
		CheckPassword = ucFALSE;
		if (m_tbPassword.Text.Value != m_tbPassword1.Text.Value)
		{
			m_tbPasswordTip.Text = UCString("Please confirm that your password is entered correctly.");
			return;
		}
		else if (m_tbPassword.Text.Value.IsEmpty() && m_tbPassword1.Text.Value.IsEmpty())
			m_tbPasswordTip.Text = UCString("If you do not wish to change your password, please leave it blank.");
		else
			m_tbPasswordTip.Text = UCString("OK");
		CheckPassword = ucTRUE;
	}
	ucVOID OnEmailModify(UCObject*, UCEventArgs*)
	{
		UCPattern Pattern(UCString("+(c|n|[.]|[_]|[%]|[+]|[-])[@]+(c|n)!([-]+(c|n))+([.]+(c|n))"));// [@] + (c | n | [.] | [_] | [%] | [] | [+] | [-])[.] + (c | n | [.] | [_] | [%] | [] | [+] | [-])"));

		CheckEmail = ucFALSE;
		ucINT Ret = Pattern.Match(m_tbEmail.Text.Value);
		if (Ret != m_tbEmail.Text.Value.GetLength())
		{
			m_tbEmailTip.Text = UCString("Please confirm that your email address is entered correctly.") + ITOS(Ret);
			return;
		}
		else
			m_tbEmailTip.Text = UCString("OK");
		CheckEmail = ucTRUE;
	}
	ucVOID OnReturnClick(UCObject*, UCEventArgs*)
	{
		Confirmed = ucFALSE;
		HideUI();
	}
	ucVOID OnConfirmClick(UCObject*, UCEventArgs*)
	{
		if (CheckPassword && CheckEmail)
		{
			Confirmed = ucTRUE;
			HideUI();
		}
	}
};

#endif //_UCProfile_H_