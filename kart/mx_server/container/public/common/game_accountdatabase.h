#ifndef _game_accountdatabase_
#define _game_accountdatabase_

#include "game_userbase.h"

class UCRObjGameAccountData : public UCRObject
{
public:
	//登录或者创建一个新用户
	UCString Login(ucINT ChannelID, ucCONST UCString& Username, ucCONST UCString& Password, UCGameUserID& GameUserID)
	{
		return UCString("local");
	}
};

#endif