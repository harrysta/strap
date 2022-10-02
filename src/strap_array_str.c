#include "strap_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct str_array *strap_resize(struct str_array *ptr,
	size_t arr_size, size_t str_size)
{
	arr_size = arr_size ? arr_size : ptr->array_size;
	str_size = str_size ? str_size : ptr->string_size;
	size_t next_size = 3 * sizeof(size_t) + arr_size + str_size;
	puts("\n-- RESIZE -- ");
	printf("arr_size: %lu -> %lu\n", ptr->array_size, arr_size);
	printf("str_size: %lu -> %lu\n", ptr->string_size, str_size);
	logd(next_size);
	struct str_array *arr = realloc(ptr, next_size);
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
	struct str_array *sarr;
	char *string;
	size_t pos;

	if (!arr)
		return NULL;
	sarr = (struct str_array*) arr->data;
	if (idx >= sarr->count)
		return NULL;
	pos = idx ? sarr->array[idx - 1] + 1 : 0;
	string = S_ARRSTR(sarr);
	return strap_string_alloc(string + pos);
}

const char* strap_array_get_cstr(const StrapArray *arr, size_t index)
{
	if (!arr || arr->type != STRAP_TYPE_STRING)
		return NULL;
	struct str_array *sarr = (struct str_array*) arr->data;
	if (index >= sarr->count)
		return NULL;
	return S_ARRISTR(sarr, index);
}

struct str_array *strap_ensure_size(struct str_array *arr, size_t newlen)
{
	size_t new_str_size = 0;
	size_t new_arr_size = 0;

	if (newlen >= arr->string_size)
		new_str_size = strap_next_pow2(newlen + 1, STRAP_INIT_STR_SIZE);
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
	struct str_array *sarr;
	size_t len;
	size_t pos;
	char *string;

	if (!arr || !str || arr->type != STRAP_TYPE_STRING)
		return arr;
	sarr = (struct str_array*) arr->data;
	len = strlen(str);
	pos = sarr->count ? sarr->array[sarr->count - 1] + 1 : 0;
	sarr = strap_ensure_size(sarr, len + 1 + pos);
	if (!sarr)
		return arr;
	arr->data = sarr;
	string = S_ARRSTR(sarr);
	memcpy(string + pos, str, len + 1);
	sarr->array[sarr->count++] = pos + len;
	return arr;
}

StrapArray *strap_array_append_string(StrapArray *arr, const StrapString *str)
{
	return strap_array_append_cstr(arr, str->data);
}

StrapArray *strap_array_insert_cstr(StrapArray *arr, size_t idx, const char *str)
{
	struct str_array *sarr;
	size_t len;
	size_t pos;
	size_t stop;
	char *string;
	size_t mvlen;
	size_t i;

	if (!arr || !str || arr->type != STRAP_TYPE_STRING)
		return arr;
	sarr = (struct str_array*) arr->data;
	if (!sarr->count || idx >= sarr->count)
		return strap_array_append_cstr(arr, str);
	len = strlen(str) + 1;
	sarr = strap_ensure_size(sarr, sarr->array[sarr->count - 1] + len);
	if (!sarr)
		return arr;
	arr->data = sarr;
	string = S_ARRSTR(sarr);
	pos = idx ? sarr->array[idx - 1] + 1 : 0;
	stop = idx ? idx : 1;
	mvlen = sarr->array[sarr->count - 1] + 1 - pos;
	memcpy(string + pos + len, string + pos, mvlen);
	memcpy(string + pos, str, len);
	for (i = sarr->count; i >= stop; i--)
		sarr->array[i] = sarr->array[i - 1] + len;
	if (!idx)
		sarr->array[0] = len - 1;
	sarr->count++;
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
	struct str_array *sarr = (struct str_array*) arr->data;
	char *string = S_ARRSTR(sarr);
	size_t s = 35;

	puts("\n-- array -- ");
	for (idx = 0 ; idx < sarr->count; idx++) {
		printf("%lu, ", sarr->array[idx]);
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
	struct str_array *sarr;
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
	sarr = (struct str_array*) arr->data;
	if (!sarr->count || idx >= sarr->count)
		return arr; /* ignore invalid index */
	len = strlen(str) + 1;
	prevlen = sarr->array[idx] + 1 - (idx ? sarr->array[idx - 1] + 1 : 0);
	diff = len - prevlen;
	if (diff > 0) {
		sarr = strap_ensure_size(sarr, sarr->array[sarr->count - 1] + diff);
		if (!sarr)
			return arr;
		arr->data = sarr;
	}
	string = S_ARRSTR(sarr);
	pos = idx ? sarr->array[idx - 1] + 1 : 0;
	mvstart = sarr->array[idx] + 1;
	mvlen = sarr->array[sarr->count - 1] - sarr->array[idx];
	mvdest = mvstart + diff;
	memmove(string + mvdest, string + mvstart, mvlen);
	strcpy(string + pos, str);
	for (i = idx; i < sarr->count; i++)
		sarr->array[i] += diff;
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
	struct str_array *sarr;
	char *string;
	size_t offset;
	size_t i;
	size_t count;

	if (!arr || !cstr || arr->type != STRAP_TYPE_STRING)
		return -1;
	sarr = (struct str_array*) arr->data;
	if (!sarr->count)
		return -1;
	string = S_ARRSTR(sarr);
	offset = 0;
	for (i = 0, count = 0; i < sarr->count && count <= n; i++) {
		if (strcmp(string + offset, cstr) == 0) {
			if (count == n)
				return i;
			count++;
		}
		offset = sarr->array[i] + 1;
	}
	return -1;
}

StrapArray *strap_array_clear_str(StrapArray *arr)
{
	((struct str_array*) arr->data)->count = 0;
	return arr;
}

StrapArray *strap_array_erase_range_str(StrapArray *arr, size_t idx, size_t n)
{
	struct str_array *sarr;
	size_t pos;
	size_t mvlen;
	size_t dst;
	size_t i;
	char *string;

	sarr = (struct str_array*) arr->data;
	if (idx >= sarr->count || n == 0)
		return arr;
	if (idx + n > sarr->count)
		n = sarr->count - idx;
	if (idx + n != sarr->count) {
		pos = sarr->array[idx - 1 + n] + 1;
		dst = idx ? sarr->array[idx - 1] + 1 : 0;
		mvlen = sarr->array[sarr->count - 1] + 1 - pos;
		string = S_ARRSTR(sarr);
		memcpy(string + dst, string + pos, mvlen);
		for (i = idx; i < sarr->count - 1; i++)
			sarr->array[i] = sarr->array[i + 1];
	}
	sarr->count -= n;
	return arr;
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
	if (idx >= sarr->count)
		return NULL;
	pos = idx ? sarr->array[idx - 1] + 1 : 0;
	len = sarr->array[idx + n - 1] - sarr->array[idx - 1];
	string = S_ARRSTR(sarr);
	narr = strap_array_nalloc(STRAP_TYPE_STRING, len);
	narr_s = (struct str_array*) narr->data;
	nstring = S_ARRSTR(narr_s);
	memcpy(nstring, string + pos, len);
	for (i = 0; i < n; i++)
		narr_s->array[i] = sarr->array[i + idx] - pos;
	narr_s->count = n;
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
	if (sarr->count <= 1)
		return arr;
	bytes = sarr->array[sarr->count - 1];
	tmp_string = malloc(bytes);
	if (!tmp_string)
		return arr;
	tmp_len_arr = malloc(sizeof(size_t)*sarr->count);
	if (!tmp_len_arr) {
		free(tmp_string);
		return arr;
	}
	string = S_ARRSTR(sarr);
	for (i = sarr->count - 1; i > 0 ; i--) {
		pos1 = i == (sarr->count - 1) ? 0 : (pos1 + len);
		pos2 = sarr->array[i - 1] + 1;
		len = sarr->array[i] - sarr->array[i - 1];
		memcpy(tmp_string + pos1, string + pos2, len);
		tmp_len_arr[sarr->count - 1 - i] = pos1 + len - 1;
	}
	pos1 += len;
	len = sarr->array[i];
	memcpy(tmp_string + pos1, string, len);
	tmp_len_arr[sarr->count - 1] = pos1 + len;
	memcpy(sarr->array, tmp_len_arr, sizeof(size_t)*sarr->count);
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
	new_arr_size = (sarr->count ? sarr->count : STRAP_INIT_CAPACITY)*sizeof(size_t);
	len = sarr->count ? sarr->array[sarr->count - 1] : 0;
	new_str_size = len ? strap_next_pow2(len, STRAP_INIT_STR_SIZE) : STRAP_INIT_STR_SIZE;
	sarr = strap_resize(sarr, new_arr_size, new_str_size);
	if (!sarr)
		return arr;
	arr->data = sarr;
	return arr;
}

StrapArray *strap_array_sort_str(StrapArray *arr, int ascending)
{
	return arr;
}

int strap_array_sprintf_str(const struct str_array *arr, char *cstr)
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

int strap_array_fprintf_str(const struct str_array *arr, FILE *stream)
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
