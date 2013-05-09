#include "StackMachine.h"
#include <map>
#include "NullVariant.h"
#include "Instruction.h"

StackMachine::StackMachine(std::shared_ptr<std::vector<STATIC_DATA*>> statics,
	std::shared_ptr<std::vector<Instruction*>> program): programCounter(0),
	dataStack(64*1024),staticsTable(statics),program(program),nameStorage(128), callStack(128)
{

}

StackMachine::~StackMachine(void)
{
}

void StackMachine::start()
{
	terminated = false;

	while (!terminated) 
	{
		std::cout << "Executing instruction @ " << programCounter << std::endl;
		(*program)[programCounter]->execute(this);
	}
}


void StackMachine::terminate()
{
	terminated = true;
}

StackMachine* StackMachine::LoadFromStructuredData(const std::string& filename)
{
	return nullptr;
}

void StackMachine::createLocal(int index)
{
	if(!callStack.size())
	{
		// throw error.
	}
	Name* name = nameStorage.getName(index);
	name->pushLocal(&NullVariant::Instance);
	callStack.top()->attachName(name);
}
void StackMachine::createGlobal(int index)
{
	nameStorage.getName(index)->setGlobal(&NullVariant::Instance);
}
