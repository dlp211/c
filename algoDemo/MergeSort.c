
#include <stdio.h>
#include <stdlib.h>

void sort(int*, int*);
void merge(int*, int*, int*);

int main(void)
{
	unsigned i;
	int arr[10] = {10,9,8,7,6,5,4,3,2,1};

	sort(&arr[0], &arr[9]);

	for (i = 0; i < 10; ++i)
		printf("%d ", arr[i]);
	printf("\n");

	return 0;
}

void sort(int *begin, int *end)
{
	if (begin < end) {
		int *middle = (end - begin) / 2 + begin;
		sort(begin, middle);
		sort(middle + 1, end);
		merge(begin, middle + 1, end);
	}
}

void merge(int *begin, int *middle, int *end)
{
	int *iter = begin;
	unsigned l_size = middle - begin;
	unsigned r_size = end - middle + 1;
	int *l_arr = malloc(sizeof(int) * l_size);
	int *l_iter = l_arr;
	int *l_iter_e = l_arr + l_size;
	int *r_arr = malloc(sizeof(int) * r_size);
	int *r_iter = r_arr;
	int *r_iter_e = r_arr + r_size;
	
	while (iter < middle) {
		*l_iter++ = *iter++;
	}
	while (iter <= end) {
		*r_iter++ = *iter++;
	}

	l_iter = l_arr;
	r_iter = r_arr;

	while (l_iter < l_iter_e && r_iter < r_iter_e) {
		if (*l_iter < *r_iter)
			*begin++ = *l_iter++;
		else
			*begin++ = *r_iter++;
	}

	while (l_iter < l_iter_e)
		*begin++ = *l_iter++;
	while (r_iter < r_iter_e)
		*begin++ = *r_iter++;

	free(l_arr);
	free(r_arr);
}