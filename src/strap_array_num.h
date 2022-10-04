#define NARR_INIT(arr, ret, strap_type, data_type) \
do {                                               \
	if (!arr || arr->type != strap_type)             \
		return ret;                                    \
	iarr = (data_type*) arr->data;                   \
} while (0)                                        \

#define STRAP_ARRAY_GET_NUM(arr, idx, strap_type, data_type) \
do {                                                         \
	size_t count;                                              \
	data_type *iarr;                                           \
                                                             \
	NARR_INIT(arr, 0, strap_type, data_type);                  \
	count = arr->count;                                        \
	return idx < count ? iarr[idx] : 0;                        \
} while (0)                                                  \

#define STRAP_ARRAY_APPEND_NUM(arr, num, strap_type, data_type) \
do {                                                            \
	data_type *iarr;                                              \
                                                                \
	NARR_INIT(arr, arr, strap_type, data_type);                   \
	iarr[arr->count++] = num;                                     \
	return arr;                                                   \
} while (0)                                                     \

#define STRAP_ARRAY_INSERT_NUM(arr, idx, num, strap_type, data_type) \
do {                                                                 \
	data_type *iarr;                                                   \
	size_t count;                                                      \
	size_t bytes;                                                      \
                                                                     \
	NARR_INIT(arr, arr, strap_type, data_type);                        \
	count = arr->count;                                                \
	if (!count && !idx || count && idx == count) {                     \
		iarr[arr->count++] = num;                                        \
		return arr;                                                      \
	} else if (count && idx >= count) {                                \
		return arr;                                                      \
	}                                                                  \
	bytes = sizeof(data_type)*(count - idx);                           \
	memcpy(&iarr[idx + 1], &iarr[idx], bytes);                         \
	iarr[idx] = num;                                                   \
	arr->count++;                                                      \
	return arr;                                                        \
} while (0)                                                          \

