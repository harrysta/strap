#pragma once

#include <stdio.h>

/* Write StrapArray to stdout */
#define s_array_printf(a) s_array_fprintf((a), stdout)
/* Write StrapString to stdout */
#define s_string_printf(a) printf("%s", s_string_get_cstr((a)))

#define STRAP_INIT_STR_SIZE 256
#define STRAP_INIT_CAPACITY 16

/* Returned from find/nfind functions */
#define STRAP_NO_MATCH ((size_t) -1)

/**
 * The types that indicate the element types upon creation of a StrapArray.
 * A StrapArray would consist of either a numerical type or a string type.
 * The difference between numerical types is the amount of bytes and the
 * representation format.
 */
typedef enum StrapType
{
	STRAP_TYPE_CHAR,
	STRAP_TYPE_SHORT,
	STRAP_TYPE_INT,
	STRAP_TYPE_LONG,

	STRAP_TYPE_FLOAT,
	STRAP_TYPE_DOUBLE,
	STRAP_TYPE_LONG_DOUBLE,

	STRAP_TYPE_STRING,

	STRAP_TYPE_NONE,
	STRAP_TYPE_COUNT
} StrapType;

/**
 * Wrapper for a string of characters. Stores a pointer to a dynamically
 * allocated char array and keeps track of its size. The actual content of the
 * string is determined by the length, which never exceeds size. When appending
 * or copying makes the string length larger than size, size is automatically
 * increased.
 */
typedef struct StrapString StrapString;

/**
 * Wrapper for an array of a specific element type. It consists of:
 *
 * * The count, that keeps track of the current number of elements inserted
 * inside the array. The count is equal to 0 when the array is empty.
 * * The capacity, which indicates the amount of elements this array can store
 * until it requires to resize the memory block containing its elements.
 * * A pointer to the dynamically allocated memory block that contains the
 * array's elements. It is automatically resized if the count exceeds the
 * capacity, without affecting the already existing elements inside it.
 */
typedef struct StrapArray StrapArray;

/* StrapString functions */

/**
 * Allocate a StrapString and copy the content of cstr up until the first
 * occurence of the terminating null character into its data. The resulting
 * StrapString should be freed with s_string_free() upon end of use.
 * Returns NULL if cstr is NULL.
 */
extern StrapString *s_string_alloc(const char *cstr);

/**
 * Like s_string_alloc(), but specify the amount of bytes the data container
 * will be allocated, if successful.
 */
extern StrapString *s_string_nalloc(const char *cstr, size_t size);

/**
 * Free the StrapString and its inner content if possible.
 */
extern void s_string_free(StrapString *str);

/**
 * Allocate a StrapString and copy the content of str into its data. The
 * content of str is not affected by this function call. The resulting
 * StrapString should be freed with s_string_free() upon end of use.
 */
extern StrapString *s_string_clone(const StrapString *str);

/**
 * Returns a pointer to the beginning of the StrapString's data.
 */
extern char *s_string_get_cstr(const StrapString *str);

/**
 * Return the number of characters the StrapString contains.
 */
extern size_t s_string_length(const StrapString *str);

/**
 * Return the size of the StrapString's current content. This shows the number
 * of bytes allocated by the StrapString to contain its content (not to be
 * confused with s_string_length()).
 */
extern size_t s_string_size(const StrapString *str);

/**
 * Copy the contents of an existing StrapString str2 to another existing
 * StrapString str1.
 */
extern StrapString *s_string_copy(StrapString *str1, const StrapString *str2);

/**
 * Appends a copy of the existing StrapString str2 to the existing StrapString
 * str1. The terminating null character of str1 is overwrittena by the first
 * character of str2.
 */
extern StrapString *s_string_concat(StrapString *str1, const StrapString *str2);

/**
 * Like s_string_copy(), but copy content from a c string instead of a
 * StrapString.
 */
extern StrapString *s_string_copy_from(StrapString *str, const char *cstr);

/**
 * Copy the content of the StrapString str into a c string cstr.
 */
extern char *s_string_copy_to(const StrapString *str, char *cstr);

/**
 * Like s_string_concat(), but append a copy of the content of a c string.
 */
extern StrapString *s_string_strcat(StrapString *str, const char *cstr);

/**
 * Like s_string_copy(), but specify the amount of characters from str2 to copy
 * to str1.
 */
extern StrapString *s_string_ncopy(StrapString *str1, const StrapString *str2, size_t n);

/**
 * Like s_string_concat(), but specify the amount of characters from str2 to
 * append to the end of str1.
 */
extern StrapString *s_string_nconcat(StrapString *str1, const StrapString *str2, size_t n);

/**
 * Like s_string_copy_from(), but specify the amount of characters from cstr to
 * to copy to str.
 */
extern StrapString *s_string_ncopy_from(StrapString *str, const char *cstr, size_t n);

/**
 * Like s_string_copy_to(), but specify the amount of characters from str to
 * copy to cstr.
 */
extern char *s_string_ncopy_to(const StrapString *str, char *cstr, size_t n);

/**
 * Like s_string_nstrcat(), but specify the amount of characters from cstr to
 * append to the end of str.
 */
extern StrapString *s_string_nstrcat(StrapString *str, const char *cstr, size_t n);

/**
 * Allocate a StrapString and copy n characters from str starting from start.
 */
extern StrapString *s_string_create_substring(const StrapString *str, size_t start, size_t n);

/**
 * Erase n bytes from str starting from start. This function does not resize
 * the size of the data container; shrinking is done with s_string_shrink().
 */
extern StrapString *s_string_erase(StrapString *str, size_t start, size_t n);

/**
 * Remove spaces at the beginning and at the end of StrapString str. The
 * remaining content is moved to the beginning of str.
 */
extern StrapString *s_string_trim(StrapString *str);

/**
 * Call strcmp with the content of str1 and str2. If either StrapString is NULL,
 * it is regarded as a lower value than its non-NULL counterpart. If both are
 * NULL, the function returns 0.
 */
extern int s_string_compare(const StrapString *str1, const StrapString *str2);

/**
 * Returns the index of the first occurence of sequence str2 found inside str1,
 * or STRAP_NO_MATCH if no match was found.
 */
extern size_t s_string_find(const StrapString *str1, const StrapString *str2);

/**
 * Returns the index of the nth occurence of sequence str2 found inside str1,
 * or STRAP_NO_MATCH if no match was found.
 */
extern size_t s_string_nfind(const StrapString *str1, const StrapString *str2, size_t n);

/**
 * Copy a list of strings into StrapArray arr, using sep as the delimiter
 * string sequence.
 */
extern StrapArray *s_string_split(StrapString *str, StrapArray *arr, const char *sep);

/**
 * Reverses the content of the StrapString str.
 */
extern StrapString *s_string_reverse(StrapString *str);

/**
 * Shrinks the size of the StrapString container while ensuring the size is
 * still large enough to contain its content.
 */
extern StrapString *s_string_shrink(StrapString *str);

/* StrapArray functions */

/**
 * Allocate a StrapArray with given type. The initial capacity is set to
 * STRAP_INIT_CAPACITY. The resulting StrapArray should be freed with
 * s_array_free() upon end of use.
 */
extern StrapArray *s_array_alloc(StrapType type);

/**
 * Like s_array_alloc(), but specify the initial capacity.
 */
extern StrapArray *s_array_nalloc(StrapType type, size_t capacity);

/**
 * Copy num amount of bytes from src into arr's element block and treat as the
 * array's element type.
 */
extern StrapArray *s_array_memcpy(StrapArray *arr, const void *src, size_t num);

/**
 * Copy num amount of elements from src into arr's element block. This function
 * is only available for numerical type arrays. For string arrays, use
 * s_array_strcpy().
 */
extern StrapArray *s_array_numcpy(StrapArray *arr, const void *src, size_t num);

/**
 * Copy num amount of elements from src into arr's element block. The src
 * should point to a string array in the form:
 * { "str1", "str2", "str3" }
 */
extern StrapArray *s_array_strcpy(StrapArray *arr, const char **src, size_t num);

/**
 * Free the StrapArray and its inner content if possible.
 */
extern void s_array_free(StrapArray *arr);

/** Return the total count of elements inside arr. */
extern size_t s_array_count(const StrapArray *arr);

/**
 * Return the capacity of current arr. The capacity is automatically resized
 * when count exceeds it during element insertion.
 */
extern size_t s_array_capacity(const StrapArray *arr);

/** Return the type of elements the given StrapArray can store */
extern StrapType s_array_type(const StrapArray *arr);

extern StrapString *s_array_create_string_from_element(const StrapArray *arr, size_t i);

/**
 * StrapArray getter functions.
 * Returns the value of the element at index i in the StrapArray.
 */
extern const char  *s_array_get_cstr(const StrapArray *arr, size_t i);
extern char      s_array_get_char(const StrapArray *arr, size_t i);
extern short     s_array_get_short(const StrapArray *arr, size_t i);
extern int       s_array_get_int(const StrapArray *arr, size_t i);
extern long      s_array_get_long(const StrapArray *arr, size_t i);

extern float       s_array_get_float(const StrapArray *arr, size_t i);
extern double      s_array_get_double(const StrapArray *arr, size_t i);
extern long double s_array_get_longdouble(const StrapArray *arr, size_t i);

/**
 * StrapArray append functions.
 * Pushes the given value to the end of the StrapArray.
 */
extern StrapArray *s_array_append_cstr(StrapArray *arr, const char *cstr);
extern StrapArray *s_array_append_string(StrapArray *arr, const StrapString *str);

extern StrapArray *s_array_append_char(StrapArray *arr, char num);
extern StrapArray *s_array_append_short(StrapArray *arr, short num);
extern StrapArray *s_array_append_int(StrapArray *arr, int num);
extern StrapArray *s_array_append_long(StrapArray *arr, long num);

extern StrapArray *s_array_append_float(StrapArray *arr, float num);
extern StrapArray *s_array_append_double(StrapArray *arr, double num);
extern StrapArray *s_array_append_longdouble(StrapArray *arr, long double num);

/**
 * Copies all the elements of arr2 to the end of arr1, if both share the same
 * element type.
 */
extern StrapArray *s_array_append_array(StrapArray *arr1, const StrapArray *arr2);

/**
 * StrapArray insert functions.
 * Inserts the given value to the specified index i of the StrapArray. The
 * elements after the index are pushed one slot forward.
 */
extern StrapArray *s_array_insert_cstr(StrapArray *arr, size_t i, const char *cstr);
extern StrapArray *s_array_insert_string(StrapArray *arr, size_t i, const StrapString *str);

extern StrapArray *s_array_insert_char(StrapArray *arr, size_t i, char num);
extern StrapArray *s_array_insert_short(StrapArray *arr, size_t i, short num);
extern StrapArray *s_array_insert_int(StrapArray *arr, size_t i, int num);
extern StrapArray *s_array_insert_long(StrapArray *arr, size_t i, long num);

extern StrapArray *s_array_insert_float(StrapArray *arr, size_t i, float num);
extern StrapArray *s_array_insert_double(StrapArray *arr, size_t i, double num);
extern StrapArray *s_array_insert_longdouble(StrapArray *arr, size_t i, long double num);

/**
 * Copies all the elements of arr2 to arr1 at index i, pushing all arr1's
 * elements after index i forward.
 */
extern StrapArray *s_array_insert_array(StrapArray *arr1, size_t i, const StrapArray *arr2);

/**
 * LIke s_array_insert_array(), but specify num elements of arr2 to copy,
 * starting at start.
 */
extern StrapArray *s_array_insert_array_range(StrapArray *arr1, size_t i, const StrapArray *arr2, size_t start, size_t n);

/**
 * StrapArray replace functions.
 * Replaces the element of the StrapArray at index i with given value.
 */
extern StrapArray *s_array_replace_cstr(StrapArray *arr, size_t i, const char *cstr);
extern StrapArray *s_array_replace_string(StrapArray *arr, size_t i, const StrapString *str);

extern StrapArray *s_array_replace_char(StrapArray *arr, size_t i, char num);
extern StrapArray *s_array_replace_short(StrapArray *arr, size_t i, short num);
extern StrapArray *s_array_replace_int(StrapArray *arr, size_t i, int num);
extern StrapArray *s_array_replace_long(StrapArray *arr, size_t i, long num);

extern StrapArray *s_array_replace_float(StrapArray *arr, size_t i, float num);
extern StrapArray *s_array_replace_double(StrapArray *arr, size_t i, double num);
extern StrapArray *s_array_replace_longdouble(StrapArray *arr, size_t i, long double num);

/**
 * StrapArray find functions.
 * Return the index of the first element inside arr that is equal to the
 * given value.
 */
extern size_t s_array_find_cstr(const StrapArray *arr, const char *cstr);
extern size_t s_array_find_string(const StrapArray *arr, StrapString *str);

extern size_t s_array_find_char(const StrapArray *arr, char num);
extern size_t s_array_find_short(const StrapArray *arr, short num);
extern size_t s_array_find_int(const StrapArray *arr, int num);
extern size_t s_array_find_long(const StrapArray *arr, long num);

extern size_t s_array_find_float(const StrapArray *arr, float num);
extern size_t s_array_find_double(const StrapArray *arr, double num);
extern size_t s_array_find_longdouble(const StrapArray *arr, long double num);

/**
 * StrapArray nfind functions.
 * Like the find functions, but specify to return the index of the nth
 * occurence of the given value inside arr.
 */
extern size_t s_array_nfind_cstr(const StrapArray *arr, const char *cstr, size_t n);
extern size_t s_array_nfind_string(const StrapArray *arr, StrapString *str, size_t n);

extern size_t s_array_nfind_char(const StrapArray *arr, char num, size_t n);
extern size_t s_array_nfind_short(const StrapArray *arr, short num, size_t n);
extern size_t s_array_nfind_int(const StrapArray *arr, int num, size_t n);
extern size_t s_array_nfind_long(const StrapArray *arr, long num, size_t n);

extern size_t s_array_nfind_float(const StrapArray *arr, float num, size_t n);
extern size_t s_array_nfind_double(const StrapArray *arr, double num, size_t n);
extern size_t s_array_nfind_longdouble(const StrapArray *arr, long double num, size_t n);

/**
 * StrapArray sum functions.
 * Performs a sum of all the elements of arr, if its elements' type is numeric.
 */
extern int           s_array_sum_int(const StrapArray *arr);
extern long          s_array_sum_long(const StrapArray *arr);
extern unsigned int  s_array_sum_uint(const StrapArray *arr);
extern unsigned long s_array_sum_ulong(const StrapArray *arr);
extern double        s_array_sum_double(const StrapArray *arr);
extern long double   s_array_sum_longdouble(const StrapArray *arr);

/**
 * Remove the element of arr at index i, pushing all elements after i 1 block
 * backward.
 */
extern StrapArray *s_array_erase(StrapArray *arr, size_t i);

/**
 * Like s_array_erase(), but specify to erase n elements starting from index i.
 */
extern StrapArray *s_array_erase_range(StrapArray *arr, size_t i, size_t n);

/**
 * Reverse the order of elements of arr.
 */
extern StrapArray *s_array_reverse(StrapArray *arr);

/**
 * Remove all elements inside arr, and setting count to 0. This function does
 * not affect the size, regardless how big it was.
 */
extern StrapArray *s_array_clear(StrapArray *arr);

/**
 * Shrinks the size of the StrapArray element block while ensuring the size is
 * still large enough to contain all existing elements.
 */
extern StrapArray *s_array_shrink(StrapArray *arr);

/**
 * Allocate a StrapArray and copy all elements of arr into it. The elements of
 * arr are not affected by this function call. The resulting StrapArray should
 * be freed with s_array_free() upon end of use.
 */
extern StrapArray *s_array_clone(const StrapArray *arr);

/**
 * Allocate a StrapArray and copy n elements starting from index i of arr into
 * it. The elements of arr are not affected by this function call. The
 * resulting StrapArray should be freed with s_array_free() upon end of use.
 */
extern StrapArray *s_array_create_subarray(const StrapArray *arr, size_t i, size_t n);

/**
 * compare the elements of arr1 to the elements of arr2. If their count is not
 * equal, the array with less elements is considered having lower value than
 * the other. If both arrays are NULL, they are considered equal, thus the
 * function returning 0.
 */
extern int s_array_compare(const StrapArray *arr1, const StrapArray *arr2);

/**
 * Writes a string that represents arr into s. Returns the total number of
 * characters written into s, or -1 on failure. StrapArrays are represented
 * as the following:
 * * string: [ "str1", "str2", "str3" ]
 * * number: [ n1, n2, n3 ]
 */
extern int s_array_sprintf(const StrapArray *arr, char *s);

/**
 * Like s_array_sprintf(), but writes the string to the stream.
 */
extern int s_array_fprintf(const StrapArray *arr, FILE *stream);

/**
 * Returns the size of a value depicted by type in bytes.
 */
extern size_t s_sizeof(StrapType type);
