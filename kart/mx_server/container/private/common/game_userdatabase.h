#ifndef _game_userdatabase_
#define _game_userdatabase_

struct UCRProAvatarData : public UCRProData
{
	ucINT		FaceID;
	ucINT		HairID;

	ucINT		BodyID;
	ucINT		HandID;

	ucINT		PantID;
	ucINT		ShoeID;

	ucINT		CarID;
};

struct UCRItemData
{
	UCString			Name;
	UCString			Description;

	ucINT				Type;
	ucINT				Price;
	UCRItemData()
	{
		Type = 0;
		Price = 0;
	}
};

class UCRObjUserData : public UCRObject
{
public:
	UCRString			NickName;

	UCRInt				Gold;					//金币
	UCRInt				Level;					//等级
	UCRInt64			Exp;					//经验

	UCRProAvatarData	AvatarData;
};

#endif