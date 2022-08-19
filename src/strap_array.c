#include "strap_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *_strap_get_string(StrapArray_str *ptr)
{
	return (char*) ptr->array + ptr->array_size;
}

static StrapArray_str *_strap_strarray_resize(StrapArray_str *ptr,
	size_t arr_size, size_t str_size)
{
	size_t new_arr_size = ptr->array_size + arr_size;
	size_t new_str_size = ptr->string_size + str_size;
	size_t new_size = 3 * sizeof(size_t) + new_arr_size + new_str_size;
	ptr = realloc(ptr, new_size);
	if (!ptr)
		return NULL;
	char *src = _strap_get_string(ptr);
	char *dst = (char*) ptr->array + new_arr_size; 
	memcpy(dst, src, ptr->string_size);
	ptr->array_size = new_arr_size;
	ptr->string_size = new_str_size;

	// printf("new_arr_size: %lu\n", new_arr_size);
	// printf("new_str_size: %lu\n", new_str_size);
	// printf("new_size: %lu\n", new_size);
	// printf("string: %s\n", _strap_get_string(ptr));
	return ptr;
}

static void _strap_array_print(StrapArray *arr)
{
	switch (arr->type) {
		StrapArray_str *arr_s;
		char *start, *string;
		size_t i;
		case STRAP_TYPE_STRING:
			arr_s = arr->data;
			string = start = _strap_get_string(arr_s);
			printf("[ ");
			for (i = 0; i < arr_s->count; i++) {
				printf("\"%s\\0\" : %lu", string, arr_s->array[i]);
				string = start + arr_s->array[i] + 1;
				if (i < arr_s->count - 1)
					printf(", ");
			}
			printf(" ], %lu bytes, %lu bytes, %lu elements\n", arr_s->count,
				arr_s->array_size, arr_s->string_size);
			return;
		default:
			return;
	}
}

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

size_t strap_array_count(StrapArray *arr)
{
	if (!arr || arr->type != STRAP_TYPE_STRING)
		return 0;
	return ((StrapArray_str*) arr->data)->count;
}

const char* strap_array_get_cstr(StrapArray *arr, size_t index)
{
	if (!arr || arr->type != STRAP_TYPE_STRING)
		return NULL;
	StrapArray_str *ptrstr = (StrapArray_str*) arr->data;
	if (index >= ptrstr->count)
		return NULL;
	if (index)
		return _strap_get_string(ptrstr) + ptrstr->array[index - 1] + 1;
	return _strap_get_string(ptrstr);
}

int strap_array_append_cstr(StrapArray *arr, const char *str)
{
	StrapArray_str *arr_s;
	char *string;
	size_t length;

	size_t pos = 0;
	size_t string_size = 0;
	size_t array_size = 0;
	if (!arr || !str || arr->type != STRAP_TYPE_STRING)
		return 1;
	arr_s = (StrapArray_str*) arr->data;
	length = strlen(str);
	if (arr_s->count)
		pos = arr_s->array[arr_s->count - 1] + 1;
	if (pos + length >= arr_s->string_size) {
		// calculate add_string_size to be greater than the new total length
		string_size = CHUNKIFY(length, S_INIT_STR_SIZE);
	}
	if (arr_s->array_size == arr_s->count * sizeof(size_t)) {
		array_size = sizeof(size_t) * S_INIT_ARR_SIZE;
	}
	if (string_size + array_size) {
		arr_s = _strap_strarray_resize(arr_s, array_size, string_size);
		if (!arr_s)
			return 1;
		arr->data = arr_s;
	}
	string = _strap_get_string(arr_s);
	strcpy(string + pos, str);
	arr_s->array[arr_s->count++] = pos + length;
	return 0;
}
