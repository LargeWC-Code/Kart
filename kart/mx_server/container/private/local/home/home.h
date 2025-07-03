#ifndef _home_
#define _home_

#include "../../../public/common/game_userbase.h"
#include "../../../public/common/game_database.h"
#include "../../../public/common/game_centerbase.h"
#include "../../../public/common/game_monitorbase.h"

#include "../../../public/common/game_accountbase.h"

#include "../../common/game_homebase.h"

struct UCGameHomeInfo
{
	UCString					Password;
};

class UCRObjGameUserData_Ext_Home : public UCRObjGameUserData_Ext
{
public:
	UCString					Update()
	{
		return UCString("succeed");
	}
};

class UCRObjGameUserData_Home : public UCRObjGameUserData
{
public:
	UCGameUserID					GameUserID;
	UCString						Password;
	ucUINT64						Token;
	UCRObjGameUserData_Ext_Home		RObjGameUserData_Ext_Home;
public:
	UCRObjGameUserData_Home()
	{
		Nickname = UCString("anonymous");

		VMoney = 0;
		Token = 0;

		RObjGameUserData_Ext_Home.BattleInfo.MatchID = -1;
		RObjGameUserData_Ext_Home.BattleInfo.LogicID = -1;
		RObjGameUserData_Ext_Home.BattleInfo.BattleID = -1;
	}
	UCString					Update()
	{
		return UCString("succeed");
	}
};

class UCRObjManager_UserData
{
public:
	UCString									LocalUrl;
	UCRContainer*								RContainer;
public:
	UCRObjGameUserData_Home* Find(ucCONST UCGameUserID& GameUserID)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home = ucNULL;

		ucINT Pos = MapID_GameUserData_Group.FindKey(GameUserID.ID);
		if (Pos >= 0)
		{
			RObjGameUserData_Home = &MapID_GameUserData_Group.GetValueAt(Pos);
			RObjGameUserData_Home->KeepValid();
			return RObjGameUserData_Home;
		}
		return ucNULL;
	}
	UCRObjGameUserData_Home* Get(ucCONST UCGameUserID& GameUserID)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home = ucNULL;

		ucINT Pos = MapID_GameUserData_Group.FindKey(GameUserID.ID);
		if (Pos >= 0)
		{
			RObjGameUserData_Home = &MapID_GameUserData_Group.GetValueAt(Pos);
			RObjGameUserData_Home->KeepValid();
		}
		else
		{
			RObjGameUserData_Home = MapID_GameUserData_Group.At(GameUserID.ID);

			ucUINT64 Key = randint64();
			if (!RContainer->AppendSource(LocalUrl + UCString("//UCRObjGameUserData/") + GameUserID.ToString() + UCString(".obj"), typeof(UCRObjGameUserData), typeof(UCRObjGameUserData_Home), RObjGameUserData_Home, ucTRUE, ucFALSE))
			{
				MapID_GameUserData_Group.Remove(GameUserID.ID);
				return ucNULL;
			}

			if (!RContainer->AppendSource(LocalUrl + UCString("//UCRObjGameUserData/UCRObjGameUserData_Ext/") + GameUserID.ToString() + UCString(".obj"), typeof(UCRObjGameUserData_Ext), typeof(UCRObjGameUserData_Ext_Home), &RObjGameUserData_Home->RObjGameUserData_Ext_Home, ucTRUE, ucTRUE, Key))
			{
				MapID_GameUserData_Group.Remove(GameUserID.ID);
				return ucNULL;
			}

			RObjGameUserData_Home->GameUserID.Set(GameUserID);
			//远程释放捆绑
			RObjGameUserData_Home->OnRemove = UCEvent(this, OnGameUserDataRemove);
		}

		return RObjGameUserData_Home;
	}
	ucVOID Remove(UCRObjGameUserData_Home* RObjGameUserData_Home)
	{
		//缓存删除
		MapID_GameUserData_Group.Remove(RObjGameUserData_Home->GameUserID.ID);
	}
private:
	ucVOID OnGameUserDataRemove(UCObject* Object, UCEventArgs*)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home = (UCRObjGameUserData_Home*)Object;

		MapID_GameUserData_Group.Remove(RObjGameUserData_Home->GameUserID.ID);
	}
private:
	UCESimpleMap<ucINT64, UCRObjGameUserData_Home>	MapID_GameUserData_Group;
};

class UCRObjGameHomeLocal : public UCRObjGameHome
{
public:
	UCEApp*						App;
	UCEConfig<UCGameHomeInfo>	GameHomeCfg;
	UCRContainer_NET			RContainer_NET;

	UCRObjGameCenter			RObjGameCenter;
	UCRObjGameMonitor			RObjGameMonitor;

	UCRObjGameData				RObjGameData[DATA_MAX];
	UCRObjGameAccount			RObjGameAccount[ACCOUNT_MAX];

	UCEArray<UCGameStoreData>	AryStoreData;
	UCRObjManager_UserData		RObjManager_UserData;
public:
	UCMySQL						MySQL;

	ucINT64						AdminKey;	
	ucDWORD						RandKey;
	UCFiber						FiberInit;
public:
	UCString Login(ucCONST UCString& Username, ucCONST UCString& Password, UCGameUserID& GameUserID, ucUINT64& Token)
	{
		return RealLogin(Username, Password, 0, GameUserID, Token);
	}
	UCString	ChangeProfile(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucCONST UCString& Password, ucCONST UCString& Nickname, ucCONST UCString& Email)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home = RObjManager_UserData.Find(GameUserID);

		if (RObjGameUserData_Home == ucNULL)
			return UCString("unvalid id");

		if (!Password.IsEmpty())
			RObjGameUserData_Home->Password = Password;
		RObjGameUserData_Home->Nickname = Nickname;
		RObjGameUserData_Home->Email = Email;

		OnRStructTimeOut(RObjGameUserData_Home, ucNULL);
		return UCString("succeed");
	}
	UCString	ChangeProfile(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucCONST UCGameUserData& GameUserData)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home_Root = RObjManager_UserData.Find(GameUserID);

		if (RObjGameUserData_Home_Root == ucNULL)
			return UCString("unvalid id");

		if (RObjGameUserData_Home_Root->Access_Level.Value < 2)
			return UCString("Permission denied");

		UCRObjGameUserData_Home* RObjGameUserData_Home = RealActive(GameUserData.Username);

		if (RObjGameUserData_Home == ucNULL)
			return UCString("active failed");

		if (!GameUserData.Password.IsEmpty())
			RObjGameUserData_Home->Password = GameUserData.Password;

		RObjGameUserData_Home->Access_Level = GameUserData.Access_Level;
		RObjGameUserData_Home->VMoney = GameUserData.VMoney;

		RObjGameUserData_Home->Nickname = GameUserData.Nickname;
		RObjGameUserData_Home->Email = GameUserData.Email;

		RObjGameUserData_Home->AryItemData.RemoveAll();
		for (ucINT i = 0; i < GameUserData.AryItemData.GetSize(); i++)
			RObjGameUserData_Home->AryItemData.Add(GameUserData.AryItemData[i]);
		
		// Assign avatar data members individually
		RObjGameUserData_Home->AvatarData.FaceID = GameUserData.AvatarData.FaceID;
		RObjGameUserData_Home->AvatarData.HairID = GameUserData.AvatarData.HairID;
		RObjGameUserData_Home->AvatarData.BodyID = GameUserData.AvatarData.BodyID;
		RObjGameUserData_Home->AvatarData.HandID = GameUserData.AvatarData.HandID;
		RObjGameUserData_Home->AvatarData.PantID = GameUserData.AvatarData.PantID;
		RObjGameUserData_Home->AvatarData.ShoeID = GameUserData.AvatarData.ShoeID;
		RObjGameUserData_Home->AvatarData.CarID = GameUserData.AvatarData.CarID;

		RObjGameUserData_Home->AvatarData.Commit();

		OnRStructTimeOut(RObjGameUserData_Home, ucNULL);
		return UCString("succeed");
	}
	UCString	GetAllUsers(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucINT Offset, ucINT Count, UCEArray<UCGameUserData>& AryUsers)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home = RObjManager_UserData.Find(GameUserID);

		if (RObjGameUserData_Home == ucNULL)
			return UCString("unvalid id");

		if (RObjGameUserData_Home->Access_Level.Value < 2)
			return UCString("Permission denied");

		UCMySQLRes ResUser;
		UCStringA CmdUser = UCString("SELECT * FROM accounts LIMIT ") + ITOS(Count) + UCString(" OFFSET ") + ITOS(Offset);
		ucINT Ret = MySQL.Query(CmdUser, &ResUser, 1);
		if (Ret == MYSQL_RET_SECCESS) 
		{
			while (ResUser.FetchRow() == 0) 
			{
				UCGameUserData GameUserData;

				GameUserData.Username = ResUser.GetRowAsString(2);

				GameUserData.Access_Level = ResUser.GetRowAsInt(5);

				GameUserData.Nickname = ResUser.GetRowAsString(6);
				GameUserData.Email = ResUser.GetRowAsString(7);

				GameUserData.VMoney = ResUser.GetRowAsInt(8);

				StringToIntArray(ResUser.GetRowAsString(9), GameUserData.AryItemData);
				
				// Load avatar data from string
				UCString strAvatar = ResUser.GetRowAsString(10);
				if (!strAvatar.IsEmpty())
				{
					UCEArray<ucINT> AryAvatar;
					StringToIntArray(strAvatar, AryAvatar);
					if (AryAvatar.GetSize() >= 7)
					{
						GameUserData.AvatarData.FaceID = AryAvatar[0];
						GameUserData.AvatarData.HairID = AryAvatar[1];
						GameUserData.AvatarData.BodyID = AryAvatar[2];
						GameUserData.AvatarData.HandID = AryAvatar[3];
						GameUserData.AvatarData.PantID = AryAvatar[4];
						GameUserData.AvatarData.ShoeID = AryAvatar[5];
						GameUserData.AvatarData.CarID = AryAvatar[6];
					}
				}

				AryUsers.Add(GameUserData);
			}
			ResUser.Free();
		}
		return UCString("no users");
	}
	// 商品管理接口实现
	UCString	GetAllStoreItems(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucINT Offset, ucINT Count, UCEArray<UCGameStoreData>& AryStores)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home = RObjManager_UserData.Find(GameUserID);

		if (RObjGameUserData_Home == ucNULL)
			return UCString("unvalid id");

		// Return data directly from memory
		ucINT StartIndex = Offset;
		ucINT EndIndex = StartIndex + Count;
		if (EndIndex > AryStoreData.GetSize())
			EndIndex = AryStoreData.GetSize();
		
		if (StartIndex < AryStoreData.GetSize())
		{
			for (ucINT i = StartIndex; i < EndIndex; i++)
			{
				AryStores.Add(AryStoreData[i]);
			}
		}
		
		return UCString("succeed");
	}
	UCString	UpdateStoreItem(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucCONST UCGameStoreData& StoreData)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home = RObjManager_UserData.Find(GameUserID);

		if (RObjGameUserData_Home == ucNULL)
			return UCString("unvalid id");

		if (RObjGameUserData_Home->Access_Level.Value < 1)
			return UCString("Permission denied");

		// Check if item exists in memory by ID
		ucINT ExistingIndex = -1;
		for (ucINT i = 0; i < AryStoreData.GetSize(); i++)
		{
			if (AryStoreData[i].ID == StoreData.ID && StoreData.ID > 0)
			{
				ExistingIndex = i;
				break;
			}
		}

		// Update database
		UCMySQLCommandBuffer MySQLCmdBuffer;
		if (ExistingIndex >= 0)
		{
			// Update existing item by ID
			MySQLCmdBuffer.SetCommand(UCString("products"));
			MySQLCmdBuffer.SetWhere(UCString("WHERE id = ") + ITOS(StoreData.ID));

			MySQLCmdBuffer.SetString(UCString("name"), StoreData.Name);
			MySQLCmdBuffer.SetString(UCString("description"), StoreData.Description);
			MySQLCmdBuffer.SetInt(UCString("type"), StoreData.Type);
			MySQLCmdBuffer.SetInt(UCString("price"), StoreData.Price);
			MySQLCmdBuffer.SetInt(UCString("res_id"), StoreData.ResID);

			ucINT result = MySQLCmdBuffer.Execute(MySQL);
			if (result == MYSQL_RET_SECCESS)
			{
				// Update memory
				AryStoreData[ExistingIndex] = StoreData;
				RContainer_NET.Log(UCString("Store item updated: ID=") + ITOS(StoreData.ID) + UCString(" Name=") + StoreData.Name + UCString("\r\n"));
				return UCString("succeed");
			}
			else
			{
				RContainer_NET.Log(UCString("Store item update error: ID=") + ITOS(StoreData.ID) + UCString(" ") + ITOS(result) + UCString("\r\n"));
				return UCString("failed");
			}
		}
		else
		{
			// Add new item
			MySQLCmdBuffer.SetCommand(UCString("products"), 1);

			MySQLCmdBuffer.SetString(UCString("name"), StoreData.Name);
			MySQLCmdBuffer.SetString(UCString("description"), StoreData.Description);
			MySQLCmdBuffer.SetInt(UCString("type"), StoreData.Type);
			MySQLCmdBuffer.SetInt(UCString("price"), StoreData.Price);
			MySQLCmdBuffer.SetInt(UCString("res_id"), StoreData.ResID);

			ucINT result = MySQLCmdBuffer.Execute(MySQL);
			if (result == MYSQL_RET_SECCESS)
			{
				// Get the auto-generated ID
				UCMySQLRes ResID;
				UCStringA CmdID = UCString("SELECT LAST_INSERT_ID()");
				ucINT RetID = MySQL.Query(CmdID, &ResID);
				if (RetID == MYSQL_RET_SECCESS && ResID.FetchRow() == 0)
				{
					ucINT NewID = ResID.GetRowAsInt(0);
					UCGameStoreData NewStoreData = StoreData;
					NewStoreData.ID = NewID;
					AryStoreData.Add(NewStoreData);
					ResID.Free();
					RContainer_NET.Log(UCString("Store item added: ID=") + ITOS(NewID) + UCString(" Name=") + StoreData.Name + UCString("\r\n"));
					return UCString("succeed");
				}
				else
				{
					ResID.Free();
					RContainer_NET.Log(UCString("Store item add error: Failed to get ID for ") + StoreData.Name + UCString("\r\n"));
					return UCString("failed");
				}
			}
			else
			{
				RContainer_NET.Log(UCString("Store item add error: ") + StoreData.Name + UCString(" ") + ITOS(result) + UCString("\r\n"));
				return UCString("failed");
			}
		}
	}
	UCString	DeleteStoreItem(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucCONST UCString& ItemName)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home = RObjManager_UserData.Find(GameUserID);

		if (RObjGameUserData_Home == ucNULL)
			return UCString("unvalid id");

		if (RObjGameUserData_Home->Access_Level.Value < 1)
			return UCString("Permission denied");

		// Find item by name to get ID
		ucINT ItemID = -1;
		for (ucINT i = 0; i < AryStoreData.GetSize(); i++)
		{
			if (AryStoreData[i].Name == ItemName)
			{
				ItemID = AryStoreData[i].ID;
				break;
			}
		}

		if (ItemID <= 0)
		{
			RContainer_NET.Log(UCString("Store item not found: ") + ItemName + UCString("\r\n"));
			return UCString("item not found");
		}

		// Delete from database by ID
		UCMySQLRes ResStore;
		UCStringA CmdStore = UCString("DELETE FROM products WHERE id = ") + ITOS(ItemID);
		ucINT Ret = MySQL.Query(CmdStore, &ResStore);
		if (Ret == MYSQL_RET_SECCESS) 
		{
			// Delete from memory
			for (ucINT i = 0; i < AryStoreData.GetSize(); i++)
			{
				if (AryStoreData[i].ID == ItemID)
				{
					AryStoreData.RemoveAt(i);
					break;
				}
			}
			
			RContainer_NET.Log(UCString("Store item deleted: ID=") + ITOS(ItemID) + UCString(" Name=") + ItemName + UCString("\r\n"));
			return UCString("succeed");
		}
		else
		{
			RContainer_NET.Log(UCString("Store item delete error: ID=") + ITOS(ItemID) + UCString(" ") + ITOS(Ret) + UCString("\r\n"));
			return UCString("failed");
		}
	}
	UCString	BuyStoreItem(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucINT ItemID)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home = RObjManager_UserData.Find(GameUserID);

		if (RObjGameUserData_Home == ucNULL)
			return UCString("unvalid id");

		// Find item by ID
		UCGameStoreData* ItemData = ucNULL;
		for (ucINT i = 0; i < AryStoreData.GetSize(); i++)
		{
			if (AryStoreData[i].ID == ItemID)
			{
				ItemData = &AryStoreData[i];
				break;
			}
		}

		if (ItemData == ucNULL)
		{
			RContainer_NET.Log(UCString("Store item not found: ID=") + ITOS(ItemID) + UCString("\r\n"));
			return UCString("item not found");
		}

		// Check if user has enough money
		if (RObjGameUserData_Home->VMoney.Value < ItemData->Price)
		{
			RContainer_NET.Log(UCString("Not enough money: User=") + ITOS(RObjGameUserData_Home->VMoney.Value) + UCString(" Required=") + ITOS(ItemData->Price) + UCString("\r\n"));
			return UCString("not enough money");
		}

		// Deduct money from user
		RObjGameUserData_Home->VMoney = RObjGameUserData_Home->VMoney.Value - ItemData->Price;

		// Add item to user's inventory
		RObjGameUserData_Home->AryItemData.Add(ItemID);

		// Update user data in database
		UCMySQLCommandBuffer MySQLCmdBuffer;
		MySQLCmdBuffer.SetCommand(UCString("accounts"));
		MySQLCmdBuffer.SetWhere(UCString("WHERE username = '") + MySQLCmdBuffer.MakeValid(RObjGameUserData_Home->Username.Value) + UCString("'"));

		MySQLCmdBuffer.SetInt(UCString("vmoney"), RObjGameUserData_Home->VMoney.Value);

		// Convert item array to string for database storage
		UCString ItemDataString = UCString("");
		for (ucINT i = 0; i < RObjGameUserData_Home->AryItemData.GetSize(); i++)
		{
			if (i > 0) ItemDataString += UCString(",");
			ItemDataString += ITOS(RObjGameUserData_Home->AryItemData[i]);
		}
		MySQLCmdBuffer.SetString(UCString("items"), ItemDataString);

		ucINT result = MySQLCmdBuffer.Execute(MySQL);
		if (result == MYSQL_RET_SECCESS)
		{
			RContainer_NET.Log(UCString("Item purchased: User=") + RObjGameUserData_Home->Username.Value + UCString(" Item=") + ItemData->Name + UCString(" ID=") + ITOS(ItemID) + UCString(" Price=") + ITOS(ItemData->Price) + UCString(" Items=") + ItemDataString + UCString("\r\n"));
			return UCString("succeed");
		}
		else
		{
			// Rollback money deduction if database update failed
			RObjGameUserData_Home->VMoney = RObjGameUserData_Home->VMoney.Value + ItemData->Price;
			RObjGameUserData_Home->AryItemData.RemoveAt(RObjGameUserData_Home->AryItemData.GetSize() - 1);
			
			RContainer_NET.Log(UCString("Purchase failed: Database error ") + ITOS(result) + UCString("\r\n"));
			return UCString("database error");
		}
	}
	UCString	ChangeAvatar(ucCONST UCGameUserID& GameUserID, ucUINT64 Token, ucCONST UCRProAvatarData& AvatarData)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home = RObjManager_UserData.Find(GameUserID);

		if (RObjGameUserData_Home == ucNULL)
			return UCString("unvalid id");

		// Assign avatar data members individually
		RObjGameUserData_Home->AvatarData.FaceID = AvatarData.FaceID;
		RObjGameUserData_Home->AvatarData.HairID = AvatarData.HairID;
		RObjGameUserData_Home->AvatarData.BodyID = AvatarData.BodyID;
		RObjGameUserData_Home->AvatarData.HandID = AvatarData.HandID;
		RObjGameUserData_Home->AvatarData.PantID = AvatarData.PantID;
		RObjGameUserData_Home->AvatarData.ShoeID = AvatarData.ShoeID;
		RObjGameUserData_Home->AvatarData.CarID = AvatarData.CarID;

		RObjGameUserData_Home->AvatarData.Commit();

		OnRStructTimeOut(RObjGameUserData_Home, ucNULL);
		return UCString("succeed");
	}
	// 服务器压力测试函数，CPU型，持续5分钟
	UCString StressTest()
	{
		UCFiberData* FiberData = GetRunFiberData();

		ucINT64 start = UCGetTickCount();
		ucINT64 duration = 6 * 60 * 1000; // 6分钟，单位毫秒
		ucINT64 dummy = 0;
		while (UCGetTickCount() - start < duration)
		{
			// CPU密集型运算
			for (ucINT i = 0; i < 2000; ++i)
				dummy += i * i;
			FiberData->Delay();
		}
		return UCString("stress test done");
	}
private:
	UCRObjGameAccount* Refresh_RObjGameAccount(ucDWORD GameAccountID)
	{
		if (GameAccountID < 0 || GameAccountID >= ACCOUNT_MAX)
			return 0;

		if (RObjGameAccount[GameAccountID].Linked)
			return &RObjGameAccount[GameAccountID];

		UCString strURL = UCString("sm://account:") + ITOS(GameAccountID) + UCString("//UCRObjGameAccount.obj");
		ucINT Code = RObjGameAccount[GameAccountID].Link(strURL);
		if (Code)
		{
			RContainer_NET.Log(UCString("连接RObjGameAccount失败:") + strURL + UCString(",Code=") + ITOS(Code) + UCString("\r\n"));
			return 0;
		}
		return &RObjGameAccount[GameAccountID];
	}
	UCRObjGameData* Refresh_RObjGameData(ucDWORD GameDataID)
	{
		if (GameDataID < 0 || GameDataID >= DATA_MAX)
			return 0;

		if (RObjGameData[GameDataID].Linked)
			return &RObjGameData[GameDataID];

		UCString strURL = UCString("sm://data:") + ITOS(GameDataID) + UCString("//UCRObjGameData.obj");
		ucINT Code = RObjGameData[GameDataID].Link(strURL);
		if (Code)
		{
			RContainer_NET.Log(UCString("连接RObjGameData失败:") + strURL + UCString(",Code=") + ITOS(Code) + UCString("\r\n"));
			return 0;
		}
		return &RObjGameData[GameDataID];
	}

	UCString RealLogin(ucCONST UCString& Username, ucCONST UCString& Password, ucINT Access_Level, UCGameUserID& GameUserID, ucUINT64& Token)
	{
		UCMySQLRes ResUser;

		UCStringA CmdUser = UCString("SELECT * FROM accounts WHERE username = '") + UCString(Username) + UCString("'");
		ucINT Ret = MySQL.Query(CmdUser, &ResUser);

		ucBOOL Succeed = ucFALSE;
		if (Ret == MYSQL_RET_SECCESS) {
			if (ResUser.FetchRow() == 0) {
				Succeed = ucTRUE;
			}
			ResUser.Free();
		}

		if (!Succeed)
		{
			UCMySQLCommandBuffer MySQLCmdBuffer;
			MySQLCmdBuffer.SetCommand(UCString("accounts"), 1);

			MySQLCmdBuffer.SetString(UCString("username"), Username);
			MySQLCmdBuffer.SetString(UCString("password"), Password);

			MySQLCmdBuffer.SetInt(UCString("token"), 0);

			MySQLCmdBuffer.SetInt(UCString("channel"), 0);
			MySQLCmdBuffer.SetInt(UCString("access_level"), Access_Level);

			MySQLCmdBuffer.SetString(UCString("nickname"), UCString());
			MySQLCmdBuffer.SetString(UCString("email"), UCString());

			MySQLCmdBuffer.SetInt(UCString("vmoney"), 100);

			MySQLCmdBuffer.SetString(UCString("items"), UCString());
			MySQLCmdBuffer.SetString(UCString("avatar"), UCString());

			ucINT result = MySQLCmdBuffer.Execute(MySQL);
			if (result == MYSQL_RET_SECCESS)
				RContainer_NET.Log(Username + UCString(" created\r\n"));
			else
				RContainer_NET.Log(Username + UCString(" create error ") + ITOS(result) + UCString("\r\n"));
		}

		Ret = MySQL.Query(CmdUser, &ResUser);
		if (Ret == MYSQL_RET_SECCESS) {
			if (ResUser.FetchRow() == 0) {
				ucINT nID = ResUser.GetRowAsInt(0);
				ucINT nToken = ResUser.GetRowAsInt(1);

				UCString strUsername = ResUser.GetRowAsString(2);
				// temporarily using plaintext as the password, just for testing functionality, later changed to encryption processing
				UCString strPassword = ResUser.GetRowAsString(3);

				if (Password != strPassword)
					return UCString("error password");

				ucINT nChannel = ResUser.GetRowAsInt(4);

				ucINT nAccess_Level = ResUser.GetRowAsInt(5);

				if (nAccess_Level < 0)
					return UCString("Permission denied");

				UCString strNickname = ResUser.GetRowAsString(6);
				UCString strEmail = ResUser.GetRowAsString(7);

				ucINT nVMoney = ResUser.GetRowAsInt(8);

				UCString strItems = ResUser.GetRowAsString(9);
				UCString strAvatar = ResUser.GetRowAsString(10);

				GameUserID.SetDataID(0);
				GameUserID.SetUserID(nID);

				UCRObjGameUserData_Home* RObjGameUserData_Home = RObjManager_UserData.Find(GameUserID);
				if (RObjGameUserData_Home)
					return UCString("succeed");

				RObjGameUserData_Home = RObjManager_UserData.Get(GameUserID);

				RObjGameUserData_Home->Username = Username;

				RObjGameUserData_Home->Token = nToken;
				RObjGameUserData_Home->Channel = nChannel;

				RObjGameUserData_Home->Access_Level = nAccess_Level;

				RObjGameUserData_Home->Nickname = strNickname;
				RObjGameUserData_Home->Email = strEmail;

				RObjGameUserData_Home->VMoney = nVMoney;

				// Load item data from string
				RObjGameUserData_Home->AryItemData.RemoveAll();
				if (!strItems.IsEmpty())
				{
					UCEArray<ucINT>	AryItems;
					StringToIntArray(strItems, AryItems);
					for (ucINT i = 0; i < AryItems.GetSize(); i++)
						RObjGameUserData_Home->AryItemData.Add(AryItems[i]);
				}

				RObjGameUserData_Home->AvatarData.FaceID = -1;
				RObjGameUserData_Home->AvatarData.HairID = -1;
				RObjGameUserData_Home->AvatarData.BodyID = -1;
				RObjGameUserData_Home->AvatarData.HandID = -1;
				RObjGameUserData_Home->AvatarData.PantID = -1;
				RObjGameUserData_Home->AvatarData.ShoeID = -1;
				RObjGameUserData_Home->AvatarData.CarID = -1;

				// Load avatar data from string
				if (!strAvatar.IsEmpty())
				{
					UCEArray<ucINT> AryAvatar;
					StringToIntArray(strAvatar, AryAvatar);
					if (AryAvatar.GetSize() >= 7)
					{
						RObjGameUserData_Home->AvatarData.FaceID = AryAvatar[0];
						RObjGameUserData_Home->AvatarData.HairID = AryAvatar[1];
						RObjGameUserData_Home->AvatarData.BodyID = AryAvatar[2];
						RObjGameUserData_Home->AvatarData.HandID = AryAvatar[3];
						RObjGameUserData_Home->AvatarData.PantID = AryAvatar[4];
						RObjGameUserData_Home->AvatarData.ShoeID = AryAvatar[5];
						RObjGameUserData_Home->AvatarData.CarID = AryAvatar[6];
					}
				}

				RObjGameUserData_Home->GameUserID = GameUserID;
				RObjGameUserData_Home->Password = Password;

				RObjGameUserData_Home->TimeOut = 60 * 10000;
				RObjGameUserData_Home->OnTimeOut = UCEvent(this, OnRStructTimeOut);

				Token = randint64();
				RObjGameUserData_Home->Token = Token;
				return UCString("succeed");
			}
		}

		return UCString("failed");
	}

	UCRObjGameUserData_Home* RealActive(ucCONST UCString& Username)
	{
		UCMySQLRes ResUser;

		UCStringA CmdUser = UCString("SELECT * FROM accounts WHERE username = '") + UCString(Username) + UCString("'");
		ucINT Ret = MySQL.Query(CmdUser, &ResUser);
		if (Ret == MYSQL_RET_SECCESS) {
			if (ResUser.FetchRow() == 0) {
				ucINT nID = ResUser.GetRowAsInt(0);
				ucINT nToken = ResUser.GetRowAsInt(1);

				UCString strUsername = ResUser.GetRowAsString(2);
				// temporarily using plaintext as the password, just for testing functionality, later changed to encryption processing
				UCString strPassword = ResUser.GetRowAsString(3);

				ucINT nChannel = ResUser.GetRowAsInt(4);

				ucINT nAccess_Level = ResUser.GetRowAsInt(5);

				UCString strNickname = ResUser.GetRowAsString(6);
				UCString strEmail = ResUser.GetRowAsString(7);

				ucINT nVMoney = ResUser.GetRowAsInt(8);

				UCString strItems = ResUser.GetRowAsString(9);
				UCString strAvatar = ResUser.GetRowAsString(10);

				UCGameUserID GameUserID;
				GameUserID.SetDataID(0);
				GameUserID.SetUserID(nID);

				UCRObjGameUserData_Home* RObjGameUserData_Home = RObjManager_UserData.Get(GameUserID);

				RObjGameUserData_Home->Username = Username;

				RObjGameUserData_Home->Token = nToken;
				RObjGameUserData_Home->Channel = nChannel;

				RObjGameUserData_Home->Access_Level = nAccess_Level;

				RObjGameUserData_Home->Nickname = strNickname;
				RObjGameUserData_Home->Email = strEmail;

				RObjGameUserData_Home->VMoney = nVMoney;

				// Load item data from string
				RObjGameUserData_Home->AryItemData.RemoveAll();
				if (!strItems.IsEmpty())
				{
					UCEArray<ucINT>	AryItems;
					StringToIntArray(strItems, AryItems);
					for (ucINT i = 0; i < AryItems.GetSize(); i++)
						RObjGameUserData_Home->AryItemData.Add(AryItems[i]);
				}

				// Load avatar data from string
				if (!strAvatar.IsEmpty())
				{
					UCEArray<ucINT> AryAvatar;
					StringToIntArray(strAvatar, AryAvatar);
					if (AryAvatar.GetSize() >= 7)
					{
						RObjGameUserData_Home->AvatarData.FaceID = AryAvatar[0];
						RObjGameUserData_Home->AvatarData.HairID = AryAvatar[1];
						RObjGameUserData_Home->AvatarData.BodyID = AryAvatar[2];
						RObjGameUserData_Home->AvatarData.HandID = AryAvatar[3];
						RObjGameUserData_Home->AvatarData.PantID = AryAvatar[4];
						RObjGameUserData_Home->AvatarData.ShoeID = AryAvatar[5];
						RObjGameUserData_Home->AvatarData.CarID = AryAvatar[6];
					}
				}

				RObjGameUserData_Home->GameUserID = GameUserID;
				RObjGameUserData_Home->Password = strPassword;

				RObjGameUserData_Home->TimeOut = 60 * 10000;
				RObjGameUserData_Home->OnTimeOut = UCEvent(this, OnRStructTimeOut);

				return RObjGameUserData_Home;
			}
		}

		return ucNULL;
	}
public:
	UCRObjGameHomeLocal()
	{
		App = UCGetApp();
		if (App->Name.IsEmpty())
			App->Name = UCString("home");

		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));

		RObjManager_UserData.LocalUrl = LocalUrl;
		RObjManager_UserData.RContainer = &RContainer_NET;

		FiberInit.FiberEvent = UCEvent(this, OnFiber_Init);
		RContainer_NET.OnEnd = UCEvent(this, OnRContainer_End);
	}
	ucVOID OnRContainer_End(UCObject* Object, UCEventArgs*)
	{
	}
	ucVOID OnRStructTimeOut(UCObject* Object, UCEventArgs*)
	{
		UCRObjGameUserData_Home* RObjGameUserData_Home = (UCRObjGameUserData_Home*)Object;

		UCMySQLCommandBuffer MySQLCmdBuffer;
		MySQLCmdBuffer.SetCommand(UCString("accounts"));
		MySQLCmdBuffer.SetWhere(UCString("WHERE username = '") + MySQLCmdBuffer.MakeValid(RObjGameUserData_Home->Username.Value) + UCString("'"));

		MySQLCmdBuffer.SetString(UCString("password"), RObjGameUserData_Home->Password);

		MySQLCmdBuffer.SetInt(UCString("access_level"), RObjGameUserData_Home->Access_Level.Value);
		MySQLCmdBuffer.SetString(UCString("nickname"), RObjGameUserData_Home->Nickname.Value);
		MySQLCmdBuffer.SetString(UCString("email"), RObjGameUserData_Home->Email.Value);
		MySQLCmdBuffer.SetInt(UCString("vmoney"), RObjGameUserData_Home->VMoney.Value);

		// Convert item array to string for database storage
		UCString ItemString;
		for (ucINT i = 0; i < RObjGameUserData_Home->AryItemData.GetSize(); i++)
		{
			if (i > 0)
				ItemString += UCString(",");
			ItemString += ITOS(RObjGameUserData_Home->AryItemData[i]);
		}
		MySQLCmdBuffer.SetString(UCString("items"), ItemString);

		// Convert avatar data to string for database storage
		UCString AvatarString = ITOS(RObjGameUserData_Home->AvatarData.FaceID) + UCString(",") +
							   ITOS(RObjGameUserData_Home->AvatarData.HairID) + UCString(",") +
							   ITOS(RObjGameUserData_Home->AvatarData.BodyID) + UCString(",") +
							   ITOS(RObjGameUserData_Home->AvatarData.HandID) + UCString(",") +
							   ITOS(RObjGameUserData_Home->AvatarData.PantID) + UCString(",") +
							   ITOS(RObjGameUserData_Home->AvatarData.ShoeID) + UCString(",") +
							   ITOS(RObjGameUserData_Home->AvatarData.CarID);
		MySQLCmdBuffer.SetString(UCString("avatar"), AvatarString);

		ucINT result = MySQLCmdBuffer.Execute(MySQL);
		if (result == MYSQL_RET_SECCESS) 
			RContainer_NET.Log(RObjGameUserData_Home->GameUserID.ToString() + UCString("changed\r\n"));
		else
			RContainer_NET.Log(RObjGameUserData_Home->GameUserID.ToString() + UCString("write error ") + ITOS(result) + UCString("\r\n"));
	}
	ucVOID OnFiber_Init(UCObject* Object, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Object;

		if (App->Index < 0)
			RContainer_NET.Log(UCString("start:") + App->Name + UCString("\r\n"));
		else
			RContainer_NET.Log(UCString("start:") + App->Name + ITOS(App->Index, UCString(":%d")) + UCString("\r\n"));

		UCGameHomeInfo* HomeInfo = GameHomeCfg.GetData();
		ucINT AppIndex = App->Index;
		if (AppIndex < 0)
			AppIndex = 0;

		UCString LocalUrl = UCString("sm://") + App->Name;
		if (App->Index != -1)
			LocalUrl += ITOS(App->Index, UCString(":%d"));

		RContainer_NET.AppendSource(LocalUrl + UCString("//UCRObjGameHome.obj"), typeof(UCRObjGameHome), typeof(UCRObjGameHomeLocal), this, ucTRUE, ucFALSE);

		RContainer_NET.AppendClient(LocalUrl + UCString("//UCRObjGameCenter.obj"), typeof(UCRObjGameCenter), &RObjGameCenter);

		for (ucINT i = 0; i < DATA_MAX; ++i)
			RContainer_NET.AppendClient(LocalUrl + UCString("//UCRObjGameData/") + ITOS(i) + UCString(".obj"), typeof(UCRObjGameData), &RObjGameData[i]);

		for (ucINT i = 0; i < ACCOUNT_MAX; ++i)
			RContainer_NET.AppendClient(LocalUrl + UCString("//UCRObjGameAccount/") + ITOS(i) + UCString(".obj"), typeof(UCRObjGameAccount), &RObjGameAccount[i]);

		RContainer_NET.AppendClient(LocalUrl + UCString("//UCRObjGameMonitor.obj"), typeof(UCRObjGameMonitor), &RObjGameMonitor);

		// 		RContainer_NET.Log(UCString("开始连接监控容器...\r\n"));
		// 		while (FiberData->IsValid())
		// 		{
		// 			ucINT Code = RObjGameMonitor.Link(UCString("sm://monitor:0//UCRObjGameMonitor.obj"), DEFAULT_PASSWORD);
		// 			if (Code == 0)
		// 				break;
		// 			//RContainer_NET.Log(UCString("监控容器连接失败") + ITOS(Code) + UCString("\r\n"));
		// 			FiberData->Every(1000);
		// 		}
		// 		RContainer_NET.Log(UCString("监控连接succeed\r\n"));
		// 
		// 		RObjGameMonitor.Gate_Register(Index, Monitor_Key, Monitor_PCU_MaxTotal, Monitor_PCU_MaxChannel, Monitor_PCU_Current);
		// 		if (Index < 0 || Monitor_PCU_MaxTotal < 0)
		// 			RContainer_NET.Log(UCString("注册监控容器失败\r\n"));
		// 		else
		// 			RContainer_NET.Log(UCString("注册监控容器succeed\r\n"));

		RContainer_NET.Log(UCString("开始连接中心容器...\r\n"));
		while (FiberData->IsValid())
		{
			ucINT Code = RObjGameCenter.Link(UCString("sm://center:0//UCRObjGameCenter.obj"));
			if (Code == 0)
				break;
			RContainer_NET.Log(UCString("中心容器连接失败") + ITOS(Code) + UCString("\r\n"));
			FiberData->Every(10000);
		}
		RContainer_NET.Log(UCString("中心容器连接succeed\r\n"));
		UCString Result = RObjGameCenter.GameHome_Update(App->Index, HomeInfo->Password, AdminKey, RandKey);
		if (Result != "succeed")
		{
			RContainer_NET.Log(UCString("中心容器验证失败，请检查配置后重启容器 : ") + Result + UCString("\r\n"));
			return;
		}

		RContainer_NET.Log(UCString("验证succeed\r\n"));

		RContainer_NET.SetAdminKey(AdminKey);

		RContainer_NET.Log(UCString("init mysql") + UCString("\r\n"));

		MySQL.Init(UCString("kartdb.mysql.database.azure.com"), UCString("LargeWC"), UCString("LargeWC@123456"), UCString("kart"));

		UCMySQLRes ResUser;
		UCStringA CmdUser = UCString("SELECT * FROM accounts WHERE username = '") + UCString("root") + UCString("'");
		ucINT Ret = MySQL.Query(CmdUser, &ResUser);

		ucBOOL Succeed = ucFALSE;
		if (Ret == MYSQL_RET_SECCESS) {
			if (ResUser.FetchRow() == 0) {
				Succeed = ucTRUE;
			}
			ResUser.Free();
		}

		if (!Succeed)
		{
			UCGameUserID GameUserID;
			ucUINT64 Token;
			RealLogin(UCString("root"), UCString("root"), 2, GameUserID, Token);
		}

		// Load store data from database to memory
		RContainer_NET.Log(UCString("Loading store data from database...\r\n"));
		UCMySQLRes ResStore;
		UCStringA CmdStore = UCString("SELECT * FROM products");
		Ret = MySQL.Query(CmdStore, &ResStore, 1);
		if (Ret == MYSQL_RET_SECCESS) 
		{
			ucINT ItemCount = 0;
			while (ResStore.FetchRow() == 0) 
			{
				UCGameStoreData StoreData;

				StoreData.ID = ResStore.GetRowAsInt(0);
				StoreData.Name = ResStore.GetRowAsString(1);
				StoreData.Description = ResStore.GetRowAsString(2);
				StoreData.Type = ResStore.GetRowAsInt(3);
				StoreData.Price = ResStore.GetRowAsInt(4);
				StoreData.ResID = ResStore.GetRowAsInt(5);

				AryStoreData.Add(StoreData);
				ItemCount++;
			}
			ResStore.Free();
			RContainer_NET.Log(UCString("Store data loaded successfully: ") + ITOS(ItemCount) + UCString(" items\r\n"));
		}
		else
		{
			RContainer_NET.Log(UCString("Failed to load store data from database\r\n"));
		}

		RContainer_NET.Log(UCString("successfully initialized\r\n"));
	}
	ucINT Init()
	{		
		UCFileExt_GetStatic().CreateFullPath(UCString("home"));
		if (!GameHomeCfg.LoadFromXML(UCString("home/home.xml")))
		{
			UCGameHomeInfo* HomeInfo = GameHomeCfg.GetData();
			HomeInfo->Password = "123456";
			GameHomeCfg.SaveToXML(UCString("home/home.xml"));
		}
		UCGameHomeInfo* HomeInfo = GameHomeCfg.GetData();

		UCGetSystemExt()->SetWindowCaption(App->Name);

		SetExceptionMode(0);

		FiberInit.Start(0);
		return 1;
	}
	ucVOID Run()
	{
		if (!Init())
			return;
		RContainer_NET.Run(App->Name, App->Index, ucFALSE, ucTRUE, 10);
	}
};

#endif	//_gamehome_H_