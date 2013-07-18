#pragma once
#include "gc.h"
#include "StackMachine.h"
#include "Variant.h"
#include "VariantReference.h"
#include "GlobalOptions.h"

GC* GC::instance = nullptr;

GC::BlockHeader::BlockHeader(BlockHeader* previous,BlockHeader* next):previous(previous),next(next),mark(false),generation(0),referencedFrom(-1)
{

}

GC::BlockHeader::BlockHeader(): previous(nullptr), next(nullptr),mark(false),generation(0),referencedFrom(-1)
{
	
}

void GC::collect(StackMachine* machine)
{
	instance->run(machine);
}

GC::DoubleLinkedList::DoubleLinkedList()
{
	start = new BlockHeader();
	end = new BlockHeader();
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


void GC::mark(BlockHeader* header)
{


	if(header == nullptr || header->mark)
	{
		// This object has already been visited. Return.
		return;
	}
	header->mark = true;
	switch(header->object->getType())
	{
	case Variant::NAME:
		mark(header->object->cast<NameVariant>()->value);
		break;
	case Variant::SCOPE:
		{
			Scope* scope = header->object->cast<Scope>();
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
	while(current != nullptr)
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
			if(GlobalOptions::isVerbose())
				std::wcout << "\t! GC found garbage of type: " << current->object->typeAsString() << std::endl;
			current->object->~Variant();

			BlockHeader* temp = objectList.erase(current);
			free(current);
			current = temp;
		}

		
	}

}

void GC::run(StackMachine* machine)
{
	// Mark objects from roots.
	VariantReference<Scope>& globalScope = machine->globalScope;
	mark(globalScope);

	sweep();
}


GC::BlockHeader* GC::VarRefToBlockHead(const VariantReference<>&ref)
{
	if(!ref.isComplexType())
		return nullptr;
	return reinterpret_cast<BlockHeader*>(((char*)ref.ref.variant)-BLOCKHEADER_ALIGNED_SIZE);
}