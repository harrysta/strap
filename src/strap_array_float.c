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

StrapArray *strap_array_replace_float(StrapArray *arr, size_t idx, float num)
{
	FLOAT_MACRO(strap_array_replace_num, arr, idx, num);
}

size_t strap_array_find_float(const StrapArray *arr, float num)
{
	return strap_array_nfind_float(arr, num, 0);
}

size_t strap_array_nfind_float(const StrapArray *arr, float num, size_t n)
{
	FLOAT_MACRO(strap_array_nfind_num, arr, num, n);
}
