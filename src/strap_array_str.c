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

static struct str_array *strap_resize(StrapArray *arr,
	size_t arr_size, size_t str_size)
{
	struct str_array *ptr;
	struct str_array *sarr;
	char *newstr;
	size_t next_size;
	return NULL;

	ptr = arr->data;
	arr_size = arr_size ? arr_size : ptr->buflen;
	str_size = str_size ? str_size : arr->capacity;
	next_size = 3 * sizeof(size_t) + arr_size + str_size;
	puts("\n-- RESIZE -- ");
	printf("arr_size: %lu -> %lu\n", ptr->buflen, arr_size);
	printf("str_size: %lu -> %lu\n", arr->capacity, str_size);
	logd(next_size);
	sarr = realloc(ptr, next_size);
	if (!sarr)
		return NULL;
	newstr = (char*) sarr->lens + arr_size;
	if (arr_size != sarr->buflen)
		memcpy(newstr, S_ARRSTR(sarr), arr->capacity);
	sarr->buflen  = arr_size;
	arr->capacity = str_size;
	return sarr;
}

StrapString *strap_array_create_string(const StrapArray *arr, size_t idx)
{
	StrapString *str;
	struct str_array *sarr;
	char *string;
	size_t pos;

	if (!arr)
		return NULL;
	sarr = (struct str_array*) arr->data;
	if (idx >= arr->count)
		return NULL;
	pos = idx ? sarr->lens[idx - 1] + 1 : 0;
	string = S_ARRSTR(sarr);
	return strap_string_alloc(string + pos);
}

const char* strap_array_get_cstr(const StrapArray *arr, size_t index)
{
	if (!arr || arr->type != STRAP_TYPE_STRING || index >= arr->count)
		return NULL;
	return str_buf(arr) + str_pos(arr, index);
}

struct str_array *strap_ensure_size(StrapArray *arr, size_t newlen)
{
	size_t new_str_size = 0;
	size_t new_arr_size = 0;
	struct str_array *sarr = arr->data;

	if (newlen >= arr->capacity)
		new_str_size = strap_next_pow2(newlen + 1, STRAP_INIT_STR_SIZE);
	if (sarr->buflen == arr->count * sizeof(size_t))
		new_arr_size = sarr->buflen * 2;
	if (new_str_size || new_arr_size) {
		sarr = strap_resize(arr, new_arr_size, new_str_size);
		if (!arr)
			return NULL;
	}
	return sarr;
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

StrapArray *strap_array_create_subarray_str(const StrapArray *arr, size_t idx, size_t n)
{
	struct str_array *sarr;
	struct str_array *narr_s;
	StrapArray *narr;
	size_t pos;
	size_t len;
	size_t i;
	char *string;
	char *nstring;

	sarr = (struct str_array*) arr->data;
	if (idx >= arr->count)
		return NULL;
	pos = idx ? sarr->lens[idx - 1] + 1 : 0;
	len = sarr->lens[idx + n - 1] - sarr->lens[idx - 1];
	string = S_ARRSTR(sarr);
	narr = strap_array_nalloc(STRAP_TYPE_STRING, len);
	narr_s = (struct str_array*) narr->data;
	nstring = S_ARRSTR(narr_s);
	memcpy(nstring, string + pos, len);
	for (i = 0; i < n; i++)
		narr_s->lens[i] = sarr->lens[i + idx] - pos;
	narr->count = n;
	return narr;
}

StrapArray *strap_array_reverse_str(StrapArray *arr)
{
	struct str_array *sarr;
	char *string;
	char *tmp_string;
	size_t *tmp_len_arr;
	size_t bytes;
	size_t pos1;
	size_t pos2;
	size_t len;
	size_t i;

	sarr = (struct str_array*) arr->data;
	if (arr->count <= 1)
		return arr;
	bytes = sarr->lens[arr->count - 1];
	tmp_string = malloc(bytes);
	if (!tmp_string)
		return arr;
	tmp_len_arr = malloc(sizeof(size_t)*arr->count);
	if (!tmp_len_arr) {
		free(tmp_string);
		return arr;
	}
	string = S_ARRSTR(sarr);
	for (i = arr->count - 1; i > 0 ; i--) {
		pos1 = i == (arr->count - 1) ? 0 : (pos1 + len);
		pos2 = sarr->lens[i - 1] + 1;
		len = sarr->lens[i] - sarr->lens[i - 1];
		memcpy(tmp_string + pos1, string + pos2, len);
		tmp_len_arr[arr->count - 1 - i] = pos1 + len - 1;
	}
	pos1 += len;
	len = sarr->lens[i];
	memcpy(tmp_string + pos1, string, len);
	tmp_len_arr[arr->count - 1] = pos1 + len;
	memcpy(sarr->lens, tmp_len_arr, sizeof(size_t)*arr->count);
	memcpy(string, tmp_string, bytes);
	free(tmp_string);
	free(tmp_len_arr);
	return arr;
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
	sarr = strap_resize(arr, new_arr_size, new_str_size);
	if (!sarr)
		return arr;
	arr->data = sarr;
	return arr;
}

StrapArray *strap_array_sort_str(StrapArray *arr, int ascending)
{
	return arr;
}
