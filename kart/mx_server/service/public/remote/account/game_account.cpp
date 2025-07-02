#include "game_account.h"

void main()
{
	CGame_AccountService* pService = new CGame_AccountService;
	pService->Run();
	delete pService;
}
