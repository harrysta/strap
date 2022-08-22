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
