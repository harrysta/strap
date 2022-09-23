#ifndef STRAP_H
#define STRAP_H

#include <stddef.h>
#include <stdio.h>

#define strap_array_printf(a) strap_array_fprintf((a), stdout)

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

#define logs(a) printf("%s: %s\n", #a, a)
#define logd(a) printf("%s: %d\n", #a, a)
#define logc(a) printf("%s: %c\n", #a, a)
#define logf(a) printf("%s: %f\n", #a, a)
#define logp(a) printf("%s: %p\n", #a, a)

typedef struct StrapString StrapString;
typedef struct StrapArray StrapArray;

extern StrapString *strap_string_alloc(const char *cstr);
extern StrapString *strap_string_nalloc(const char *cstr, size_t size);
extern void         strap_string_free(StrapString *str);
extern StrapString *strap_string_clone(const StrapString *str);
extern char        *strap_string_get_cstr(const StrapString *str);

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

extern StrapArray *strap_array_alloc(StrapType type);
extern StrapArray *strap_array_nalloc(StrapType type, size_t size);
extern void        strap_array_free(StrapArray *arr);
extern StrapArray *strap_array_clone(const StrapArray *arr); // TODO

extern size_t strap_array_count(const StrapArray *arr); // TODO
extern size_t strap_array_size(const StrapArray *arr); // TODO
extern size_t strap_array_capacity(const StrapArray *arr); // TODO

extern void *strap_array_begin(StrapArray *arr); // TODO
extern void *strap_array_end(StrapArray *arr); // TODO

extern StrapString *strap_array_create_string(const StrapArray *arr, size_t i); // TODO
extern StrapString *strap_array_copy_string(const StrapArray *arr, size_t i, StrapString *str); // TODO

extern const char *strap_array_get_cstr(const StrapArray *arr, size_t i);
extern short       strap_array_get_short(const StrapArray *arr, size_t i); // TODO
extern int         strap_array_get_int(const StrapArray *arr, size_t i); // TODO
extern long        strap_array_get_long(const StrapArray *arr, size_t i); // TODO
extern float       strap_array_get_float(const StrapArray *arr, size_t i); // TODO
extern double      strap_array_get_double(const StrapArray *arr, size_t i); // TODO

extern StrapArray *strap_array_append_string(StrapArray *arr, const StrapString *str);
extern StrapArray *strap_array_append_cstr(StrapArray *arr, const char *cstr);
extern StrapArray *strap_array_append_short(StrapArray *arr, short num); // TODO
extern StrapArray *strap_array_append_int(StrapArray *arr, int num); // TODO
extern StrapArray *strap_array_append_long(StrapArray *arr, long num); // TODO
extern StrapArray *strap_array_append_float(StrapArray *arr, float num); // TODO
extern StrapArray *strap_array_append_double(StrapArray *arr, double num); // TODO
extern StrapArray *strap_array_append_array(StrapArray *arr1, const StrapArray *arr2); // TODO

extern StrapArray *strap_array_insert_string(StrapArray *arr, size_t i, const StrapString *str);
extern StrapArray *strap_array_insert_cstr(StrapArray *arr, size_t i, const char *cstr);
extern StrapArray *strap_array_insert_short(StrapArray *arr, size_t i, short num); // TODO
extern StrapArray *strap_array_insert_int(StrapArray *arr, size_t i, int num); // TODO
extern StrapArray *strap_array_insert_long(StrapArray *arr, size_t i, long num); // TODO
extern StrapArray *strap_array_insert_float(StrapArray *arr, size_t i, float num); // TODO
extern StrapArray *strap_array_insert_double(StrapArray *arr, size_t i, double num); // TODO
extern StrapArray *strap_array_insert_array(StrapArray *arr1, size_t i, const StrapArray *arr2); // TODO
extern StrapArray *strap_array_insert_array_range(StrapArray *arr1, size_t i, const StrapArray *arr2, size_t start, size_t n); // TODO

extern StrapArray *strap_array_replace_string(StrapArray *arr, size_t i, const StrapString *str);
extern StrapArray *strap_array_replace_cstr(StrapArray *arr, size_t i, const char *cstr);
extern StrapArray *strap_array_replace_short(StrapArray *arr, size_t i, short num); // TODO
extern StrapArray *strap_array_replace_int(StrapArray *arr, size_t i, int num); // TODO
extern StrapArray *strap_array_replace_long(StrapArray *arr, size_t i, long num); // TODO
extern StrapArray *strap_array_replace_float(StrapArray *arr, size_t i, float num); // TODO
extern StrapArray *strap_array_replace_double(StrapArray *arr, size_t i, double num); // TODO

extern size_t strap_array_find_string(const StrapArray *arr, StrapString *str);
extern size_t strap_array_find_cstr(const StrapArray *arr, const char *cstr);
extern size_t strap_array_find_short(const StrapArray *arr, short num); // TODO
extern size_t strap_array_find_int(const StrapArray *arr, int num); // TODO
extern size_t strap_array_find_long(const StrapArray *arr, long num); // TODO
extern size_t strap_array_find_float(const StrapArray *arr, float num); // TODO
extern size_t strap_array_find_double(const StrapArray *arr, double num); // TODO

extern size_t strap_array_nfind_string(const StrapArray *arr, StrapString *str, size_t n);
extern size_t strap_array_nfind_cstr(const StrapArray *arr, const char *cstr, size_t n);
extern size_t strap_array_nfind_short(const StrapArray *arr, short num, size_t n); // TODO
extern size_t strap_array_nfind_int(const StrapArray *arr, int num, size_t n); // TODO
extern size_t strap_array_nfind_long(const StrapArray *arr, long num, size_t n); // TODO
extern size_t strap_array_nfind_float(const StrapArray *arr, float num, size_t n); // TODO
extern size_t strap_array_nfind_double(const StrapArray *arr, double num, size_t n); // TODO

extern short  strap_array_sum_short(const StrapArray *arr); // TODO
extern int    strap_array_sum_int(const StrapArray *arr); // TODO
extern long   strap_array_sum_long(const StrapArray *arr); // TODO
extern float  strap_array_sum_float(const StrapArray *arr); // TODO
extern double strap_array_sum_double(const StrapArray *arr); // TODO

extern StrapArray *strap_array_clear(StrapArray *arr);
extern StrapArray *strap_array_erase(StrapArray *arr, size_t i);
extern StrapArray *strap_array_erase_range(StrapArray *arr, size_t i, size_t n);
extern StrapArray *strap_array_clear(StrapArray *arr); // TODO
extern StrapArray *strap_array_create_subarray(const StrapArray *arr, size_t i, size_t n);
extern StrapArray *strap_array_reverse(StrapArray *arr); // TODO
extern StrapArray *strap_array_shrink(StrapArray *arr); // TODO
extern StrapArray *strap_array_sort(StrapArray *arr, int ascending); // TODO

extern int strap_array_sprintf(const StrapArray *arr, char *cstr);
extern int strap_array_fprintf(const StrapArray *arr, FILE *stream);

#endif
