#include "strap_internal.h"
#include "strap_internal_num_array.h"

float strap_array_get_float(const StrapArray *arr, size_t idx)
{
	struct float_array *farr;

	if (!arr || arr->type != STRAP_TYPE_INT)
		return 0;
	farr = (struct float_array*) arr->data;
	return farr->array[idx];
}

StrapArray *strap_array_append_float(StrapArray *arr, float num)
{
	struct float_array *farr;

	if (!arr || arr->type != STRAP_TYPE_INT)
		return arr;
	farr = (struct float_array*) arr->data;
	STRAP_ARRAY_APPEND_NUM(farr, num);
}
