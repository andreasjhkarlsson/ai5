#include "ListVariant.h"
#include "RuntimeError.h"

ListVariant::ListVariant(void): Variant(LIST)
{
	list = shared_var_list(new std::vector<Variant*>());
}


ListVariant::~ListVariant(void)
{
	cleanup();
}

void ListVariant::cleanup()
{
	if(this->list != nullptr)
	{
		for(int i=0;i<list->size();i++)
		{
			(*list)[i]->release();
		}
		this->list = nullptr;
	}
}

void ListVariant::print()
{
	std::wcout << L"ListVariant: " << *toString() << std::endl;
}

double ListVariant::toFloating()
{
	return 0.0;
}
__int64 ListVariant::toInteger64()
{
	return 0;
}
int ListVariant::toInteger32()
{
	return 0;
}
bool ListVariant::toBoolean()
{
	return list->size() > 0;
}
shared_string ListVariant::toString()
{
	std::wstring* str = new std::wstring();

	(*str)+=L"[";
	bool first = true;
	for(Variant* var: *list)
	{
		if(!first)
			(*str)+=L", ";
		(*str) += *var->toString();

		first = false;
	}

	(*str) += L"]";

	return shared_string(str);
}


void ListVariant::addElement(Variant* var)
{
	var->addRef();
	list->push_back(var);
}

Variant* ListVariant::getElement(size_t index)
{
	if(index >= list->size() || index < 0)
		throw RuntimeError(L"List index out of bounds!");
	return (*list)[index];
}

void ListVariant::setElement(size_t index,Variant* var)
{
	if(index >= list->size() || index < 0)
		throw RuntimeError(L"List index out of bounds!");
	// Add reference to new value.
	var->addRef();
	// Release old value!
	(*list)[index]->release();
	// Assign new value.
	(*list)[index] = var;
}


size_t ListVariant::size()
{
	return list->size();
}

void ListVariant::deleteAt(size_t index)
{
	list->erase(list->begin()+index);
}