#include <iostream>
#include <ctime>
#include <memory>
#include <io.h>
#include <fcntl.h>
#include "ProgramLoader.h"
#include "StackMachine.h"
#include "Instruction.h"
#include "Variant.h"


int main() 
{
	 _setmode(_fileno(stdout), _O_U16TEXT);

	std::shared_ptr<StackMachine> machine = ProgramLoader::LoadFromFile("..\\AI5Compiler\\test.aic");

	clock_t begin = clock();

	int returnCode=machine->start();

	std::wcout << L"Program ended with code: " << returnCode << std::endl;

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	std::wcout << "Execution time: " << elapsed_secs << std::endl;
}