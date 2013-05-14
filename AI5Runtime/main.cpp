#include <iostream>

#include "StackMachine.h"
#include "Instruction.h"
#include "Variant.h"
#include <ctime>
#include <memory>
#include "ProgramLoader.h"

int main() 
{
	std::shared_ptr<StackMachine> machine = ProgramLoader::LoadFromFile("..\\AI5Compiler\\test.aic");

	clock_t begin = clock();

	machine->start();

	//sm.start();
	//sm.getDataStack()->pop()->print();

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << "Execution time: " << elapsed_secs << std::endl;
}