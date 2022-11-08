#include "strap_internal.h"
#include "strap_array_num.h"
#include <string.h>

#define SUM_F(arr, sum) SUM_NUM(arr, sum, SWITCH_F)
#define SUM_I(arr, sum) SUM_NUM(arr, sum, SWITCH_I)

#define SUM_NUM(arr, sum, SWITCH)	\
do {					\
	num_ptr narr;			\
	StrapType type;			\
	size_t count;			\
	size_t i;			\
					\
	sum = 0;			\
	count = arr->count;		\
	type = arr->type;		\
	narr.i8 = arr->data;		\
	for (i = 0; i < count; i++) {	\
		SWITCH(sum, type, narr)	\
	}				\
} while (0)

#define SWITCH_I(sum, type, narr)				\
switch (type) {							\
	case STRAP_TYPE_CHAR:  sum += narr.i8[i];  break;	\
	case STRAP_TYPE_SHORT: sum += narr.i16[i]; break;	\
	case STRAP_TYPE_INT:   sum += narr.i32[i]; break;	\
	case STRAP_TYPE_LONG:  sum += narr.i64[i]; break;	\
	default: break;						\
}

#define SWITCH_F(sum, type, narr)					\
switch (type) {								\
	case STRAP_TYPE_FLOAT:       sum += narr.f32[i];  break;	\
	case STRAP_TYPE_DOUBLE:      sum += narr.f64[i];  break;	\
	case STRAP_TYPE_LONG_DOUBLE: sum += narr.f128[i]; break;	\
	default: break;							\
}

int num_resize_capacity(StrapArray *arr, size_t capacity)
{
	void *ndata = realloc(arr->data, capacity*s_sizeof(arr->type));
	if (!ndata)
		return 1;
	arr->data = ndata;
	arr->capacity = capacity;
	return 0;
}

int s_array_sum_int(const StrapArray *arr)
{
	int sum;

	if (!arr)
		return 0;
	SUM_I(arr, sum);
	return sum;
}

long s_array_sum_long(const StrapArray *arr)
{
	long sum;

	if (!arr)
		return 0;
	SUM_I(arr, sum);
	return sum;
}

unsigned int s_array_sum_uint(const StrapArray *arr)
{
	unsigned int sum;

	if (!arr)
		return 0;
	SUM_I(arr, sum);
	return sum;
}

unsigned long s_array_sum_ulong(const StrapArray *arr)
{
	unsigned long sum;

	if (!arr)
		return 0;
	SUM_I(arr, sum);
	return sum;
}

double s_array_sum_double(const StrapArray *arr)
{
	double sum;

	if (!arr)
		return 0;
	SUM_F(arr, sum);
	return sum;
}

long double s_array_sum_longdouble(const StrapArray *arr)
{
	long double sum;

	if (!arr)
		return 0;
	SUM_F(arr, sum);
	return sum;
}

/* char */

char s_array_get_char(const StrapArray *arr, size_t idx)
{
	CHAR_MACRO(s_array_get_num, arr, idx);
}

StrapArray *s_array_append_char(StrapArray *arr, char num)
{
	CHAR_MACRO(s_array_append_num, arr, num);
}

StrapArray *s_array_insert_char(StrapArray *arr, size_t idx, char num)
{
	CHAR_MACRO(s_array_insert_num, arr, idx, num);
}

StrapArray *s_array_replace_char(StrapArray *arr, size_t idx, char num)
{
	CHAR_MACRO(s_array_replace_num, arr, idx, num);
}

size_t s_array_find_char(const StrapArray *arr, char num)
{
	return s_array_nfind_char(arr, num, 0);
}

size_t s_array_nfind_char(const StrapArray *arr, char num, size_t n)
{
	CHAR_MACRO(s_array_nfind_num, arr, num, n);
}

/* short */

short s_array_get_short(const StrapArray *arr, size_t idx)
{
		SHORT_MACRO(s_array_get_num, arr, idx);
}

StrapArray *s_array_append_short(StrapArray *arr, short num)
{
		SHORT_MACRO(s_array_append_num, arr, num);
}

StrapArray *s_array_insert_short(StrapArray *arr, size_t idx, short num)
{
		SHORT_MACRO(s_array_insert_num, arr, idx, num);
}

StrapArray *s_array_replace_short(StrapArray *arr, size_t idx, short num)
{
		SHORT_MACRO(s_array_replace_num, arr, idx, num);
}

size_t s_array_find_short(const StrapArray *arr, short num)
{
		return s_array_nfind_short(arr, num, 0);
}

size_t s_array_nfind_short(const StrapArray *arr, short num, size_t n)
{
		SHORT_MACRO(s_array_nfind_num, arr, num, n);
}

/* int */

int s_array_get_int(const StrapArray *arr, size_t idx)
{
	INT_MACRO(s_array_get_num, arr, idx);
}

StrapArray *s_array_append_int(StrapArray *arr, int num)
{
	INT_MACRO(s_array_append_num, arr, num);
}

StrapArray *s_array_insert_int(StrapArray *arr, size_t idx, int num)
{
	INT_MACRO(s_array_insert_num, arr, idx, num);
}

StrapArray *s_array_replace_int(StrapArray *arr, size_t idx, int num)
{
	INT_MACRO(s_array_replace_num, arr, idx, num);
}

size_t s_array_find_int(const StrapArray *arr, int num)
{
	return s_array_nfind_int(arr, num, 0);
}

size_t s_array_nfind_int(const StrapArray *arr, int num, size_t n)
{
	INT_MACRO(s_array_nfind_num, arr, num, n);
}

/* long */

long s_array_get_long(const StrapArray *arr, size_t idx)
{
	LONG_MACRO(s_array_get_num, arr, idx);
}

StrapArray *s_array_append_long(StrapArray *arr, long num)
{
	LONG_MACRO(s_array_append_num, arr, num);
}

StrapArray *s_array_insert_long(StrapArray *arr, size_t idx, long num)
{
	LONG_MACRO(s_array_insert_num, arr, idx, num);
}

StrapArray *s_array_replace_long(StrapArray *arr, size_t idx, long num)
{
	LONG_MACRO(s_array_replace_num, arr, idx, num);
}

size_t s_array_find_long(const StrapArray *arr, long num)
{
	return s_array_nfind_long(arr, num, 0);
}

size_t s_array_nfind_long(const StrapArray *arr, long num, size_t n)
{
	LONG_MACRO(s_array_nfind_num, arr, num, n);
}

/* float */

float s_array_get_float(const StrapArray *arr, size_t idx)
{
	FLOAT_MACRO(s_array_get_num, arr, idx);
}

StrapArray *s_array_append_float(StrapArray *arr, float num)
{
	FLOAT_MACRO(s_array_append_num, arr, num);
}

StrapArray *s_array_insert_float(StrapArray *arr, size_t idx, float num)
{
	FLOAT_MACRO(s_array_insert_num, arr, idx, num);
}

StrapArray *s_array_replace_float(StrapArray *arr, size_t idx, float num)
{
	FLOAT_MACRO(s_array_replace_num, arr, idx, num);
}

size_t s_array_find_float(const StrapArray *arr, float num)
{
	return s_array_nfind_float(arr, num, 0);
}

size_t s_array_nfind_float(const StrapArray *arr, float num, size_t n)
{
	FLOAT_MACRO(s_array_nfind_num, arr, num, n);
}

/* double */

double s_array_get_double(const StrapArray *arr, size_t idx)
{
	DOUBLE_MACRO(s_array_get_num, arr, idx);
}

StrapArray *s_array_append_double(StrapArray *arr, double num)
{
	DOUBLE_MACRO(s_array_append_num, arr, num);
}

StrapArray *s_array_insert_double(StrapArray *arr, size_t idx, double num)
{
	DOUBLE_MACRO(s_array_insert_num, arr, idx, num);
}

StrapArray *s_array_replace_double(StrapArray *arr, size_t idx, double num)
{
	DOUBLE_MACRO(s_array_replace_num, arr, idx, num);
}

size_t s_array_find_double(const StrapArray *arr, double num)
{
	return s_array_nfind_double(arr, num, 0);
}

size_t s_array_nfind_double(const StrapArray *arr, double num, size_t n)
{
	DOUBLE_MACRO(s_array_nfind_num, arr, num, n);
}

/* long double */

long double s_array_get_longdouble(const StrapArray *arr, size_t idx)
{
	LONGDOUBLE_MACRO(s_array_get_num, arr, idx);
}

StrapArray *s_array_append_longdouble(StrapArray *arr, long double num)
{
	LONGDOUBLE_MACRO(s_array_append_num, arr, num);
}

StrapArray *s_array_insert_longdouble(StrapArray *arr, size_t idx, long double num)
{
	LONGDOUBLE_MACRO(s_array_insert_num, arr, idx, num);
}

StrapArray *s_array_replace_longdouble(StrapArray *arr, size_t idx, long double num)
{
	LONGDOUBLE_MACRO(s_array_replace_num, arr, idx, num);
}

size_t s_array_find_longdouble(const StrapArray *arr, long double num)
{
	return s_array_nfind_longdouble(arr, num, 0);
}

size_t s_array_nfind_longdouble(const StrapArray *arr, long double num, size_t n)
{
	LONGDOUBLE_MACRO(s_array_nfind_num, arr, num, n);
}
