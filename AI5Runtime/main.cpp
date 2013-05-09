#include <iostream>

#include "StackMachine.h"
#include "Instruction.h"
#include "Variant.h"
#include <ctime>
#include <memory>

int main() 
{

	auto program = std::shared_ptr<std::vector<Instruction*>>(new std::vector<Instruction*>());
	auto statics = std::shared_ptr<std::vector<STATIC_DATA*>>(new std::vector<STATIC_DATA*>());
	StackMachine sm(statics,program);
	
	/*
	char start = 0;
	char increment = 1;
	__int64 limit = 100000000;
	int i = 33;
	int j = 34;
	int trueJump = 2;
	int jump = -8;	
	program->push_back(new Instruction(Instruction::CREATE_GLOBAL,&j));
	program->push_back(new Instruction(Instruction::PUSH_LARGE_INTEGER,&limit));
	program->push_back(new Instruction(Instruction::ASSIGN_NAME,&j));
	program->push_back(new Instruction(Instruction::CREATE_GLOBAL,&i));
	program->push_back(new Instruction(Instruction::PUSH_SMALL_INTEGER,&start));
	program->push_back(new Instruction(Instruction::ASSIGN_NAME,&i));
	program->push_back(new Instruction(Instruction::PUSH_NAME,&i));
	program->push_back(new Instruction(Instruction::PUSH_SMALL_INTEGER,&increment));
	program->push_back(new Instruction(Instruction::ADDITION,nullptr));
	program->push_back(new Instruction(Instruction::DOUBLE_TOP,nullptr));
	program->push_back(new Instruction(Instruction::ASSIGN_NAME,&i));
	program->push_back(new Instruction(Instruction::PUSH_NAME,&j));
	program->push_back(new Instruction(Instruction::GREATER,nullptr));
	program->push_back(new Instruction(Instruction::JUMP_LONG_RELATIVE_IF_TRUE,&trueJump));
	program->push_back(new Instruction(Instruction::JUMP_LONG_RELATIVE,&jump));
	program->push_back(new Instruction(Instruction::TERMINATE,nullptr));

	sm.getNameStorage()->createIndexForName("i",i);
	sm.getNameStorage()->createIndexForName("j",j);
	*/
	
	
	/*
	int a = 35;
	int b = 36;
	__int64 num = 3;
	program->push_back(new Instruction(Instruction::CALL_NAME,&a));
	program->push_back(new Instruction(Instruction::TERMINATE,nullptr));
	program->push_back(new Instruction(Instruction::PUSH_LARGE_INTEGER,&num));
	program->push_back(new Instruction(Instruction::PUSH_NAME,&b));
	program->push_back(new Instruction(Instruction::ADDITION,nullptr));
	program->push_back(new Instruction(Instruction::RET,nullptr));

	sm.getNameStorage()->createIndexForName("a",35)->setGlobal(new UserFunctionVariant(2));
	sm.getNameStorage()->createIndexForName("b",36)->setGlobal(IntegerVariant::BYTE_TABLE[12+128]);
	*/
	
	



	int hello = 10;
	program->push_back(new Instruction(Instruction::CALL_NAME,&hello));
	sm.getNameStorage()->createIndexForName("hello",hello);
	program->push_back(new Instruction(Instruction::TERMINATE,nullptr));


	clock_t begin = clock();

	sm.start();
	//sm.getDataStack()->pop()->print();

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << "Execution time: " << elapsed_secs << std::endl;
}