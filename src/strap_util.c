#include "strap_internal.h"

size_t strap_next_pow2(size_t n, size_t min)
{
	if (n < min)
		return min;
	size_t p;
	if (n && !(n & (n - 1)))
		return n;
	p = 1;
	while (p < n)
		p <<= 1;
	return p;
}

size_t strap_sizeof(StrapType type)
{
	switch (type) {
		case STRAP_TYPE_CHAR:
			return sizeof(char);
		case STRAP_TYPE_SHORT:
			return sizeof(short);
		case STRAP_TYPE_INT:
			return sizeof(int);
		case STRAP_TYPE_LONG_INT:
			return sizeof(long);
		case STRAP_TYPE_FLOAT:
			return sizeof(float);
		case STRAP_TYPE_DOUBLE:
			return sizeof(double);
		case STRAP_TYPE_STRING:
		case STRAP_TYPE_COUNT:
		break;
	}
	return 0;
}
