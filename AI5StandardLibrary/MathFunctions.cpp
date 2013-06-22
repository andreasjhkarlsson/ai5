#include "MathFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\FloatingVariant.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include "..\AI5Runtime\Integer64Variant.h"
#include <cmath>
#include <functional>
#include <memory>
using namespace std::placeholders;

MathFunctions::MathFunctions(void)
{
}


MathFunctions::~MathFunctions(void)
{
}



Variant* MathFunctions::absolute(Variant** args,int argsSize)
{
	Variant* arg = args[0];

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

Variant* MathFunctions::acosine(Variant** args,int argsSize)
{
	return new FloatingVariant(acos(args[0]->toFloating()));
}
Variant* MathFunctions::asine(Variant** args,int argsSize)
{
	return new FloatingVariant(asin(args[0]->toFloating()));
}
Variant* MathFunctions::atangent(Variant** args,int argsSize)
{
	return new FloatingVariant(atan(args[0]->toFloating()));
}
Variant* MathFunctions::cosine(Variant** args,int argsSize)
{
	return new FloatingVariant(cos(args[0]->toFloating()));
}
Variant* MathFunctions::ceiling(Variant** args,int argsSize)
{
	return new Integer32Variant((int)(args[0]->toFloating()+1));
}
Variant* MathFunctions::exponent(Variant** args,int argsSize)
{
	return new FloatingVariant(exp(args[0]->toFloating()));
}
Variant* MathFunctions::floor(Variant** args,int argsSize)
{
	return new Integer32Variant((int)args[0]->toFloating());
}
Variant* MathFunctions::logarithm(Variant** args,int argsSize)
{
	return new FloatingVariant(log(args[0]->toFloating()));
}
Variant* MathFunctions::modulus(Variant** args,int argsSize)
{
	Variant* arg1 = args[0];
	Variant* arg2 = args[1];

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


Variant* MathFunctions::_round(Variant** args,int argsSize)
{
	
	if(argsSize > 1)
	{
		int decimals = args[1]->toInteger32();

		double operand = args[0]->toFloating();
		
		operand *= pow(10,decimals);
		operand = ((int)(operand+0.5)) / pow(10,decimals);
		return new FloatingVariant(operand);
	}
	else
	{
		return new Integer32Variant((int)(args[0]->toFloating()+0.5));
	}
}
Variant* MathFunctions::sine(Variant** args,int argsSize)
{
	return new FloatingVariant(sin(args[0]->toFloating()));
}
Variant* MathFunctions::_sqrt(Variant** args,int argsSize)
{
	return new FloatingVariant(sqrt(args[0]->toFloating()));
}


Variant* MathFunctions::tangent(Variant** args,int argsSize)
{
		return new FloatingVariant(tan(args[0]->toFloating()));
}


void MathFunctions::registerFunctions(StackMachine* machine)
{
	
	std::shared_ptr<MathFunctions> instance(new MathFunctions);

	machine->addBuiltInFunction(L"abs",std::bind(&absolute,instance,_1,_2));
	machine->addBuiltInFunction(L"acos",std::bind(&acosine,instance,_1,_2));
	machine->addBuiltInFunction(L"asin",std::bind(&asine,instance,_1,_2));
	machine->addBuiltInFunction(L"atan",std::bind(&atangent,instance,_1,_2));
	machine->addBuiltInFunction(L"cos",std::bind(&cosine,instance,_1,_2));
	machine->addBuiltInFunction(L"ceiling",std::bind(&ceiling,instance,_1,_2));
	machine->addBuiltInFunction(L"exp",std::bind(&exponent,instance,_1,_2));
	machine->addBuiltInFunction(L"floor",std::bind(&floor,instance,_1,_2));
	machine->addBuiltInFunction(L"log",std::bind(&logarithm,instance,_1,_2));
	machine->addBuiltInFunction(L"mod",std::bind(&modulus,instance,_1,_2));
	//machine->addBuiltInFunction(L"random",random);
	machine->addBuiltInFunction(L"round",std::bind(&_round,instance,_1,_2));
	machine->addBuiltInFunction(L"sin",std::bind(&sine,instance,_1,_2));
	machine->addBuiltInFunction(L"sqrt",std::bind(&_sqrt,instance,_1,_2));
	//machine->addBuiltInFunction(L"srandom",srandom);
	machine->addBuiltInFunction(L"tan",std::bind(&tangent,instance,_1,_2));
}