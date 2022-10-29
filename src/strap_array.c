#include "strap_array_str.h"
#include "strap_array_num.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define s_array_sfprintf(arr, s, prtf, pf, is_sprintf)                       \
do {                                                                             \
	const char *prefix = "";                                                       \
	num_ptr numarr;                                                      \
	StrapType type;                                                                \
	size_t count;                                                                  \
	char *buf;                                                                     \
	size_t i;                                                                      \
	int pos;                                                                       \
                                                                                 \
	if (!arr || !s)                                                                \
		return -1;                                                                   \
	count = arr->count;                                                            \
	type = arr->type;                                                              \
	buf = str_buf(arr);                                                            \
	numarr.i8 = arr->data;                                                         \
	if (!count)                                                                    \
		return prtf(s, "[]");                                                        \
	pos = prtf(s, "[");                                                            \
	for (i = 0; i < count; i++) {                                                  \
		switch (type) {                                                              \
			case STRAP_TYPE_CHAR:        pf("%s%d", numarr.i8[i]);   break;            \
			case STRAP_TYPE_SHORT:       pf("%s%d", numarr.i16[i]);  break;            \
			case STRAP_TYPE_INT:         pf("%s%d", numarr.i32[i]);  break;            \
			case STRAP_TYPE_LONG:        pf("%s%ld",numarr.i64[i]);  break;            \
			case STRAP_TYPE_FLOAT:       pf("%s%g", numarr.f32[i]);  break;            \
			case STRAP_TYPE_DOUBLE:      pf("%s%g", numarr.f64[i]);  break;            \
			case STRAP_TYPE_LONG_DOUBLE: pf("%s%Lg",numarr.f128[i]); break;            \
			case STRAP_TYPE_STRING:      pf("%s\"%s\"", buf + str_pos(arr, i)); break; \
			default: return -1;                                                        \
		}                                                                            \
		prefix = ", ";                                                               \
	}                                                                              \
	pos += prtf(s + (is_sprintf ? pos : 0) , "]");                                 \
	return pos;                                                                    \
} while (0)

#define SPRINTF(fmt, arg) pos += sprintf(s + pos, fmt, prefix, arg)
#define FPRINTF(fmt, arg) pos += fprintf(s, fmt, prefix, arg)

StrapArray *s_array_nalloc_internal(StrapType type, size_t capacity, size_t buflen)
{
	StrapArray *array;
	struct str_array *sarr;
	void *data;

	switch (type) {
		case STRAP_TYPE_CHAR:
		case STRAP_TYPE_SHORT:
		case STRAP_TYPE_INT:
		case STRAP_TYPE_LONG:
		case STRAP_TYPE_FLOAT:
		case STRAP_TYPE_DOUBLE:
		case STRAP_TYPE_LONG_DOUBLE:
			data = malloc(capacity*s_sizeof(type));
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

StrapArray *s_array_nalloc(StrapType type, size_t capacity)
{
	return s_array_nalloc_internal(type, capacity, STRAP_INIT_STR_SIZE);
}

StrapArray *s_array_alloc(StrapType type)
{
	return s_array_nalloc(type, STRAP_INIT_CAPACITY);
}

StrapArray *s_array_memcpy(StrapArray *arr, const void *src, size_t num)
{
	const int C = STRAP_INIT_CAPACITY;
	const char *buf;
	const char *s;
	size_t ncapacity;
	StrapType type;
	ushort *nulls;
	ushort len;
	ushort nlen;
	size_t i;

	if (!arr)
		return NULL;
	type = arr->type;
	ncapacity = ((num + C) / C) * C;
	if (ncapacity > arr->capacity && num_resize_capacity(arr, ncapacity))
		return arr;
	switch (type) {
		case STRAP_TYPE_CHAR:
		case STRAP_TYPE_SHORT:
		case STRAP_TYPE_INT:
		case STRAP_TYPE_LONG:
		case STRAP_TYPE_FLOAT:
		case STRAP_TYPE_DOUBLE:
		case STRAP_TYPE_LONG_DOUBLE:
			memcpy(arr->data, src, num*s_sizeof(type));
			break;
		case STRAP_TYPE_STRING:
			buf = s = (const char*) src;
			nulls = malloc(num*sizeof *nulls);
			for (i = 0; i < num; i++) {
				buf = strchr(buf + 1, 0);
				nulls[i] = buf - s;
			}
			len = buf - s + 1;
			nlen = s_next_pow2(len, STRAP_INIT_STR_SIZE);
			if (len > str_sarr(arr)->buflen && str_resize_buf(arr, nlen)) {
				free(nulls);
				return arr;
			}
			memcpy(&str_null(arr, 0), nulls, num*sizeof *nulls);
			memcpy(str_buf(arr), s, buf - s + 1);
			free(nulls);
			break;
		default:
			return NULL;
	}
	arr->count = num;
	return arr;
}

StrapArray *s_array_strcpy(StrapArray *arr, const char **src, size_t num)
{
	const int C = STRAP_INIT_CAPACITY;
	size_t ncapacity;
	ushort *nulls;
	ushort prev;
	ushort nlen;
	ushort len;
	char *buf;
	size_t i;

	if (!arr || arr->type != STRAP_TYPE_STRING)
		return arr;
	ncapacity = ((num + C) / C) * C;
	if (ncapacity > arr->capacity && num_resize_capacity(arr, ncapacity))
		return arr;
	nulls = malloc(num*sizeof *nulls);
	for (i = 0; i < num; i++) {
		prev = i ? nulls[i - 1] + 1 : 0;
		nulls[i] = strlen(src[i]) + prev;
	}
	len = nulls[num - 1] + 1;
	nlen = s_next_pow2(len, STRAP_INIT_STR_SIZE);
	if (len > str_sarr(arr)->buflen && str_resize_buf(arr, nlen)) {
		free(nulls);
		return arr;
	}
	buf = str_buf(arr);
	for (i = 0; i < num; i++)
		buf += sprintf(buf, "%s", src[i]) + 1;
	memcpy(&str_null(arr, 0), nulls, num*sizeof *nulls);
	arr->count = num;
	free(nulls);
	return arr;
}

void s_array_free(StrapArray *arr)
{
	if (!arr)
		return;
	if (arr->type == STRAP_TYPE_STRING)
		free(str_sarr(arr)->nulls);
	free(arr->data);
	free(arr);
}

StrapArray *s_array_clone(const StrapArray *arr)
{
	const int C = STRAP_INIT_CAPACITY;
	StrapArray *newarr;
	StrapType type;
	size_t new_buflen = 0;
	size_t new_capacity;
	size_t count;

	if (!arr)
		return NULL;
	if (!arr->count)
		return s_array_alloc(arr->type);
	type = arr->type;
	count = arr->count;
	new_capacity = ((count + C) / C) * C;
	if (type == STRAP_TYPE_STRING) {
		new_buflen = count ? str_null(arr, count - 1) : 0;
		new_buflen = s_next_pow2(new_buflen + 1, STRAP_INIT_STR_SIZE);
	}
	newarr = s_array_nalloc_internal(type, new_capacity, new_buflen);
	switch (type) {
		case STRAP_TYPE_CHAR:
		case STRAP_TYPE_SHORT:
		case STRAP_TYPE_INT:
		case STRAP_TYPE_LONG:
		case STRAP_TYPE_FLOAT:
		case STRAP_TYPE_DOUBLE:
		case STRAP_TYPE_LONG_DOUBLE:
			memcpy(newarr->data, arr->data, count*s_sizeof(type));
			break;
		case STRAP_TYPE_STRING:
			memcpy(str_buf(newarr), str_buf(arr), str_null(arr, count - 1) + 1);
			memcpy(str_sarr(newarr)->nulls, str_sarr(arr)->nulls, sizeof(ushort)*count);
			break;
		default:
			return NULL;
	}
	newarr->count = count;
	return newarr;
}

size_t s_array_count(const StrapArray *arr)
{
	return arr ? arr->count : 0;
}

size_t s_array_capacity(const StrapArray *arr)
{
	return arr ? arr->capacity : 0;
}

StrapType s_array_type(const StrapArray *arr)
{
	return arr ? arr->type : STRAP_TYPE_NONE;
}

StrapArray *s_array_clear(StrapArray *arr)
{
	if (!arr)
		return NULL;
	arr->count = 0;
	return arr;
}

StrapArray *s_array_erase(StrapArray *arr, size_t i)
{
	return s_array_erase_range(arr, i, 1);
}

StrapArray *s_array_erase_range(StrapArray *arr, size_t idx, size_t n)
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
		case STRAP_TYPE_LONG:
		case STRAP_TYPE_FLOAT:
		case STRAP_TYPE_DOUBLE:
		case STRAP_TYPE_LONG_DOUBLE:
			if (idx + n < count) {
				buf = arr->data;
				len = s_sizeof(arr->type);
				memcpy(buf + idx*len, buf + (idx + n)*len, (count - (idx + n))*len);
			}
			break;
		default:
			return arr;
	}
	arr->count -= n;
	return arr;
}

StrapArray *s_array_create_subarray(const StrapArray *arr, size_t idx, size_t n)
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
	newarr = s_array_nalloc(arr->type, n);
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

StrapArray *s_array_reverse(StrapArray *arr)
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
	size_t start;
	size_t end;

	if (!arr)
		return NULL;
	count = arr->count;
	start = 0;
	end = count - 1;
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
out_free_tmpnulls:
			free(tmpnulls);
out_free_tmpbuf:
			free(tmpbuf);
			break;
		case STRAP_TYPE_CHAR:         NUM_REVERSE(arr, start, end, char);        break;
		case STRAP_TYPE_SHORT:        NUM_REVERSE(arr, start, end, short);       break;
		case STRAP_TYPE_INT:          NUM_REVERSE(arr, start, end, int);         break;
		case STRAP_TYPE_LONG:         NUM_REVERSE(arr, start, end, long);        break;
		case STRAP_TYPE_FLOAT:        NUM_REVERSE(arr, start, end, float);       break;
		case STRAP_TYPE_DOUBLE:       NUM_REVERSE(arr, start, end, double);      break;
		case STRAP_TYPE_LONG_DOUBLE:  NUM_REVERSE(arr, start, end, long double); break;
		default: return NULL;
	}
	return arr;
}

StrapArray *s_array_shrink(StrapArray *arr)
{
	const int C = STRAP_INIT_CAPACITY;
	size_t new_capacity;
	size_t new_buflen;
	size_t count;

	if (!arr)
		return NULL;
	count = arr->count;
	new_capacity = ((count + C) / C) * C;
	switch (arr->type) {
		case STRAP_TYPE_STRING:
			new_buflen = s_next_pow2(str_null(arr, count - 1), STRAP_INIT_STR_SIZE);
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
		case STRAP_TYPE_LONG:
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

int s_array_compare(const StrapArray *arr1, const StrapArray *arr2)
{
	num_ptr narr1, narr2;
	size_t count1, count2;
	char *buf1, *buf2;
	StrapType type;
	int diff;
	size_t i;

	if (!arr1 && !arr2)
		return 0;
	if (!arr1 || !arr2 || arr1->type != arr2->type)
		return -1;
	count1 = arr1->count;
	count2 = arr2->count;
	type = arr1->type;
	if (count1 != count2)
		return (int) count1 - (int) count2;
	narr1.i8 = arr1->data;
	narr2.i8 = arr2->data;
	buf1 = str_buf(arr1);
	buf2 = str_buf(arr2);
	for (i = 0; i < count1; i++) {
		switch (type) {
			case STRAP_TYPE_CHAR:        diff = narr1.i8[i] - narr2.i8[i];     break;
			case STRAP_TYPE_SHORT:       diff = narr1.i16[i] - narr2.i16[i];   break;
			case STRAP_TYPE_INT:         diff = narr1.i32[i] - narr2.i32[i];   break;
			case STRAP_TYPE_LONG:        diff = narr1.i64[i] - narr2.i64[i];   break;
			case STRAP_TYPE_FLOAT:       diff = narr1.f32[i] - narr2.f32[i];   break;
			case STRAP_TYPE_DOUBLE:      diff = narr1.f64[i] - narr2.f64[i];   break;
			case STRAP_TYPE_LONG_DOUBLE: diff = narr1.f128[i] - narr2.f128[i]; break;
			case STRAP_TYPE_STRING:
				diff = strcmp(buf1 + str_pos(arr1, i), buf2 + str_pos(arr2, i)); break;
			default: return -1;
		}
		if (diff) {
			return diff;
		}
	}
	return 0;
}

int s_array_sprintf(const StrapArray *arr, char *s)
{
	s_array_sfprintf(arr, s, sprintf, SPRINTF, 1);
}

int s_array_fprintf(const StrapArray *arr, FILE *s)
{
	s_array_sfprintf(arr, s, fprintf, FPRINTF, 0);
}
