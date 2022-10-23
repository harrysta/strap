#include "strap_internal.h"
#include "strap_array_num.h"
#include <string.h>

short strap_array_get_short(const StrapArray *arr, size_t idx)
{
	SHORT_MACRO(strap_array_get_num, arr, idx);
}

StrapArray *strap_array_append_short(StrapArray *arr, short num)
{
	SHORT_MACRO(strap_array_append_num, arr, num);
}

StrapArray *strap_array_insert_short(StrapArray *arr, size_t idx, short num)
{
	SHORT_MACRO(strap_array_insert_num, arr, idx, num);
}

StrapArray *strap_array_replace_short(StrapArray *arr, size_t idx, short num)
{
	SHORT_MACRO(strap_array_replace_num, arr, idx, num);
}

size_t strap_array_find_short(const StrapArray *arr, short num)
{
	return strap_array_nfind_short(arr, num, 0);
}

size_t strap_array_nfind_short(const StrapArray *arr, short num, size_t n)
{
	SHORT_MACRO(strap_array_nfind_num, arr, num, n);
}
