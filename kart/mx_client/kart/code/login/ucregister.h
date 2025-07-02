/********************************************************************
生成时间：	2021/03/19 11:17:03
文 件 名：	ucregister.h
所 有 者：	
			
*********************************************************************/
#ifndef _UCRegister_H_
#define _UCRegister_H_
#include "ucregister.ui.h"

class UCRegister : public UCRegister_UI
{
public:
	UCEvent					OnRegisterSucceed;

	UCGameUserID			GameUserID;
	ucUINT64				Token;
	UCString				GameHomeUrl;
private:
	UCRObjGameHome*			RObjGameHome;

	UCFiber					FiberTip;
public:
	UCRegister(UCRObjGameHome* RObjGameHome)	//构造函数
	{
		//默认不可见
		Visible = ucFALSE;
		Enable = ucFALSE;

		Token = 0;

		this->RObjGameHome = RObjGameHome;

		OnClick += UCEvent(this, OnThisClick);

		FiberTip.FiberEvent = UCEvent(this, OnFiberTip);
		FiberTip.Start(0);
	}
	~UCRegister()	//析构函数
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
			for (ucINT i = 0; i < 255; i += 2)
			{
				m_imgTip.Alpha = i;
				FiberData->Every(100);
			}
			FiberData->Delay();
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
	ucVOID OnThisClick(UCObject* Sender, UCEventArgs*)
	{
		UCString strRet = RObjGameHome->Login(m_tbUsername.Text.Value, m_tbPassword.Text.Value, GameUserID, Token);
		if (strRet == "succeed")
			OnRegisterSucceed.Run(this, 0);
		else
			WBox(strRet);
	}
};

#endif //_UCRegister_H_