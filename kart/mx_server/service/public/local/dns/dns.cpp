#include "dns.h"

void main()
{
	CDnsService* pService = new CDnsService;
	pService->Run();
	delete pService;
}
