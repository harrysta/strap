#include "strap_internal.h"

#include <stdlib.h>
#include <string.h>

StrapArray *strap_array_alloc(StrapType type)
{
	StrapArray *array;
	void *data;

	switch (type) {
		StrapArray_str *arr_s;
		case STRAP_TYPE_STRING:
			arr_s = malloc(sizeof *arr_s);
			if (!arr_s)
				return NULL;
			arr_s->string_size = S_INIT_STR_SIZE;
			arr_s->array_size = sizeof(size_t) * S_INIT_ARR_SIZE;
			arr_s->count = 0;
			data = arr_s;
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

size_t strap_array_count(const StrapArray *arr)
{
	if (!arr || arr->type != STRAP_TYPE_STRING)
		return 0;
	return ((StrapArray_str*) arr->data)->count;
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
		default:
			return -1;
	}
	return -1;
}
