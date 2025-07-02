/*
	file : public_base
	author : LargeWC
	created : 2020/04/14
*/

#ifndef _public_base_
#define _public_base_

//MagicX默认渠道
#define CANAL_MAGICX	0
#define CANAL_MAX		1

struct CUserGameID
{
	ucDWORD				DBIndex;		//隶属的平台DB ID
	ucDWORD				Index;			//自身ID

	CUserGameID() {}
	CUserGameID(const CUserGameID& in)
	{
		DBIndex = in.DBIndex;
		Index = in.Index;
	}
	CUserGameID(ucINT64 ID64)
	{
		DBIndex = ID64 >> 32;
		Index = ID64;
	}
	void Set(const CUserGameID& in)
	{
		DBIndex = in.DBIndex;
		Index = in.Index;
	}
	void Set(ucINT64 ID64)
	{
		DBIndex = ID64 >> 32;
		Index = ID64;
	}
	ucINT64	ToInt64() const
	{
		return ucINT64(DBIndex) << 32 | ucINT64(Index);
	}
	UCString ToString() const
	{
		return ITOS(DBIndex, UCString("%08x:")) + ITOS(Index, UCString("%08x"));
	}
};

#endif