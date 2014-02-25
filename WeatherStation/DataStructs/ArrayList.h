/*
 =======================================================================================================================
 File       : ArrayList.h
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

#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include "mbed.h"
#include "List.h"

template<class E = void> // Generic Template
class ArrayList: public List<E> {
public:

	/**
	 * Constructs an empty list with an initial capacity of ten.
	 */
	ArrayList();

	/**
	 * Constructs an empty list with the specified initial capacity.
	 *
	 * @param initialCapacity 	- the initial capacity of the list
	 */
	ArrayList(unsigned int initialCapacity);

	/**
	 * Destroys this list.
	 */
	virtual ~ArrayList();

	/**
	 * Increases the capacity of this <tt>ArrayList</tt> instance, if
	 * necessary, to ensure that it can hold at least the number of elements
	 * specified by the minimum capacity argument.
	 *
	 * @param   minCapacity   the desired minimum capacity
	 */
	void ensureCapacity(int minCapacity);

	/**
	 * Returns a shallow copy of this <tt>ArrayList</tt> instance.  (The
	 * elements themselves are not copied.)
	 *
	 * @return a clone of this <tt>ArrayList</tt> instance
	 */
	ArrayList<E>* clone();

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

protected:

	/**
	 * Removes from this list all of the elements whose index is between
	 * {@code fromIndex}, inclusive, and {@code toIndex}, exclusive.
	 * Shifts any succeeding elements to the left (reduces their index).
	 * This call shortens the list by {@code (toIndex - fromIndex)} elements.
	 * (If {@code toIndex==fromIndex}, this operation has no effect.)
	 *
	 * @throws IndexOutOfBoundsException if {@code fromIndex} or
	 *         {@code toIndex} is out of range
	 *         ({@code fromIndex < 0 ||
	 *          fromIndex >= size() ||
	 *          toIndex > size() ||
	 *          toIndex < fromIndex})
	 */
	void removeRange(int fromIndex, int toIndex);

private:

	/**
	 * The maximum size of array to allocate.
	 * Some VMs reserve some header words in an array.
	 * Attempts to allocate larger arrays may result in
	 * OutOfMemoryError: Requested array size exceeds VM limit
	 */
	static const int MAX_ARRAY_SIZE = Integer.MAX_VALUE - 8;

	/**
	 * Default initial capacity.
	 */
	static const unsigned int DEFAULT_CAPACITY = 10;

	/**
	 * Shared empty array instance used for empty instances.
	 */
//	static const E* EMPTY_ELEMENTDATA[] = { }; /* XXX */
	/**
	 * The array buffer into which the elements of the ArrayList are stored.
	 * The capacity of the ArrayList is the length of this array buffer. Any
	 * empty ArrayList with elementData == EMPTY_ELEMENTDATA will be expanded to
	 * DEFAULT_CAPACITY when the first element is added.
	 */
	E _elementData[];

	/**
	 * The size of the ArrayList (the number of elements it contains).
	 *
	 * @serial
	 */
	unsigned int _size;

	void ensureCapacityInternal(int minCapacity);
	void ensureExplicitCapacity(int minCapacity);

	/**
	 * Increases the capacity to ensure that it can hold at least the
	 * number of elements specified by the minimum capacity argument.
	 *
	 * @param minCapacity the desired minimum capacity
	 */
	void grow(int minCapacity);

	static int hugeCapacity(int minCapacity);

	/*
	 * Private remove method that skips bounds checking and does not
	 * return the value removed.
	 */
	void fastRemove(int index);

	/**
	 * Checks if the given index is in range.  If not, throws an appropriate
	 * runtime exception.  This method does *not* check if the index is
	 * negative: It is always used immediately prior to an array access,
	 * which throws an ArrayIndexOutOfBoundsException if index is negative.
	 */
	void rangeCheck(int index);

	/**
	 * A version of rangeCheck used by add and addAll.
	 */
	void rangeCheckForAdd(int index);

	/**
	 * Constructs an IndexOutOfBoundsException detail message.
	 * Of the many possible refactorings of the error handling code,
	 * this "outlining" performs best with both server and client VMs.
	 */
	char* outOfBoundsMsg(int index);

	bool batchRemove(List<E> c, bool complement);

	static void subListRangeCheck(int fromIndex, int toIndex, int size);
};

#endif /* ARRAYLIST_H_ */
