/*
 * file: sorted-list.c
 * author: David L Patrzeba (c) holder
 * author: Jesse Ziegler (c) holder
 * license: MIT (http://opensource.org/licenses/MIT) (c) 2013
 */

#include <stdio.h>
#include "sorted-list.h"
#define SUCCESS 1

/* Run in O(1) time
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list.
 *
 * arg: cf is a comparator function to use to keep the list sorted.
 *
 * return: Non-Null SortedListPtr, NULL otherwise.
 */

/*TODO v3 of this library:
 * This should take a Destroyer function pointer to free the stored
 * objects*/
SortedListPtr SLCreate( CompareFuncT cf ) {
  SortedListPtr sorted_list = malloc( sizeof(SortedList) );

  //checks to see if what was given to us is valid
  if ( cf && sorted_list ) {
    *sorted_list = (SortedList) { cf, NULL, NULL, 0 };
    return sorted_list;
  }
  return NULL;
}

/* Runs in O(n) time
 *
 * This function is DEPRECATED
 *
 * SLDestroy destroys a list, freeing all dynamically allocated memory allocated
 * by the library.
 *
 * arg: list is a pointer to the sorted list to destroy.
 *
 * WARNING: SLDestroy does not free the objects stored in the list.
 * WARNING: IT IS THE END USERS RESPONSIBILITY TO FREE THEIR OBJECTS.
 */
void SLDestroy(SortedListPtr list) {

  //checks to see if what was given to us is valid
  if ( !list ) {
    return;
  }

  Container *container = list->tail->prev;
  for ( ; container; container = container->prev ) {
    free ( container->next );
  }
  if ( list->head ) {
    free ( list->head );
  }
  free(list);
}

/* Runs in O(1) time.
 * A helper function for inserting into an empty list.
 *
 * arg: list is a pointer to the SortedList that the object should be inserted.
 * arg: newObj is the object to be inserted.
 *
 * return: 1 on success, 0 otherwise.
 */
int insertEmpty ( SortedListPtr list, void *newObj ) {

  Container *container = malloc ( sizeof(Container) );

  //check to make sure malloc didn't return NULL
  if ( container ) {
    *container = (Container) { newObj, NULL, NULL, 2 };
    list->head = list->tail = container;
    ++list->size;
    return SUCCESS;
  }
  return 0;
}

/* Runs in O(1) time.
 * A helper function to insert a container at the begining of a list.
 *
 * arg: list is the poiner to the sorted list.
 * arg: newObj is a pointer to the obj to be inserted.
 * arg: container is the pointer to the container that will hold the object.
 *
 * returns SUCCESS on completion
 */
int insertBegin ( SortedListPtr list, void *newObj, Container *container ) {
  *container = (Container) { newObj, list->head, NULL, 2 };
  list->head = list->head->prev = container;
  ++list->size;
  return SUCCESS;
}

/* Runs in O(1) time.
 * A helper function to insert a container in between other containers.
 *
 * arg: list is the pointer to the sorted list.
 * arg: newObj is a void pointer to the object to insert into the list.
 *
 * returns SUCCESS on completion
 */
int insertIn (
    SortedListPtr list,
    void *newObj,
    Container* current,
    Container *container
) {
  *container = (Container) { newObj, current, current->prev, 2 };
  current->prev = current->prev->next = container;
  ++list->size;
  return SUCCESS;
}

/* Runs in O(1) time.
 * A helper function to insert a container at the end of a list.
 *
 * arg : list is the pointer to the sorted list.
 * arg: newObj is a pointer to the obj to be inserted.
 * arg: container is the pointer to the container that will hold the object.
 *
 * returns SUCCESS on completion
 */
int insertEnd ( SortedListPtr list, void *newObj, Container *container ) {
  *container = (Container) { newObj, NULL, list->tail, 2 };
  list->tail = list->tail->next = container;
  ++list->size;
  return SUCCESS;
}

/* Runs in O(n) time.
 * A helper method for inserting a new object into an non-empty sorted list.
 *
 * arg: list is a pointer to the SortedList that the object should be inserted.
 * arg: newObj is the object to be inserted.
 *
 * return: 1 on success, 0 otherwise.
 */
int insert ( SortedListPtr list, void *newObj ) {

  Container *current;
  Container *container = malloc ( sizeof(Container) );

  //checks to see if what was given to us is valid and make sure malloc didn't
  //return null
  if ( !container || !list || !newObj ) { return 0; }

  if ( list->compare ( newObj, list->head->value ) >= 0 ) {
    return insertBegin ( list, newObj, container);
  }

  if ( list->compare ( newObj, list->tail->value ) <= 0 ) {
    return insertEnd( list, newObj, container );
  }

  //iterate over the list until we find an insert spot
  for ( current = list->head->next;
        list->compare ( newObj, current->value ) < 0 && current->next;
        current = current->next ) { ; /*No Operation*/ }
  return insertIn ( list, newObj, current, container );
}

/* Runs in O(n) time.
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * arg: list is a pointer to SortedList for the new object to be inserted.
 * arg: newObj is a pointer to a new object to be inserted.
 *
 * return: 1 on success, 0 otherwise.
 */
int SLInsert ( SortedListPtr list, void *newObj ) {

  //checks to see if what was given to us is valid
  if ( !list || !newObj ) {
    return 0;
  }

  if ( list->size ) {
    return insert(list, newObj);
  }
  return insertEmpty(list, newObj);
}

/* Runs in O(n) time.
 * THIS FUNCTION HAS BEEN DEPRECATED
 *
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.
 *
 * arg: list is a pointer to SortedList for the matching object be deleted.
 * arg: newObj is a pointer to the object to be deleted.
 *
 * return: 1 on success, 0 otherwise.
 *
 * WARNING: IF YOU REMOVE FROM THE LIST AND YOU DON'T HAVE A POINTER TO THE
 * OBJECT TO FREE IT YOU WILL LEAK MEMORY.
 */
int SLRemove(SortedListPtr list, void *newObj) {
  //This function is part of the memory safe operations and at the bottom of the
  //file.
  return (SLGet(list, newObj) ? 1 : 0);
}

/* Runs in O(1) time.
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * arg: SortedListPtr to a list to generate a new SortedListIterator.
 *
 * returns a non-NULL SortedListIterT object, else returns NULL.
 */
SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {

  //checks to see if what was given to us is valid
  if ( !list || !list->size ) {
    return NULL;
  }

  SortedListIteratorPtr iter = malloc( sizeof( SortedListIterator ) );
  if ( iter ) {
    *iter = ( SortedListIterator ) { list->head };
    ++iter->iterator->count;
    return iter;
  }
  return NULL;
}

/* Runs in O(1) time.
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  And does not effect the list.
 *
 * arg: iter is a SortedListIteratorPointer which should be destroyed.
 */
void SLDestroyIterator(SortedListIteratorPtr iter) {

  //checks to see if what was given to us is valid
  if ( !iter ) {
    return;
  }
  free ( iter );
}

/* Runs in O(1) time for normal case, but can degrade to O(n).
 * SLNextItem returns the next object in the list encapsulated by the
 * given iterator.  It returns a NULL when the end of the list
 * has been reached.
 *
 * This iterator has the following characterstics:
 *
 * It begins by pointing to the
 * first item in the list when it is created.  If you add new items to the front
 * of the list, the iterator does not know, nor care about them.
 *
 * If it is pointing to an object that hasn't been removed, it returns the void*
 * and moves to the next item.
 *
 * If the item it is pointing to has been removed, it checks to see
 * if it is the last thing pointing to it, if it is, it frees the containers
 * and moves onto the next object. It does this until it reaches the end of
 * the list or an object value that hasn't been freed. When it gets to this
 * value it returns it and then moves to the next item.
 *
 * If you insert an item inbetween a valid item and a removed item, the iterator
 * does not know about it and will not return it.
 *
 * arg: iter is a SortListIteratorPtr which will return the next object.
 * return: Returns a void* to the next object in the sorted list.
 */
void *SLNextItem(SortedListIteratorPtr iter) {

  Container *temp = NULL;

  //checks to see if what was given to us is valid
  if ( !iter || !iter->iterator ) {
    return NULL;
  }

  //Move past removed items until a non-removed pointer is found or hit the
  //end of the list.  Frees the container if it is the last thing pointing to
  //a removed container.
  while ( iter->iterator && iter->iterator->count <= 0 ) {
    ++iter->iterator->count;
    //Garbage Collection when reference count equals 0
    if ( iter->iterator->count == 0 ) {
      temp = iter->iterator;
      iter->iterator = iter->iterator->next;
      free ( temp );
    } else {
      iter->iterator = iter->iterator->next;
      //if the next container is removed decrement the count away from 0
      if ( iter->iterator->count < 0 )
        --iter->iterator->count;
      //else the next container is valid and increment away from 0
      else
        ++iter->iterator->count;
    }
  }

  //make sure we aren't at the EOL
  if ( !iter->iterator ) { return NULL; }

  //check for last item in the list
  if ( !iter->iterator->next ) {
    temp = iter->iterator;
    --temp->count;
    iter->iterator = NULL;
    return temp->value;
  }

  //handle normal iteration
  --iter->iterator->count;
  iter->iterator = iter->iterator->next;
  ++iter->iterator->count;
  return iter->iterator->prev->value;
}

/*******************************************************************************
 * THESE FUNCTIONS HELP YOU TO NOT LEAK MEMORY AND ARE IN ADDITION TO THE
 * ORIGINAL DEFINED API
 ******************************************************************************/

/* Runs in O(1) time.
 * A helper function for actually removing an item from the list
 *
 * arg: list is a pointer to the SortedList to remove a Container from.
 * arg: current is a pointer to the first object that matches the looked for
 * value.
 * arg: compareTo is >= 0, if it isn't 0 then the value is not in the list.
 *
 * returns a void* to the object contained in the list that is being removed so
 * that the library user can manage the memory properly.
 */
void *getValue ( SortedListPtr list, Container *current, int compareTo ) {

  void * ret = current->value;

  //make sure current isn't off the EOL and compareTo is 0
  if ( !current || compareTo ) { return NULL; }

  if ( current->prev ) {
    current->prev->next = current->next;
    if ( current->count != 2 ) {
      ++current->next->count;
    }
  }
  //handle the front of the list
  else { list->head = current->next; }

  if ( current->next ) {
    current->next->prev = current->prev;
    if ( current->count != 2) {
      ++current->prev->count;
    }
  }
  //handle the end of the list
  else { list->tail = current->prev; }

  current->count -= 2;

  //free the container if nothing is pointing to it.
  if ( !current->count ) {
    free ( current );
  }
  else { current->count *= -1; }

  --list->size;
  return ret;
}

/* Runs in O(n) time.
 * SLGet replace SLRemove.  It has the same functionality except it returns the
 * void* in so the library user can manage their memory.
 *
 * arg: list is a pointer to SortedList for the matching object be deleted.
 * arg: newObj is a pointer to the object to be deleted.
 *
 * return: void* of the removed object on success, NULL otherwise.
 */
void *SLGet(SortedListPtr list, void *newObj) {

  Container *current = list->head;
  int compareTo = 0;

  //checks to see if what was given to us is valid
  if ( !list || !newObj || !list->size ) {
    return NULL;
  }

  //iterate until we find a match
  for ( ;
        current && (compareTo = list->compare ( newObj, current->value )) < 0;
        current = current->next ) { ;/* No Operation  */ }
  return getValue ( list, current, compareTo );
}


/* Runs in O(n) time.
 * After calling this function, all the values in the list will have been freed.
 * This function should be called before calling SLDestroy.  This helps the
 * library user properly manage their memory.
 *
 * arg: list is a SortedListPtr that all values should be destroyed for.
 * arg: destroy is a function pointer to a method that knows how to free the
 * void* object
 *
 * WARNING: The list behavior is undefined after calling this function as
 * everything will be freed
 */
void SLDestroyValuesAndList( SortedListPtr list, DestroyFuncT destroy ) {

  SortedListIteratorPtr iter = SLCreateIterator(list);
  void * value = NULL;

  //checks to see if what was given to us is valid
  if ( !list || !destroy ) { return; }

  while ( ( value = SLNextItem(iter) ) ) {
    destroy ( value );
  }
  SLDestroy( list );
}

