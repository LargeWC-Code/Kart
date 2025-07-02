#include "game_center.h"

void main()
{
	CCenterService* pService = new CCenterService;
	pService->Run();
	delete pService;
}
