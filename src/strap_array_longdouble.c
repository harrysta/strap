#include "strap_internal.h"
#include "strap_array_num.h"
#include <string.h>

long double strap_array_get_longdouble(const StrapArray *arr, size_t idx)
{
	LONGDOUBLE_MACRO(strap_array_get_num, arr, idx);
}

StrapArray *strap_array_append_longdouble(StrapArray *arr, long double num)
{
	LONGDOUBLE_MACRO(strap_array_append_num, arr, num);
}

StrapArray *strap_array_replace_longdouble(StrapArray *arr, size_t idx, long double num)
{
	LONGDOUBLE_MACRO(strap_array_replace_num, arr, idx, num);
}

size_t strap_array_find_longdouble(const StrapArray *arr, long double num)
{
	return strap_array_nfind_longdouble(arr, num, 0);
}

size_t strap_array_nfind_longdouble(const StrapArray *arr, long double num, size_t n)
{
	LONGDOUBLE_MACRO(strap_array_nfind_num, arr, num, n);
}
