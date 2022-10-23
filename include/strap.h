#pragma once

#include <stddef.h>
#include <stdio.h>

#define strap_array_printf(a) strap_array_fprintf((a), stdout)
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

extern StrapString *strap_string_alloc(const char *cstr);
extern StrapString *strap_string_nalloc(const char *cstr, size_t size);
extern void         strap_string_free(StrapString *str);
extern StrapString *strap_string_clone(const StrapString *str);

extern char  *strap_string_get_cstr(const StrapString *str);
extern size_t strap_string_length(const StrapString *str);
extern size_t strap_string_size(const StrapString *str);

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
extern size_t       strap_string_nfind(const StrapString *str1, const StrapString *str2, size_t n);
extern StrapArray  *strap_string_split(StrapString *str, StrapArray *arr, const char *sep);
extern StrapString *strap_string_reverse(StrapString *str);
extern StrapString *strap_string_shrink(StrapString *str);

/* StrapArray functions */

extern StrapArray *strap_array_alloc(StrapType type);
extern StrapArray *strap_array_nalloc(StrapType type, size_t size);
extern void        strap_array_free(StrapArray *arr);
extern StrapArray *strap_array_clone(const StrapArray *arr);

extern size_t    strap_array_count(const StrapArray *arr);
extern size_t    strap_array_capacity(const StrapArray *arr);
extern StrapType strap_array_type(const StrapArray *arr);

// get

extern StrapString *strap_array_create_string(const StrapArray *arr, size_t i);
extern StrapString *strap_array_copy_string(const StrapArray *arr, size_t i, StrapString *str);
extern const char  *strap_array_get_cstr(const StrapArray *arr, size_t i);

extern char      strap_array_get_char(const StrapArray *arr, size_t i);
extern short     strap_array_get_short(const StrapArray *arr, size_t i);
extern int       strap_array_get_int(const StrapArray *arr, size_t i);
extern long      strap_array_get_long(const StrapArray *arr, size_t i);

extern float       strap_array_get_float(const StrapArray *arr, size_t i);
extern double      strap_array_get_double(const StrapArray *arr, size_t i);
extern long double strap_array_get_longdouble(const StrapArray *arr, size_t i);

// append

extern StrapArray *strap_array_append_cstr(StrapArray *arr, const char *cstr);
extern StrapArray *strap_array_append_string(StrapArray *arr, const StrapString *str);
extern StrapArray *strap_array_append_array(StrapArray *arr1, const StrapArray *arr2);

extern StrapArray *strap_array_append_char(StrapArray *arr, char num);
extern StrapArray *strap_array_append_short(StrapArray *arr, short num);
extern StrapArray *strap_array_append_int(StrapArray *arr, int num);
extern StrapArray *strap_array_append_long(StrapArray *arr, long num);

extern StrapArray *strap_array_append_float(StrapArray *arr, float num);
extern StrapArray *strap_array_append_double(StrapArray *arr, double num);
extern StrapArray *strap_array_append_longdouble(StrapArray *arr, long double num);

// insert

extern StrapArray *strap_array_insert_cstr(StrapArray *arr, size_t i, const char *cstr);
extern StrapArray *strap_array_insert_string(StrapArray *arr, size_t i, const StrapString *str);
extern StrapArray *strap_array_insert_array(StrapArray *arr1, size_t i, const StrapArray *arr2);
extern StrapArray *strap_array_insert_array_range(StrapArray *arr1, size_t i, const StrapArray *arr2, size_t start, size_t n);

extern StrapArray *strap_array_insert_char(StrapArray *arr, size_t i, char num);
extern StrapArray *strap_array_insert_short(StrapArray *arr, size_t i, short num);
extern StrapArray *strap_array_insert_int(StrapArray *arr, size_t i, int num);
extern StrapArray *strap_array_insert_long(StrapArray *arr, size_t i, long num);

extern StrapArray *strap_array_insert_float(StrapArray *arr, size_t i, float num);
extern StrapArray *strap_array_insert_double(StrapArray *arr, size_t i, double num);
extern StrapArray *strap_array_insert_longdouble(StrapArray *arr, size_t i, long double num);

// replace

extern StrapArray *strap_array_replace_cstr(StrapArray *arr, size_t i, const char *cstr);
extern StrapArray *strap_array_replace_string(StrapArray *arr, size_t i, const StrapString *str);

extern StrapArray *strap_array_replace_char(StrapArray *arr, size_t i, char num);
extern StrapArray *strap_array_replace_short(StrapArray *arr, size_t i, short num);
extern StrapArray *strap_array_replace_int(StrapArray *arr, size_t i, int num);
extern StrapArray *strap_array_replace_long(StrapArray *arr, size_t i, long num);

extern StrapArray *strap_array_replace_float(StrapArray *arr, size_t i, float num);
extern StrapArray *strap_array_replace_double(StrapArray *arr, size_t i, double num);
extern StrapArray *strap_array_replace_longdouble(StrapArray *arr, size_t i, long double num);

// find

extern size_t strap_array_find_cstr(const StrapArray *arr, const char *cstr);
extern size_t strap_array_find_string(const StrapArray *arr, StrapString *str);

extern size_t strap_array_find_char(const StrapArray *arr, char num);
extern size_t strap_array_find_short(const StrapArray *arr, short num);
extern size_t strap_array_find_int(const StrapArray *arr, int num);
extern size_t strap_array_find_long(const StrapArray *arr, long num);

extern size_t strap_array_find_float(const StrapArray *arr, float num);
extern size_t strap_array_find_double(const StrapArray *arr, double num);
extern size_t strap_array_find_longdouble(const StrapArray *arr, long double num);

// nfind

extern size_t strap_array_nfind_cstr(const StrapArray *arr, const char *cstr, size_t n);
extern size_t strap_array_nfind_string(const StrapArray *arr, StrapString *str, size_t n);

extern size_t strap_array_nfind_char(const StrapArray *arr, char num, size_t n);
extern size_t strap_array_nfind_short(const StrapArray *arr, short num, size_t n);
extern size_t strap_array_nfind_int(const StrapArray *arr, int num, size_t n);
extern size_t strap_array_nfind_long(const StrapArray *arr, long num, size_t n);

extern size_t strap_array_nfind_float(const StrapArray *arr, float num, size_t n);
extern size_t strap_array_nfind_double(const StrapArray *arr, double num, size_t n);
extern size_t strap_array_nfind_longdouble(const StrapArray *arr, long double num, size_t n);

// sum

extern int           strap_array_sum_int(const StrapArray *arr);
extern long          strap_array_sum_long(const StrapArray *arr);
extern unsigned int  strap_array_sum_uint(const StrapArray *arr);
extern unsigned long strap_array_sum_ulong(const StrapArray *arr);
extern double        strap_array_sum_double(const StrapArray *arr);
extern long double   strap_array_sum_longdouble(const StrapArray *arr);

// other

extern StrapArray *strap_array_erase(StrapArray *arr, size_t i);
extern StrapArray *strap_array_erase_range(StrapArray *arr, size_t i, size_t n);
extern StrapArray *strap_array_create_subarray(const StrapArray *arr, size_t i, size_t n);
extern StrapArray *strap_array_reverse(StrapArray *arr);
extern StrapArray *strap_array_sort(StrapArray *arr, int ascending);
extern StrapArray *strap_array_clear(StrapArray *arr);
extern StrapArray *strap_array_shrink(StrapArray *arr);

/* Miscellaneous functions */

extern int strap_array_sprintf(const StrapArray *arr, char *s);
extern int strap_array_snprintf(const StrapArray *arr, char *s, size_t n);
extern int strap_array_fprintf(const StrapArray *arr, FILE *stream);
extern size_t strap_sizeof(StrapType type);
