#pragma once
#include <random>
#include "..\AI5Runtime\types.h"
class MathFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~MathFunctions(void);
private:
	MathFunctions(void);
	VariantReference<> absolute(CallInfo* callInfo);
	VariantReference<> acosine(CallInfo* callInfo);
	VariantReference<> asine(CallInfo* callInfo);
	VariantReference<> atangent(CallInfo* callInfo);
	VariantReference<> cosine(CallInfo* callInfo);
	VariantReference<> ceiling(CallInfo* callInfo);
	VariantReference<> exponent(CallInfo* callInfo);
	VariantReference<> floor(CallInfo* callInfo);
	VariantReference<> logarithm(CallInfo* callInfo);
	VariantReference<> modulus(CallInfo* callInfo);
	VariantReference<> random(CallInfo* callInfo);
	VariantReference<> _round(CallInfo* callInfo);
	VariantReference<> sine(CallInfo* callInfo);
	VariantReference<> _sqrt(CallInfo* callInfo);
	VariantReference<> srandom(CallInfo* callInfo);
	VariantReference<> tangent(CallInfo* callInfo);
	std::mt19937 rng;
};

