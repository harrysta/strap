#pragma once

#include <stddef.h>
#include <stdio.h>

#define s_array_printf(a) s_array_fprintf((a), stdout)
#define STRAP_INIT_STR_SIZE 256
#define STRAP_INIT_CAPACITY 16
#define STRAP_NO_MATCH ((size_t) -1)

typedef enum StrapType
{
	STRAP_TYPE_CHAR,
	STRAP_TYPE_SHORT,
	STRAP_TYPE_INT,
	STRAP_TYPE_LONG,

	STRAP_TYPE_FLOAT,
	STRAP_TYPE_DOUBLE,
	STRAP_TYPE_LONG_DOUBLE,
	STRAP_TYPE_STRING,

	STRAP_TYPE_NONE,
	STRAP_TYPE_COUNT
} StrapType;

#define logs(a) printf("%s: %s\n", #a, a)
#define logd(a) printf("%s: %d\n", #a, a)
#define logc(a) printf("%s: %c\n", #a, a)
#define logf(a) printf("%s: %f\n", #a, a)
#define logp(a) printf("%s: %p\n", #a, a)

typedef struct StrapString StrapString;
typedef struct StrapArray StrapArray;

/* StrapString functions */

extern StrapString *s_string_alloc(const char *cstr);
extern StrapString *s_string_nalloc(const char *cstr, size_t size);
extern void         s_string_free(StrapString *str);
extern StrapString *s_string_clone(const StrapString *str);

extern char  *s_string_get_cstr(const StrapString *str);
extern size_t s_string_length(const StrapString *str);
extern size_t s_string_size(const StrapString *str);

extern StrapString *s_string_copy(StrapString *str1, const StrapString *str2);
extern StrapString *s_string_concat(StrapString *str1, const StrapString *str2);
extern StrapString *s_string_copy_from(StrapString *str, const char *cstr);
extern char        *s_string_copy_to(const StrapString *str, char *cstr);
extern StrapString *s_string_strcat(StrapString *str, const char *cstr);

extern StrapString *s_string_ncopy(StrapString *str1, const StrapString *str2, size_t n);
extern StrapString *s_string_nconcat(StrapString *str1, const StrapString *str2, size_t n);
extern StrapString *s_string_ncopy_from(StrapString *str, const char *cstr, size_t n);
extern char        *s_string_ncopy_to(const StrapString *str, char *cstr, size_t n);
extern StrapString *s_string_nstrcat(StrapString *str, const char *cstr, size_t n);

extern StrapString *s_string_create_substring(const StrapString *str, size_t start, size_t n);
extern StrapString *s_string_erase(StrapString *str, size_t start, size_t n);
extern StrapString *s_string_trim(StrapString *str);
extern int          s_string_compare(const StrapString *str1, const StrapString *str2);
extern size_t       s_string_find(const StrapString *str1, const StrapString *str2);
extern size_t       s_string_nfind(const StrapString *str1, const StrapString *str2, size_t n);
extern StrapArray  *s_string_split(StrapString *str, StrapArray *arr, const char *sep);
extern StrapString *s_string_reverse(StrapString *str);
extern StrapString *s_string_shrink(StrapString *str);

/* StrapArray functions */

extern StrapArray *s_array_alloc(StrapType type);
extern StrapArray *s_array_nalloc(StrapType type, size_t size);
extern StrapArray *s_array_memcpy(StrapArray *arr, const void *src, size_t num);
extern StrapArray *s_array_strcpy(StrapArray *arr, const char **src, size_t num);
extern void        s_array_free(StrapArray *arr);

extern size_t    s_array_count(const StrapArray *arr);
extern size_t    s_array_capacity(const StrapArray *arr);
extern StrapType s_array_type(const StrapArray *arr);

// get

extern StrapString *s_array_create_string(const StrapArray *arr, size_t i);
extern StrapString *s_array_copy_string(const StrapArray *arr, size_t i, StrapString *str);
extern const char  *s_array_get_cstr(const StrapArray *arr, size_t i);

extern char      s_array_get_char(const StrapArray *arr, size_t i);
extern short     s_array_get_short(const StrapArray *arr, size_t i);
extern int       s_array_get_int(const StrapArray *arr, size_t i);
extern long      s_array_get_long(const StrapArray *arr, size_t i);

extern float       s_array_get_float(const StrapArray *arr, size_t i);
extern double      s_array_get_double(const StrapArray *arr, size_t i);
extern long double s_array_get_longdouble(const StrapArray *arr, size_t i);

// append

extern StrapArray *s_array_append_cstr(StrapArray *arr, const char *cstr);
extern StrapArray *s_array_append_string(StrapArray *arr, const StrapString *str);
extern StrapArray *s_array_append_array(StrapArray *arr1, const StrapArray *arr2);

extern StrapArray *s_array_append_char(StrapArray *arr, char num);
extern StrapArray *s_array_append_short(StrapArray *arr, short num);
extern StrapArray *s_array_append_int(StrapArray *arr, int num);
extern StrapArray *s_array_append_long(StrapArray *arr, long num);

extern StrapArray *s_array_append_float(StrapArray *arr, float num);
extern StrapArray *s_array_append_double(StrapArray *arr, double num);
extern StrapArray *s_array_append_longdouble(StrapArray *arr, long double num);

// insert

extern StrapArray *s_array_insert_cstr(StrapArray *arr, size_t i, const char *cstr);
extern StrapArray *s_array_insert_string(StrapArray *arr, size_t i, const StrapString *str);
extern StrapArray *s_array_insert_array(StrapArray *arr1, size_t i, const StrapArray *arr2);
extern StrapArray *s_array_insert_array_range(StrapArray *arr1, size_t i, const StrapArray *arr2, size_t start, size_t n);

extern StrapArray *s_array_insert_char(StrapArray *arr, size_t i, char num);
extern StrapArray *s_array_insert_short(StrapArray *arr, size_t i, short num);
extern StrapArray *s_array_insert_int(StrapArray *arr, size_t i, int num);
extern StrapArray *s_array_insert_long(StrapArray *arr, size_t i, long num);

extern StrapArray *s_array_insert_float(StrapArray *arr, size_t i, float num);
extern StrapArray *s_array_insert_double(StrapArray *arr, size_t i, double num);
extern StrapArray *s_array_insert_longdouble(StrapArray *arr, size_t i, long double num);

// replace

extern StrapArray *s_array_replace_cstr(StrapArray *arr, size_t i, const char *cstr);
extern StrapArray *s_array_replace_string(StrapArray *arr, size_t i, const StrapString *str);

extern StrapArray *s_array_replace_char(StrapArray *arr, size_t i, char num);
extern StrapArray *s_array_replace_short(StrapArray *arr, size_t i, short num);
extern StrapArray *s_array_replace_int(StrapArray *arr, size_t i, int num);
extern StrapArray *s_array_replace_long(StrapArray *arr, size_t i, long num);

extern StrapArray *s_array_replace_float(StrapArray *arr, size_t i, float num);
extern StrapArray *s_array_replace_double(StrapArray *arr, size_t i, double num);
extern StrapArray *s_array_replace_longdouble(StrapArray *arr, size_t i, long double num);

// find

extern size_t s_array_find_cstr(const StrapArray *arr, const char *cstr);
extern size_t s_array_find_string(const StrapArray *arr, StrapString *str);

extern size_t s_array_find_char(const StrapArray *arr, char num);
extern size_t s_array_find_short(const StrapArray *arr, short num);
extern size_t s_array_find_int(const StrapArray *arr, int num);
extern size_t s_array_find_long(const StrapArray *arr, long num);

extern size_t s_array_find_float(const StrapArray *arr, float num);
extern size_t s_array_find_double(const StrapArray *arr, double num);
extern size_t s_array_find_longdouble(const StrapArray *arr, long double num);

// nfind

extern size_t s_array_nfind_cstr(const StrapArray *arr, const char *cstr, size_t n);
extern size_t s_array_nfind_string(const StrapArray *arr, StrapString *str, size_t n);

extern size_t s_array_nfind_char(const StrapArray *arr, char num, size_t n);
extern size_t s_array_nfind_short(const StrapArray *arr, short num, size_t n);
extern size_t s_array_nfind_int(const StrapArray *arr, int num, size_t n);
extern size_t s_array_nfind_long(const StrapArray *arr, long num, size_t n);

extern size_t s_array_nfind_float(const StrapArray *arr, float num, size_t n);
extern size_t s_array_nfind_double(const StrapArray *arr, double num, size_t n);
extern size_t s_array_nfind_longdouble(const StrapArray *arr, long double num, size_t n);

// sum

extern int           s_array_sum_int(const StrapArray *arr);
extern long          s_array_sum_long(const StrapArray *arr);
extern unsigned int  s_array_sum_uint(const StrapArray *arr);
extern unsigned long s_array_sum_ulong(const StrapArray *arr);
extern double        s_array_sum_double(const StrapArray *arr);
extern long double   s_array_sum_longdouble(const StrapArray *arr);

// other

extern StrapArray *s_array_erase(StrapArray *arr, size_t i);
extern StrapArray *s_array_erase_range(StrapArray *arr, size_t i, size_t n);
extern StrapArray *s_array_reverse(StrapArray *arr);
extern StrapArray *s_array_sort(StrapArray *arr, int ascending);
extern StrapArray *s_array_clear(StrapArray *arr);
extern StrapArray *s_array_shrink(StrapArray *arr);
extern StrapArray *s_array_clone(const StrapArray *arr);
extern StrapArray *s_array_create_subarray(const StrapArray *arr, size_t i, size_t n);
extern int s_array_compare(const StrapArray *arr1, const StrapArray *arr2);

/* Miscellaneous functions */

extern int s_array_sprintf(const StrapArray *arr, char *s);
extern int s_array_snprintf(const StrapArray *arr, char *s, size_t n);
extern int s_array_fprintf(const StrapArray *arr, FILE *stream);
extern size_t s_sizeof(StrapType type);
