#ifndef _game_homebase_
#define _game_homebase_

#include "game_userdatabase.h"

struct UCAvatarData
{
	ucINT		FaceID;
	ucINT		HairID;

	ucINT		BodyID;
	ucINT		HandID;

	ucINT		PantID;
	ucINT		ShoeID;

	ucINT		CarID;

	ucVOID operator =(ucCONST UCString& strData)
	{

	}
};

enum ITEM_TYPE
{
	ITEM_TYPE_FACE,
	ITEM_TYPE_HAIR,
	ITEM_TYPE_BODY,
	ITEM_TYPE_HAND,
	ITEM_TYPE_PANT,
	ITEM_TYPE_SHOE,
	ITEM_TYPE_CAR
};

// 商品数据结构
struct UCGameStoreData : UCRProData
{
	ucINT			ID;
	UCString		Name;
	UCString		Description;
	ucINT			Type;
	ucINT			Price;
	ucINT			ResID;

	UCGameStoreData()
	{
		ID = 0;
		Price = 0;
		ResID = 0;
	}
	UCGameStoreData(ucCONST UCGameStoreData& in)
	{
		ID = in.ID;
		Name = in.Name;
		Description = in.Description;
		Type = in.Type;
		Price = in.Price;
		ResID = in.ResID;
	}
};

struct UCGameUserData : UCRProData
{
	UCString				Username;
	UCString				Password;

	ucINT					Access_Level;
	ucINT					VMoney;

	UCString				Nickname;
	UCString				Email;

	UCEArray<ucINT>			AryItemData;
	UCAvatarData			AvatarData;
};

class UCRObjGameHome : public UCRObject
{
public:
	UCString	Login(ucCONST UCString& Username, ucCONST UCString& Password, UCGameUserID& GameUserID, ucUINT64& Token)
	{
		return UCString("local");
	}
	UCString	ChangeProfile(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucCONST UCString& Password, ucCONST UCString& Nicknname, ucCONST UCString& Email)
	{
		return UCString("local");
	}
	UCString	ChangeProfile(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucCONST UCGameUserData& GameUserData)
	{
		return UCString("local");
	}
	UCString	GetAllUsers(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucINT Offset, ucINT Count, UCEArray<UCGameUserData>& AryUsers)
	{
		return UCString("local");
	}
	// 商品管理接口
	UCString	GetAllStoreItems(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucINT Offset, ucINT Count, UCEArray<UCGameStoreData>& AryStores)
	{
		return UCString("local");
	}
	UCString	UpdateStoreItem(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucCONST UCGameStoreData& StoreData)
	{
		return UCString("local");
	}
	UCString	DeleteStoreItem(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucCONST UCString& ItemName)
	{
		return UCString("local");
	}
	UCString	BuyStoreItem(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucINT ItemID)
	{
		return UCString("local");
	}
	UCString	ChangeAvatar(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucCONST UCRProAvatarData& AvatarData)
	{
		return UCString("local");
	}

// 	UCString	Activate(ucCONST UCGameUserID& GameUserID, ucCONST ucDWORD& AccountID, ucCONST ucDWORD& AccountKey, ucDWORD& HomeKey)
// 	{
// 		return UCString("local");
// 	}
};

// 将ucINT数组转换为逗号分隔的字符串
UCString IntArrayToString(ucCONST UCEArray<ucINT>& AryData)
{
	UCString strResult;
	for (ucINT i = 0; i < AryData.GetSize(); i++)
	{
		if (i > 0)
			strResult += UCString(",");
		strResult += ITOS(AryData[i]);
	}
	return strResult;
}

// 将逗号分隔的字符串转换为ucINT数组
ucVOID StringToIntArray(ucCONST UCString& strValue, UCEArray<ucINT>& AryData)
{
	AryData.RemoveAll();

	ucINT nCurr = 0;
	ucINT nLast = 0;
	while (nCurr >= 0)
	{
		nCurr = strValue.Find(',', nCurr);
		if (nCurr >= 0)
		{
			UCString strData = UCString(strValue.Mid(nLast, nCurr - nLast));
			ucINT nData = STOI(strData);

			AryData.Add(nData);

			nCurr++;
			nLast = nCurr;
		}
	}

	if (nLast < strValue.GetLength())
	{
		UCString strData = UCString(strValue.Mid(nLast, strValue.GetLength() - nLast));
		ucINT nData = STOI(strData);

		AryData.Add(nData);
	}
}

class UCRObjGameUserData : public UCRObject
{
public:
	UCRString				Username;

	UCRInt					Token;
	UCRInt					Channel;

	UCRInt					Access_Level;

	UCRString				Nickname;
	UCRString				Email;

	UCRInt					VMoney;
	UCRArray<ucINT>			AryItemData;
	UCRProAvatarData		AvatarData;
public:
	UCRObjGameUserData()
	{
		Token = 0;
		Channel = 0;
		Access_Level = 0;

		VMoney = 0;
	}
	UCString					Update()
	{
		return UCString("local");
	}
};

struct UCRProBattleInfo :public UCRProData
{
	ucINT				MatchID;
	ucINT				LogicID;
	ucINT				BattleID;
};

class UCRObjGameUserData_Ext : public UCRObject
{
public:
	UCRProBattleInfo			BattleInfo;
public:
	UCString					Update()
	{
		return UCString("local");
	}
};

#endif