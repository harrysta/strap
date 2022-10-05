#include "strap_internal.h"

#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 32

int strap_array_sprintf_element_i8(const StrapArray *arr, char *buf, size_t idx)
{
	char *iarr = (char*) arr->data;
	return sprintf(buf, "%d", iarr[idx]);
}

int strap_array_sprintf_element_i16(const StrapArray *arr, char *buf, size_t idx)
{
	short *iarr = (short*) arr->data;
	return sprintf(buf, "%d", iarr[idx]);
}

int strap_array_sprintf_element_i32(const StrapArray *arr, char *buf, size_t idx)
{
	int *iarr = (int*) arr->data;
	return sprintf(buf, "%d", iarr[idx]);
}

int strap_array_sprintf_element_i64(const StrapArray *arr, char *buf, size_t idx)
{
	long *iarr = (long*) arr->data;
	return sprintf(buf, "%ld", iarr[idx]);
}

int strap_array_sprintf_element_u8(const StrapArray *arr, char *buf, size_t idx)
{
	char *iarr = (char*) arr->data;
	return sprintf(buf, "%u", iarr[idx]);
}

int strap_array_sprintf_element_u16(const StrapArray *arr, char *buf, size_t idx)
{
	short *iarr = (short*) arr->data;
	return sprintf(buf, "%u", iarr[idx]);
}

int strap_array_sprintf_element_u32(const StrapArray *arr, char *buf, size_t idx)
{
	int *iarr = (int*) arr->data;
	return sprintf(buf, "%u", iarr[idx]);
}

int strap_array_sprintf_element_u64(const StrapArray *arr, char *buf, size_t idx)
{
	long *iarr = (long*) arr->data;
	return sprintf(buf, "%lu", iarr[idx]);
}

int strap_array_sprintf_element_size_t(const StrapArray *arr, char *buf, size_t idx)
{
	size_t *iarr = (size_t*) arr->data;
	return sprintf(buf, "%lu", iarr[idx]);
}

int strap_array_sprintf_element_f32(const StrapArray *arr, char *buf, size_t idx)
{
	float *iarr = (float*) arr->data;
	return sprintf(buf, "%g", iarr[idx]);
}

int strap_array_sprintf_element_f64(const StrapArray *arr, char *buf, size_t idx)
{
	double *iarr = (double*) arr->data;
	return sprintf(buf, "%g", iarr[idx]);
}


StrapArray *strap_array_alloc(StrapType type)
{
	return strap_array_nalloc(type, STRAP_INIT_CAPACITY);
}

StrapArray *strap_array_nalloc(StrapType type, size_t capacity)
{
	StrapArray *array;
	struct str_array *sarr;
	void *data;

	switch (type) {
		case STRAP_TYPE_CHAR:
		case STRAP_TYPE_SHORT:
		case STRAP_TYPE_INT:
		case STRAP_TYPE_LONG_INT:
		case STRAP_TYPE_FLOAT:
		case STRAP_TYPE_DOUBLE:
			data = malloc(capacity*strap_sizeof(type));
			if (!data)
				return NULL;
			break;
		case STRAP_TYPE_STRING:
			sarr = malloc(sizeof *sarr);
			if (!sarr)
				return NULL;
			// sarr->capacity = capacity > STRAP_INIT_STR_SIZE ? capacity : STRAP_INIT_STR_SIZE;
			sarr->array_size = sizeof(size_t) * capacity;
			// sarr->count = 0;
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
	array->capacity = capacity;
	array->count  = 0;
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
	return arr ? arr->count : 0;
}

size_t strap_array_size(const StrapArray *arr)
{
	return arr ? arr->capacity : 0;
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
	arr->count = 0;
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
	char buf[BUF_SIZE];
	int (*func)(const StrapArray*, char*, size_t) = NULL;
	int n;
	size_t i;
	size_t count;
	char *sep;
	int n_total;

	if (!arr)
		return -1;
	count =  arr->count;
	if (!count)
		return fprintf(stream, "[]");
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			return strap_array_fprintf_str(arr->data, stream);
		case STRAP_TYPE_INT:
		case STRAP_TYPE_I32:
			func = strap_array_sprintf_element_i32;
			break;
		case STRAP_TYPE_FLOAT:
		case STRAP_TYPE_F32:
			func = strap_array_sprintf_element_f32;
			break;
		default:
			return -1;
	}
	n_total = fprintf(stream, "[");
	for (i = 0; i < count; i++) {
		n = func(arr, buf, i);
		sep = i == count - 1 ? "]" : ", ";
		n_total += fprintf(stream, "%.*s%s", n, buf, sep);
	}
	return n_total;
}
