/********************************************************************
生成时间：	2021/03/16 14:43:59
文 件 名：	uclogin.h
所 有 者：	
			
*********************************************************************/
#ifndef _UCLogin_H_
#define _UCLogin_H_
#include "uclogin.ui.h"

class UCLogin : public UCLogin_UI
{
public:
	UCEvent					OnLoginPrepare;
	UCEvent					OnLoginSucceed;
	UCEvent					OnRegister;

	UCGameUserID			GameUserID;
	ucUINT64				Token;
private:
	UCRObjGameHome*			RObjGameHome;

	UCFiber					FiberTip;
	UCFiber					FiberUI;
public:
	UCLogin(UCRObjGameHome* RObjGameHome)	//构造函数
	{
// 		m_imgLogo.Visible = 0;
// 
// 		m_lbKart.Visible = 0;
// 		m_lbMini.Visible = 0;

		//默认不可见
		Visible = ucFALSE;
		Enable = ucFALSE;

		Token = 0;

		m_tbUsername.Text = UCString("123");
		m_tbPassword.Text = UCString("123");

		this->RObjGameHome = RObjGameHome;
		
		m_btRegister.OnClick += UCEvent(this, OnRegisterClick);
		OnClick += UCEvent(this, OnThisClick);

		FiberTip.FiberEvent = UCEvent(this, OnFiberTip);
		FiberTip.Start(0);
	}
	~UCLogin()	//析构函数
	{
	}
	ucVOID OnFiberTip(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;
		while (FiberData->IsValid())
		{
			for (ucINT i = 0; i < 255; i += 2)
			{
				m_imgTip.Alpha = 255 - i;
				FiberData->Every(100);
			}
			for (ucINT i = 0; i < 255; i+= 2)
			{
				m_imgTip.Alpha = i;
				FiberData->Every(100);
			}
		}
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
	ucVOID OnRegisterClick(UCObject* Sender, UCEventArgs*)
	{
		OnRegister.Run(this, 0);
	}
	ucVOID OnThisClick(UCObject* Sender, UCEventArgs*)
	{
		if (!RObjGameHome->Linked)
			OnLoginPrepare.Run(this, ucNULL);
		UCString strRet = RObjGameHome->Login(m_tbUsername.Text.Value, m_tbPassword.Text.Value, GameUserID, Token);
		if (strRet == "succeed")
			OnLoginSucceed.Run(this, 0);
		else
			m_tbTip.Text = strRet;
	}
};

#endif //_UCLogin_H_