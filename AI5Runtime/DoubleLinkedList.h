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
private:
	T* start;
	T* end;
};