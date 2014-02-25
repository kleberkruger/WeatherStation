/*
 =======================================================================================================================
 File       : List.h
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

#ifndef LIST_H_
#define LIST_H_

template<class E = void> // Generic Template
class List {
public:

	/**
	 * Returns the number of elements in this list. If this list contains
	 * more than MAX(unsigned int) elements, returns MAX(unsigned int).
	 *
	 * @return the number of elements in this list
	 */
	virtual unsigned int size() = 0;

	/**
	 * Returns true if this list contains no elements.
	 *
	 * @return true if this list contains no elements
	 */
	virtual bool isEmpty() = 0;

	/**
	 * Returns true if this list contains the specified element.
	 *
	 * @param e element whose presence in this list is to be tested
	 * @return true if this list contains the specified element
	 * @throws ClassCastException if the type of the specified element
	 *         is incompatible with this list
	 * @throws NullPointerException if the specified element is null and this
	 *         list does not permit null elements
	 */
	virtual bool contains(E *e) = 0;

	/**
	 * Returns an array containing all of the elements in this list in proper
	 * sequence (from first to last element).
	 *
	 * The returned array will be "safe" in that no references to it are
	 * maintained by this list.  (In other words, this method must allocate a
	 * new array even if this list is backed by an array).
	 * The caller is thus free to modify the returned array.
	 *
	 * This method acts as bridge between array-based and collection-based
	 * APIs.
	 *
	 * @return an array containing all of the elements in this list in proper
	 *         sequence
	 */
	virtual E* toArray() = 0;

	// Modification Operations

	/**
	 * Appends the specified element to the end of this list (optional
	 * operation).
	 *
	 * Lists that support this operation may place limitations on what
	 * elements may be added to this list.  In particular, some
	 * lists will refuse to add null elements, and others will impose
	 * restrictions on the type of elements that may be added.  List
	 * classes should clearly specify in their documentation any restrictions
	 * on what elements may be added.
	 *
	 * @param e element to be appended to this list
	 * @return true (as specified by {@link Collection#add})
	 * @throws UnsupportedOperationException if the add operation
	 *         is not supported by this list
	 * @throws ClassCastException if the class of the specified element
	 *         prevents it from being added to this list
	 * @throws NullPointerException if the specified element is null and this
	 *         list does not permit null elements
	 * @throws IllegalArgumentException if some property of this element
	 *         prevents it from being added to this list
	 */
	virtual bool add(E *e) = 0;

	/**
	 * Removes the first occurrence of the specified element from this list,
	 * if it is present (optional operation).  If this list does not contain
	 * the element, it is unchanged.  More formally, removes the element with
	 * the lowest index i such that (o==null&nbsp;?&nbsp;get(i)==null&nbsp;:&nbsp;o.equals(get(i)))
	 * (if such an element exists).  Returns true if this list
	 * contained the specified element (or equivalently, if this list changed
	 * as a result of the call).
	 *
	 * @param o element to be removed from this list, if present
	 * @return true if this list contained the specified element
	 * @throws ClassCastException if the type of the specified element
	 *         is incompatible with this list
	 * @throws NullPointerException if the specified element is null and this
	 *         list does not permit null elements
	 * @throws UnsupportedOperationException if the remove operation
	 *         is not supported by this list
	 */
	virtual bool remove(E *o) = 0;

	// Bulk Modification Operations

	/**
	 * Returns true if this list contains all of the elements of the
	 * specified collection.
	 *
	 * @param c collection to be checked for containment in this list
	 * @return true if this list contains all of the elements of the
	 *         specified collection
	 * @throws ClassCastException if the types of one or more elements in the
	 *         specified collection are incompatible with this list
	 * @throws NullPointerException if the specified collection contains one
	 *         or more null elements and this list does not permit null
	 *         elements, or if the specified collection is null
	 */
	virtual bool containsAll(List<E> *c) = 0;

	/**
	 * Appends all of the elements in the specified collection to the end of
	 * this list, in the order that they are returned by the specified
	 * collection's iterator (optional operation).  The behavior of this
	 * operation is undefined if the specified collection is modified while
	 * the operation is in progress.  (Note that this will occur if the
	 * specified collection is this list, and it's nonempty.)
	 *
	 * @param c collection containing elements to be added to this list
	 * @return true if this list changed as a result of the call
	 * @throws UnsupportedOperationException if the addAll operation
	 *         is not supported by this list
	 * @throws ClassCastException if the class of an element of the specified
	 *         collection prevents it from being added to this list
	 * @throws NullPointerException if the specified collection contains one
	 *         or more null elements and this list does not permit null
	 *         elements, or if the specified collection is null
	 * @throws IllegalArgumentException if some property of an element of the
	 *         specified collection prevents it from being added to this list
	 */
	virtual bool addAll(List<E> *c) = 0;

	/**
	 * Inserts all of the elements in the specified collection into this
	 * list at the specified position (optional operation).  Shifts the
	 * element currently at that position (if any) and any subsequent
	 * elements to the right (increases their indices).  The new elements
	 * will appear in this list in the order that they are returned by the
	 * specified collection's iterator.  The behavior of this operation is
	 * undefined if the specified collection is modified while the
	 * operation is in progress.  (Note that this will occur if the specified
	 * collection is this list, and it's nonempty.)
	 *
	 * @param index index at which to insert the first element from the
	 *              specified collection
	 * @param c collection containing elements to be added to this list
	 * @return true if this list changed as a result of the call
	 * @throws UnsupportedOperationException if the addAll operation
	 *         is not supported by this list
	 * @throws ClassCastException if the class of an element of the specified
	 *         collection prevents it from being added to this list
	 * @throws NullPointerException if the specified collection contains one
	 *         or more null elements and this list does not permit null
	 *         elements, or if the specified collection is null
	 * @throws IllegalArgumentException if some property of an element of the
	 *         specified collection prevents it from being added to this list
	 * @throws IndexOutOfBoundsException if the index is out of range
	 *         (index &lt; 0 || index &gt; size())
	 */
	virtual bool addAll(unsigned int index, List<E> *c) = 0;

	/**
	 * Removes from this list all of its elements that are contained in the
	 * specified collection (optional operation).
	 *
	 * @param c collection containing elements to be removed from this list
	 * @return true if this list changed as a result of the call
	 * @throws UnsupportedOperationException if the removeAll operation
	 *         is not supported by this list
	 * @throws ClassCastException if the class of an element of this list
	 *         is incompatible with the specified collection
	 * @throws NullPointerException if this list contains a null element and the
	 *         specified collection does not permit null elements, or if the
	 *         specified collection is null
	 */
	virtual bool removeAll(List<E> *c) = 0;

	/**
	 * Retains only the elements in this list that are contained in the
	 * specified collection (optional operation).  In other words, removes
	 * from this list all of its elements that are not contained in the
	 * specified collection.
	 *
	 * @param c collection containing elements to be retained in this list
	 * @return true if this list changed as a result of the call
	 * @throws UnsupportedOperationException if the retainAll operation
	 *         is not supported by this list
	 * @throws ClassCastException if the class of an element of this list
	 *         is incompatible with the specified collection
	 * @throws NullPointerException if this list contains a null element and the
	 *         specified collection does not permit null elements, or if the
	 *         specified collection is null
	 */
	virtual bool retainAll(List<E> *c) = 0;

	/**
	 * Removes all of the elements from this list (optional operation).
	 * The list will be empty after this call returns.
	 *
	 * @throws UnsupportedOperationException if the clear operation
	 *         is not supported by this list
	 */
	virtual void clear() = 0;

	// Comparison

	/**
	 * Compares the specified object with this list for equality.  Returns
	 * true if and only if the specified object is also a list, both
	 * lists have the same size, and all corresponding pairs of elements in
	 * the two lists are equal.  (Two elements e1 and
	 * e2 are equal if (e1==null ? e2==null :
	 * e1.equals(e2)).)  In other words, two lists are defined to be
	 * equal if they contain the same elements in the same order.  This
	 * definition ensures that the equals method works properly across
	 * different implementations of the List interface.
	 *
	 * @param o the object to be compared for equality with this list
	 * @return true if the specified object is equal to this list
	 */
	virtual bool equals(List<E> *o) = 0;

	// Positional Access Operations

	/**
	 * Inserts the specified element at the specified position in this list
	 * (optional operation).  Shifts the element currently at that position
	 * (if any) and any subsequent elements to the right (adds one to their
	 * indices).
	 *
	 * @param index index at which the specified element is to be inserted
	 * @param element element to be inserted
	 * @throws UnsupportedOperationException if the add operation
	 *         is not supported by this list
	 * @throws ClassCastException if the class of the specified element
	 *         prevents it from being added to this list
	 * @throws NullPointerException if the specified element is null and
	 *         this list does not permit null elements
	 * @throws IllegalArgumentException if some property of the specified
	 *         element prevents it from being added to this list
	 * @throws IndexOutOfBoundsException if the index is out of range
	 *         (index &lt; 0 || index &gt; size())
	 */
	virtual void add(unsigned int index, E *element) = 0;

	/**
	 * Removes the element at the specified position in this list (optional
	 * operation).  Shifts any subsequent elements to the left (subtracts one
	 * from their indices).  Returns the element that was removed from the
	 * list.
	 *
	 * @param index the index of the element to be removed
	 * @return the element previously at the specified position
	 * @throws UnsupportedOperationException if the remove operation
	 *         is not supported by this list
	 * @throws IndexOutOfBoundsException if the index is out of range
	 *         (index &lt; 0 || index &gt;= size())
	 */
	virtual E* remove(unsigned int index) = 0;

	/**
	 * Returns the element at the specified position in this list.
	 *
	 * @param index index of the element to return
	 * @return the element at the specified position in this list
	 * @throws IndexOutOfBoundsException if the index is out of range
	 *         (index &lt; 0 || index &gt;= size())
	 */
	virtual E* get(unsigned int index) = 0;

	/**
	 * Replaces the element at the specified position in this list with the
	 * specified element (optional operation).
	 *
	 * @param index index of the element to replace
	 * @param element element to be stored at the specified position
	 * @return the element previously at the specified position
	 * @throws UnsupportedOperationException if the set operation
	 *         is not supported by this list
	 * @throws ClassCastException if the class of the specified element
	 *         prevents it from being added to this list
	 * @throws NullPointerException if the specified element is null and
	 *         this list does not permit null elements
	 * @throws IllegalArgumentException if some property of the specified
	 *         element prevents it from being added to this list
	 * @throws IndexOutOfBoundsException if the index is out of range
	 *         (index &lt; 0 || index &gt;= size())
	 */
	virtual E* set(unsigned int index, E *element) = 0;

	// Search Operations

	/**
	 * Returns the index of the first occurrence of the specified element
	 * in this list, or -1 if this list does not contain the element.
	 * More formally, returns the lowest index i such that
	 * (o==null&nbsp;?&nbsp;get(i)==null&nbsp;:&nbsp;o.equals(get(i))),
	 * or -1 if there is no such index.
	 *
	 * @param e element to search for
	 * @return the index of the first occurrence of the specified element in
	 *         this list, or -1 if this list does not contain the element
	 * @throws ClassCastException if the type of the specified element
	 *         is incompatible with this list
	 * @throws NullPointerException if the specified element is null and this
	 *         list does not permit null elements
	 */
	virtual int indexOf(E *e) = 0;

	/**
	 * Returns the index of the last occurrence of the specified element
	 * in this list, or -1 if this list does not contain the element.
	 * More formally, returns the highest index i such that
	 * (o==null&nbsp;?&nbsp;get(i)==null&nbsp;:&nbsp;o.equals(get(i))),
	 * or -1 if there is no such index.
	 *
	 * @param o element to search for
	 * @return the index of the last occurrence of the specified element in
	 *         this list, or -1 if this list does not contain the element
	 * @throws ClassCastException if the type of the specified element
	 *         is incompatible with this list
	 * @throws NullPointerException if the specified element is null and this
	 *         list does not permit null elements
	 */
	virtual int lastIndexOf(E *e) = 0;

	// View

	/**
	 * Returns a view of the portion of this list between the specified
	 * fromIndex, inclusive, and toIndex, exclusive. (If fromIndex and toIndex
	 * are equal, the returned list is empty.)  The returned list is backed by
	 * this list, so non-structural changes in the returned list are reflected
	 * in this list, and vice-versa.
	 * The returned list supports all of the optional list operations supported
	 * by this list.
	 *
	 * This method eliminates the need for explicit range operations (of
	 * the sort that commonly exist for arrays).  Any operation that expects
	 * a list can be used as a range operation by passing a subList view
	 * instead of a whole list.  For example, the following idiom
	 * removes a range of elements from a list:
	 *      list.subList(from, to).clear();
	 * Similar idioms may be constructed for indexOf and
	 * lastIndexOf, and all of the algorithms in the
	 * Collections class can be applied to a subList.
	 *
	 * The semantics of the list returned by this method become undefined if
	 * the backing list (i.e., this list) is structurally modified in
	 * any way other than via the returned list.  (Structural modifications are
	 * those that change the size of this list, or otherwise perturb it in such
	 * a fashion that iterations in progress may yield incorrect results.)
	 *
	 * @param fromIndex low endpoint (inclusive) of the subList
	 * @param toIndex high endpoint (exclusive) of the subList
	 * @return a view of the specified range within this list
	 * @throws IndexOutOfBoundsException for an illegal endpoint index value
	 *         (fromIndex &lt; 0 || toIndex &gt; size ||
	 *         fromIndex &gt; toIndex)
	 */
	virtual List<E>* subList(unsigned int fromIndex, unsigned int toIndex) = 0;
};

#endif /* LIST_H_ */
