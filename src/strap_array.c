#include "strap_array_str.h"

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
	return sprintf(buf, "\"%s\"", str_buf(arr) + str_pos(arr, idx));
}

StrapArray *strap_array_nalloc_internal(StrapType type, size_t capacity, size_t buflen)
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
			sarr = malloc(sizeof *sarr - 1 + buflen);
			if (!sarr)
				return NULL;
			sarr->buflen = buflen;
			sarr->nulls = malloc(sizeof(ushort)*capacity);
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

StrapArray *strap_array_nalloc(StrapType type, size_t capacity)
{
	return strap_array_nalloc_internal(type, capacity, STRAP_INIT_STR_SIZE);
}

StrapArray *strap_array_alloc(StrapType type)
{
	return strap_array_nalloc(type, STRAP_INIT_CAPACITY);
}

void strap_array_free(StrapArray *arr)
{
	if (!arr)
		return;
	if (arr->type == STRAP_TYPE_STRING)
		free(str_sarr(arr)->nulls);
	free(arr->data);
	free(arr);
}

StrapArray *strap_array_clone(const StrapArray *arr)
{
	const int C = STRAP_INIT_CAPACITY;
	StrapArray *newarr;
	StrapType type;
	size_t buflen;
	size_t new_buflen;
	size_t new_capacity;
	size_t count;

	if (!arr)
		return NULL;
	if (!arr->count)
		return strap_array_alloc(arr->type);
	type = arr->type;
	count = arr->count;
	buflen = str_sarr(arr)->buflen;
	new_capacity = ((count + C) / C) * C;
	if (type == STRAP_TYPE_STRING) {
		new_buflen = count ? str_null(arr, count - 1) : 0;
		new_buflen = strap_next_pow2(new_buflen + 1, STRAP_INIT_STR_SIZE);
	}
	newarr = strap_array_nalloc_internal(type, new_capacity, new_buflen);
	switch (type) {
		case STRAP_TYPE_STRING:
			memcpy(str_buf(newarr), str_buf(arr), str_null(arr, count - 1) + 1);
			memcpy(str_sarr(newarr)->nulls, str_sarr(arr)->nulls, sizeof(ushort)*count);
			break;
		case STRAP_TYPE_INT:
			memcpy(newarr->data, arr->data, count*strap_sizeof(type));
			break;
		default:
			return NULL;
	}
	newarr->count = count;
	return newarr;
}

size_t strap_array_count(const StrapArray *arr)
{
	return arr ? arr->count : 0;
}

size_t strap_array_capacity(const StrapArray *arr)
{
	return arr ? arr->capacity : 0;
}

StrapType strap_array_type(const StrapArray *arr)
{
	return arr ? arr->type : STRAP_TYPE_NONE;
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

StrapArray *strap_array_erase_range(StrapArray *arr, size_t idx, size_t n)
{
	size_t i;
	size_t count;
	size_t pos;
	size_t mvpos;
	size_t mvlen;
	size_t len;
	char *buf;
	ushort *nulls;

	if (!arr)
		return NULL;
	count = arr->count;
	if (!n || idx >= count)
		return arr;
	n = idx + n > count ? count - idx : n;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			if (idx + n < count) {
				pos = str_pos(arr, idx);
				mvpos = str_pos(arr, idx + n);
				mvlen = str_null(arr, n) - pos;
				buf = str_buf(arr);
				memcpy(buf + pos, buf + mvpos, mvlen);
			}
			nulls = str_sarr(arr)->nulls;
			for (i = idx; i < count - 1; i++)
				nulls[i] = nulls[i + 1];
			break;
		case STRAP_TYPE_CHAR:
		case STRAP_TYPE_SHORT:
		case STRAP_TYPE_INT:
		case STRAP_TYPE_LONG_INT:
		case STRAP_TYPE_FLOAT:
		case STRAP_TYPE_DOUBLE:
		case STRAP_TYPE_LONG_DOUBLE:
			if (idx + n < count) {
				buf = arr->data;
				len = strap_sizeof(arr->type);
				memcpy(buf + idx*len, buf + (idx + n)*len, (count - (idx + n))*len);
			}
			break;
		default:
			return arr;
	}
	arr->count -= n;
	return arr;
}

StrapArray *strap_array_create_subarray(const StrapArray *arr, size_t idx, size_t n)
{
	size_t i;
	size_t count;
	size_t pos;
	size_t len;
	char *buf;
	char *newbuf;
	ushort *nulls;
	ushort *newnulls;
	StrapArray *newarr;

	// WARNING buflen should be manually set to fit all buf; current impl will
	// cause UB with large string elements
	if (!arr || n == 0)
		return NULL;
	count = arr->count;
	if (idx >= count)
		return NULL;
	n = idx + n > count ? count - idx : n;
	newarr = strap_array_nalloc(arr->type, n);
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			pos = str_pos(arr, idx);
			nulls = str_sarr(arr)->nulls;
			newnulls = str_sarr(newarr)->nulls;
			for (i = 0; i < n; i++)
				newnulls[i] = nulls[i + idx] - pos;
			len = str_null(arr, n) - pos + 1;
			buf = str_buf(arr);
			newbuf = str_buf(newarr);
			break;
		default:
			return NULL;
	}
	memcpy(newbuf, buf + pos, len);
	newarr->count = n;
	return newarr;
}

StrapArray *strap_array_reverse(StrapArray *arr)
{
	size_t i;
	size_t count;
	size_t len;
	size_t sumlen;
	size_t pos;
	char *buf;
	char *tmpbuf;
	ushort *nulls;
	ushort *tmpnulls;

	if (!arr)
		return NULL;
	count = arr->count;
	if (!count)
		return arr;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			tmpbuf = malloc(str_null(arr, count - 1));
			if (!tmpbuf)
				goto out_free_tmpbuf;
			tmpnulls = malloc(sizeof *tmpnulls*count);
			if (!tmpnulls)
				goto out_free_tmpnulls;
			sumlen = 0;
			buf = str_buf(arr);
			nulls = str_sarr(arr)->nulls;
			for (i = 0; i < count; i++) {
				pos = str_pos(arr, count - 1 - i);
				len = str_null(arr, count - 1 - i) - pos;
				memcpy(tmpbuf + sumlen, buf + pos, len + 1);
				sumlen += len + 1;
				tmpnulls[i] = sumlen - 1;
			}
			memcpy(buf, tmpbuf, sumlen);
			memcpy(nulls, tmpnulls, sizeof *nulls*count);
			break;
		default:
			return NULL;
	}
out_free_tmpnulls:
	free(tmpnulls);
out_free_tmpbuf:
	free(tmpbuf);
	return arr;
}

StrapArray *strap_array_shrink(StrapArray *arr)
{
	const int C = STRAP_INIT_CAPACITY;
	size_t new_capacity;
	size_t new_buflen;
	size_t count;
	void *ndata;

	if (!arr)
		return NULL;
	count = arr->count;
	new_capacity = ((count + C) / C) * C;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			new_buflen = strap_next_pow2(str_null(arr, count - 1), STRAP_INIT_STR_SIZE);
			if (new_capacity != arr->capacity) {
				if (str_resize_capacity(arr, new_capacity)) /* Non-zero value indicates resize failure */
					return arr;
			}
			if (new_buflen != str_sarr(arr)->buflen) {
				str_resize_buf(arr, new_buflen); /* No need to check since it's the last statement */
			}
			return arr;
		case STRAP_TYPE_CHAR:
		case STRAP_TYPE_SHORT:
		case STRAP_TYPE_INT:
		case STRAP_TYPE_LONG_INT:
		case STRAP_TYPE_FLOAT:
		case STRAP_TYPE_DOUBLE:
		case STRAP_TYPE_LONG_DOUBLE:
			if (new_capacity != arr->capacity)
				num_resize_capacity(arr, new_capacity);
			return arr;
		default:
			return NULL;
	}
	return arr;
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
	const char *prefix = "";

	if (!arr || !ptr)
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
				len = sarr->nulls[i] - (i ? sarr->nulls[i - 1] : 0);
				if (len > bytes)
					bytes = len;
			}
			break;
		case STRAP_TYPE_CHAR: func = strap_array_sprintf_element_i8; break;
		case STRAP_TYPE_SHORT: func = strap_array_sprintf_element_i16; break;
		case STRAP_TYPE_INT: func = strap_array_sprintf_element_i32; break;
		case STRAP_TYPE_LONG_INT: func = strap_array_sprintf_element_i64; break;
		case STRAP_TYPE_FLOAT: func = strap_array_sprintf_element_f32; break;
		case STRAP_TYPE_DOUBLE: func = strap_array_sprintf_element_f64; break;
		// case STRAP_TYPE_LONG_DOUBLE: func = strap_array_sprintf_element_f128; break;
		default:
			return -1;
	}
	buf = malloc(bytes);
	if (!buf)
		return 0;
	n_total = printfunc(ptr, "[");
	for (i = 0; i < count; i++) {
		n = func(arr, buf, i);
		n_total += printfunc(ptr + is_string*n_total, "%s%.*s", prefix, n, buf);
		prefix = ", ";
	}
	n_total += printfunc(ptr + is_string*n_total, "]");
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
