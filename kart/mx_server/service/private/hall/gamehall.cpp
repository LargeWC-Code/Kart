#include "GameHall.h"
void main()
{
	CGameHallService* pService = new CGameHallService;
	pService->Run();
	delete pService;
}
