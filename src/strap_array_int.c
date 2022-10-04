#include "strap_internal.h"
#include "strap_array_num.h"
#include <stdio.h>
#include <string.h>

#define INT_MACRO(MACRO, ...) MACRO(__VA_ARGS__, STRAP_TYPE_INT, int)

int strap_array_get_int(const StrapArray *arr, size_t idx)
{
	INT_MACRO(STRAP_ARRAY_GET_NUM, arr, idx);
}

StrapArray *strap_array_append_int(StrapArray *arr, int num)
{
	INT_MACRO(STRAP_ARRAY_APPEND_NUM, arr, num);
}

StrapArray *strap_array_insert_int(StrapArray *arr, size_t idx, int num)
{
	INT_MACRO(STRAP_ARRAY_INSERT_NUM, arr, idx, num);
}

int strap_array_fprintf_int(const struct num_array *narr, FILE *stream)
{
	int n;
	int sum;
	size_t count;
	size_t i;
	int *iarr;

	count = narr->count;
	if (!count)
		return fprintf(stream, "[]");
	iarr = (int*) narr->array;
	sum = fprintf(stream, "[");
	for (i = 0; i < count; i++) {
		n = fprintf(stream, "%d", iarr[i]);
		if (n < 0) {
			return -1;
		}
		sum += n;
		if (i != count - 1)
			sum += fprintf(stream, ", ");
	}
	sum += fprintf(stream, "]");
	return sum;
}

