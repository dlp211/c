/******************************************************************************
 *    FILE: SelectionSort.c                                                           *
 *    AUTHOR: David L Patrzeba                                                *
 *    E-MAIL: david.patrzeba@gmail.com                                        *                                  *
 *                                                                            *
 *    The MIT Liscense                                                        *
 *    Copyright (c) 2013 David L patrzeba                                     *
 *                                                                            *
 *    Permission is hereby granted, free of charge, to any person obtaining a *
 *    copy of this software and associated documentation files (the           *
 *    "Software"), to deal in the Software without restriction, including     *
 *    without limitation the rights to use, copy, modify, merge, publish,     *
 *    distribute, sublicense, and/or sell copies of the Software, and to      *
 *    permit persons to whom the Software is furnished to do so, subject to   *
 *    the following conditions:                                               *
 *                                                                            *
 *    The above copyright notice and this permission notice shall be included *
 *    in all copies or substantial portions of the Software.                  *
 *                                                                            *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS *
 *    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              *
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  *
 *    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    *
 *    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    *
 *    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       *
 *    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  *
 *                                                                            *
 ******************************************************************************/

/*************************************************************
 * Just a simple Selection Sort Demonstration using an int[] *
 *************************************************************/

 #include <stdio.h>
 #define ARRAYSIZE sizeof(arr)/sizeof(arr[0])

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void sort(int *arr, unsigned size)
{
	unsigned i, j;
	int *smallest;
	for(i = 0; i < size - 1; ++i) {
		smallest = &arr[i];
		for(j = i + 1; j < size; ++j) {
			if(arr[j] < *smallest)
				smallest = &arr[j];
		}
		swap(smallest, &arr[i]);
	}

}

int main(int argc, char **argv)
{
	unsigned i;
	int arr[10] = {10,9,8,7,6,5,4,3,2,1};
 	sort(arr, ARRAYSIZE);
 	for(i = 0; i < ARRAYSIZE; ++i)
 		printf("%d ", arr[i]);
 	printf("\n");

 	return 0;
}