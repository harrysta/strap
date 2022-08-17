#ifndef STRAP_H
#define STRAP_H

#include <stddef.h>

#define strap_length(a) strap_count(a)

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
extern size_t strap_count(StrapArray *arr);

extern StrapNode* strap_get(StrapArray *arr, unsigned int index);
extern void strap_append_string(StrapArray *arr, const char *str);
extern void strap_add_string(StrapArray *arr, unsigned int index, const char *str);
extern void strap_remove(StrapArray *arr, unsigned int index);
extern void strap_node_strcpy(StrapNode *node, void *dst);

#endif
