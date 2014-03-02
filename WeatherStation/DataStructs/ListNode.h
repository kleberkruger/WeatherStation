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

	/**
	 * Constructs a list node.
	 *
	 * @param prev
	 * @param element
	 * @param next
	 */
	ListNode(E *element, ListNode<E> *prev = NULL, ListNode<E> *next = NULL) {
		this->item = element;
		this->prev = prev;
		this->next = next;
	}

	virtual ~ListNode();

private:

	E* item;
	ListNode<E>* prev;
	ListNode<E>* next;
};

#endif /* LISTNODE_H_ */
