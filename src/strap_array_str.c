#include "strap_internal.h"
#include "strap_array_str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prt(StrapArray *arr)
{
	size_t idx;
	char c;
	struct str_array *sarr = (struct str_array*) arr->data;
	char *string = sarr->buf;
	size_t s = 40;


	puts("");
	for (idx = 0; idx <= s; idx++) {
		printf("%-4lu", idx);
	}
	puts("");
	puts("---------------------------------------------------------------------------------------------------------");
	for (idx = 0; idx <= s; idx++) {
		c = string[idx];
		printf("%c   ",  c ? c : '-');
	}
	puts("");
	for (idx = 0 ; idx < arr->count; idx++) {
		printf("%d, ", sarr->lens[idx]);
	}
	puts("");
	strap_array_printf(arr);
	puts("");
}

void prts(const char *str, int len)
{
	char s[512];
	int i;

	memcpy(s, str, len);
	for (i = 0; i < len; i++) {
		if (s[i] == 0) {
			s[i] = '-';
		}
	}
	printf("\"%.*s\" - %d", len, s, len);
}

int str_resize_capacity(StrapArray *arr, size_t capacity)
{
	struct str_array *sarr;
	ushort *lens;

	sarr = arr->data;
	lens = realloc(sarr->lens, sizeof *lens*capacity);
	if (!lens)
		return 1;
	sarr->lens = lens;
	arr->capacity = capacity;
	return 0;
}

int str_resize_buf(StrapArray *arr, size_t buflen)
{
	struct str_array *sarr;

	sarr = realloc(arr->data, sizeof *sarr - 1 + buflen);
	if (!sarr)
		return 1;
	sarr->buflen = buflen;
	arr->data = sarr;
	return 0;
}

StrapString *strap_array_create_string(const StrapArray *arr, size_t idx)
{
	size_t pos;
	char *buf;

	if (!arr || arr->type != STRAP_TYPE_STRING || idx >= arr->count)
		return NULL;
	if (idx >= arr->count)
		return NULL;
	pos = str_pos(arr, idx);
	buf = str_buf(arr);
	return strap_string_alloc(buf + pos);
}

const char* strap_array_get_cstr(const StrapArray *arr, size_t index)
{
	if (!arr || arr->type != STRAP_TYPE_STRING || index >= arr->count)
		return NULL;
	return str_buf(arr) + str_pos(arr, index);
}

StrapArray *strap_array_append_cstr(StrapArray *arr, const char *str)
{
	size_t count;
	size_t len;
	size_t pos;
	char *buf;

	if (!arr || !str || arr->type != STRAP_TYPE_STRING)
		return arr;
	count = arr->count;
	len = strlen(str);
	str_check_size(arr, len, arr);
	buf = str_buf(arr);
	pos = str_pos(arr, count);
	memcpy(buf + pos, str, len + 1);
	str_len(arr, arr->count++) = pos + len;
	return arr;
}

StrapArray *strap_array_append_string(StrapArray *arr, const StrapString *str)
{
	return strap_array_append_cstr(arr, str->data);
}

StrapArray *strap_array_insert_cstr(StrapArray *arr, size_t idx, const char *str)
{
	size_t count;
	size_t len;
	size_t mvlen;
	size_t pos;
	char *buf;
	size_t i;
	ushort *lens;

	if (!arr || !str || arr->type != STRAP_TYPE_STRING)
		return arr;
	count = arr->count;
	len = strlen(str);
	str_check_size(arr, len, arr);
	buf = str_buf(arr);
	if (!count || idx >= count) {
		pos = str_pos(arr, count);
		memcpy(buf + pos, str, len + 1);
		str_len(arr, arr->count++) = pos + len;
		return arr;
	}
	pos = str_pos(arr, idx);
	mvlen = str_len(arr, count - 1) - pos;
	memcpy(buf + pos + len + 1, buf + pos, mvlen + 1);
	memcpy(buf + pos, str, len + 1);
	lens = str_sarr(arr)->lens;
	for (i = count; i >= max(idx, 1); i--)
		lens[i] = lens[i - 1] + len + 1;
	if (!idx)
		lens[0] = len;
	arr->count++;
	return arr;
}

StrapArray *strap_array_insert_string(StrapArray *arr, size_t i, const StrapString *str)
{
	return strap_array_insert_cstr(arr, i, str->data);
}

StrapArray *strap_array_replace_cstr(StrapArray *arr, size_t idx, const char *str)
{
	size_t count;
	size_t pos;
	size_t mvpos;
	size_t len;
	size_t prevlen;
	size_t mvlen;
	size_t i;
	char *buf;

	if (!arr || !str || arr->type != STRAP_TYPE_STRING)
		return arr;
	count = arr->count;
	if (idx >= count)
		return arr;
	len = strlen(str);
	prevlen = strlen(str_buf(arr) + str_pos(arr, idx));
	str_check_size(arr, len - prevlen, arr);
	buf = str_buf(arr);
	pos = str_pos(arr, idx);
	if (idx == count - 1) {
		memcpy(buf + pos, str, len + 1);
		str_len(arr, idx) = len + idx ? str_len(arr, idx - 1) : 0;
		return arr;
	}
	mvpos = str_pos(arr, idx + 1);
	mvlen = str_len(arr, count - 1) - mvpos;
	memcpy(buf + pos + len + 1, buf + mvpos, mvlen + 1);
	memcpy(buf + pos, str, len + 1);
	for (i = idx; i < count; i++)
		str_len(arr, i) += len - prevlen;
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
	size_t i;
	size_t offset;
	size_t counted;
	size_t count;
	ushort *lens;
	char *buf;

	if (!arr || !cstr || arr->type != STRAP_TYPE_STRING)
		return -1;
	count = arr->count;
	if (!count)
		return -1;
	buf = str_buf(arr);
	lens = str_sarr(arr)->lens;
	offset = 0;
	for (i = 0, counted = 0; i < count && counted <= n; i++) {
		if (strcmp(buf + offset, cstr) == 0) {
			if (counted == n)
				return i;
			counted++;
		}
		offset = lens[i] + 1;
	}
	return -1;
}

StrapArray *strap_array_shrink_str(StrapArray *arr)
{
	// FIXME crashes next function that requires resize
	struct str_array *sarr;
	size_t new_str_size;
	size_t new_arr_size;
	size_t len;

	if (!arr)
		return NULL;
	sarr = (struct str_array*) arr->data;
	new_arr_size = (arr->count ? arr->count : STRAP_INIT_CAPACITY)*sizeof(size_t);
	len = arr->count ? sarr->lens[arr->count - 1] : 0;
	new_str_size = len ? strap_next_pow2(len, STRAP_INIT_STR_SIZE) : STRAP_INIT_STR_SIZE;
	// sarr = strap_resize(arr, new_arr_size, new_str_size);
	if (!sarr)
		return arr;
	arr->data = sarr;
	return arr;
}

StrapArray *strap_array_sort_str(StrapArray *arr, int ascending)
{
	return arr;
}
