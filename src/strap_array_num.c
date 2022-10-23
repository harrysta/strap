#include "strap_internal.h"
#include "strap_array_num.h"

#define SUM_F(arr, sum) SUM_NUM(arr, sum, SWITCH_F)
#define SUM_I(arr, sum) SUM_NUM(arr, sum, SWITCH_I)

#define SUM_NUM(arr, sum, SWITCH) \
do {                              \
	union num_array_t narr;         \
	StrapType type;                 \
	size_t count;                   \
	size_t i;                       \
                                  \
	sum = 0;                        \
	count = arr->count;             \
	type = arr->type;               \
	narr.i8 = arr->data;            \
	for (i = 0; i < count; i++) {   \
		SWITCH(sum, type, narr)       \
	}                               \
} while (0)

#define SWITCH_I(sum, type, narr)                      \
switch (type) {                                        \
	case STRAP_TYPE_CHAR:     sum += narr.i8[i];  break; \
	case STRAP_TYPE_SHORT:    sum += narr.i16[i]; break; \
	case STRAP_TYPE_INT:      sum += narr.i32[i]; break; \
	case STRAP_TYPE_LONG: sum += narr.i64[i]; break; \
	default: break;                                      \
}

#define SWITCH_F(sum, type, narr)                          \
switch (type) {                                            \
	case STRAP_TYPE_FLOAT:       sum += narr.f32[i];  break; \
	case STRAP_TYPE_DOUBLE:      sum += narr.f64[i];  break; \
	case STRAP_TYPE_LONG_DOUBLE: sum += narr.f128[i]; break; \
	default: break;                                          \
}

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
	int sum;

	if (!arr)
		return 0;
	SUM_I(arr, sum);
	return sum;
}

long strap_array_sum_long(const StrapArray *arr)
{
	long sum;

	if (!arr)
		return 0;
	SUM_I(arr, sum);
	return sum;
}

unsigned int strap_array_sum_uint(const StrapArray *arr)
{
	unsigned int sum;

	if (!arr)
		return 0;
	SUM_I(arr, sum);
	return sum;
}

unsigned long strap_array_sum_ulong(const StrapArray *arr)
{
	unsigned long sum;

	if (!arr)
		return 0;
	SUM_I(arr, sum);
	return sum;
}

double strap_array_sum_double(const StrapArray *arr)
{
	double sum;

	if (!arr)
		return 0;
	SUM_F(arr, sum);
	return sum;
}

long double strap_array_sum_longdouble(const StrapArray *arr)
{
	long double sum;

	if (!arr)
		return 0;
	SUM_F(arr, sum);
	return sum;
}

