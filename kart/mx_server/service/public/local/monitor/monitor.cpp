#include "monitor.h"

void main()
{
	CMonitorService* pService = new CMonitorService;
	pService->Run();
	delete pService;
}
