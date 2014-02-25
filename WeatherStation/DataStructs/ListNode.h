/*
 * ListNode.h
 *
 *  Created on: 23/02/2014
 *      Author: Kleber
 */

#ifndef LISTNODE_H_
#define LISTNODE_H_

template<class E = void> // Generic Template
class ListNode {
public:

	ListNode(E *element) {
		item = element;
		prev = next = NULL;
	}

	ListNode(ListNode<E> *prev, E *element, ListNode<E> *next) {
		this->item = element;
		this->next = next;
		this->prev = prev;
	}

	virtual ~ListNode();

private:

	E* item;
	ListNode<E>* next;
	ListNode<E>* prev;
};

#endif /* LISTNODE_H_ */
