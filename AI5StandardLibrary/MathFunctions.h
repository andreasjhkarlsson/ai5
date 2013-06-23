#pragma once
#include "functions.h"
#include <random>
class Variant;
class StackMachine;
class MathFunctions :
	public Functions
{
public:
	static void registerFunctions(StackMachine* machine);
	~MathFunctions(void);
private:
	MathFunctions(void);
	Variant* absolute(Variant** args,int argsSize);
	Variant* acosine(Variant** args,int argsSize);
	Variant* asine(Variant** args,int argsSize);
	Variant* atangent(Variant** args,int argsSize);
	Variant* cosine(Variant** args,int argsSize);
	Variant* ceiling(Variant** args,int argsSize);
	Variant* exponent(Variant** args,int argsSize);
	Variant* floor(Variant** args,int argsSize);
	Variant* logarithm(Variant** args,int argsSize);
	Variant* modulus(Variant** args,int argsSize);
	Variant* random(Variant** args,int argsSize);
	Variant* _round(Variant** args,int argsSize);
	Variant* sine(Variant** args,int argsSize);
	Variant* _sqrt(Variant** args,int argsSize);
	Variant* srandom(Variant** args,int argsSize);
	Variant* tangent(Variant** args,int argsSize);
	std::mt19937 rng;
};

