#include "ListVariant.h"
#include "RuntimeError.h"

ListVariant::ListVariant(void): Variant(TYPE)
{
	list = shared_var_list(new std::vector<Variant*>());
}


ListVariant::~ListVariant(void)
{
	cleanup();
}

void ListVariant::cleanup()
{
	Variant::cleanup();
	if(this->list != nullptr)
	{
		for(int i=0;i<list->size();i++)
		{
			(*list)[i]->release();
		}
		this->list = nullptr;
	}
}

std::wostream& ListVariant::format(std::wostream& stream) const
{
	stream << L"ListVariant: " << *toString();
	return stream;
}

double ListVariant::toFloating() const
{
	return 0.0;
}
__int64 ListVariant::toInteger64() const
{
	return 0;
}
int ListVariant::toInteger32() const
{
	return 0;
}
bool ListVariant::toBoolean() const
{
	return list->size() > 0;
}
shared_string ListVariant::toString() const
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

bool ListVariant::equal(Variant* other)
{
	if(getType() != other->getType())
		return false;

	ListVariant* otherList = static_cast<ListVariant*>(other);

	if(list->size() != otherList->list->size())
		return false;

	for(size_t index = 0; index < list->size(); index++)
	{
		if(!(*list)[index]->equal((*otherList->list)[index]))
			return false;
	}

	return true;

}