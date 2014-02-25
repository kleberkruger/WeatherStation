/*
 =======================================================================================================================
 File       : Queue.h
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

#ifndef QUEUE_H_
#define QUEUE_H_

#include "mbed.h"

template<class E = void> // Generic Template
class Queue {
public:

	/**
	 * Inserts the specified element into this queue if it is possible to do so
	 * immediately without violating capacity restrictions, returning
	 * true upon success and throwing an IllegalStateException
	 * if no space is currently available.
	 *
	 * @param e the element to add
	 * @return true (as specified by {@link Collection#add})
	 * @throws IllegalStateException if the element cannot be added at this
	 *         time due to capacity restrictions
	 * @throws ClassCastException if the class of the specified element
	 *         prevents it from being added to this queue
	 * @throws NullPointerException if the specified element is null and
	 *         this queue does not permit null elements
	 * @throws IllegalArgumentException if some property of this element
	 *         prevents it from being added to this queue
	 */
	bool add(E *e);

	/**
	 * Inserts the specified element into this queue if it is possible to do
	 * so immediately without violating capacity restrictions.
	 * When using a capacity-restricted queue, this method is generally
	 * preferable to {@link #add}, which can fail to insert an element only
	 * by throwing an exception.
	 *
	 * @param e the element to add
	 * @return true if the element was added to this queue, else
	 *         false
	 * @throws ClassCastException if the class of the specified element
	 *         prevents it from being added to this queue
	 * @throws NullPointerException if the specified element is null and
	 *         this queue does not permit null elements
	 * @throws IllegalArgumentException if some property of this element
	 *         prevents it from being added to this queue
	 */
	bool offer(E *e);

	/**
	 * Retrieves and removes the head of this queue.  This method differs
	 * from {@link #poll poll} only in that it throws an exception if this
	 * queue is empty.
	 *
	 * @return the head of this queue
	 * @throws NoSuchElementException if this queue is empty
	 */
	E* remove();

	/**
	 * Retrieves and removes the head of this queue,
	 * or returns null if this queue is empty.
	 *
	 * @return the head of this queue, or null if this queue is empty
	 */
	E* poll();

	/**
	 * Retrieves, but does not remove, the head of this queue.  This method
	 * differs from {@link #peek peek} only in that it throws an exception
	 * if this queue is empty.
	 *
	 * @return the head of this queue
	 * @throws NoSuchElementException if this queue is empty
	 */
	E* element();

	/**
	 * Retrieves, but does not remove, the head of this queue,
	 * or returns null if this queue is empty.
	 *
	 * @return the head of this queue, or null if this queue is empty
	 */
	E* peek();
};

#endif /* QUEUE_H_ */
