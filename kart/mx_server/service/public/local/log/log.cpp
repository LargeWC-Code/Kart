#include "log.h"

void main()
{
	CLogService* pService = new CLogService;
	pService->Run();
	delete pService;
}
