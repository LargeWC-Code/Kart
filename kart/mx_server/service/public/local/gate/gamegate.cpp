#include "gamegate.h"

void main()
{
	CGateService* pService = new CGateService;
	pService->Run();
	delete pService;
}
