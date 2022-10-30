#include "strap_internal.h"
#include "strap_array_str.h"
#include "strap_array_num.h"
#include <string.h>

int str_resize_capacity(StrapArray *arr, size_t capacity)
{
	struct str_array *sarr;
	ushort *nulls;

	sarr = arr->data;
	nulls = realloc(sarr->nulls, sizeof *nulls*capacity);
	if (!nulls)
		return 1;
	sarr->nulls = nulls;
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

StrapString *s_array_create_string_from_element(const StrapArray *arr, size_t idx)
{
	size_t pos;
	char *buf;
	char numbuf[32];
	num_ptr numarr;

	if (!arr || idx >= arr->count)
		return NULL;
	numarr.i8 = arr->data;
	switch (arr->type) {
		case STRAP_TYPE_CHAR:        sprintf(numbuf, "%d",  numarr.i8[idx]);   break;
		case STRAP_TYPE_SHORT:       sprintf(numbuf, "%d",  numarr.i16[idx]);  break;
		case STRAP_TYPE_INT:         sprintf(numbuf, "%d",  numarr.i32[idx]);  break;
		case STRAP_TYPE_LONG:        sprintf(numbuf, "%ld", numarr.i64[idx]);  break;
		case STRAP_TYPE_FLOAT:       sprintf(numbuf, "%g",  numarr.f32[idx]);  break;
		case STRAP_TYPE_DOUBLE:      sprintf(numbuf, "%g",  numarr.f64[idx]);  break;
		case STRAP_TYPE_LONG_DOUBLE: sprintf(numbuf, "%Lg", numarr.f128[idx]); break;
		case STRAP_TYPE_STRING:
			pos = str_pos(arr, idx);
			buf = str_buf(arr);
			return s_string_alloc(buf + pos);
		default:
			return NULL;
	}
	return s_string_alloc(numbuf);
}

const char* s_array_get_cstr(const StrapArray *arr, size_t index)
{
	if (!arr || arr->type != STRAP_TYPE_STRING || index >= arr->count)
		return NULL;
	return str_buf(arr) + str_pos(arr, index);
}

StrapArray *s_array_append_cstr(StrapArray *arr, const char *str)
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
	str_null(arr, arr->count++) = pos + len;
	return arr;
}

StrapArray *s_array_append_string(StrapArray *arr, const StrapString *str)
{
	return s_array_append_cstr(arr, str ? str->data : NULL);
}

StrapArray *s_array_insert_cstr(StrapArray *arr, size_t idx, const char *str)
{
	size_t count;
	size_t len;
	size_t mvlen;
	size_t pos;
	char *buf;
	size_t i;
	ushort *nulls;

	if (!arr || !str || arr->type != STRAP_TYPE_STRING)
		return arr;
	count = arr->count;
	len = strlen(str);
	str_check_size(arr, len, arr);
	buf = str_buf(arr);
	if (!count || idx >= count) {
		pos = str_pos(arr, count);
		memcpy(buf + pos, str, len + 1);
		str_null(arr, arr->count++) = pos + len;
		return arr;
	}
	pos = str_pos(arr, idx);
	mvlen = str_null(arr, count - 1) - pos;
	memcpy(buf + pos + len + 1, buf + pos, mvlen + 1);
	memcpy(buf + pos, str, len + 1);
	nulls = str_sarr(arr)->nulls;
	for (i = count; i >= max(idx, 1); i--)
		nulls[i] = nulls[i - 1] + len + 1;
	if (!idx)
		nulls[0] = len;
	arr->count++;
	return arr;
}

StrapArray *s_array_insert_string(StrapArray *arr, size_t i, const StrapString *str)
{
	return s_array_insert_cstr(arr, i, str ? str->data : NULL);
}

StrapArray *s_array_replace_cstr(StrapArray *arr, size_t idx, const char *str)
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
		str_null(arr, idx) = len;
		return arr;
	}
	mvpos = str_pos(arr, idx + 1);
	mvlen = str_null(arr, count - 1) - mvpos;
	memcpy(buf + pos + len + 1, buf + mvpos, mvlen + 1);
	memcpy(buf + pos, str, len + 1);
	for (i = idx; i < count; i++)
		str_null(arr, i) += len - prevlen;
	return arr;
}

StrapArray *s_array_replace_string(StrapArray *arr, size_t i, const StrapString *str)
{
	return s_array_replace_cstr(arr, i, str ? str->data : NULL);
}

size_t s_array_find_string(const StrapArray *arr, StrapString *str)
{
	return s_array_find_cstr(arr, str ? str->data : NULL);
}

size_t s_array_find_cstr(const StrapArray *arr, const char *cstr)
{
	return s_array_nfind_cstr(arr, cstr, 0);
}

size_t s_array_nfind_string(const StrapArray *arr, StrapString *str, size_t n)
{
	return s_array_nfind_cstr(arr, str ? str->data : NULL, n);
}

size_t s_array_nfind_cstr(const StrapArray *arr, const char *cstr, size_t n)
{
	size_t i;
	size_t offset;
	size_t counted;
	size_t count;
	ushort *nulls;
	char *buf;

	if (!arr || !cstr || arr->type != STRAP_TYPE_STRING)
		return STRAP_NO_MATCH;
	count = arr->count;
	if (!count)
		return STRAP_NO_MATCH;
	buf = str_buf(arr);
	nulls = str_sarr(arr)->nulls;
	offset = 0;
	for (i = 0, counted = 0; i < count && counted <= n; i++) {
		if (!strcmp(buf + offset, cstr)) {
			if (counted == n)
				return i;
			counted++;
		}
		offset = nulls[i] + 1;
	}
	return STRAP_NO_MATCH;
}
