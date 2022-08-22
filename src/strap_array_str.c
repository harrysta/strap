#include "strap.h"
#include "strap_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static StrapArray_str *strap_resize(StrapArray_str *ptr,
	size_t arr_size, size_t str_size)
{
	arr_size = arr_size ? arr_size : ptr->array_size;
	str_size = str_size ? str_size : ptr->string_size;
	StrapArray_str *arr = realloc(ptr, 3 * sizeof(size_t) + arr_size + str_size);
	if (!arr)
		return NULL;
	char *newstr = (char*) arr->array + arr_size;
	if (arr_size != arr->array_size)
		memcpy(newstr, S_ARRSTR(arr), arr->string_size);
	arr->array_size  = arr_size;
	arr->string_size = str_size;
	return arr;
}

const char* strap_array_get_cstr(const StrapArray *arr, size_t index)
{
	if (!arr || arr->type != STRAP_TYPE_STRING)
		return NULL;
	StrapArray_str *arr_s = (StrapArray_str*) arr->data;
	if (index >= arr_s->count)
		return NULL;
	return S_ARRISTR(arr_s, index);
}

StrapArray *strap_array_append_cstr(StrapArray *arr, const char *str)
{
	StrapArray_str *arr_s;
	char *string;
	size_t len;
	size_t newlen;
	size_t lastlen;
	size_t pos;
	size_t new_str_size = 0;
	size_t new_arr_size = 0;

	if (!arr || !str || arr->type != STRAP_TYPE_STRING)
		return arr;
	arr_s = (StrapArray_str*) arr->data;
	len = strlen(str);
	pos = arr_s->count ? arr_s->array[arr_s->count - 1] + 1 : 0;
	newlen = len + 1 + pos;
	if (newlen >= arr_s->string_size)
		new_str_size = strap_next_pow2(newlen + 1, S_INIT_STR_SIZE);
	if (arr_s->array_size == arr_s->count * sizeof(size_t))
		new_arr_size = arr_s->array_size * 2;
	if (new_str_size || new_arr_size) {
		arr_s = strap_resize(arr_s, new_arr_size, new_str_size);
		if (!arr_s)
			return arr;
		arr->data = arr_s;
	}
	string = S_ARRSTR(arr_s);
	memcpy(string + pos, str, len + 1);
	arr_s->array[arr_s->count++] = pos + len;
	return arr;
}

StrapArray *strap_array_insert_cstr(StrapArray *arr, size_t i, const char *cstr)
{
	return NULL;
}

StrapArray *strap_array_replace_cstr(StrapArray *arr, size_t i, const char *cstr)
{
	return NULL;
}

size_t *strap_array_find_cstr(const StrapArray *arr, const char *cstr)
{
	return NULL;
}

int strap_array_sprintf_str(const StrapArray_str *arr, char *cstr)
{
	int n;
	int sum;
	size_t i;

	if (!arr->count)
		return sprintf(cstr, "[]");
	sum = sprintf(cstr, "[");
	for (i = 0; i < arr->count; i++) {
		n = sprintf(cstr + sum, "\"%s\"", S_ARRISTR(arr, i));
		if (n < 0) {
			*cstr = '\0';
			return -1;
		}
		sum += n;
		if (i != arr->count - 1)
			sum += sprintf(cstr + sum, ", ");
	}
	sum += sprintf(cstr + sum, "]");
	return sum;
}

int strap_array_fprintf_str(const StrapArray_str *arr, FILE *stream)
{
	int n;
	int sum;
	size_t i;

	if (!arr->count)
		return fprintf(stream, "[]");
	sum = fprintf(stream, "[");
	for (i = 0; i < arr->count; i++) {
		n = fprintf(stream, "\"%s\"", S_ARRISTR(arr, i));
		if (n < 0) {
			return -1;
		}
		sum += n;
		if (i != arr->count - 1)
			sum += fprintf(stream, ", ");
	}
	sum += fprintf(stream, "]");
	return sum;
}
