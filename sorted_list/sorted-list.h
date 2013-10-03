#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 * file: sorted-list.h
 * author: David L Patrzeba (c) holder
 * author: Jesse Ziegler (c) holder
 * license: MIT (http://opensource.org/licenses/MIT) (c) 2013
 */
#include <stdlib.h>

/*
 * This library runs mostly in O(n) time.  To see individual function O() run
 * times see the sorted-list.c comments for and see the readme for additional
 * memory managment advice.
 */

/*
 * A container for holding a pointer to the object in the list and the next
 * container in the list.
 * param: value is a pointer to an unkown object that is held in the list.
 * param: next is a pointer to Container to navigate the list.
 * param: prev is a pointer to Container to navigate the list.
 * param: count is an int that will be used to keep a count of persistant ptrs.
 */
struct Container {
  void *value;
  struct Container *next;
  struct Container *prev;
  int count;
};
typedef struct Container Container;

/*
 * Pointer to a Comparator function for keeping the list sorted.
 *
 * You can expect a comparator function to return an value < 0 if the 1st object
 * is smaller, 0 if the two objects are equal, and a value > 0 if the 2nd object
 * is smaller.
 */
typedef int (*CompareFuncT)(void *, void *);

/*
 * Pointer to a destroy function to all the library user to properly manage
 * memory when removing items from the list.
 */
typedef int (*DestroyFuncT)(void *);

/*
 * Sorted list type used for building a sorted linked list.
 * param: compare is a function to compare to objects.
 * param: head is a pointer to the first Container in the list.
 * param: tail is a pointer to the last Container in the list.
 * param: size is an unsigned int that holds the number of containers in the
 * list.
 */
struct SortedList {
  CompareFuncT compare;
  Container *head;
  Container *tail;
  unsigned size;
};
typedef struct SortedList* SortedListPtr;
typedef struct SortedList SortedList;

/*
 * Iterator type for user to "walk" through the list item by item, from
 * beginning to end.
 * param: iterator is the current container in the iteration.
 */
struct SortedListIterator {
  Container *iterator;
};
typedef struct SortedListIterator* SortedListIteratorPtr;
typedef struct SortedListIterator SortedListIterator;

/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list.
 *
 * If the function succeeds, it returns a (non-NULL) SortedListT object.
 * Else, it returns NULL.
 */
SortedListPtr SLCreate(CompareFuncT cf);

/* SLDestroy destroys a list, freeing all dynamically allocated memory. */
void SLDestroy(SortedListPtr list);

/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 */
int SLInsert(SortedListPtr list, void *newObj);

/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 */
int SLRemove(SortedListPtr list, void *newObj);

/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL SortedListIterT object.
 * Else, it returns NULL.
 */
SortedListIteratorPtr SLCreateIterator(SortedListPtr list);

/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affectt the original list used to create
 * the iterator in any way.
 */
void SLDestroyIterator(SortedListIteratorPtr iter);

/*
 * SLNextItem returns the next object in the list encapsulated by the
 * given iterator.  It should return a NULL when the end of the list
 * has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 */
void *SLNextItem(SortedListIteratorPtr iter);

/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.
 *
 * arg: list is a pointer to SortedList for the matching object be deleted.
 * arg: newObj is a pointer to the object to be deleted.
 * return: void * on success, NULL otherwise.
 * WARNING: IF YOU REMOVE FROM THE LIST AND YOU DON'T HAVE A POINTER TO THE
 * OBJECT TO FREE IT YOU WILL LEAK MEMORY.
 */
void *SLGet(SortedListPtr list, void *newObj);

/*
 * After calling this function, all the values in the list will have been free
 */
void SLDestroyValuesAndList( SortedListPtr list, DestroyFuncT destroy );

#endif

