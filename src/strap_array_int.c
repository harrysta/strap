#include "strap_internal.h"
#include "strap_array_num.h"

int strap_array_get_int(const StrapArray *arr, size_t idx)
{
	return strap_array_get_num(arr, idx, STRAP_TYPE_INT).int_t;
}

StrapArray *strap_array_append_int(StrapArray *arr, int num)
{
	return strap_array_append_num(arr, (num_t) { .int_t = num }, STRAP_TYPE_INT);
}

int strap_array_fprintf_int(const struct int_array *iarr, FILE *stream)
{
	int n;
	int sum;
	size_t i;

	if (!iarr->count)
		return fprintf(stream, "[]");
	sum = fprintf(stream, "[");
	for (i = 0; i < iarr->count; i++) {
		n = fprintf(stream, "%d", iarr->array[i]);
		if (n < 0) {
			return -1;
		}
		sum += n;
		if (i != iarr->count - 1)
			sum += fprintf(stream, ", ");
	}
	sum += fprintf(stream, "]");
	return sum;
}

