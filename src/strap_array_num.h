#include <stdlib.h>

#define NARR_INITT(arr, ret, strap_type) \
do {                                     \
	if (!arr || arr->type != strap_type)   \
		return ret;                          \
	narr = arr->data;                      \
} while (0)                              \

#define STRAP_ARRAY_GET_NUM(arr, idx, strap_type, data_type) \
do {                                                         \
	struct num_array *narr;                                    \
	size_t count;                                              \
	data_type *iarr;                                           \
                                                             \
	NARR_INITT(arr, 0, strap_type);                            \
	count = narr->count;                                       \
	iarr = (data_type*) narr->array;                           \
	return idx < count ? iarr[idx] : 0;                        \
} while (0)                                                  \

#define STRAP_ARRAY_APPEND_NUM(arr, num, strap_type, data_type) \
do {                                                            \
	struct num_array *narr;                                       \
	data_type *iarr;                                              \
                                                                \
	NARR_INITT(arr, arr, strap_type);                             \
	iarr = (data_type*) narr->array;                              \
	iarr[narr->count++] = num;                                    \
	return arr;                                                   \
} while (0)                                                     \

#define STRAP_ARRAY_INSERT_NUM(arr, idx, num, strap_type, data_type) \
do {                                                                 \
	struct num_array *narr;                                            \
	data_type *iarr;                                                   \
	size_t count;                                                      \
	size_t bytes;                                                      \
                                                                     \
	NARR_INITT(arr, arr, strap_type);                                  \
	count = narr->count;                                               \
	iarr = (data_type*) narr->array;                                   \
	if (!count && !idx || count && idx == count) {                     \
		iarr[narr->count++] = num;                                       \
		return arr;                                                      \
	} else if (count && idx >= count) {                                \
		return arr;                                                      \
	}                                                                  \
	bytes = sizeof(data_type)*(count - idx);                           \
	memcpy(&iarr[idx + 1], &iarr[idx], bytes);                         \
	iarr[idx] = num;                                                   \
	narr->count++;                                                     \
	return arr;                                                        \
} while (0)                                                          \

struct num_array;
