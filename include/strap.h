#ifndef STRAP_H
#define STRAP_H

#include <stddef.h>

typedef enum {
	STRAP_TYPE_SHORT,
	STRAP_TYPE_INT,
	STRAP_TYPE_LONG,
	STRAP_TYPE_FLOAT,
	STRAP_TYPE_DOUBLE,
	STRAP_TYPE_STRING,
	STRAP_TYPE_CSTR,
	STRAP_TYPE_COUNT
} StrapType;

typedef struct StrapString StrapString;
typedef struct StrapArray StrapArray;

extern StrapString *strap_string_alloc(const char *cstr);
extern StrapString *strap_string_clone(const StrapString *str);
extern char        *strap_string_get_cstr(const StrapString *str);
extern size_t       strap_string_length(const StrapString *str);

extern StrapString *strap_string_copy(StrapString *str1, const StrapString *str2);
extern StrapString *strap_string_concat(StrapString *str1, const StrapString *str2);
extern StrapString *strap_string_copy_from(StrapString *str, const char *cstr);
extern char        *strap_string_copy_to(const StrapString *str, char *cstr);
extern StrapString *strap_string_strcat(StrapString *str, const char *cstr);

extern StrapString *strap_string_ncopy(StrapString *str1, const StrapString *str2, size_t n);
extern StrapString *strap_string_nconcat(StrapString *str1, const StrapString *str2, size_t n);
extern StrapString *strap_string_ncopy_from(StrapString *str, const char *cstr, size_t n);
extern char        *strap_string_ncopy_to(const StrapString *str, char *cstr, size_t n);
extern StrapString *strap_string_nstrcat(StrapString *str, const char *cstr, size_t n);

extern StrapString *strap_string_create_substring(const StrapString *str, size_t start, size_t n);
extern StrapString *strap_string_erase(StrapString *str, size_t start, size_t n);
extern StrapString *strap_string_trim(StrapString *str);
extern int          strap_string_compare(const StrapString *str1, const StrapString *str2);
extern size_t       strap_string_find(const StrapString *str1, const StrapString *str2);
extern StrapArray  *strap_string_split(StrapString *str, const char *sep);
extern StrapString *strap_string_reverse(StrapString *str);
extern StrapString *strap_string_shrink(StrapString *str);

extern StrapArray *strap_array_alloc(StrapType type);
extern void        strap_array_free(StrapArray *arr);
extern StrapArray *strap_array_clone(const StrapArray *arr);
extern size_t      strap_array_count(const StrapArray *arr);

extern StrapString *strap_array_create_string(const StrapArray *arr, size_t i);
extern StrapString *strap_array_copy_string(const StrapArray *arr, size_t i, StrapString *str);
extern StrapArray  *strap_array_append_string(StrapArray *arr, const StrapString *str);
extern StrapArray  *strap_array_insert_string(StrapArray *arr, size_t i, const StrapString *str);
extern StrapArray  *strap_array_replace_string(StrapArray *arr, size_t i, const StrapString *str);
extern size_t      *strap_array_find_string(const StrapArray *arr, StrapString *str);

extern const char  *strap_array_get_cstr(const StrapArray *arr, size_t i);
extern StrapArray  *strap_array_append_cstr(StrapArray *arr, const char *cstr);
extern StrapArray  *strap_array_insert_cstr(StrapArray *arr, size_t i, const char *cstr);
extern StrapArray  *strap_array_replace_cstr(StrapArray *arr, size_t i, const char *cstr);
extern size_t      *strap_array_find_cstr(const StrapArray *arr, const char *cstr);

extern short      *strap_array_get_short(const StrapArray *arr, size_t i);
extern StrapArray *strap_array_append_short(StrapArray *arr, short num);
extern StrapArray *strap_array_insert_short(StrapArray *arr, size_t i, short num);
extern StrapArray *strap_array_replace_short(StrapArray *arr, size_t i, short num);
extern size_t     *strap_array_find_short(const StrapArray *arr, short num);

extern int        *strap_array_get_int(const StrapArray *arr, size_t i);
extern StrapArray *strap_array_append_int(StrapArray *arr, int num);
extern StrapArray *strap_array_insert_int(StrapArray *arr, size_t i, int num);
extern StrapArray *strap_array_replace_int(StrapArray *arr, size_t i, int num);
extern size_t     *strap_array_find_int(const StrapArray *arr, int num);

extern long       *strap_array_get_long(const StrapArray *arr, size_t i);
extern StrapArray *strap_array_append_long(StrapArray *arr, long num);
extern StrapArray *strap_array_insert_long(StrapArray *arr, size_t i, long num);
extern StrapArray *strap_array_replace_long(StrapArray *arr, size_t i, long num);
extern size_t     *strap_array_find_long(const StrapArray *arr, long num);

extern float      *strap_array_get_float(const StrapArray *arr, size_t i);
extern StrapArray *strap_array_append_float(StrapArray *arr, float num);
extern StrapArray *strap_array_insert_float(StrapArray *arr, size_t i, float num);
extern StrapArray *strap_array_replace_float(StrapArray *arr, size_t i, float num);
extern size_t     *strap_array_find_float(const StrapArray *arr, float num);

extern double     *strap_array_get_double(const StrapArray *arr, size_t i);
extern StrapArray *strap_array_append_double(StrapArray *arr, double num);
extern StrapArray *strap_array_insert_double(StrapArray *arr, size_t i, double num);
extern StrapArray *strap_array_replace_double(StrapArray *arr, size_t i, double num);
extern size_t     *strap_array_find_double(const StrapArray *arr, double num);

extern StrapArray  *strap_array_concat(StrapArray *arr1, const StrapArray *arr2);
extern StrapArray  *strap_array_insert_array(StrapArray *arr1, size_t i, const StrapArray *arr2, size_t start, size_t n);
extern StrapArray  *strap_array_clear(StrapArray *arr);
extern StrapArray  *strap_array_create_subarray(const StrapArray *arr, size_t start, size_t n);
extern StrapArray  *strap_array_remove(StrapArray *arr, size_t i);
extern StrapArray  *strap_array_reverse(StrapArray *arr);
extern StrapArray  *strap_array_shrink(StrapArray *arr);
extern StrapArray  *strap_array_sort(StrapArray *arr, int ascending);
extern StrapArray  *strap_array_printf(const StrapArray *arr, char *cstr);

#endif
