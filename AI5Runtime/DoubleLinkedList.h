#pragma once

// A simple generic double linked list for fast insertion/deletion operations.
// Nodes can inherit from DoubleLinkedList<t>::Node to provide needed members.
template <class T>
class DoubleLinkedList
{
public:

	struct Node
	{
		Node(): next(nullptr),previous(nullptr), sentinel(false)
		{
		}
		T* next;
		T* previous;
		bool sentinel;
	};

	DoubleLinkedList()
	{
		// Create sentinel nodes.
		start = new T();
		start->sentinel = true;
		end = new T();
		end->sentinel = true;

		// Link start and end together.
		start->next = end;
		end->previous = start;
	}

	bool empty()
	{
		return this->firstElement()->sentinel;
	}

	// Adds node to the end of list.
	void push_back(T* node)
	{
		node->previous = end->previous;
		node->next = end;
		end->previous->next = node;
		end->previous = node;
	}
	// Adds node to front of list.
	void push_front(T* node)
	{
		node->previous = start;
		node->next = start->next;
		start->next = node;
	}
	// Erases node from list and returns pointer to next node.
	T* erase(T* node)
	{
		node->previous->next = node->next;
		node->next->previous = node->previous;
		return node->next;
	}
	// Returns pointer to the first real node in list.
	T* firstElement()
	{
		return start->next;
	}

	T* lastElement()
	{
		return end->previous;
	}

	// Take ownership of all nodes in other by inserting them into this list
	// and removing them from other. Since it's a double linked list
	void splice(DoubleLinkedList* other)
	{
		// Other list is empty. No point in splicing.
		if(other->empty())
			return;

		// If this list is empty we insert a "dummy" node
		// into the list so the list can be spliced properly.
		// The dummy is erased afterwards. A bit of a hack but it works.
		T* dummy = nullptr;
		if(this->empty())
		{
			dummy = new T();
			this->push_back(dummy);
		}

		// Relink nodes.
		this->lastElement()->next = other->firstElement();
		other->firstElement()->previous = this->lastElement();
		this->end->previous = other->lastElement();
		other->lastElement()->next = this->end;

		// Make other list empty.
		other->start->next = other->end;
		other->end->previous = other->start;

		if(dummy != nullptr)
		{
			this->erase(dummy);
			delete dummy;
		}
	}

private:
	T* start;
	T* end;
};