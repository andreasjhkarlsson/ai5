#include "math_functions.h"
#include "..\AI5Runtime\FloatingVariant.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include "..\AI5Runtime\Integer64Variant.h"
#include <cmath>
namespace AI5StandardLibrary
{

Variant* absolute(Variant** args,int argsSize)
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

Variant* acosine(Variant** args,int argsSize)
{
	return new FloatingVariant(acos(args[0]->toFloating()));
}
Variant* asine(Variant** args,int argsSize)
{
	return new FloatingVariant(asin(args[0]->toFloating()));
}
Variant* atangent(Variant** args,int argsSize)
{
	return new FloatingVariant(atan(args[0]->toFloating()));
}
Variant* cosine(Variant** args,int argsSize)
{
	return new FloatingVariant(cos(args[0]->toFloating()));
}
Variant* ceiling(Variant** args,int argsSize)
{
	return new Integer32Variant((int)(args[0]->toFloating()+1));
}
Variant* exponent(Variant** args,int argsSize)
{
	return new FloatingVariant(exp(args[0]->toFloating()));
}
Variant* floor(Variant** args,int argsSize)
{
	return new Integer32Variant((int)args[0]->toFloating());
}
Variant* logarithm(Variant** args,int argsSize)
{
	return new FloatingVariant(log(args[0]->toFloating()));
}
Variant* modulus(Variant** args,int argsSize)
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
Variant* random(Variant** args,int argsSize);
Variant* _round(Variant** args,int argsSize)
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
Variant* sine(Variant** args,int argsSize)
{
	return new FloatingVariant(sin(args[0]->toFloating()));
}
Variant* _sqrt(Variant** args,int argsSize)
{
	return new FloatingVariant(sqrt(args[0]->toFloating()));
}
Variant* srandom(Variant** args,int argsSize);
Variant* tangent(Variant** args,int argsSize)
{
		return new FloatingVariant(tan(args[0]->toFloating()));
}

}