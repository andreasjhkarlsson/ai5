#include "ListVariant.h"


ListVariant::ListVariant(void): Variant(LIST)
{
	list = shared_var_list(new std::vector<Variant*>());
}


ListVariant::~ListVariant(void)
{
	for(int i=0;i<list->size();i++)
	{
		(*list)[i]->release();
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
__int64 ListVariant::toInteger()
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

Variant* ListVariant::getElement(int index)
{
	return (*list)[index];
}