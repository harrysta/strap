#ifndef STRAP_INTERNAL_H
#define STRAP_INTERNAL_H

#include "strap.h"
#include <stdio.h>

#define S_ARRSTR(a) (char*) (a)->array + (a)->array_size
#define S_ARRISTR(a,i) S_ARRSTR(a) + (i ? (a)->array[(i) - 1] + 1 : 0)
#define S_STRLEN(a) (a) ? strlen((a)) : 0

struct StrapString {
	size_t length;
	size_t size;
	char *data;
};

struct StrapArray {
	StrapType type;
	void *data;
};

/*
* array[] contains the index of the null terminating char of each element
* from string[], which is used for:
*
*   Getter: string + array[i-1] + 1
*   Length: array[i] - array[i-1]
*
* Note: the above will not apply for the first element, as it doesn't/can't
* refer to the previous element.
*/
typedef struct {
	size_t count;
	size_t array_size;
	size_t string_size;
	size_t array[STRAP_INIT_CAPACITY];
	char string[STRAP_INIT_STR_SIZE];
} StrapArray_str;

struct num_array {
	size_t count;
	size_t capacity;
	char array[1];
};

struct int_array {
	size_t count;
	size_t capacity;
	int array[STRAP_INIT_CAPACITY];
};

struct float_array {
	size_t count;
	size_t capacity;
	float array[STRAP_INIT_CAPACITY];
};

union num {
	short short_t;
	int int_t;
	long long_t;
	float float_t;
	double double_t;
};

typedef union num num_t;

extern size_t strap_next_pow2(size_t n, size_t min);

extern StrapArray *strap_array_erase_range_str(StrapArray *arr, size_t i, size_t n);
extern StrapArray *strap_array_clear_str(StrapArray *arr);
extern StrapArray *strap_array_create_subarray_str(const StrapArray *arr, size_t i, size_t n);
extern StrapArray *strap_array_reverse_str(StrapArray *arr);
extern StrapArray *strap_array_shrink_str(StrapArray *arr);
extern StrapArray *strap_array_sort_str(StrapArray *arr, int ascending);


extern int strap_array_sprintf_str(const StrapArray_str *arr, char *cstr);
extern int strap_array_fprintf_str(const StrapArray_str *arr, FILE *stream);
extern int strap_array_fprintf_int(const struct int_array *iarr, FILE *stream);

#endif
