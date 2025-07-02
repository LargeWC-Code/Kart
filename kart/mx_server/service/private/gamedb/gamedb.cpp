#include "gamedb.h"

void main()
{
	CGameDBService* pService = new CGameDBService;
	pService->Run();
	delete pService;
}
