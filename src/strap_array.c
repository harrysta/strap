#include "strap_internal.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define BUF_SIZE 32

#define STRAP_ARRAY_SFPRINTF_FUNC(ptr, format, func, type) \
do {                                                       \
	int n;                                                   \
	va_list args;                                            \
                                                           \
	va_start(args, format);                                  \
	n = func((type*) ptr, format, args);                     \
	va_end(args);                                            \
	return n;                                                \
} while (0)


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

int strap_array_sprintf_element_str(const StrapArray *arr, char *buf, size_t idx)
{
	struct str_array *sarr = arr->data;
	return sprintf(buf, "\"%s\"", S_ARRISTR(sarr, idx));
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
			sarr->buflen = sizeof(size_t) * capacity;
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
			return ((struct str_array*) arr->data)->buflen / sizeof(size_t);
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

int strap_array_sfprintf_internal(const StrapArray *arr, void *ptr,
	int (*printfunc)(void*, const char*, ...), int is_string)
{
	size_t count;
	size_t bytes;
	char *buf;
	int (*func)(const StrapArray*, char*, size_t) = NULL;
	size_t len;
	struct str_array *sarr;
	int n_total;
	size_t i;
	int n;
	char *sep;

	if (!arr)
		return -1;
	count =  arr->count;
	bytes = BUF_SIZE;
	if (!count)
		return printfunc(ptr, "[]");
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			func = strap_array_sprintf_element_str;
			sarr = arr->data;
			for (i = 0; i < count; i++) {
				len = sarr->lens[i] - (i ? sarr->lens[i - 1] : 0);
				if (len > bytes)
					bytes = len;
			}
			break;
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
	buf = malloc(bytes);
	if (!buf)
		return 0;
	n_total = printfunc(ptr, "[");
	for (i = 0; i < count; i++) {
		n = func(arr, buf, i);
		sep = i == count - 1 ? "]" : ", ";
		n_total += printfunc(ptr + is_string*n_total, "%.*s%s", n, buf, sep);
	}
	free(buf);
	return n_total;
}

int strap_array_sprintf_func(void *ptr, const char *format, ...)
{
	STRAP_ARRAY_SFPRINTF_FUNC(ptr, format, vsprintf, char);
}

int strap_array_fprintf_func(void *ptr, const char *format, ...)
{
	STRAP_ARRAY_SFPRINTF_FUNC(ptr, format, vfprintf, FILE);
}

int strap_array_sprintf(const StrapArray *arr, char *cstr)
{
	return strap_array_sfprintf_internal(arr, cstr, strap_array_sprintf_func, 1);
}

int strap_array_fprintf(const StrapArray *arr, FILE *stream)
{
	return strap_array_sfprintf_internal(arr, stream, strap_array_fprintf_func, 0);
}
