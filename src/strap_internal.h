#pragma once

#include <stddef.h>
#include "strap.h"
#include <stdlib.h>

#define max(x, y) ((x) > (y) ? (x) : (y))

struct StrapString {
	size_t length;
	size_t size;
	char *data;
};

struct StrapArray {
	StrapType type;
	size_t capacity;
	size_t count;
	void *data;
};

extern size_t s_next_pow2(size_t n, size_t min);

/**
 *  Returns the count of occurences char c appears from str to str + bytes
 */
extern size_t s_charcount(const char *str, char c, size_t bytes);
