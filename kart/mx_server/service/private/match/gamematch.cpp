#include "gamematch.h"

void main()
{
	CGameMatchService* pService = new CGameMatchService;
	pService->Run();
	delete pService;
}
