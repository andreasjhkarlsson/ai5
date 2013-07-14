#pragma once
#include <random>
class Variant;
class StackMachineThread;
class CallInfo;
class MathFunctions
{
public:
	static void registerFunctions(StackMachineThread* machine);
	~MathFunctions(void);
private:
	MathFunctions(void);
	Variant* absolute(CallInfo* callInfo);
	Variant* acosine(CallInfo* callInfo);
	Variant* asine(CallInfo* callInfo);
	Variant* atangent(CallInfo* callInfo);
	Variant* cosine(CallInfo* callInfo);
	Variant* ceiling(CallInfo* callInfo);
	Variant* exponent(CallInfo* callInfo);
	Variant* floor(CallInfo* callInfo);
	Variant* logarithm(CallInfo* callInfo);
	Variant* modulus(CallInfo* callInfo);
	Variant* random(CallInfo* callInfo);
	Variant* _round(CallInfo* callInfo);
	Variant* sine(CallInfo* callInfo);
	Variant* _sqrt(CallInfo* callInfo);
	Variant* srandom(CallInfo* callInfo);
	Variant* tangent(CallInfo* callInfo);
	std::mt19937 rng;
};

