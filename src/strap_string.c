#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "strap_types.h"
#include "strap_util.h"

#define logs(a) printf("%s: %s\n", #a, a)
#define logd(a) printf("%s: %lu\n", #a, a)
#define logf(a) printf("%s: %f\n", #a, a)

// increase str by size in bytes
static StrapString *strap_resize(StrapString *str, size_t size)
{
	char *data = realloc(str->data, size);
	if (!data)
		return NULL;
	str->data = data;
	str->size = size;
	return str;
}

StrapString *strap_string_alloc(const char *str)
{
	size_t size;
	size_t length;
	StrapString *newstr;

	newstr = malloc(sizeof *newstr);
	if (!newstr)
		return NULL;
	length = str ? strlen(str) : 0;
	size = str ? strap_next_pow2(length) : S_INIT_STR_SIZE;
	newstr->data = malloc(size);
	if (!newstr->data) {
		free(newstr);
		return NULL;
	}
	memcpy(newstr->data, str, length);
	newstr->data[length] = '\0';
	newstr->size = size;
	newstr->length = length;
	return newstr;
}

StrapString *strap_string_clone(const StrapString *str)
{
	if (!str)
		return NULL;
	return strap_string_alloc(str->data);
}

char *strap_string_get_cstr(const StrapString *str)
{
	return str ? str->data : NULL;
}

size_t strap_string_length(const StrapString *str)
{
	return str ? str->length : 0;
}

StrapString *strap_string_copy(StrapString *str1, const StrapString *str2)
{
	return strap_string_copy_from(str1, str2 ? str2->data : NULL);
}

StrapString *strap_string_concat(StrapString *str1, const StrapString *str2)
{
	return strap_string_nconcat(str1, str2, strap_string_length(str2));
}

StrapString *strap_string_copy_from(StrapString *str, const char *cstr)
{
	return strap_string_ncopy_from(str, cstr, cstr ? strlen(cstr) : 0);
}

char *strap_string_copy_to(const StrapString *str, char *cstr)
{
	return strap_string_ncopy_to(str, cstr, str ? str->length : 0);
}

StrapString *strap_string_strcat(StrapString *str, const char *cstr)
{
	return strap_string_nstrcat(str, cstr, cstr ? strlen(cstr) : 0);
}

StrapString *strap_string_ncopy(StrapString *str1, const StrapString *str2, size_t n)
{
	return strap_string_ncopy_from(str1, str2 ? str2->data : NULL, n);
}

StrapString *strap_string_nconcat(StrapString *str1, const StrapString *str2, size_t n)
{
	size_t newlen;

	if (!str1)
		return NULL;
	if (!str2)
		return str1;
	if (n > str2->length)
		n = str2->length;
	newlen = str1->length + n;
	if (newlen > str1->size) {
		StrapString *s = strap_resize(str1, strap_next_pow2(newlen));
		if (!s)
			return str1;
		str1 = s;
	}
	memcpy(str1->data + str1->length, str2->data, n);
	str1->length = newlen;
	return str1;
}

StrapString *strap_string_ncopy_from(StrapString *str, const char *cstr, size_t n)
{
	if (!str)
		return NULL;
	if (!cstr){
		str->length = 0;
		str->data[0] = '\0';
		return str;
	}
	if (n > str->size) {
		StrapString *s = strap_resize(str, strap_next_pow2(n));
		if (!s)
			return str;
		str = s;
	}
	memcpy(str->data, cstr, n);
	str->length = n;
	str->data[n] = '\0';
	return str;
}

char *strap_string_ncopy_to(const StrapString *str, char *cstr, size_t n)
{
	if (!cstr)
		return NULL;
	if (!str) {
		cstr[0] = '\0';
		return cstr;
	}
	if (n > str->length)
		n = str->length;
	memcpy(cstr, str->data, n);
	cstr[n] = '\0';
	return cstr;
}

StrapString *strap_string_nstrcat(StrapString *str, const char *cstr, size_t n)
{
	size_t newlen;

	if (!str)
		return NULL;
	if (!cstr)
		return str;
	newlen = str->length + n;
	if (newlen > str->size) {
		StrapString *s = strap_resize(str, strap_next_pow2(newlen));
		if (!s)
			return str;
		str = s;
	}
	memcpy(str->data + str->length, cstr, n);
	str->length = newlen;
	return str;
}

StrapString *strap_string_substring(const StrapString *str, size_t start, size_t n)
{
}

StrapString *strap_string_erase(StrapString *str, size_t start, size_t end)
{
}

StrapString *strap_string_trim(StrapString *str)
{
}

int strap_string_compare(const StrapString *str1, const StrapString *str2)
{
	if (!str1 && !str2)
		return 0;
	if (!str1 && str2)
		return -1;
	if (str1 && !str2)
		return 1;
	return strcmp(str1->data, str2->data);
}

size_t strap_string_find(const StrapString *str1, const StrapString *str2)
{
}

StrapArray *strap_string_split(StrapString *str, const char *sep)
{
}

StrapString *strap_string_reverse(StrapString *str)
{
	char tmp;
	char *start;
	char *end;

	if (!str)
		return NULL;
	start = str->data;
	end = str->data + str->length - 1;
	while(start < end){
		tmp = *start;
		*start = *end;
		*end = tmp;
		start++;
		end--;
	}
	return str;
}

void strap_string_shrink(StrapString *str)
{
}
