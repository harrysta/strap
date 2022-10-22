#include "strap_internal.h"
#include "strap_array_num.h"
#include <string.h>

#define CHAR_MACRO(MACRO, ...) MACRO(__VA_ARGS__, STRAP_TYPE_CHAR, char)

char strap_array_get_char(const StrapArray *arr, size_t idx)
{
	CHAR_MACRO(strap_array_get_num, arr, idx);
}

StrapArray *strap_array_append_char(StrapArray *arr, char num)
{
	CHAR_MACRO(strap_array_append_num, arr, num);
}

StrapArray *strap_array_insert_char(StrapArray *arr, size_t idx, char num)
{
	CHAR_MACRO(strap_array_insert_num, arr, idx, num);
}

StrapArray *strap_array_replace_char(StrapArray *arr, size_t idx, char num)
{
	CHAR_MACRO(strap_array_replace_num, arr, idx, num);
}

size_t strap_array_find_char(const StrapArray *arr, char num)
{
	return strap_array_nfind_char(arr, num, 0);
}

size_t strap_array_nfind_char(const StrapArray *arr, char num, size_t n)
{
	CHAR_MACRO(strap_array_nfind_num, arr, num, n);
}
