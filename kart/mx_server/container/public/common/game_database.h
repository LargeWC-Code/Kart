#ifndef _game_database_
#define _game_database_

class UCRObjGameData : public UCRObject
{
public:
	UCRObjGameData()
	{
	}
	// 从缓存中激活数据
	UCString Active(ucCONST UCString& RObjectUrl, ucINT64& PassKey)
	{
		return UCString("local");
	}
	// 把内存送到远程镜像
	UCString Mirror(ucCONST UCString& RObjectUrl, ucCONST UCRMessage& RMessage, ucINT64& PassKey)
	{
		return UCString("local");
	}
};

#endif