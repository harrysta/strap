#include "strap_internal.h"
#include "strap_array_num.h"
#include <string.h>

int strap_array_get_int(const StrapArray *arr, size_t idx)
{
	INT_MACRO(strap_array_get_num, arr, idx);
}

StrapArray *strap_array_append_int(StrapArray *arr, int num)
{
	INT_MACRO(strap_array_append_num, arr, num);
}

StrapArray *strap_array_insert_int(StrapArray *arr, size_t idx, int num)
{
	INT_MACRO(strap_array_insert_num, arr, idx, num);
}

StrapArray *strap_array_replace_int(StrapArray *arr, size_t idx, int num)
{
	INT_MACRO(strap_array_replace_num, arr, idx, num);
}

size_t strap_array_find_int(const StrapArray *arr, int num)
{
	return strap_array_nfind_int(arr, num, 0);
}

size_t strap_array_nfind_int(const StrapArray *arr, int num, size_t n)
{
	INT_MACRO(strap_array_nfind_num, arr, num, n);
}
