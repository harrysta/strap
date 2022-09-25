#include "strap_internal.h"
#include "strap_internal_num_array.h"

int strap_array_get_int(const StrapArray *arr, size_t idx)
{
	struct int_array *iarr;

	if (!arr || arr->type != STRAP_TYPE_INT)
		return 0;
	iarr = (struct int_array*) arr->data;
	return iarr->array[idx];
}

StrapArray *strap_array_append_int(StrapArray *arr, int num)
{
	struct int_array *iarr;

	if (!arr || arr->type != STRAP_TYPE_INT)
		return arr;
	iarr = (struct int_array*) arr->data;
	STRAP_ARRAY_APPEND_NUM(iarr, num);
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
