#include "strap_internal.h"
#include "strap_array_num.h"

float strap_array_get_float(const StrapArray *arr, size_t idx)
{
	return strap_array_get_num(arr, idx, STRAP_TYPE_FLOAT).float_t;
}

StrapArray *strap_array_append_float(StrapArray *arr, float num)
{
	return strap_array_append_num(arr, (num_t) { .float_t = num }, STRAP_TYPE_FLOAT);
}
