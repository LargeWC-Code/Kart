#ifndef _accountbase_
#define _accountbase_

#include "game_userbase.h"

//账号登录服
class UCRObjGameAccount : public UCRObject
{
public:
	UCString Login(ucCONST UCString& Account, ucCONST UCString& Password, UCGameUserID& GameUserID, ucDWORD& AccountKey)
	{
		return UCString("local");
	}
	UCString GetInfo(ucCONST UCGameUserID& GameUserID, ucDWORD AccountKey, UCString& Account, UCString& Password)
	{
		return UCString("local");
	}
};

#endif