#ifndef STRAP_ARRAY_STR_H
#define STRAP_ARRAY_STR_H

#include "strap_internal.h"

/* Macros used to access str_array content from StrapArray */
#define str_sarr(arr) ((struct str_array*) arr->data)
#define str_buf(arr) (str_sarr(arr)->buf)
#define str_null(arr, idx) (str_sarr(arr)->nulls[idx])
#define str_pos(arr, idx) ((idx) ? (str_sarr(arr)->nulls[(idx) - 1] + 1) : 0)

#define str_check_size(arr, len, ret)                                                 \
do {                                                                                  \
	struct str_array *sarr = arr->data;                                                 \
	ushort nullpos = str_null(arr, arr->count - 1);                                     \
	if (nullpos + len > sarr->buflen) {                                                 \
		if (str_resize_buf(arr, strap_next_pow2(nullpos + len + 1, STRAP_INIT_STR_SIZE))) \
			return ret;                                                                     \
	}                                                                                   \
	if (arr->count == arr->capacity) {                                                  \
		if (str_resize_capacity(arr, arr->capacity + STRAP_INIT_CAPACITY))                \
			return ret;                                                                     \
	}                                                                                   \
} while (0)

/**
 * nulls[] contains the index of the null terminating char of each element
 * from buf[], which is used for:
 *
 *   Getter: buf + nulls[i-1] + 1
 *   Length: nulls[i] - nulls[i-1]
 *
 * Note: the above will not apply for the first element, as it doesn't/can't
 * refer to the previous element. In general, these values and should be
 * accessed and manipulated using the str macros defined above.
 *
 * buf[] contains the string elements separated by a null terminating char:
 *
 *   [ s t r 1 \0 s t r 2 \0 s t r i n g 3 \0 ]
 *   count = 3, nulls = [4, 9, 17]
 *
 */
struct str_array {
	size_t buflen;
	ushort *nulls;
	char buf[1];
};

extern int str_resize_capacity(StrapArray *arr, size_t capacity);
extern int str_resize_buf(StrapArray *arr, size_t buflen);

extern struct str_array* str_resize(StrapArray *arr,
	size_t capacity, size_t buflen);

#endif
