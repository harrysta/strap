#include "strap_internal.h"
#include "strap_array_num.h"
#include <string.h>

double strap_array_get_double(const StrapArray *arr, size_t idx)
{
	DOUBLE_MACRO(strap_array_get_num, arr, idx);
}

StrapArray *strap_array_append_double(StrapArray *arr, double num)
{
	DOUBLE_MACRO(strap_array_append_num, arr, num);
}

StrapArray *strap_array_replace_double(StrapArray *arr, size_t idx, double num)
{
	DOUBLE_MACRO(strap_array_replace_num, arr, idx, num);
}

size_t strap_array_find_double(const StrapArray *arr, double num)
{
	return strap_array_nfind_double(arr, num, 0);
}

size_t strap_array_nfind_double(const StrapArray *arr, double num, size_t n)
{
	DOUBLE_MACRO(strap_array_nfind_num, arr, num, n);
}
