#pragma once



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
		start = new T();
		start->sentinel = true;
		end = new T();
		end->sentinel = true;

		start->next = end;
		end->previous = start;
	}
	void push_back(T* node)
	{
		node->previous = end->previous;
		node->next = end;
		end->previous->next = node;
		end->previous = node;
	}
	void push_front(T* node)
	{
		node->previous = start;
		node->next = start->next;
		start->next = node;
	}
	T* erase(T* node)
	{
		node->previous->next = node->next;
		node->next->previous = node->previous;
		return node->next;
	}
	T* firstElement()
	{
		return start->next;

	}
private:
	T* start;
	T* end;
};