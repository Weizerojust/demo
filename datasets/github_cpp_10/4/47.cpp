#ifndef GUARD_LINKEDLIST_IS_H
#define GUARD_LINKEDLIST_IS_H
#include "Node.cpp"
#include <cstdlib>
#include <cstddef>
#include <stdexcept>

namespace InsertionSortNS {
	class LinkedList {
	private:
		InsertionSortNS::Node* head;
		InsertionSortNS::Node* tail;
		std::size_t length;
	public:
		void push_back(InsertionSortNS::NodeData data);
		void insert(int index, InsertionSortNS::NodeData data);
		InsertionSortNS::NodeData remove(int index);
		InsertionSortNS::NodeData get(int index);
		void clear();
		int size();
		LinkedList();
		~LinkedList();
		void merge(InsertionSortNS::LinkedList* other);
	};
}

inline InsertionSortNS::LinkedList::LinkedList() {
	this->head = NULL;
	this->tail = NULL;
	this->length = 0;
}


inline InsertionSortNS::LinkedList::~LinkedList() {
	if (this->head) {
		InsertionSortNS::Node* current = head;
		InsertionSortNS::Node* next = current->getNext();
		delete current;
		while (next) {
			current = next;
			next = current->getNext();
			delete current;
		}
		head = NULL;
		tail = NULL;
		this->length = 0;
	}
}

inline void InsertionSortNS::LinkedList::push_back(InsertionSortNS::NodeData data) {
	if (!this->head) {
		InsertionSortNS::Node* newNode = new InsertionSortNS::Node(data);
		this->head = newNode;
		this->tail = newNode;
		head->setNext(0);
		tail->setNext(0);
	}
	else {
		InsertionSortNS::Node* newNode = new InsertionSortNS::Node(data);
		tail->setNext(newNode);
		tail = newNode;
	}
	++length;
}

inline void InsertionSortNS::LinkedList::insert(int index, InsertionSortNS::NodeData data) {
	if (this->size() < index || index < 0) {
		throw std::out_of_range("Index out of bounds");
	}
	else {
		InsertionSortNS::Node* newNode = new InsertionSortNS::Node(data);
		int counter = 0;
		InsertionSortNS::Node* current = head;
		InsertionSortNS::Node* previous = NULL;
		while (counter != index) {
			previous = current;
			current = current->getNext();
			counter++;
		}

		
		if (!previous) {
			newNode->setNext(current);
			head = newNode;
			if (!head->getNext()) {
				tail = head;
			}
		}
		else if (!current) { 
			previous->setNext(newNode);
			tail = newNode;
		}
		else { 
			previous->setNext(newNode);
			newNode->setNext(current);
		}
		++length;
	}
}
inline InsertionSortNS::NodeData InsertionSortNS::LinkedList::remove(int index) {
	
	if (this->size() <= index || index < 0) {
		throw std::out_of_range("Index out of bounds");
	}
	else {
		int counter = 0;
		InsertionSortNS::Node* current = head;
		InsertionSortNS::Node* previous = NULL;
		while (counter != index) {
			previous = current;
			current = current->getNext();
			counter++;
		}
		InsertionSortNS::NodeData data = current->getData();

		if (!previous) { 
						 
			head = current->getNext();
			if (!current->getNext()) {
				tail = head;
			}
			delete current;
			current = NULL;
		}
		else if (!current->getNext()) { 
			previous->setNext(0);
			tail = previous;
			delete current;
			current = NULL;
		}
		else { 
			previous->setNext(current->getNext());
			delete current;
			current = NULL;
		}
		--length;
		return data;
	}
}

inline InsertionSortNS::NodeData InsertionSortNS::LinkedList::get(int index) {
	
	if (this->size() <= index || index < 0) {
		throw std::out_of_range("Index out of bounds");
	}
	else {
		int counter = 0;
		InsertionSortNS::Node* current = head;
		while (current->getNext() && counter != index) {
			current = current->getNext();
			counter++;
		}
		return current->getData();
	}
}

inline int InsertionSortNS::LinkedList::size() {
	return static_cast<int>(this->length);
}


inline void InsertionSortNS::LinkedList::clear() {
	this->~LinkedList();
}


inline void InsertionSortNS::LinkedList::merge(InsertionSortNS::LinkedList* other)
{
	for (int i = 0; other->size() != 0; i++)
	{
		this->push_back(other->remove(0));
	}
}

#endif
