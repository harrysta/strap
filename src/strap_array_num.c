#include "strap_internal.h"
#include <string.h>

#define PRE_FUNC(ret)                 \
	if (!arr || arr->type != type)      \
		return ret;                       \
	size_t bytes = strap_sizeof(type);  \
	struct num_array *narr = arr->data; \

num_t strap_array_get_num(const StrapArray* arr, size_t idx, StrapType type)
{
	num_t nt;

	nt.long_t = 0;
	PRE_FUNC(nt);
	memcpy(&nt, (narr->array) + bytes*idx, bytes);
	return nt;
}

StrapArray* strap_array_append_num(StrapArray *arr, num_t num, StrapType type)
{
	PRE_FUNC(arr);
	memcpy(&narr->array + bytes*narr->count++, &num, bytes);
	return arr;
}
