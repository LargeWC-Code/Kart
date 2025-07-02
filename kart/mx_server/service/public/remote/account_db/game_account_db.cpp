#include "game_account_db.h"

void main()
{
	CGame_AccountDBService* pService = new CGame_AccountDBService;
	pService->Run();
	delete pService;
}
