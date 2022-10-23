#include "strap_internal.h"
#include "strap_array_num.h"
#include <string.h>

long strap_array_get_long(const StrapArray *arr, size_t idx)
{
	LONG_MACRO(strap_array_get_num, arr, idx);
}

StrapArray *strap_array_append_long(StrapArray *arr, long num)
{
	LONG_MACRO(strap_array_append_num, arr, num);
}

StrapArray *strap_array_insert_long(StrapArray *arr, size_t idx, long num)
{
	LONG_MACRO(strap_array_insert_num, arr, idx, num);
}

StrapArray *strap_array_replace_long(StrapArray *arr, size_t idx, long num)
{
	LONG_MACRO(strap_array_replace_num, arr, idx, num);
}

size_t strap_array_find_long(const StrapArray *arr, long num)
{
	return strap_array_nfind_long(arr, num, 0);
}

size_t strap_array_nfind_long(const StrapArray *arr, long num, size_t n)
{
	LONG_MACRO(strap_array_nfind_num, arr, num, n);
}
