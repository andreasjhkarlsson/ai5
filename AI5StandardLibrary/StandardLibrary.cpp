#include "..\AI5Runtime\StackMachine.h"
#include "StandardLibrary.h"
#include "functions.h"
#include "math.h"

namespace AI5StandardLibrary
{
	void registerFunctions(StackMachine* machine)
	{
		machine->addBuiltInFunction(L"hello",hello);
		machine->addBuiltInFunction(L"sleep",sleep);
		machine->addBuiltInFunction(L"printline",printline);
		machine->addBuiltInFunction(L"getint",getint);
		machine->addBuiltInFunction(L"stringlen",stringlen);
		machine->addBuiltInFunction(L"ubound",ubound);
		machine->addBuiltInFunction(L"arrayadd",arrayadd);

		/* Math functions */
		machine->addBuiltInFunction(L"abs",absolute);
		machine->addBuiltInFunction(L"acos",acosine);
		machine->addBuiltInFunction(L"asin",asine);
		machine->addBuiltInFunction(L"atan",atangent);
		machine->addBuiltInFunction(L"cos",cosine);
		machine->addBuiltInFunction(L"ceiling",ceiling);
		machine->addBuiltInFunction(L"exp",exponent);
		machine->addBuiltInFunction(L"floor",floor);
		machine->addBuiltInFunction(L"log",logarithm);
		machine->addBuiltInFunction(L"mod",modulus);
		//machine->addBuiltInFunction(L"random",random);
		machine->addBuiltInFunction(L"round",_round);
		machine->addBuiltInFunction(L"sin",sine);
		machine->addBuiltInFunction(L"sqrt",_sqrt);
		//machine->addBuiltInFunction(L"srandom",srandom);
		machine->addBuiltInFunction(L"tan",tangent);


		machine->addMacro(L"autoitpid",Macros::MyPID);
		machine->addMacro(L"stacksize",Macros::StackSize);
	}
}