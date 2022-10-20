#define narr_init(arr, ret, strap_type, data_type) \
do {                                               \
	if (!arr || arr->type != strap_type)             \
		return ret;                                    \
	iarr = (data_type*) arr->data;                   \
} while (0)

#define strap_array_get_num(arr, idx, strap_type, data_type) \
do {                                                         \
	size_t count;                                              \
	data_type *iarr;                                           \
                                                             \
	narr_init(arr, 0, strap_type, data_type);                  \
	count = arr->count;                                        \
	return idx < count ? iarr[idx] : 0;                        \
} while (0)

#define num_check_size(arr, capacity, ret)                             \
do {                                                                   \
	if (arr->count == arr->capacity) {                                   \
		if (num_resize_capacity(arr, arr->capacity + STRAP_INIT_CAPACITY)) \
			return ret;                                                      \
	}                                                                    \
} while (0)

#define strap_array_append_num(arr, num, strap_type, data_type)        \
do {                                                                   \
	data_type *iarr;                                                     \
                                                                       \
	narr_init(arr, arr, strap_type, data_type);                          \
	num_check_size(arr, capacity, arr);                                  \
	iarr[arr->count++] = num;                                            \
	return arr;                                                          \
} while (0)

#define strap_array_insert_num(arr, idx, num, strap_type, data_type) \
do {                                                                 \
	data_type *iarr;                                                   \
	size_t count;                                                      \
	size_t bytes;                                                      \
                                                                     \
	narr_init(arr, arr, strap_type, data_type);                        \
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
} while (0)

