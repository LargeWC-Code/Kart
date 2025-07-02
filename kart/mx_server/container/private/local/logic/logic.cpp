#include "logic.h"

void main()
{
	UCRObjGameLogicLocal* RObjGameLogicLocal = new UCRObjGameLogicLocal;
	RObjGameLogicLocal->Run();
	delete RObjGameLogicLocal;
}
