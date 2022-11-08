#pragma once

#include "strap.h"
#include <sys/types.h>

#define CHAR_MACRO(MACRO, ...) MACRO(__VA_ARGS__, STRAP_TYPE_CHAR, char)
#define SHORT_MACRO(MACRO, ...) MACRO(__VA_ARGS__, STRAP_TYPE_SHORT, short)
#define INT_MACRO(MACRO, ...) MACRO(__VA_ARGS__, STRAP_TYPE_INT, int)
#define LONG_MACRO(MACRO, ...) MACRO(__VA_ARGS__, STRAP_TYPE_LONG, long)
#define FLOAT_MACRO(MACRO, ...) MACRO(__VA_ARGS__, STRAP_TYPE_FLOAT, float)
#define DOUBLE_MACRO(MACRO, ...) MACRO(__VA_ARGS__, STRAP_TYPE_DOUBLE, double)
#define LONGDOUBLE_MACRO(MACRO, ...) MACRO(__VA_ARGS__, STRAP_TYPE_LONG_DOUBLE, long double)

#define narr_init(arr, ret, s_type, data_type)						\
do {											\
	if (!arr || arr->type != s_type)						\
		return ret;								\
	iarr = (data_type*) arr->data;							\
} while (0)

#define num_check_size(arr, ret)							\
do {								                        \
	if (arr->count == arr->capacity) {						\
		if (num_resize_capacity(arr, arr->capacity + STRAP_INIT_CAPACITY))	\
			return ret;							\
	}										\
} while (0)

#define s_array_get_num(arr, idx, s_type, data_type)	\
do {							\
	size_t count;					\
	data_type *iarr;				\
							\
	narr_init(arr, 0, s_type, data_type);		\
	count = arr->count;				\
	return idx < count ? iarr[idx] : 0;		\
} while (0)

#define s_array_append_num(arr, num, s_type, data_type)	\
do {							\
	data_type *iarr;				\
							\
	narr_init(arr, arr, s_type, data_type);		\
	num_check_size(arr, arr);			\
	iarr[arr->count++] = num;			\
	return arr;					\
} while (0)

#define s_array_insert_num(arr, idx, num, s_type, data_type)	\
do {								\
	data_type *iarr;					\
	size_t count;						\
	size_t bytes;						\
								\
	narr_init(arr, arr, s_type, data_type);			\
	num_check_size(arr, arr);				\
	count = arr->count;					\
	if (!count && !idx || count && idx == count) {		\
		iarr[arr->count++] = num;			\
		return arr;					\
	} else if (count && idx >= count) {			\
		return arr;					\
	}							\
	bytes = sizeof(data_type)*(count - idx);		\
	memcpy(&iarr[idx + 1], &iarr[idx], bytes);		\
	iarr[idx] = num;					\
	arr->count++;						\
	return arr;						\
} while (0)

#define s_array_replace_num(arr, idx, num, s_type, data_type)	\
do {								\
	data_type *iarr;					\
								\
	narr_init(arr, arr, s_type, data_type);			\
	if (idx > arr->count - 1)				\
		return arr;					\
	iarr[idx] = num;					\
	return arr;						\
} while (0)

#define s_array_nfind_num(arr, num, n, s_type, data_type)		\
do {								        \
	data_type *iarr;						\
	size_t counted;							\
	size_t count;							\
	size_t i;							\
								        \
	narr_init(arr, -1, s_type, data_type);				\
	count = arr->count;						\
	if (!count)							\
		return -1;						\
	for (i = 0, counted = 0; i < count && counted <= n; i++) {	\
		if (iarr[i] == num) {					\
			if (counted == n)				\
				return i;				\
			counted++;					\
		}							\
	}								\
	return -1;							\
} while (0)

#define s_array_sum_num(arr, sum_type, s_type, data_type)	\
do {								\
	data_type *iarr;					\
	sum_type sum;						\
	size_t count;						\
	size_t i;						\
								\
	if (!arr || arr->type == STRAP_TYPE_STRING)		\
		return 0;					\
	iarr = (data_type*) arr->data;				\
	count = arr->count;					\
	sum = 0;						\
	for (i = 0; i < count; i++)				\
			sum += (sum_type) iarr[i];		\
	return sum;						\
} while (0)

#define NUM_REVERSE(arr, start, end, data_type)	\
do {						\
	data_type tmp;				\
	data_type *numarr;			\
						\
	numarr = arr->data;			\
	while (start < end) {			\
		tmp = numarr[start];		\
		numarr[start++] = numarr[end];	\
		numarr[end--] = tmp;		\
	}					\
} while (0)


typedef union {
	char        *i8;
	short       *i16;
	int         *i32;
	long        *i64;
	float       *f32;
	double      *f64;
	long double *f128;
} num_ptr;

extern int num_resize_capacity(StrapArray *arr, size_t capacity);
