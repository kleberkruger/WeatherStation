/*
 =======================================================================================================================
 File       : DLinkedList.cpp
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Kleber Kruger
 Email      : kleberkruger@gmail.com
 Date       : 2014-02-24
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Usual Data Structures
 =======================================================================================================================
 */

#include "DLinkedList.h"

template<class E>
DLinkedList<E>::DLinkedList() {
	this->_size = 0;
	this->first = NULL;
	this->last = NULL;
}

template<class E>
DLinkedList<E>::DLinkedList(List<E> *list) {

	this->_size = 0;
	this->first = NULL;
	this->last = NULL;

	addAll(list);
}

template<class E>
DLinkedList<E>::~DLinkedList() {
	clear();
}

template<class E>
E* DLinkedList<E>::getFirst() {
	const ListNode<E> *f = first;
//	if (f == NULL)
//		throw new NoSuchElementException();
	return f->item;
}

template<class E>
E* DLinkedList<E>::getLast() {
	const ListNode<E> *l = last;
//	if (l == NULL)
//		throw new NoSuchElementException();
	return l->item;
}

template<class E>
void DLinkedList<E>::addFirst(E e) {
	linkFirst(e);
}

template<class E>
void DLinkedList<E>::addLast(E e) {
	linkLast(e);
}

template<class E>
E* DLinkedList<E>::removeFirst() {
	const ListNode<E> *f = first;
//	if (f == null)
//		throw new NoSuchElementException();
	return unlinkFirst(f);
}

template<class E>
E* DLinkedList<E>::removeLast() {
	const ListNode<E> *l = last;
//	if (l == null)
//		throw new NoSuchElementException();
	return unlinkLast(l);
}

template<class E>
unsigned int DLinkedList<E>::size() {
	return _size;
}

template<class E>
bool DLinkedList<E>::isEmpty() {
	return _size == 0;
}

template<class E>
bool DLinkedList<E>::contains(E *o) {
	return indexOf(o) != -1;
}

template<class E>
E* DLinkedList<E>::toArray() {

	E result[] = new E[_size];
	int i = 0;

	for (ListNode<E> *x = first; x != NULL; x = x->next)
		result[i++] = x->item;

	return result;
}

template<class E>
bool DLinkedList<E>::add(E *e) {
	linkLast(e);
	return true;
}

template<class E>
bool DLinkedList<E>::remove(E *element) {

	if (element == NULL) {
		for (ListNode<E> *x = first; x != NULL; x = x->next) {
			if (x->item == NULL) {
				unlink(x);
				return true;
			}
		}
	} else {
		for (ListNode<E> *x = first; x != NULL; x = x->next) {
			if (element == x->item) {
				unlink(x);
				return true;
			}
		}
	}

	return false;
}

template<class E>
bool DLinkedList<E>::containsAll(List<E> *c) {
	return false;
}

template<class E>
bool DLinkedList<E>::addAll(List<E> *c) {
	return addAll(_size, c);
}

template<class E>
bool DLinkedList<E>::addAll(unsigned int index, List<E> *c) {

	return false;
//
//	checkPositionIndex(index);
//
//	E a[] = c->toArray();
//	int numNew = a->length;
//	if (numNew == 0)
//		return false;
//
//	ListNode<E> *pred, *succ;
//
//	if (index == _size) {
//		succ = NULL;
//		pred = last;
//	} else {
//		succ = node(index);
//		pred = succ->prev;
//	}
//
//	for (Object o : a) {
//
//		E *e = (E *) o;
//		Node<E> *newNode = new ListNode<E>(pred, e, null);
//
//		if (pred == null)
//			first = newNode;
//		else
//			pred->next = newNode;
//
//		pred = newNode;
//	}
//
//	if (succ == NULL) {
//		last = pred;
//	} else {
//		pred->next = succ;
//		succ->prev = pred;
//	}
//
//	_size += numNew;
//
//	return true;
}

template<class E>
bool DLinkedList<E>::removeAll(List<E> *c) {
	return false;
}

template<class E>
bool DLinkedList<E>::retainAll(List<E> *c) {
	return false;
}

template<class E>
void DLinkedList<E>::clear() {

	for (ListNode<E> *x = first; x != NULL;) {

		ListNode<E> *next = x->next;

		x->item = NULL;
		x->next = NULL;
		x->prev = NULL;

		x = next;
	}

	first = last = NULL;
	_size = 0;
}

template<class E>
bool DLinkedList<E>::equals(List<E> *o) {
	return false;
}

template<class E>
void DLinkedList<E>::add(unsigned int index, E *element) {
	return false;
}

template<class E>
E* DLinkedList<E>::remove(unsigned int index) {
	checkElementIndex(index);
	return unlink(node(index));
}

template<class E>
E* DLinkedList<E>::get(unsigned int index) {
	checkElementIndex(index);
	return node(index)->item;
}

template<class E>
E* DLinkedList<E>::set(unsigned int index, E *element) {
	checkElementIndex(index);
	ListNode<E> *x = node(index);
	E *oldVal = x->item;
	x->item = element;
	return oldVal;
}

template<class E>
int DLinkedList<E>::indexOf(E *e) {
	int index = 0;
	if (e == NULL) {
		for (ListNode<E> *x = first; x != NULL; x = x->next) {
			if (x->item == NULL)
				return index;
			index++;
		}
	} else {
		for (ListNode<E> *x = first; x != NULL; x = x->next) {
			if (e == x->item)
				return index;
			index++;
		}
	}
	return -1;
}

template<class E>
int DLinkedList<E>::lastIndexOf(E *e) {
	int index = _size;
	if (e == NULL) {
		for (ListNode<E> *x = last; x != NULL; x = x->prev) {
			index--;
			if (x->item == NULL)
				return index;
		}
	} else {
		for (ListNode<E> *x = last; x != NULL; x = x->prev) {
			index--;
			if (e == x->item)
				return index;
		}
	}
	return -1;
}

template<class E>
List<E>* DLinkedList<E>::subList(unsigned int fromIndex, unsigned int toIndex) {
	return NULL;
}

template<class E>
void DLinkedList<E>::linkFirst(E *e) {

	const ListNode<E> *f = first;
	const ListNode<E> newNode = new ListNode<E>(NULL, e, f);

	first = newNode;

	if (f == NULL)
		last = newNode;
	else
		f->prev = newNode;

	_size++;
}

template<class E>
E* DLinkedList<E>::unlinkFirst(ListNode<E> *f) {

	const E *element = f->item;
	const ListNode<E> *next = f->next;

	f->item = NULL;
	f->next = NULL;
	first = next;

	if (next == NULL)
		last = NULL;
	else
		next->prev = NULL;

	_size--;

	return element;
}

template<class E>
E* DLinkedList<E>::unlinkLast(ListNode<E> *l) {

	const E *element = l->item;
	const ListNode<E> *prev = l->prev;

	l->item = NULL;
	l->prev = NULL;
	last = prev;

	if (prev == NULL)
		first = NULL;
	else
		prev->next = NULL;

	_size--;

	return element;
}

template<class E>
E* DLinkedList<E>::unlink(ListNode<E> *x) {

	const E *element = x->item;
	const ListNode<E> *next = x->next;
	const ListNode<E> *prev = x->prev;

	if (prev == NULL) {
		first = next;
	} else {
		prev->next = next;
		x->prev = NULL;
	}

	if (next == NULL) {
		last = prev;
	} else {
		next->prev = prev;
		x->next = NULL;
	}

	x->item = NULL;
	_size--;

	return element;
}

template<class E>
bool DLinkedList<E>::isElementIndex(int index) {
	return (index >= 0 && index < size);
}

template<class E>
bool DLinkedList<E>::isPositionIndex(int index) {
	return (index >= 0 && index <= size);
}

template<class E>
ListNode<E>* DLinkedList<E>::node(int index) {
	if (index < (_size >> 1)) {
		ListNode<E> *x = first;
		for (int i = 0; i < index; i++)
			x = x->next;
		return x;
	} else {
		ListNode<E> *x = last;
		for (int i = size - 1; i > index; i--)
			x = x->prev;
		return x;
	}
}

template<class E>
char* DLinkedList<E>::outOfBoundsMsg(int index) {
	return "outOfBoundsMsg()";
//	return "Index: " + index + ", Size: " + _size;
}

template<class E>
void DLinkedList<E>::checkElementIndex(int index) {
//	if (!isElementIndex(index))
//		throw new IndexOutOfBoundsException(outOfBoundsMsg(index));
}

template<class E>
void DLinkedList<E>::checkPositionIndex(int index) {
//	if (!isPositionIndex(index))
//		throw new IndexOutOfBoundsException(outOfBoundsMsg(index));
}

// Queue operations.

template<class E>
E* DLinkedList<E>::peek() {
	const ListNode<E> *f = first;
	return (f == NULL) ? NULL : f->item;
}

template<class E>
E* DLinkedList<E>::element() {
	return getFirst();
}

template<class E>
E* DLinkedList<E>::poll() {
	const ListNode<E> *f = first;
	return (f == NULL) ? NULL : unlinkFirst(f);
}

template<class E>
E* DLinkedList<E>::remove() {
	return removeFirst();
}

template<class E>
bool DLinkedList<E>::offer(E *e) {
	return add(e);
}

// Dequeue operations

template<class E>
bool DLinkedList<E>::offerFirst(E *e) {
	addFirst(e);
	return true;
}

template<class E>
bool DLinkedList<E>::offerLast(E *e) {
	addLast(e);
	return true;
}

template<class E>
E* DLinkedList<E>::peekFirst() {
	const ListNode<E> *f = first;
	return (f == NULL) ? NULL : f->item;
}

template<class E>
E* DLinkedList<E>::peekLast() {
	const ListNode<E> *l = last;
	return (l == NULL) ? NULL : l->item;
}

template<class E>
E* DLinkedList<E>::pollFirst() {
	const ListNode<E> *f = first;
	return (f == NULL) ? NULL : unlinkFirst(f);
}

template<class E>
E* DLinkedList<E>::pollLast() {
	const ListNode<E> *l = last;
	return (l == NULL) ? NULL : unlinkLast(l);
}

template<class E>
void push(E *e) {
	addFirst(e);
}

template<class E>
E* DLinkedList<E>::pop() {
	return removeFirst();
}

template<class E>
bool DLinkedList<E>::removeFirstOccurrence(E *e) {
	return remove(e);
}

template<class E>
bool DLinkedList<E>::removeLastOccurrence(E *e) {
	if (e == NULL) {
		for (ListNode<E> *x = last; x != NULL; x = x->prev) {
			if (x->item == NULL) {
				unlink(x);
				return true;
			}
		}
	} else {
		for (ListNode<E> *x = last; x != NULL; x = x->prev) {
			if (e == x->item) {
				unlink(x);
				return true;
			}
		}
	}
	return false;
}
