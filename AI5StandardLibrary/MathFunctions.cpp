#include "MathFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\FloatingVariant.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include "..\AI5Runtime\Integer64Variant.h"
#include "..\AI5Runtime\CallInfo.h"
#include <cmath>
#include <functional>
#include <memory>
#include <ctime>
using namespace std::placeholders;

MathFunctions::MathFunctions(void)
{
	rng.seed(time(NULL));
}


MathFunctions::~MathFunctions(void)
{
}



Variant* MathFunctions::absolute(CallInfo* callInfo)
{

	callInfo->validateArgCount(1,1);
	Variant* arg = callInfo->getArg(0);

	if(arg->isInteger32Type())
	{
		return new Integer32Variant(abs(arg->toInteger32()));
	} else if(arg->isInteger64Type())
	{
		return new Integer64Variant(abs(arg->toInteger64()));
	} else
	{
		return new FloatingVariant(abs(arg->toFloating()));
	}

}

Variant* MathFunctions::acosine(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new FloatingVariant(acos(callInfo->getFloatingArg(0)));
}
Variant* MathFunctions::asine(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new FloatingVariant(asin(callInfo->getFloatingArg(0)));
}
Variant* MathFunctions::atangent(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new FloatingVariant(atan(callInfo->getFloatingArg(0)));
}
Variant* MathFunctions::cosine(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new FloatingVariant(cos(callInfo->getFloatingArg(0)));
}
Variant* MathFunctions::ceiling(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new Integer32Variant((int)(callInfo->getFloatingArg(0)+1));
}
Variant* MathFunctions::exponent(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new FloatingVariant(exp(callInfo->getFloatingArg(0)));
}
Variant* MathFunctions::floor(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new Integer32Variant((int)callInfo->getFloatingArg(0));
}
Variant* MathFunctions::logarithm(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new FloatingVariant(log(callInfo->getFloatingArg(0)));
}
Variant* MathFunctions::modulus(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,2);
	Variant* arg1 = callInfo->getArg(0);
	Variant* arg2 = callInfo->getArg(1);

	if(arg1->isIntegerType() && arg2->isIntegerType())
	{
		if(arg1->isInteger32Type() && arg2->isInteger32Type())
		{
			return new Integer32Variant(arg1->toInteger32() % arg2->toInteger32());	
		}
		else
		{
			return new Integer64Variant(arg1->toInteger64() % arg2->toInteger64());	
		}
	}
	else
	{
		return new FloatingVariant(fmod(arg1->toFloating(),arg2->toFloating()));	
	}
}


Variant* MathFunctions::_round(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,2);
	if(callInfo->getArgCount() > 1)
	{
		int decimals = callInfo->getInt32Arg(1);

		double operand = callInfo->getFloatingArg(0);
		
		operand *= pow(10,decimals);
		operand = ((int)(operand+0.5)) / pow(10,decimals);
		return new FloatingVariant(operand);
	}
	else
	{
		return new Integer32Variant((int)(callInfo->getFloatingArg(0)+0.5));
	}
}
Variant* MathFunctions::sine(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new FloatingVariant(sin(callInfo->getFloatingArg(0)));
}
Variant* MathFunctions::_sqrt(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new FloatingVariant(sqrt(callInfo->getFloatingArg(0)));
}


Variant* MathFunctions::tangent(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
		return new FloatingVariant(tan(callInfo->getFloatingArg(0)));
}

Variant* MathFunctions::random(CallInfo* callInfo)
{
	callInfo->validateArgCount(0,3);
	double min=0.0,max = 1.0;
	bool floating = true;

	if(callInfo->getArgCount() >= 1)
		min = callInfo->getFloatingArg(0);
	if(callInfo->getArgCount() >= 2)
		max = callInfo->getFloatingArg(1);
	if(callInfo->getArgCount() >= 3)
		floating = !callInfo->getBoolArg(2);

	// This is weird, but the logic is that
	// if there are only one argument, that argument is intepreted as max.
	if(callInfo->getArgCount() == 1)
	{
		max = min;
		min = 0;
	}

	Variant* result = nullptr;

	if(floating)
	{
		result = new FloatingVariant(std::uniform_real_distribution<>(min,max)(rng));
	}
	else
	{
		result = new Integer32Variant(std::uniform_int_distribution<>((int)min,(int)max)(rng));
	}

	return result;
}

Variant* MathFunctions::srandom(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);

	rng.seed(callInfo->getInt32Arg(0));

	return nullptr;
}

void MathFunctions::registerFunctions(StackMachine* machine)
{
	
	std::shared_ptr<MathFunctions> instance(new MathFunctions);

	machine->addBuiltInFunction(L"abs",std::bind(&absolute,instance,_1));
	machine->addBuiltInFunction(L"acos",std::bind(&acosine,instance,_1));
	machine->addBuiltInFunction(L"asin",std::bind(&asine,instance,_1));
	machine->addBuiltInFunction(L"atan",std::bind(&atangent,instance,_1));
	machine->addBuiltInFunction(L"cos",std::bind(&cosine,instance,_1));
	machine->addBuiltInFunction(L"ceiling",std::bind(&ceiling,instance,_1));
	machine->addBuiltInFunction(L"exp",std::bind(&exponent,instance,_1));
	machine->addBuiltInFunction(L"floor",std::bind(&floor,instance,_1));
	machine->addBuiltInFunction(L"log",std::bind(&logarithm,instance,_1));
	machine->addBuiltInFunction(L"mod",std::bind(&modulus,instance,_1));
	machine->addBuiltInFunction(L"random",std::bind(&random,instance,_1));
	machine->addBuiltInFunction(L"round",std::bind(&_round,instance,_1));
	machine->addBuiltInFunction(L"sin",std::bind(&sine,instance,_1));
	machine->addBuiltInFunction(L"sqrt",std::bind(&_sqrt,instance,_1));
	machine->addBuiltInFunction(L"srandom",std::bind(&srandom,instance,_1));
	machine->addBuiltInFunction(L"tan",std::bind(&tangent,instance,_1));
}