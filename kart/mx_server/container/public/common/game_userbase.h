#ifndef _game_userbase_
#define _game_userbase_

enum
{
	Channel_MagicX	= 0,
	//无效渠道
	Channel_Unvalid = 0xFFF
};

#define CHANNEL_MAX			1

#define DATA_USERMAX		0x7FFFFFFF

enum
{
	UserID_Unvalid = 0xFFFFFFFF
};

struct UCGameUserID
{
	ucINT64	ID;

	UCGameUserID() 
	{
		ID = 0xFFFFFFFFFFFFFFFFU;
	}
	UCGameUserID(ucCONST UCGameUserID& in)
	{
		this->ID = in.ID;
	}
	UCGameUserID(ucINT64 ID)
	{
		this->ID = ID;
	}
	ucVOID Set(ucCONST UCGameUserID& in)
	{
		this->ID = in.ID;
	}
	ucVOID Set(ucINT64 ID)
	{
		this->ID = ID;
	}
	//数据库ID
	ucDWORD GetDataID() ucCONST
	{
		return (ID >> 32) & 0xFFFFFFFF;
	}
	ucVOID SetDataID(ucDWORD DataID)
	{
		ID = (ID & 0x00000000FFFFFFFFU) | (ucINT64(DataID) << 32);
	}
	//玩家ID
	ucDWORD GetUserID() ucCONST
	{
		return ID;
	}
	ucVOID SetUserID(ucDWORD UserID)
	{
		ID = (ID & 0xFFFFFFFF00000000U) | UserID;
	}
	UCString ToString() ucCONST
	{
		ucDWORD DataID = (ID >> 32) & 0xFFFFFFFF;
		ucDWORD UserID = ID;

		return ITOS(DataID, UCString("%08x:")) + ITOS(UserID, UCString("%08x"));
	}
	ucVOID FromString(ucCONST UCString& String)
	{
		ucUINT64 DataID = STOI(String.Left(8), UCString("%08d"));
		ucUINT64 UserID = STOI(String.Right(8), UCString("%08d"));

		ID = DataID << 32 | (UserID & 0x00000000FFFFFFFFU);
	}
};

struct UCGameUserExt
{
	UCStr32		Password;

	ucINT		ChannelID;
	ucINT		Access_Level;		//0 User 1 Manager 2 Root

	UCStr32		EMail;
	ucINT		EMoney;
	UCGameUserExt()
	{
		ChannelID = 0;
		Access_Level = 0;

		EMoney = 0;
	}
};

#endif