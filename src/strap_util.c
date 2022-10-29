#include "strap_internal.h"
#include <string.h>

size_t s_next_pow2(size_t n, size_t min)
{
	size_t p;

	if (n < min)
		return min;
	if (n && !(n & (n - 1)))
		return n;
	p = 1;
	while (p < n)
		p <<= 1;
	return p;
}

size_t s_charcount(const char *str, char c, size_t bytes)
{
	ptrdiff_t b = bytes;
	size_t count = 0;
	const char *pch = strchr(str, c);
	while (pch && pch - str < b) {
		count++;
		pch = strchr(pch + 1, c);
	}
	return count;
}

size_t s_sizeof(StrapType type)
{
	switch (type) {
		case STRAP_TYPE_CHAR:
			return sizeof(char);
		case STRAP_TYPE_SHORT:
			return sizeof(short);
		case STRAP_TYPE_INT:
			return sizeof(int);
		case STRAP_TYPE_LONG:
			return sizeof(long);
		case STRAP_TYPE_FLOAT:
			return sizeof(float);
		case STRAP_TYPE_DOUBLE:
			return sizeof(double);
		case STRAP_TYPE_LONG_DOUBLE:
			return sizeof(long double);
		default:
			return 0;
	}
	return 0;
}
