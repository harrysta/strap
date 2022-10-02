#include "strap_internal.h"

#include <stdlib.h>
#include <string.h>

StrapArray *strap_array_alloc(StrapType type)
{
	return strap_array_nalloc(type, STRAP_INIT_CAPACITY);
}

StrapArray *strap_array_nalloc(StrapType type, size_t capacity)
{
	StrapArray *array;
	struct num_array *narr;
	struct str_array *sarr;
	char element_size;
	void *data;

	switch (type) {
		case STRAP_TYPE_CHAR:
		case STRAP_TYPE_SHORT:
		case STRAP_TYPE_INT:
		case STRAP_TYPE_LONG:
		case STRAP_TYPE_FLOAT:
		case STRAP_TYPE_DOUBLE:
			element_size = strap_sizeof(type);
			narr = malloc(sizeof(size_t)*2 - 1 + capacity*element_size);
			if (!narr)
				return NULL;
			narr->capacity = capacity;
			narr->count = 0;
			data = narr;
			break;
		case STRAP_TYPE_STRING:
			sarr = malloc(sizeof *sarr);
			if (!sarr)
				return NULL;
			sarr->capacity = capacity > STRAP_INIT_STR_SIZE ? capacity : STRAP_INIT_STR_SIZE;
			sarr->array_size = sizeof(size_t) * capacity;
			sarr->count = 0;
			data = sarr;
			break;
		default:
			return NULL;
	}
	array = malloc(sizeof *array);
	if (!array) {
		free(data);
		return NULL;
	}
	array->type = type;
	array->data = data;
	return array;
}

void strap_array_free(StrapArray *arr)
{
	if (!arr)
		return;
	free(arr->data);
	free(arr);
}

StrapArray *strap_array_clone(const StrapArray *arr)
{
	return NULL; // TODO
}

size_t strap_array_count(const StrapArray *arr)
{
	if (!arr)
		return 0;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return ((struct str_array*) arr->data)->count;
		case STRAP_TYPE_INT:
		case STRAP_TYPE_FLOAT:
			return ((struct num_array*) arr->data)->count;
		default:
			return 0;
	}
		
	return 0;
}

size_t strap_array_size(const StrapArray *arr)
{
	if (!arr)
		return 0;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return ((struct str_array*) arr->data)->capacity;
		default:
			return 0;
	}
	return 0;
}

size_t strap_array_capacity(const StrapArray *arr)
{
	if (!arr)
		return 0;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return ((struct str_array*) arr->data)->array_size / sizeof(size_t);
		default:
			return 0;
	}
	return 0;
}

StrapArray *strap_array_clear(StrapArray *arr)
{
	if (!arr)
		return NULL;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return strap_array_clear_str(arr);
		default:
		return arr;
	}
	return arr;
}

StrapArray *strap_array_erase(StrapArray *arr, size_t i)
{
	return strap_array_erase_range(arr, i, 1);
}

StrapArray *strap_array_erase_range(StrapArray *arr, size_t i, size_t n)
{
	if (!arr)
		return NULL;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return strap_array_erase_range_str(arr, i, n);
		default:
			return arr;
	}
	return arr;
}

StrapArray *strap_array_create_subarray(const StrapArray *arr, size_t idx, size_t n)
{
	if (!arr || n == 0)
		return NULL;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return strap_array_create_subarray_str(arr, idx, n);
		default:
			return NULL;
	}
	return NULL;
}

StrapArray *strap_array_reverse(StrapArray *arr)
{
	if (!arr)
		return NULL;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return strap_array_reverse_str(arr);
		default:
			return NULL;
	}
	return NULL;
}

StrapArray *strap_array_shrink(StrapArray *arr)
{
	if (!arr)
		return NULL;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return strap_array_shrink_str(arr);
		default:
			return NULL;
	}
	return NULL;
}


StrapArray *strap_array_sort(StrapArray *arr, int ascending)
{
	if (!arr)
		return NULL;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return strap_array_sort_str(arr, ascending);
		default:
			return NULL;
	}
	return NULL;
}


int strap_array_sprintf(const StrapArray *arr, char *cstr)
{
	if (!arr)
		return -1;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return strap_array_sprintf_str(arr->data, cstr);
		default:
			return -1;
	}
	return -1;
}

int strap_array_fprintf(const StrapArray *arr, FILE *stream)
{
	if (!arr)
		return -1;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return strap_array_fprintf_str(arr->data, stream);
		case STRAP_TYPE_INT:
			return strap_array_fprintf_int(arr->data, stream);
		default:
			return -1;
	}
	return -1;
}
