#include "strap_internal.h"

float strap_array_get_float(const StrapArray *arr, size_t idx)
{
	struct num_array *narr;
	size_t count;
	float *iarr;

	if (!arr || arr->type != STRAP_TYPE_FLOAT)
		return 0;
	narr = arr->data;
	count = narr->count;
	iarr = (float*) narr->array;
	return idx < count ? iarr[idx] : 0;
}

StrapArray *strap_array_append_float(StrapArray *arr, float num)
{
	struct num_array *narr;
	float *iarr;

	if (!arr || arr->type != STRAP_TYPE_FLOAT)
		return arr;
	narr = arr->data;
	iarr = (float*) narr->array;
	iarr[narr->count++] = num;
	return arr;
}
