/*
	file : hall.robj
	author : LargeWC
	created : 2020/04/16
*/

#ifndef _bi_robj_
#define _bi_robj_

class CGame_BI_RCObj : public UCVF_RemoteCallObj
{
public:
	void Login(ucINT64 userid, int roleid, int guid, UCString IP, int gameid, int sourceid)
	{
	}
	void NewReg(ucINT64 userid, int roleid, int guid, UCString UserName, int sourceid, UCString IEMI)
	{
	}
	void LoginOut(ucINT64 userid, int roleid, int guid, int gameid, int sourceid, int logintime)
	{
	}
	void Money(ucINT64 userid, int roleid, int guid, int reason, int itemid, int type, int MType, int Realneed, int lefts, int source)
	{
	}
	void ShopItem(ucINT64 userid, int roleid, int guid, UCString UserName, int itemid, int itemtype, int buytype, int Realneed, int lefts, int source)
	{
	}

};

#endif