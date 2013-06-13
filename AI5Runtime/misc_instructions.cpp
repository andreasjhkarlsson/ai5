#include "misc_instructions.h"

// Inlining recursive function?? Crazy shit!!
ListVariant* createList(std::stack<int> subscripts)
{

	int count = subscripts.top();
	subscripts.pop();

	ListVariant* list = new ListVariant();
	if(subscripts.empty())
	{
		for(int i=0;i<count;i++)
			list->addElement(&NullVariant::Instance);
		return list;
	}

	for(int i=0;i<count;i++)
	{
		list->addElement(createList(subscripts));
	}

	return list;

}


void redimList(Variant* listVar,std::stack<int> subscripts)
{
	if(!listVar->isListType())
		throw RuntimeError(L"Can only redim list types!");

	ListVariant* list = static_cast<ListVariant*>(listVar);

	int count = subscripts.top();
	subscripts.pop();

	// If this is the terminating list, add NullVariants or
	// remove elements with no recursion.
	if(subscripts.empty())
	{
		// SHould elements be added?
		if(count > list->size())
		{
			for(size_t i=0;i<(count-list->size()+1);i++)
			{
				list->addElement(&NullVariant::Instance);
			}
		}
		// Or should they be removed?
		else if(count < list->size())
		{
			size_t initialListSize = list->size();
			for(size_t i=list->size()-1;i >= (initialListSize-count);i--)
			{
				list->deleteAt(i);
			}
		}

	}
	// We must add or trim nested structure.
	// This involves recursive calls.
	else
	{
		// Add new lists and recurse into them?
		if(count >= list->size())
		{
			for(size_t i=0;i<count;i++)
			{
				if(i >= list->size())
				{
					list->addElement(new ListVariant());
				}

				// This indicates that we're adding at least another subscript.
				if(subscripts.size()>=1 && !list->getElement(i)->isListType())
				{
					// Replace the current bastard value in this position
					// with a new fresh list which can be populated
					// with further subscript(s).
					list->setElement(i,new ListVariant());
				}
				redimList(list->getElement(i),subscripts);
			}
		}
		// Remove nested lists! Only recurse into the ones still left
		// in the structure (duh).
		else if(count < list->size())
		{
			for(size_t i=list->size();i --> 0 ;)
			{
				if(i>=count)
				{
					list->deleteAt(i);
					continue;
				}
				// This indicates that we're adding at least another subscript.
				if(subscripts.size()>=1 && !list->getElement(i)->isListType())
				{
					// Replace the current bastard value in this position
					// with a new fresh list which can be populated
					// with further subscript(s).
					list->setElement(i,new ListVariant());
				}
				redimList(list->getElement(i),subscripts);
			}
		}
	}
}