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
	if (arr == NULL)
		return;

	StrapArrayString *array = (StrapArrayString*) arr;
	size_t len = arr->array[arr->array_count - 1].accumulated_length;
	for (int i = 0; i < len; i++) {
		if (array->char_array[i]) {
		printf("%c", array->char_array[i]);
		} else {
			printf(",");
		}
	}
	printf("\n");
	free(arr->array);
	free(arr);
}


StrapNode* strap_string_get(StrapArray *arr, unsigned int index)
{
	return NULL;
}

void strap_string_append(StrapArray *arr, const char *str)
{
	StrapArrayString *array;
	StrapNode *node, *previous_node;
	size_t length, previous_length;

	if (arr == NULL || arr->type != STRAP_STRING) {
		return;
	}
	array = (StrapArrayString*) arr;
	length = 0;
	while (str[length++] != '\0');
	if (arr->array_count == 0) {
		node = arr->array;
		previous_length = 0;
	} else {
		node = arr->array + arr->array_count;
		previous_node = node - 1;
		previous_length = previous_node->accumulated_length;
	}
	node->ptr = array->char_array + previous_length;
	node->accumulated_length = previous_length + length;
	strcpy(node->ptr, str);
	arr->array_count++;
}

void strap_string_add(StrapArray *arr, unsigned int index, const char *str)
{
}

void strap_remove(StrapArray *arr, unsigned int index)
{
}

