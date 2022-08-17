#include "strap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRAP_STRING_ARRAY_SIZE 256
#define STRAP_CAPACITY 16

struct StrapNode {
	size_t accumulated_length;
	void *ptr;
};

struct StrapArray {
	StrapType type;
	size_t array_size;
	unsigned int array_count;
	struct StrapNode *array;
};

typedef struct {
	StrapArray head;
	size_t char_array_size;
	char char_array[1];
} StrapArrayString;

StrapArray* strap_alloc(StrapType type)
{
	void *ptr;
	StrapArray *arr;
	size_t size;

	switch (type) {
		case STRAP_STRING:
			size = STRAP_STRING_ARRAY_SIZE;
			ptr = malloc(sizeof(StrapArrayString) + size - 1);
			break;
		default:
			return NULL;
	}
	if (ptr == NULL)
		return NULL;
	arr = (StrapArray*) ptr;
	arr->type = type;
	arr->array_count = 0;
	arr->array_size = size;
	arr->array = malloc(STRAP_CAPACITY*sizeof *arr->array);
	if (arr->array == NULL) {
		free(arr);
		return NULL;
	}
	return arr;
}

void strap_free(StrapArray *arr)
{
	if (arr == NULL) {
		return;
	}
	free(arr->array);
	free(arr);
}

size_t strap_count(StrapArray *arr) {
	return arr == NULL ? 0 : arr->array_count;
}

StrapNode* strap_get(StrapArray *arr, unsigned int index)
{
	if (arr == NULL || index >= arr->array_count) {
		return NULL;
	}
	return arr->array + index;
}

void strap_append_string(StrapArray *arr, const char *str)
{
	StrapArrayString *arrstr;
	StrapNode *node;
	size_t length, previous_length;

	if (arr == NULL || arr->type != STRAP_STRING) {
		return;
	}
	arrstr = (StrapArrayString*) arr;
	length = 0;
	while (str[length++] != '\0');
	if (arr->array_count == 0) {
		node = arr->array;
		previous_length = 0;
	} else {
		node = arr->array + arr->array_count;
		previous_length = (node - 1)->accumulated_length;
	}
	node->ptr = arrstr->char_array + previous_length;
	node->accumulated_length = previous_length + length;
	strcpy(node->ptr, str);
	arr->array_count++;
}

void strap_add_string(StrapArray *arr, unsigned int index, const char *str)
{
}

void strap_remove(StrapArray *arr, unsigned int index)
{
}


void strap_node_strcpy(StrapNode *node, void *dst)
{
	if (node == NULL) {
		return;
	}
	strcpy(dst, node->ptr);
}
