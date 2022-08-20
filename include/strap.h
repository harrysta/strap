#ifndef STRAP_H
#define STRAP_H

#include <stddef.h>

typedef enum {
	STRAP_TYPE_INT,
	STRAP_TYPE_FLOAT,
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

extern StrapString *strap_string_substring(const StrapString *str, size_t start, size_t n);
extern StrapString *strap_string_erase(StrapString *str, size_t start, size_t end);
extern StrapString *strap_string_trim(StrapString *str);
extern int          strap_string_compare(const StrapString *str1, const StrapString *str2);
extern size_t       strap_string_find(const StrapString *str1, const StrapString *str2);
extern StrapArray  *strap_string_split(StrapString *str, const char *sep);
extern StrapString *strap_string_reverse(StrapString *str);
extern StrapString *strap_string_shrink(StrapString *str);


extern StrapArray *strap_array_alloc(StrapType type);
extern void        strap_array_free(StrapArray *arr);
extern size_t      strap_array_count(StrapArray *arr);
extern int         strap_array_append_cstr(StrapArray *arr, const char *cstr);
extern int         strap_array_append_string(StrapArray *arr, const StrapString *str);

extern const char *strap_array_get_cstr(StrapArray *arr, size_t index);

#endif
