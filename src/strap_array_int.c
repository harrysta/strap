#include "strap_internal.h"
#include <stdio.h>
#include <string.h>

int strap_array_get_int(const StrapArray *arr, size_t idx)
{
	size_t count;
	int *iarr;

	NARR_INIT(0, STRAP_TYPE_INT);
	count = narr->count;
	iarr = (int*) narr->array;
	return idx < count ? iarr[idx] : 0;
}

StrapArray *strap_array_append_int(StrapArray *arr, int num)
{
	int *iarr;

	NARR_INIT(0, STRAP_TYPE_INT);
	iarr = (int*) narr->array;
	iarr[narr->count++] = num;
	return arr;
	// TODO ensure size
}

StrapArray *strap_array_insert_int(StrapArray *arr, size_t idx, int num)
{
	size_t count;
	int *iarr;

	NARR_INIT(0, STRAP_TYPE_INT);
	count = narr->count;
	iarr = (int*) narr->array;
	if (!count && !idx || count && idx == count) {
		iarr[narr->count++] = num;
		return arr;
	} else if (count && idx >= count) {
		return arr;
	}
	memcpy(&iarr[idx + 1], &iarr[idx], sizeof(int)*(count - idx));
	iarr[idx] = num;
	narr->count++;
	return arr;
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

