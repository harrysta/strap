#ifndef STRAP_INTERNAL_H
#define STRAP_INTERNAL_H

#include "strap.h"
#include <stdio.h>

#define S_ARRSTR(a) (char*) (a)->lens + (a)->buflen
#define S_ARRISTR(a,i) S_ARRSTR(a) + (i ? (a)->lens[(i) - 1] + 1 : 0)
#define S_STRLEN(a) (a) ? strlen((a)) : 0

#define str_sarr(arr) ((struct str_array*) arr->data)
#define str_lens(arr) str_sarr(arr)->lens
#define str_buf(arr) (char*) (str_sarr(arr)->lens) + (sizeof *str_sarr(arr)->lens)*arr->capacity
#define str_pos(arr, idx) ((idx) ? (str_sarr(arr)->lens[(idx) - 1] + 1) : 0)
#define str_len(arr, idx) str_sarr(arr)->lens[idx] - ((idx) ? str_sarr(arr)->lens[(idx) - 1] : 0)
#define max(x, y) ((x) > (y) ? (x) : (y))

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
struct str_array {
	size_t buflen;
	unsigned short lens[STRAP_INIT_CAPACITY];
	char buf[STRAP_INIT_STR_SIZE];
};

extern size_t strap_next_pow2(size_t n, size_t min);

extern StrapArray *strap_array_erase_range_str(StrapArray *arr, size_t i, size_t n);
extern StrapArray *strap_array_clear_str(StrapArray *arr);
extern StrapArray *strap_array_create_subarray_str(const StrapArray *arr, size_t i, size_t n);
extern StrapArray *strap_array_reverse_str(StrapArray *arr);
extern StrapArray *strap_array_shrink_str(StrapArray *arr);
extern StrapArray *strap_array_sort_str(StrapArray *arr, int ascending);


extern int strap_array_sprintf_str(const struct str_array *arr, char *cstr);
extern int strap_array_fprintf_str(const struct str_array *arr, FILE *stream);

#endif
