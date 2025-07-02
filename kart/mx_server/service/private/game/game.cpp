#include "game.h"
CGameData sGameData;
void main()
{
	CGameService* pService = new CGameService;
	pService->Run();
	delete pService;
}
