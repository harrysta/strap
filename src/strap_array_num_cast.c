#include "strap_internal.h"

int num_char_as_int(const StrapArray *arr, size_t idx)
{
	return ((char*)arr->data)[idx];
}

int num_short_as_int(const StrapArray *arr, size_t idx)
{
		return ((short*)arr->data)[idx];
}

int num_int_as_int(const StrapArray *arr, size_t idx)
{
		return ((int*)arr->data)[idx];
}

int num_long_as_int(const StrapArray *arr, size_t idx)
{
		return ((long*)arr->data)[idx];
}

long num_char_as_long(const StrapArray *arr, size_t idx)
{
	return ((char*)arr->data)[idx];
}

long num_short_as_long(const StrapArray *arr, size_t idx)
{
		return ((short*)arr->data)[idx];
}

long num_int_as_long(const StrapArray *arr, size_t idx)
{
		return ((int*)arr->data)[idx];
}

long num_long_as_long(const StrapArray *arr, size_t idx)
{
		return ((long*)arr->data)[idx];
}

unsigned int num_char_as_uint(const StrapArray *arr, size_t idx)
{
	return ((char*)arr->data)[idx];
}

unsigned int num_short_as_uint(const StrapArray *arr, size_t idx)
{
		return ((short*)arr->data)[idx];
}

unsigned int num_int_as_uint(const StrapArray *arr, size_t idx)
{
		return ((int*)arr->data)[idx];
}

unsigned int num_long_as_uint(const StrapArray *arr, size_t idx)
{
		return ((long*)arr->data)[idx];
}

unsigned long num_char_as_ulong(const StrapArray *arr, size_t idx)
{
	return ((char*)arr->data)[idx];
}

unsigned long num_short_as_ulong(const StrapArray *arr, size_t idx)
{
		return ((short*)arr->data)[idx];
}

unsigned long num_int_as_ulong(const StrapArray *arr, size_t idx)
{
		return ((int*)arr->data)[idx];
}

unsigned long num_long_as_ulong(const StrapArray *arr, size_t idx)
{
		return ((long*)arr->data)[idx];
}

double num_float_as_double(const StrapArray *arr, size_t idx)
{
	return ((float*)arr->data)[idx];
}

double num_double_as_double(const StrapArray *arr, size_t idx)
{
	return ((double*)arr->data)[idx];
}

double num_longdouble_as_double(const StrapArray *arr, size_t idx)
{
	return ((long double*)arr->data)[idx];
}

long double num_float_as_longdouble(const StrapArray *arr, size_t idx)
{
	return ((float*)arr->data)[idx];
}

long double num_double_as_longdouble(const StrapArray *arr, size_t idx)
{
	return ((double*)arr->data)[idx];
}

long double num_longdouble_as_longdouble(const StrapArray *arr, size_t idx)
{
	return ((long double*)arr->data)[idx];
}

