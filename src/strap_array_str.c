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

StrapString *strap_array_create_string(const StrapArray *arr, size_t idx)
{
	StrapString *str;
	StrapArray_str *arr_s;
	char *string;
	size_t pos;

	if (!arr)
		return NULL;
	arr_s = (StrapArray_str*) arr->data;
	if (idx >= arr_s->count)
		return NULL;
	pos = idx ? arr_s->array[idx - 1] + 1 : 0;
	string = S_ARRSTR(arr_s);
	return strap_string_alloc(string + pos);
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

StrapArray_str *strap_ensure_size(StrapArray_str *arr, size_t newlen)
{
	size_t new_str_size = 0;
	size_t new_arr_size = 0;

	if (newlen >= arr->string_size)
		new_str_size = strap_next_pow2(newlen + 1, S_INIT_STR_SIZE);
	if (arr->array_size == arr->count * sizeof(size_t))
		new_arr_size = arr->array_size * 2;
	if (new_str_size || new_arr_size) {
		arr = strap_resize(arr, new_arr_size, new_str_size);
		if (!arr)
			return NULL;
	}
	return arr;
}

StrapArray *strap_array_append_cstr(StrapArray *arr, const char *str)
{
	StrapArray_str *arr_s;
	size_t len;
	size_t pos;
	char *string;

	if (!arr || !str || arr->type != STRAP_TYPE_STRING)
		return arr;
	arr_s = (StrapArray_str*) arr->data;
	len = strlen(str);
	pos = arr_s->count ? arr_s->array[arr_s->count - 1] + 1 : 0;
	arr_s = strap_ensure_size(arr_s, len + 1 + pos);
	if (!arr_s)
		return arr;
	arr->data = arr_s;
	string = S_ARRSTR(arr_s);
	memcpy(string + pos, str, len + 1);
	arr_s->array[arr_s->count++] = pos + len;
	return arr;
}

StrapArray *strap_array_append_string(StrapArray *arr, const StrapString *str)
{
	return strap_array_append_cstr(arr, str->data);
}

StrapArray *strap_array_insert_cstr(StrapArray *arr, size_t idx, const char *str)
{
	StrapArray_str *arr_s;
	size_t len;
	size_t pos;
	size_t stop;
	char *string;
	size_t mvlen;
	size_t i;

	if (!arr || !str || arr->type != STRAP_TYPE_STRING)
		return arr;
	arr_s = (StrapArray_str*) arr->data;
	if (!arr_s->count || idx >= arr_s->count)
		return strap_array_append_cstr(arr, str);
	len = strlen(str) + 1;
	arr_s = strap_ensure_size(arr_s, arr_s->array[arr_s->count - 1] + len);
	if (!arr_s)
		return arr;
	arr->data = arr_s;
	string = S_ARRSTR(arr_s);
	pos = idx ? arr_s->array[idx - 1] + 1 : 0;
	stop = idx ? idx : 1;
	mvlen = arr_s->array[arr_s->count - 1] + 1 - pos;
	memcpy(string + pos + len, string + pos, mvlen);
	memcpy(string + pos, str, len);
	for (i = arr_s->count; i >= stop; i--)
		arr_s->array[i] = arr_s->array[i - 1] + len;
	if (!idx)
		arr_s->array[0] = len - 1;
	arr_s->count++;
	return arr;
}

StrapArray *strap_array_insert_string(StrapArray *arr, size_t i, const StrapString *str)
{
	return strap_array_insert_cstr(arr, i, str->data);
}

void ptr(StrapArray *arr)
{
	size_t idx;
	char c;
	StrapArray_str *arr_s = (StrapArray_str*) arr->data;
	char *string = S_ARRSTR(arr_s);
	size_t s = 35;

	puts("\n-- array -- ");
	for (idx = 0 ; idx < arr_s->count; idx++) {
		printf("%lu, ", arr_s->array[idx]);
	}

	puts("\n-- string -- ");
	for (idx = 0; idx <= s; idx++) {
		printf("%-4lu", idx);
	}
	puts("");
	puts("-----------------------------------------------------------------------------------");
	for (idx = 0; idx <= s; idx++) {
		c = string[idx];
		printf("%c   ",  c ? c : '-');
	}
	puts("");
}

StrapArray *strap_array_replace_cstr(StrapArray *arr, size_t idx, const char *str)
{
	StrapArray_str *arr_s;
	size_t len;
	size_t prevlen;
	size_t diff;
	char *string;
	size_t pos;
	size_t mvlen;
	size_t mvstart;
	size_t mvdest;
	size_t i;

	if (!arr || !str || arr->type != STRAP_TYPE_STRING)
		return arr;
	arr_s = (StrapArray_str*) arr->data;
	if (!arr_s->count || idx >= arr_s->count)
		return arr; /* ignore invalid index */
	len = strlen(str) + 1;
	prevlen = arr_s->array[idx] + 1 - (idx ? arr_s->array[idx - 1] + 1 : 0);
	diff = len - prevlen;
	if (diff > 0) {
		arr_s = strap_ensure_size(arr_s, arr_s->array[arr_s->count - 1] + diff);
		if (!arr_s)
			return arr;
		arr->data = arr_s;
	}
	string = S_ARRSTR(arr_s);
	pos = idx ? arr_s->array[idx - 1] + 1 : 0;
	mvstart = arr_s->array[idx] + 1;
	mvlen = arr_s->array[arr_s->count - 1] - arr_s->array[idx];
	mvdest = mvstart + diff;
	memmove(string + mvdest, string + mvstart, mvlen);
	strcpy(string + pos, str);
	for (i = idx; i < arr_s->count; i++)
		arr_s->array[i] += diff;
	return arr;
}

StrapArray *strap_array_replace_string(StrapArray *arr, size_t i, const StrapString *str)
{
	return strap_array_replace_cstr(arr, i, str->data);
}

size_t strap_array_find_string(const StrapArray *arr, StrapString *str)
{
	return strap_array_find_cstr(arr, str->data);
}

size_t strap_array_find_cstr(const StrapArray *arr, const char *cstr)
{
	return strap_array_nfind_cstr(arr, cstr, 0);
}

size_t strap_array_nfind_string(const StrapArray *arr, StrapString *str, size_t n)
{
	return strap_array_nfind_cstr(arr, str->data, n);
}

size_t strap_array_nfind_cstr(const StrapArray *arr, const char *cstr, size_t n)
{
	StrapArray_str *arr_s;
	char *string;
	size_t offset;
	size_t i;
	size_t count;

	if (!arr || !cstr || arr->type != STRAP_TYPE_STRING)
		return -1;
	arr_s = (StrapArray_str*) arr->data;
	if (!arr_s->count)
		return -1;
	string = S_ARRSTR(arr_s);
	offset = 0;
	for (i = 0, count = 0; i < arr_s->count && count <= n; i++) {
		if (strcmp(string + offset, cstr) == 0) {
			if (count == n)
				return i;
			count++;
		}
		offset = arr_s->array[i] + 1;
	}
	return -1;
}

StrapArray *strap_array_clear_str(StrapArray *arr)
{
	((StrapArray_str*) arr->data)->count = 0;
	return arr;
}

StrapArray *strap_array_erase_range_str(StrapArray *arr, size_t idx, size_t n)
{
	StrapArray_str *arr_s;
	size_t pos;
	size_t mvlen;
	size_t dst;
	size_t i;
	char *string;

	arr_s = (StrapArray_str*) arr->data;
	if (idx >= arr_s->count || n == 0)
		return arr;
	if (idx + n > arr_s->count)
		n = arr_s->count - idx;
	if (idx + n != arr_s->count) {
		pos = arr_s->array[idx - 1 + n] + 1;
		dst = idx ? arr_s->array[idx - 1] + 1 : 0;
		mvlen = arr_s->array[arr_s->count - 1] + 1 - pos;
		string = S_ARRSTR(arr_s);
		memcpy(string + dst, string + pos, mvlen);
		for (i = idx; i < arr_s->count - 1; i++)
			arr_s->array[i] = arr_s->array[i + 1];
	}
	arr_s->count -= n;
	return arr;
}

StrapArray *strap_array_create_subarray_str(const StrapArray *arr, size_t idx, size_t n)
{
	StrapArray_str *arr_s;
	StrapArray_str *narr_s;
	StrapArray *narr;
	size_t pos;
	size_t len;
	size_t i;
	char *string;
	char *nstring;

	arr_s = (StrapArray_str*) arr->data;
	if (idx >= arr_s->count)
		return NULL;
	pos = idx ? arr_s->array[idx - 1] + 1 : 0;
	len = arr_s->array[idx + n - 1] - arr_s->array[idx - 1];
	string = S_ARRSTR(arr_s);
	narr = strap_array_nalloc(STRAP_TYPE_STRING, len);
	narr_s = (StrapArray_str*) narr->data;
	nstring = S_ARRSTR(narr_s);
	memcpy(nstring, string + pos, len);
	for (i = 0; i < n; i++)
		narr_s->array[i] = arr_s->array[i + idx] - pos;
	narr_s->count = n;
	return narr;
}

StrapArray *strap_array_reverse_str(StrapArray *arr)
{
	StrapArray_str *arr_s;
	StrapArray_str *tmparr_s;
	StrapArray *tmparr;
	char *string;
	char *tmpstr;
	size_t bytes;
	size_t pos;
	size_t i;

	arr_s = (StrapArray_str*) arr->data;
	if (arr_s->count <= 1)
		return arr;
	tmparr = strap_array_nalloc(STRAP_TYPE_STRING, arr_s->count);
	string = S_ARRSTR(arr_s);
	if (!tmparr)
		return arr;
	for (i = arr_s->count - 1; i > 0; i--) {
		pos = arr_s->array[i - 1] + 1;
		strap_array_append_cstr(tmparr, string + pos);
	}
	strap_array_append_cstr(tmparr, string);
	tmparr_s = (StrapArray_str*) tmparr->data;
	tmpstr = S_ARRSTR(tmparr_s);
	bytes = arr_s->array[arr_s->count - 1];
	memcpy(string, tmpstr, bytes);
	memcpy(arr_s->array, tmparr_s->array, sizeof(size_t)*arr_s->count);
	strap_array_free(tmparr);
	return arr;
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
