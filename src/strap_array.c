#include "strap_internal.h"

#include <stdlib.h>
#include <string.h>

StrapArray *strap_array_alloc(StrapType type)
{
	return strap_array_nalloc(type, STRAP_INIT_CAPACITY);
}

StrapArray *strap_array_nalloc(StrapType type, size_t size)
{
	StrapArray *array;
	void *data;
	void *arr;
	struct num_array *narr;

	switch (type) {
		StrapArray_str *arr_s;
		case STRAP_TYPE_STRING:
			arr_s = malloc(sizeof *arr_s);
			if (!arr_s)
				return NULL;
			arr_s->string_size = size > STRAP_INIT_STR_SIZE ? size : STRAP_INIT_STR_SIZE;
			arr_s->array_size = sizeof(size_t) * size;
			arr_s->count = 0;
			data = arr_s;
			goto array_init;
		case STRAP_TYPE_INT:   arr = malloc(sizeof(struct int_array));   break;
		case STRAP_TYPE_FLOAT: arr = malloc(sizeof(struct float_array)); break;
		default:
		return NULL;
	}
	// this section is only for num types: string should skip this
	if (!arr)
		return NULL;
	narr = (struct num_array*) arr;
	narr->count = 0;
	narr->capacity = size;
	data = arr;
array_init:
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
			return ((StrapArray_str*) arr->data)->count;
		case STRAP_TYPE_INT:
			return ((struct int_array*) arr->data)->count;
		case STRAP_TYPE_FLOAT:
			return ((struct int_array*) arr->data)->count;
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
			return ((StrapArray_str*) arr->data)->string_size;
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
			return ((StrapArray_str*) arr->data)->array_size / sizeof(size_t);
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
