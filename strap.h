#ifndef STRAP_H
#define STRAP_H

#include <stddef.h>

typedef enum {
	STRAP_INT,
	STRAP_FLOAT,
	STRAP_STRING,
	STRAP_COUNT
} StrapType;

typedef struct StrapArray StrapArray;
typedef struct StrapNode StrapNode;

extern StrapArray* strap_alloc(StrapType type);
extern void strap_free(StrapArray *arr);

extern StrapNode* strap_string_get(StrapArray *arr, unsigned int index);
extern void strap_string_append(StrapArray *arr, const char *str);
extern void strap_string_add(StrapArray *arr, unsigned int index, const char *str);
extern void strap_remove(StrapArray *arr, unsigned int index);

#endif
