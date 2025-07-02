#include "gate.h"

ucVOID main()
{
	UCRObjGameGateLocal* RObjGameGateLocal = new UCRObjGameGateLocal;
	RObjGameGateLocal->Run();
	delete RObjGameGateLocal;
}
