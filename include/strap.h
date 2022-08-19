#ifndef STRAP_H
#define STRAP_H

#include <stddef.h>

typedef enum {
	STRAP_TYPE_INT,
	STRAP_TYPE_FLOAT,
	STRAP_TYPE_STRING,
	STRAP_TYPE_COUNT
} StrapType;

typedef struct StrapArray StrapArray;
typedef struct StrapString StrapString;

extern StrapString *strap_string_alloc(const char *str);
extern StrapString *strap_string_copy(StrapString *string, const StrapString *src);
extern StrapString *strap_string_concat(StrapString *string, const StrapString *src);

extern StrapString *strap_string_copy_from(StrapString *string, const char *src);
extern char        *strap_string_copy_to(const StrapString *string, char* str);
extern StrapString *strap_string_strcat(StrapString *string, const char *src);
extern size_t       strap_string_length(const StrapString *string);

extern StrapArray *strap_array_alloc(StrapType type);
extern void        strap_array_free(StrapArray *arr);
extern size_t      strap_array_count(StrapArray *arr);
extern const char *strap_array_get_str(StrapArray *arr, size_t index);
extern int         strap_array_append_str(StrapArray *arr, const char *str);

#endif
