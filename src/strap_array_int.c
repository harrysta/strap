#include "strap_internal.h"
#include "strap_array_num.h"
#include <stdio.h>
#include <string.h>

#define INT_MACRO(MACRO, ...) MACRO(__VA_ARGS__, STRAP_TYPE_INT, int)

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
