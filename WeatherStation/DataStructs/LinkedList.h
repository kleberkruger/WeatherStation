/*
 =======================================================================================================================
 File       : LinkedList.h
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
#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "mbed.h"

#include "List.h"
#include "ListNode.h"

template<class E = void> // Generic Template
class LinkedList: public List<E> {
public:

	/**
	 * Constructs an empty list or a list containing the elements of the
	 * specified collection, in the order they are returned by the
	 * collection's iterator.
	 *
	 * @param  c the collection whose elements are to be placed into this list
	 * @throws NullPointerException if the specified collection is null
	 */
	LinkedList(List<E> *c);

	/**
	 * Destroys this list.
	 */
	virtual ~LinkedList();

	/**
	 * Returns the first element in this list.
	 *
	 * @return the first element in this list
	 * @throws NoSuchElementException if this list is empty
	 */
	E* getFirst();

	/**
	 * Returns the last element in this list.
	 *
	 * @return the last element in this list
	 * @throws NoSuchElementException if this list is empty
	 */
	E* getLast();

	/**
	 * Inserts the specified element at the beginning of this list.
	 *
	 * @param e the element to add
	 */
	void addFirst(E e);

	/**
	 * Appends the specified element to the end of this list.
	 *
	 * This method is equivalent to {@link #add}.
	 *
	 * @param e the element to add
	 */
	void addLast(E e);

	/**
	 * Removes and returns the first element from this list.
	 *
	 * @return the first element from this list
	 * @throws NoSuchElementException if this list is empty
	 */
	E* removeFirst();

	/**
	 * Removes and returns the last element from this list.
	 *
	 * @return the last element from this list
	 * @throws NoSuchElementException if this list is empty
	 */
	E* removeLast();

	/*
	 * Information
	 */
	unsigned int size();
	bool isEmpty();
	bool contains(E *e);

	/*
	 * Conversion
	 */
	E* toArray();

	/*
	 * Modification Operations
	 */
	bool add(E *e);
	bool remove(E *o);

	/*
	 * Bulk Modification Operations
	 */
	bool containsAll(List<E> *c);
	bool addAll(List<E> *c);
	bool addAll(unsigned int index, List<E> *c);
	bool removeAll(List<E> *c);
	bool retainAll(List<E> *c);
	void clear();

	/*
	 * Comparison
	 */
	bool equals(List<E> *o);

	/*
	 * Positional Access Operations
	 */
	void add(unsigned int index, E *element);
	E* remove(unsigned int index);
	E* get(unsigned int index);
	E* set(unsigned int index, E *element);

	/*
	 * Search Operations
	 */
	int indexOf(E *e);
	int lastIndexOf(E *e);

	/*
	 * View
	 */
	List<E>* subList(unsigned int fromIndex, unsigned int toIndex);

	// Queue operations.

	/**
	 * Retrieves, but does not remove, the head (first element) of this list.
	 *
	 * @return the head of this list, or {@code null} if this list is empty
	 */
	E* peek();

	/**
	 * Retrieves, but does not remove, the head (first element) of this list.
	 *
	 * @return the head of this list
	 * @throws NoSuchElementException if this list is empty
	 */
	E* element();

	/**
	 * Retrieves and removes the head (first element) of this list.
	 *
	 * @return the head of this list, or {@code null} if this list is empty
	 */
	E* poll();

	/**
	 * Retrieves and removes the head (first element) of this list.
	 *
	 * @return the head of this list
	 * @throws NoSuchElementException if this list is empty
	 */
	E* remove();

	/**
	 * Adds the specified element as the tail (last element) of this list.
	 *
	 * @param e the element to add
	 * @return {@code true} (as specified by {@link Queue#offer})
	 */
	bool offer(E *e);

	// Dequeue operations

	/**
	 * Inserts the specified element at the front of this list.
	 *
	 * @param e the element to insert
	 * @return {@code true} (as specified by {@link Dequeue#offerFirst})
	 */
	bool offerFirst(E *e);

	/**
	 * Inserts the specified element at the end of this list.
	 *
	 * @param e the element to insert
	 * @return {@code true} (as specified by {@link Dequeue#offerLast})
	 */
	bool offerLast(E *e);

	/**
	 * Retrieves, but does not remove, the first element of this list,
	 * or returns {@code null} if this list is empty.
	 *
	 * @return the first element of this list, or {@code null}
	 *         if this list is empty
	 */
	E* peekFirst();

	/**
	 * Retrieves, but does not remove, the last element of this list,
	 * or returns {@code null} if this list is empty.
	 *
	 * @return the last element of this list, or {@code null}
	 *         if this list is empty
	 */
	E* peekLast();

	/**
	 * Retrieves and removes the first element of this list,
	 * or returns {@code null} if this list is empty.
	 *
	 * @return the first element of this list, or {@code null} if
	 *     this list is empty
	 */
	E* pollFirst();

	/**
	 * Retrieves and removes the last element of this list,
	 * or returns {@code null} if this list is empty.
	 *
	 * @return the last element of this list, or {@code null} if
	 *     this list is empty
	 */
	E* pollLast();

	/**
	 * Pushes an element onto the stack represented by this list.  In other
	 * words, inserts the element at the front of this list.
	 *
	 * <p>This method is equivalent to {@link #addFirst}.
	 *
	 * @param e the element to push
	 */
	void push(E *e);

	/**
	 * Pops an element from the stack represented by this list.  In other
	 * words, removes and returns the first element of this list.
	 *
	 * This method is equivalent to {@link #removeFirst()}.
	 *
	 * @return the element at the front of this list (which is the top
	 *         of the stack represented by this list)
	 * @throws NoSuchElementException if this list is empty
	 */
	E* pop();

	/**
	 * Removes the first occurrence of the specified element in this
	 * list (when traversing the list from head to tail).  If the list
	 * does not contain the element, it is unchanged.
	 *
	 * @param o element to be removed from this list, if present
	 * @return {@code true} if the list contained the specified element
	 */
	bool removeFirstOccurrence(E *e);

	/**
	 * Removes the last occurrence of the specified element in this
	 * list (when traversing the list from head to tail).  If the list
	 * does not contain the element, it is unchanged.
	 *
	 * @param o element to be removed from this list, if present
	 * @return {@code true} if the list contained the specified element
	 */
	bool removeLastOccurrence(E *e);

private:

	unsigned int _size;
	ListNode<E> *first;
	ListNode<E> *last;

	/**
	 * Links e as first element.
	 */
	void linkFirst(E *e);

	/**
	 * Unlinks non-null first node f.
	 */
	E* unlinkFirst(ListNode<E> *f);

	/**
	 * Unlinks non-null last node l.
	 */
	E* unlinkLast(ListNode<E> *l);

	/**
	 * Unlinks non-null node x.
	 */
	E* unlink(ListNode<E> *x);

	/**
	 * Tells if the argument is the index of an existing element.
	 */
	bool isElementIndex(int index);

	/**
	 * Tells if the argument is the index of a valid position for an
	 * iterator or an add operation.
	 */
	bool isPositionIndex(int index);

	/**
	 * Returns the (non-null) Node at the specified element index.
	 */
	ListNode<E>* node(int index);

	/**
	 * Constructs an IndexOutOfBoundsException detail message.
	 * Of the many possible refactorings of the error handling code,
	 * this "outlining" performs best with both server and client VMs.
	 */
	char* outOfBoundsMsg(int index);

	void checkElementIndex(int index);
	void checkPositionIndex(int index);
};

#endif /* LINKEDLIST_H_ */
