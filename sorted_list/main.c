/*
 * sorted-list.c
 */

#include <string.h>
#include <stdio.h>
#include "sorted-list.h"

int array[10] = {20,18,16,14,12,10,8,6,4,2}; /*Main Test Array*/

int compareInts(void *p1, void *p2) {
  int i1 = *(int*)p1;
  int i2 = *(int*)p2;

  return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
  double d1 = *(double*)p1;
  double d2 = *(double*)p2;

  return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
  char *s1 = p1;
  char *s2 = p2;

  return strcmp(s1, s2);
}

void nl(){
  printf("\n");
}

void printSortedList( SortedListPtr sl, char* s ) {

  printf("%s\n", s);
  SortedListIteratorPtr slip = SLCreateIterator(sl);
  int *printValue = NULL;
  while ( ( printValue = (int*) SLNextItem(slip) ) ) {
    printf("%d ", *printValue);
  }
  printf("\n");
  SLDestroyIterator(slip);
}

void  insertBeginingTest ( SortedListPtr sl ) {

  SLInsert(sl, &array[3]);
  printSortedList(sl, "Insert Beginning Test");
  nl();
}

void removeBeginingTest( SortedListPtr sl ) {
  SLRemove ( sl, &array[3] );
  printSortedList(sl, "Remove Begining Test");
  nl();
}

void insertEndTest( SortedListPtr sl ){

  SLInsert(sl, &array[7]);
  printSortedList(sl, "Insert End test" );
  nl();
}

void removeEndTest( SortedListPtr sl ){

  SLRemove(sl, &array[7]);

  printSortedList(sl, "Remove End Test");
  nl();
}

void buildListDuplicateTest( SortedListPtr sl ){

  int i =0;

  for(;i<10;i++){
  SLInsert(sl, &array[i]);
  }
  printSortedList(sl, "Creating entire List, should have one Duplicate");
  nl();
}

void removeDuplicateTest( SortedListPtr sl ){
  SLRemove(sl, &array[5]);
  printSortedList(sl, "Removing duplicate. "
      "Only one item is Removed from the list.");
  nl();
}



void iteratorRemoveTest( SortedListPtr sl ){

  printf("Iterator Remove Test");
  //Put an iterator to head of list
  SortedListIteratorPtr slip = SLCreateIterator(sl);

  SLNextItem(slip);
  SLNextItem(slip);

  printSortedList(sl,"");
  /*Removing iterators current item and proceeding item*/
  SLRemove(sl, &array[2]);
  SLRemove(sl, &array[3]);


  printf("Iterator current item: %d", *(int*) SLNextItem(slip));

  printSortedList(sl, "" );
  nl();
  SLDestroyIterator(slip);

 SLInsert(sl, &array[2]);
 SLInsert(sl, &array[3]);

}


void complexIteratorRemoveTest( SortedListPtr sl ){
  printf("Multiple Iterator Remove Test");
  /*Creating Three Iterators*/
  SortedListIteratorPtr slip = SLCreateIterator(sl);
  SortedListIteratorPtr slip2 = SLCreateIterator(sl);
  SortedListIteratorPtr slip3 = SLCreateIterator(sl);

  /*Moving each iterator two spaces*/
  SLNextItem(slip);
  SLNextItem(slip);
  SLNextItem(slip2);
  SLNextItem(slip2);
  SLNextItem(slip3);
  SLNextItem(slip3);

  printSortedList(sl,"");

  SLRemove(sl, &array[2]);
  SLRemove(sl, &array[3]);

  SLInsert(sl, &array[3]);

  printf("1st Iterator current item: %d", *(int*) SLNextItem(slip));
  printf("\n2nd Iterator current item: %d", *(int*) SLNextItem(slip2));
  printf("\n3rd Iterator current item: %d", *(int*) SLNextItem(slip3));

  printSortedList(sl, "" );

  SLDestroyIterator(slip);
  SLDestroyIterator(slip2);
  SLDestroyIterator(slip3);
}

int main(void) {

  //EMPTY LIST
  SortedListPtr sl = SLCreate(compareInts);
  //Start with a list of just 10

  SLInsert(sl, &array[5]);

  //Insert into begining of list, print
  insertBeginingTest( sl );
  //Remove that same item and print.
  removeBeginingTest(sl);

  //List is still list of just 10 here
  insertEndTest(sl);
  removeEndTest(sl);
  buildListDuplicateTest(sl);
  removeDuplicateTest(sl);
  iteratorRemoveTest(sl);
  complexIteratorRemoveTest(sl);

  SLDestroy(sl);
  return EXIT_SUCCESS;
}

