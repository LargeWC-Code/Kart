#ifndef _game_matchbase_
#define _game_matchbase_

#include "game_logicbase.h"

#define	MATCH_TIME_MIN	10

class UCRObjGameMatch : public UCRObject
{
public:
	// 匹配计数器，会一直累加
	UCRInt		TotalTick;
	// 正在匹配的数量
	UCRInt		Count;

	UCRInt		CancelTick;
public:
	UCString	Match(ucCONST UCGameUserID& GameUserID, ucUINT64 Token)
	{
		return UCString("local");
	}
	UCString	StopMatch(ucCONST UCGameUserID& GameUserID, ucUINT64 Token)
	{
		return UCString("local");
	}
};

#endif