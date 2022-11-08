#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "strap_internal.h"

#define STRLEN(a) (a) ? strlen((a)) : 0

#define str_check_size(s, len, ret)			\
do {							\
	if (len > s->size && str_resize(s, len))	\
		return ret;				\
} while (0)

static int str_resize(StrapString *str, size_t size)
{
	char *data = realloc(str->data, size);
	if (!data)
		return 1;
	str->data = data;
	str->size = size;
	return 0;
}

static StrapString *s_string_nalloc_internal(const char *str, size_t size, size_t len)
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

StrapString *s_string_alloc(const char *str)
{
	size_t len = STRLEN(str);
	size_t size = s_next_pow2(len, STRAP_INIT_STR_SIZE);
	return s_string_nalloc_internal(str, size, len);
}

StrapString *s_string_nalloc(const char *str, size_t size)
{
	return s_string_nalloc_internal(str, size, STRLEN(str));
}

void s_string_free(StrapString *str)
{
	if (!str)
		return;
	free(str->data);
	free(str);
}

StrapString *s_string_clone(const StrapString *str)
{
	if (!str)
		return NULL;
	return s_string_alloc(str->data);
}

char *s_string_get_cstr(const StrapString *str)
{
	return str ? str->data : NULL;
}

size_t s_string_length(const StrapString *str)
{
	return str ? str->length : 0;
}

size_t s_string_size(const StrapString *str)
{
	return str ? str->size : 0;
}

StrapString *s_string_copy(StrapString *str1, const StrapString *str2)
{
	return s_string_copy_from(str1, s_string_get_cstr(str2));
}

StrapString *s_string_concat(StrapString *str1, const StrapString *str2)
{
	return s_string_nconcat(str1, str2, s_string_length(str2));
}

StrapString *s_string_copy_from(StrapString *str, const char *cstr)
{
	return s_string_ncopy_from(str, cstr, STRLEN(cstr));
}

char *s_string_copy_to(const StrapString *str, char *cstr)
{
	return s_string_ncopy_to(str, cstr, s_string_length(str));
}

StrapString *s_string_strcat(StrapString *str, const char *cstr)
{
	return s_string_nstrcat(str, cstr, STRLEN(cstr));
}

StrapString *s_string_ncopy(StrapString *str1, const StrapString *str2, size_t n)
{
	return s_string_ncopy_from(str1, s_string_get_cstr(str2), n);
}

StrapString *s_string_nconcat(StrapString *str1, const StrapString *str2, size_t n)
{
	return s_string_nstrcat(str1, s_string_get_cstr(str2), n);
}

StrapString *s_string_ncopy_from(StrapString *str, const char *cstr, size_t n)
{
	size_t size;

	if (!str)
		return NULL;
	if (!cstr){
		str->length = 0;
		str->data[0] = '\0';
		return str;
	}
	size = s_next_pow2(n, STRAP_INIT_STR_SIZE);
	str_check_size(str, size, str);
	memcpy(str->data, cstr, n);
	str->length = n;
	str->data[n] = '\0';
	return str;
}

char *s_string_ncopy_to(const StrapString *str, char *cstr, size_t n)
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

StrapString *s_string_nstrcat(StrapString *str, const char *cstr, size_t n)
{
	size_t newlen;
	size_t size;

	if (!str || !cstr)
		return str;
	newlen = str->length + n;
	size = s_next_pow2(newlen, STRAP_INIT_STR_SIZE);
	str_check_size(str, size, str);
	memcpy(str->data + str->length, cstr, n);
	str->length = newlen;
	return str;
}

StrapString *s_string_create_substring(const StrapString *str, size_t start, size_t n)
{
	StrapString *nstr;

	if (!str)
		return NULL;
	nstr = s_string_alloc(NULL);
	if (start >= str->length)
		return nstr;
	return s_string_ncopy_from(nstr, str->data + start, n);
}

StrapString *s_string_erase(StrapString *str, size_t start, size_t n)
{
	size_t len;

	if (!str || start >= str->length || !n)
		return str;
	len = str->length;
	if (start + n > len)
		n = len - start;
	memcpy(str->data + start, str->data + start + n, len - n);
	str->data[str->length -=n] = '\0';
	return str;
}

StrapString *s_string_trim(StrapString *str)
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

int s_string_compare(const StrapString *str1, const StrapString *str2)
{
	if (!str1 && !str2)
		return 0;
	if (!str1 && str2)
		return -1;
	if (str1 && !str2)
		return 1;
	return strcmp(str1->data, str2->data);
}

size_t s_string_nfind(const StrapString *str1, const StrapString *str2, size_t n)
{
	size_t i, j, found;
	size_t len1, len2;
	char *buf1, *buf2;

	if (!(str1 && str2))
		return -1;
	i = 0;
	len1 = str1->length;
	len2 = str2->length;
	buf1 = str1->data;
	buf2 = str2->data;
	found = -1;
	while (i < len1) {
		j = 0;
		if (buf1[i++] == buf2[j++]) {
			while (j < len2 && buf1[i++] == buf2[j++]);
			if (j == len2) {
				found++;
				if (found == n) {
					return i - j;
				}
			}
		}
	}
	return STRAP_NO_MATCH;
}

size_t s_string_find(const StrapString *str1, const StrapString *str2)
{
	return s_string_nfind(str1, str2, 0);
}

StrapArray *s_string_split(StrapString *str, StrapArray *arr, const char *sep)
{
	size_t len;
	size_t seplen;
	size_t splitlen;
	size_t i;
	char *buf;

	if (!str || !arr || !sep || arr->type != STRAP_TYPE_STRING)
		return arr;
	s_array_clear(arr);
	len = str->length;
	buf = calloc(len, sizeof*buf);
	seplen = strlen(sep);
	i = 0;
	splitlen = 0;
	while (i <= len) {
		if (memcmp(sep, str->data + i, seplen) == 0 || i == len) {
			memcpy(buf, str->data + i - splitlen, splitlen);
			buf[splitlen] = '\0';
			s_array_append_cstr(arr, buf);
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

StrapString *s_string_reverse(StrapString *str)
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

StrapString *s_string_shrink(StrapString *str)
{
	if (str)
		str_resize(str, s_next_pow2(str->length, STRAP_INIT_STR_SIZE));
	return str;
}
