#pragma once

#include "strap.h"
#include <stdlib.h>

#define max(x, y) ((x) > (y) ? (x) : (y))

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

struct StrapString {
	size_t length;
	size_t size;
	char *data;
};

struct StrapArray {
	StrapType type;
	size_t capacity;
	size_t count;
	void *data;
};

extern size_t s_next_pow2(size_t n, size_t min);

extern StrapArray *s_array_erase_range_str(StrapArray *arr, size_t i, size_t n);
extern StrapArray *s_array_clear_str(StrapArray *arr);
extern StrapArray *s_array_create_subarray_str(const StrapArray *arr, size_t i, size_t n);
extern StrapArray *s_array_reverse_str(StrapArray *arr);
extern StrapArray *s_array_shrink_str(StrapArray *arr);
extern StrapArray *s_array_sort_str(StrapArray *arr, int ascending);
