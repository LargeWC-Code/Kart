#ifndef _game_gatebase_
#define _game_gatebase_

#include "game_userbase.h"

#define GATEMODE_MIRROR			0
#define GATEMODE_ROUTE			1

#define GATETYPE_TCP			0
#define GATETYPE_WS				1
#define GATETYPE_WSS			2

class UCRObjGameGate : public UCRObject
{
public:
	UCString	GetAccountID(ucINT& AccountID)
	{
		AccountID = -1;
		return UCString("local");
	}
	UCString	GetHomeID(ucINT& HomeID)
	{
		HomeID = -1;
		return UCString("local");
	}
	UCString	GetMatchID(ucINT& MatchID)
	{
		MatchID = -1;
		return UCString("local");
	}
};

#endif