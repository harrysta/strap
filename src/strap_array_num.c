#include "strap_internal.h"
#include "strap_array_num.h"

int num_resize_capacity(StrapArray *arr, size_t capacity)
{
	void *ndata = realloc(arr->data, capacity*strap_sizeof(arr->type));
	if (!ndata)
		return 1;
	arr->data = ndata;
	arr->capacity = capacity;
	return 0;
}

int strap_array_sum_int(const StrapArray *arr)
{
	int (*get_func)(const StrapArray*, size_t);
	int sum;
	size_t count;
	size_t i;

	if (!arr)
		return 0;
	count = arr->count;
	if (!count)
		return 0;
	switch (arr->type) {
		case STRAP_TYPE_CHAR:
			get_func = num_char_as_int;
			break;
		case STRAP_TYPE_SHORT:
			get_func = num_short_as_int;
			break;
		case STRAP_TYPE_INT:
			get_func = num_int_as_int;
			break;
		case STRAP_TYPE_LONG_INT:
			get_func = num_long_as_int;;
			break;
		default:
			return 0;
	}
	sum = 0;
	for (i = 0; i < count; i++)
		sum += get_func(arr, i);
	return sum;
}

long strap_array_sum_long(const StrapArray *arr)
{
	long (*get_func)(const StrapArray*, size_t);
	long sum;
	size_t count;
	size_t i;

	if (!arr)
		return 0;
	count = arr->count;
	if (!count)
		return 0;
	switch (arr->type) {
		case STRAP_TYPE_CHAR:
			get_func = num_char_as_long;
			break;
		case STRAP_TYPE_SHORT:
			get_func = num_short_as_long;
			break;
		case STRAP_TYPE_INT:
			get_func = num_int_as_long;
			break;
		case STRAP_TYPE_LONG_INT:
			get_func = num_long_as_long;;
			break;
		default:
			return 0;
	}
	sum = 0;
	for (i = 0; i < count; i++)
		sum += get_func(arr, i);
	return sum;
}

unsigned int strap_array_sum_uint(const StrapArray *arr)
{
	unsigned int (*get_func)(const StrapArray*, size_t);
	unsigned int sum;
	size_t count;
	size_t i;

	if (!arr)
		return 0;
	count = arr->count;
	if (!count)
		return 0;
	switch (arr->type) {
		case STRAP_TYPE_CHAR:
			get_func = num_char_as_uint;
			break;
		case STRAP_TYPE_SHORT:
			get_func = num_short_as_uint;
			break;
		case STRAP_TYPE_INT:
			get_func = num_int_as_uint;
			break;
		case STRAP_TYPE_LONG_INT:
			get_func = num_long_as_uint;;
			break;
		default:
			return 0;
	}
	sum = 0;
	for (i = 0; i < count; i++)
		sum += get_func(arr, i);
	return sum;
}

unsigned long strap_array_sum_ulong(const StrapArray *arr)
{
	unsigned long (*get_func)(const StrapArray*, size_t);
	unsigned long sum;
	size_t count;
	size_t i;

	if (!arr)
		return 0;
	count = arr->count;
	if (!count)
		return 0;
	switch (arr->type) {
		case STRAP_TYPE_CHAR:
			get_func = num_char_as_ulong;
			break;
		case STRAP_TYPE_SHORT:
			get_func = num_short_as_ulong;
			break;
		case STRAP_TYPE_INT:
			get_func = num_int_as_ulong;
			break;
		case STRAP_TYPE_LONG_INT:
			get_func = num_long_as_ulong;;
			break;
		default:
			return 0;
	}
	sum = 0;
	for (i = 0; i < count; i++)
		sum += get_func(arr, i);
	return sum;
}

double strap_array_sum_double(const StrapArray *arr)
{
	double (*get_func)(const StrapArray*, size_t);
	double sum;
	size_t count;
	size_t i;

	if (!arr)
		return 0;
	count = arr->count;
	if (!count)
		return 0;
	switch (arr->type) {
		case STRAP_TYPE_FLOAT:
			get_func = num_float_as_double;
			break;
		case STRAP_TYPE_DOUBLE:
			get_func = num_double_as_double;
			break;
		case STRAP_TYPE_LONG_DOUBLE:
			get_func = num_longdouble_as_double;
			break;
		default:
			return 0;
	}
	sum = 0;
	for (i = 0; i < count; i++)
		sum += get_func(arr, i);
	return sum;
}

long double strap_array_sum_longdouble(const StrapArray *arr)
{
	long double (*get_func)(const StrapArray*, size_t);
	long double sum;
	size_t count;
	size_t i;

	if (!arr)
		return 0;
	count = arr->count;
	if (!count)
		return 0;
	switch (arr->type) {
		case STRAP_TYPE_FLOAT:
			get_func = num_float_as_longdouble;
			break;
		case STRAP_TYPE_DOUBLE:
			get_func = num_double_as_longdouble;
			break;
		case STRAP_TYPE_LONG_DOUBLE:
			get_func = num_longdouble_as_longdouble;
			break;
		default:
			return 0;
	}
	sum = 0;
	for (i = 0; i < count; i++)
		sum += get_func(arr, i);
	return sum;
}

