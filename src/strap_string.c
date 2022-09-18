#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "strap_internal.h"

static StrapString *strap_resize(StrapString *str, size_t size)
{
	char *data = realloc(str->data, size);
	if (!data)
		return NULL;
	str->data = data;
	str->size = size;
	return str;
}

StrapString *strap_string_nalloc_internal(const char *str, size_t size, size_t len)
{
	StrapString *newstr;

	newstr = malloc(sizeof *newstr);
	if (!newstr)
		return NULL;
	newstr->data = malloc(size);
	if (!newstr->data) {
		free(newstr);
		return NULL;
	}
	memcpy(newstr->data, str, len);
	newstr->data[len] = '\0';
	newstr->size = size;
	newstr->length = len;
	return newstr;
}

StrapString *strap_string_alloc(const char *str)
{
	size_t len = S_STRLEN(str);
	size_t size = strap_next_pow2(len, S_INIT_STR_SIZE);
	return strap_string_nalloc_internal(str, size, len);
}

StrapString *strap_string_nalloc(const char *str, size_t size)
{
	return strap_string_nalloc_internal(str, size, S_STRLEN(str));
}

void strap_string_free(StrapString *str)
{
	if (!str)
		return;
	free(str->data);
	free(str);
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

size_t strap_string_size(const StrapString *str)
{
	return str ? str->size : 0;
}

StrapString *strap_string_copy(StrapString *str1, const StrapString *str2)
{
	return strap_string_copy_from(str1, strap_string_get_cstr(str2));
}

StrapString *strap_string_concat(StrapString *str1, const StrapString *str2)
{
	return strap_string_nconcat(str1, str2, strap_string_length(str2));
}

StrapString *strap_string_copy_from(StrapString *str, const char *cstr)
{
	return strap_string_ncopy_from(str, cstr, S_STRLEN(cstr));
}

char *strap_string_copy_to(const StrapString *str, char *cstr)
{
	return strap_string_ncopy_to(str, cstr, strap_string_length(str));
}

StrapString *strap_string_strcat(StrapString *str, const char *cstr)
{
	return strap_string_nstrcat(str, cstr, S_STRLEN(cstr));
}

StrapString *strap_string_ncopy(StrapString *str1, const StrapString *str2, size_t n)
{
	return strap_string_ncopy_from(str1, strap_string_get_cstr(str2), n);
}

StrapString *strap_string_nconcat(StrapString *str1, const StrapString *str2, size_t n)
{
	return strap_string_nstrcat(str1, strap_string_get_cstr(str2), n);
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
		StrapString *s = strap_resize(str, strap_next_pow2(n, S_INIT_STR_SIZE));
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
		StrapString *s = strap_resize(str, strap_next_pow2(newlen, S_INIT_STR_SIZE));
		if (!s)
			return str;
		str = s;
	}
	memcpy(str->data + str->length, cstr, n);
	str->length = newlen;
	return str;
}

StrapString *strap_string_create_substring(const StrapString *str, size_t start, size_t n)
{
	StrapString *nstr;

	if (!str)
		return NULL;
	nstr = strap_string_alloc(NULL);
	if (start >= str->length)
		return nstr;
	return strap_string_ncopy_from(nstr, str->data + start, n);
}

StrapString *strap_string_erase(StrapString *str, size_t start, size_t n)
{
	if (!str)
		return NULL;
	if (start >= str->length)
		return str;
	if (!n)
		return str;
	if (start + n > str->length)
		n = str->length - start;
	memcpy(str->data + start, str->data + start + n, str->length - n);
	str->length -= n;
	str->data[str->length] = '\0';
	return str;
}

StrapString *strap_string_trim(StrapString *str)
{
	char *start;
	char *end;
	size_t len;

	if (!str)
		return NULL;
	if (!str->length)
		return str;
	start = str->data;
	end = str->data + str->length - 1;
	while (isspace(*start) && start != end) start++;
	while (isspace(*end) && end >= start) end--;
	len = end - start + 1;
	memcpy(str->data, start, len);
	str->data[len] = '\0';
	str->length = len;
	return str;
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
	size_t i, j;

	if (!(str1 && str2))
		return -1;
	i = 0;
	while (i < str1->length) {
		j = 0;
		if (str1->data[i++] == str2->data[j++]) {
			while (j < str2->length && str1->data[i++] == str2->data[j++]);
			if (j == str2->length) {
				return i - j;
			}
		}
	}
	return -1;
}

StrapArray *strap_string_split(StrapString *str, StrapArray *arr, const char *sep)
{
	StrapArray_str *arr_s;
	size_t len;
	size_t seplen;
	size_t splitlen;
	size_t i, j;
	char *buf;

	if (!str || !arr || !sep || arr->type != STRAP_TYPE_STRING)
		return arr;
	strap_array_clear(arr);
	len = str->length;
	buf = calloc(len, sizeof*buf);
	seplen = strlen(sep);
	i = 0;
	splitlen = 0;
	while (i <= len) {
		if (memcmp(sep, str->data + i, seplen) == 0 || i == len) {
			memcpy(buf, str->data + i - splitlen, splitlen);
			buf[splitlen] = '\0';
			strap_array_append_cstr(arr, buf);
			splitlen = 0;
			i += seplen;
		} else {
			splitlen++;
			i++;
		}
	}
	free(buf);
	return arr;
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
	while (start < end){
		tmp = *start;
		*start = *end;
		*end = tmp;
		start++;
		end--;
	}
	return str;
}

StrapString *strap_string_shrink(StrapString *str)
{
	if (!str)
		return NULL;
	return strap_resize(str, strap_next_pow2(str->length, S_INIT_STR_SIZE));
}
