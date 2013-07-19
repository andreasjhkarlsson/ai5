#include "misc_instructions.h"

// Inlining recursive function?? Crazy shit!!
ListVariant* createList(std::stack<unsigned int> subscripts)
{

	unsigned int count = subscripts.top();
	subscripts.pop();

	ListVariant* list = ListVariant::Create();
	if(subscripts.empty())
	{
		for(unsigned int i=0;i<count;i++)
			list->addElement(VariantReference<>::NullReference());
		return list;
	}

	for(unsigned int i=0;i<count;i++)
	{
		list->addElement(createList(subscripts));
	}

	return list;

}


void redimList(VariantReference<ListVariant>& listVar,std::stack<unsigned int> subscripts)
{
	if(!listVar.isListType())
		throw RuntimeError(L"Can only redim list types!");

	unsigned int count = subscripts.top();
	subscripts.pop();

	// If this is the terminating list, add NullVariants or
	// remove elements with no recursion.
	if(subscripts.empty())
	{
		// SHould elements be added?
		if(count > listVar->size())
		{
			for(size_t i=0;i<(count-listVar->size()+1);i++)
			{
				listVar->addElement(VariantReference<>::NullReference());
			}
		}
		// Or should they be removed?
		else if(count < listVar->size())
		{
			for(size_t i=listVar->size()-1;i >= (count);i--)
			{
				listVar->deleteAt(i);
			}
		}

	}
	// We must add or trim nested structure.
	// This involves recursive calls.
	else
	{
		// Add new lists and recurse into them?
		if(count >= listVar->size())
		{
			for(size_t i=0;i<count;i++)
			{
				if(i >= listVar->size())
				{
					listVar->addElement(ListVariant::Create());
				}

				// This indicates that we're adding at least another subscript.
				if(subscripts.size()>=1 && !listVar->getElement(i).isListType())
				{
					// Replace the current bastard value in this position
					// with a new fresh list which can be populated
					// with further subscript(s).
					listVar->setElement(i,ListVariant::Create());
				}
				VariantReference<ListVariant> element = listVar->getElement(i).cast<ListVariant>();
				redimList(element,subscripts);
			}
		}
		// Remove nested lists! Only recurse into the ones still left
		// in the structure (duh).
		else if(count < listVar->size())
		{
			for(size_t i=listVar->size();i --> 0 ;)
			{
				if(i>=count)
				{
					listVar->deleteAt(i);
					continue;
				}
				// This indicates that we're adding at least another subscript.
				if(subscripts.size()>=1 && !listVar->getElement(i).isListType())
				{
					// Replace the current bastard value in this position
					// with a new fresh list which can be populated
					// with further subscript(s).
					listVar->setElement(i,ListVariant::Create());
				}
				VariantReference<ListVariant> element = listVar->getElement(i).cast<ListVariant>();
				redimList(element,subscripts);
			}
		}
	}
}