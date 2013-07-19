#pragma once
#include "gc.h"
#include "StackMachine.h"
#include "Variant.h"
#include "VariantReference.h"
#include "GlobalOptions.h"
#include "misc.h"

GC* GC::instance = nullptr;

void GC::init()
{
	instance = new GC();
}


GC::BlockHeader::BlockHeader(BlockHeader* previous,BlockHeader* next):previous(previous),next(next),mark(false),generation(0),referencedFrom(-1), sentinel(false)
{

}

GC::BlockHeader::BlockHeader(): previous(nullptr), next(nullptr),mark(false),generation(0),referencedFrom(-1), sentinel(false)
{
	
}

void GC::collect(StackMachine* machine)
{
	instance->run(machine);
}

GC::DoubleLinkedList::DoubleLinkedList()
{
	start = new BlockHeader();
	start->sentinel = true;
	end = new BlockHeader();
	end->sentinel = true;

	start->next = end;
	end->previous = start;
}
void GC::DoubleLinkedList::push_back(BlockHeader* block)
{
	block->previous = end->previous;
	block->next = end;
	end->previous->next = block;
	end->previous = block;
}
void GC::DoubleLinkedList::push_front(BlockHeader* block)
{
	block->previous = start;
	block->next = start->next;
	start->next = block;
}
GC::BlockHeader* GC::DoubleLinkedList::erase(BlockHeader* block)
{
	block->previous->next = block->next;
	block->next->previous = block->previous;
	return block->next;
}

GC::BlockHeader* GC::DoubleLinkedList::firstElement()
{
	return start->next;
}


void GC::trackObject(BlockHeader* object)
{
	objectList.push_back(object);
}


void GC::addStaticObject(BlockHeader* object)
{
	object->generation = GENERATION_STATIC;
	staticList.push_front(object);
}

void GC::mark(BlockHeader* header)
{


	if(header == nullptr || header->mark)
	{
		// This object should not be processed.
		return;
	}
	header->mark = true;
	switch(header->object->getType())
	{
	case Variant::NAME_REFERENCE:
	case Variant::NAME:
		mark(header->object->cast<NameVariant>()->value);
		break;
	case Variant::LIST:
		{
			ListVariant* listVar = header->object->cast<ListVariant>();
			for(size_t i=0;i<listVar->list->size();i++)
			{
				mark((*listVar->list)[i]);
			}
		}
	case Variant::HASH_MAP:
		{
			HashMapVariant* hashMapVar = header->object->cast<HashMapVariant>();
			for(auto it = hashMapVar->map.begin();it != hashMapVar->map.end();it++)
			{
				mark(it->first);
				mark(it->second);
			}

		}
	case Variant::SCOPE:
		{
			Scope* scope = header->object->cast<Scope>();
			int i=0;
			for(auto it = scope->lookup.begin();it!=scope->lookup.end();it++)
			{
				mark(it->second);

			}
		}
		break;
	}
}

void GC::mark(const VariantReference<>&ref)
{
	BlockHeader* head = VarRefToBlockHead(ref);
	mark(head);
}


void GC::sweep()
{
	BlockHeader* current = objectList.firstElement();
	while(!current->sentinel)
	{
		if(current->next == nullptr)
			break;
		if(current->mark)
		{
			current->mark = false; // Clear mark for next cycle.
			current = current->next;
		}
		else
		{

			DebugOut(L"GC") << "Found garbage of type " << current->object->typeAsString();


			BlockHeader* temp = objectList.erase(current);
			freeObject(current);
			current = temp;
		}
	}
}

void GC::run(StackMachine* machine)
{

	DebugOut(L"GC") << "Running cycle.";
	// Find all roots and mark them.
	// Roots are:
	// * Global scope
	// * Local scopes
	// * Data stack
	// * CallFrames
	// * Static objects

	BlockHeader* current = staticList.firstElement();
	while(!current->sentinel)
	{
		current->mark = false;
		mark(current);
		current = current->next;
	}

	// Mark objects from roots.
	VariantReference<Scope>& globalScope = machine->globalScope;
	VarRefToBlockHead(globalScope)->mark = false;
	mark(globalScope);

	sweep();
}


GC::BlockHeader* GC::VarRefToBlockHead(const VariantReference<>&ref)
{
	if(!ref.isComplexType())
		return nullptr;
	return reinterpret_cast<BlockHeader*>(((char*)ref.ref.variant)-BLOCKHEADER_ALIGNED_SIZE);
}

void GC::cleanup()
{
	instance->freeAll();
}

void GC::freeAll()
{

	DebugOut(L"GC") << L"Freeing all remaining objects.";


	// Clear all dynamic objects.
	BlockHeader* current = objectList.firstElement();
	while(!current->sentinel)
	{
		BlockHeader* next = current->next;
		freeObject(current);
		current = next;
	}

	// Clear all static objects.
	current = staticList.firstElement();
	while(!current->sentinel)
	{
		BlockHeader* next = current->next;
		freeObject(current);
		current = next;
	}

}

void GC::freeObject(BlockHeader* header)
{
	header->object->~Variant();
	free(header);
}