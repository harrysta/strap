#include "strap_internal.h"
#include "strap_array_num.h"

#define FLOAT_MACRO(MACRO, ...) MACRO(__VA_ARGS__, STRAP_TYPE_FLOAT, float)

float strap_array_get_float(const StrapArray *arr, size_t idx)
{
	FLOAT_MACRO(strap_array_get_num, arr, idx);
}

StrapArray *strap_array_append_float(StrapArray *arr, float num)
{
	FLOAT_MACRO(strap_array_append_num, arr, num);
}
