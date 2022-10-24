#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strap.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ERROR_MESSAGE_SIZE 512
#define SIZE_LARGE 9999
#define SIZE_EIGHT 8
#define SIZE_MED 999
#define EPSILON 1e-7

#define TEST_BEFORE_EACH(a) test_before_each = a
#define TEST_AFTER_EACH(a) test_after_each = a
#define FLOAT_EQUALS(a,b) fabs(a - b) < EPSILON
#define DOUBLE_EQUALS(a,b) fabsl(a - b) < EPSILON
#define puts_h(s) puts(ANSI_COLOR_MAGENTA s ANSI_COLOR_RESET)

#define TEST_RUN(a)                                                      \
do {                                                                     \
	test_before_each();                                                    \
	printf("%s: ", #a);                                                    \
	res = a();                                                             \
	switch (res) {                                                         \
		case 1:                                                              \
			printf(ANSI_COLOR_GREEN "PASS" ANSI_COLOR_RESET "\n");             \
			pass_count++;                                                      \
			break;                                                             \
		case 2:                                                              \
			printf(ANSI_COLOR_YELLOW "NOT IMPLEMENTED" ANSI_COLOR_RESET "\n"); \
			break;                                                             \
		default:                                                             \
			printf(ANSI_COLOR_RED "FAILED" ANSI_COLOR_RESET "\n");             \
			printf("%s", error_message);                                       \
	}                                                                      \
	test_count++;                                                          \
	test_after_each();                                                     \
} while (0)

#define TEST_ASSERT_TRUE(a)                                            \
if (!(a)) {                                                            \
	int n = 0;                                                           \
	memset(error_message, 0, ERROR_MESSAGE_SIZE);                        \
	n = sprintf(error_message ,"  expected TRUE\n");                     \
	n += sprintf(error_message + n, "  at %s\n", #a);                    \
	n += sprintf(error_message + n, "  at %s:%d\n", __FILE__, __LINE__); \
	return 0;                                                            \
}

#define TEST_ASSERT_FALSE(a)                                           \
if ((a)) {                                                             \
	int n = 0;                                                           \
	memset(error_message, 0, ERROR_MESSAGE_SIZE);                        \
	n = sprintf(error_message ,"  expected FALSE\n");                    \
	n += sprintf(error_message + n, "  at %s\n", #a);                    \
	n += sprintf(error_message + n, "  at %s:%d\n", __FILE__, __LINE__); \
	return 0;                                                            \
}

static int pass_count;
static int test_count;
static char error_message[ERROR_MESSAGE_SIZE];
static void (*test_before_each)(void);
static void (*test_after_each)(void);

/**
 * This is plainly used to count the total functions in this file that are
 * used for tests against the TEST_RUN() calls. If their total count differs,
 * it means functions are being unused.
 */
typedef int test_t;

void test_prep();
void test_cleanup();

StrapArray *arr, *arr2;
StrapString *string, *string2, *string3, *string4;

test_t test_string_alloc_null()
{
	string = strap_string_alloc(NULL);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), ""));
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

test_t test_string_alloc_empty()
{
	string = strap_string_alloc("");
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), ""));
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

test_t test_string_alloc_string()
{
	char str[] = "string";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), str));
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(str));
	return 1;
}

test_t test_string_alloc_array()
{
	char buf[SIZE_EIGHT];

	memset(buf, 'a', SIZE_EIGHT);
	buf[7] = '\0'; /* always required for c strings */

	string = strap_string_alloc(buf);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "aaaaaaa"));
	TEST_ASSERT_TRUE(strap_string_length(string) == SIZE_EIGHT - 1);
	return 1;
}

test_t test_string_alloc_large()
{
	char buf[SIZE_LARGE];
	memset(buf, 'a', SIZE_LARGE);
	buf[SIZE_LARGE - 1] = 0;
	string = strap_string_alloc(buf);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), buf));
	TEST_ASSERT_TRUE(strap_string_length(string) == SIZE_LARGE - 1);
	return 1;
}

test_t test_string_clone_null()
{
	string = strap_string_clone(NULL);
	TEST_ASSERT_TRUE(!string);
	return 1;
}

test_t test_string_clone_empty()
{
	string2 = strap_string_alloc("");
	string = strap_string_clone(string2);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), ""));
	TEST_ASSERT_TRUE(strap_string_length(string) == strap_string_length(string2));
	return 1;
}

test_t test_string_clone_string()
{
	string2 = strap_string_alloc("first");
	string = strap_string_clone(string2);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "first"));
	TEST_ASSERT_TRUE(strap_string_length(string) == strap_string_length(string2));
	return 1;
}


test_t test_string_clone_large()
{
	char buf[SIZE_LARGE];

	memset(buf, 'a', SIZE_LARGE);
	buf[SIZE_LARGE - 1] = 0;
	string2 = strap_string_alloc(buf);
	string = strap_string_clone(string2);

	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), buf));
	TEST_ASSERT_TRUE(strap_string_length(string) == SIZE_LARGE - 1);
	return 1;
}
test_t test_string_copy_null()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_copy(string, NULL));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), ""));
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

test_t test_string_copy_empty()
{
	string = strap_string_alloc("first");
	string2 = strap_string_alloc(NULL);
	TEST_ASSERT_TRUE(strap_string_copy(string, string2));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), ""));
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

test_t test_string_copy_string()
{
	string = strap_string_alloc("first");
	string2 = strap_string_alloc("second");
	TEST_ASSERT_TRUE(strap_string_copy(string, string2));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "second"));
	TEST_ASSERT_TRUE(strap_string_length(string) == 6);
	return 1;
}

test_t test_string_copy_large()
{
	char buf[SIZE_LARGE];
	memset(buf, 'a', SIZE_LARGE);
	buf[SIZE_LARGE - 1] = 0;
	string = strap_string_alloc("first");
	string2 = strap_string_alloc(buf);
	TEST_ASSERT_TRUE(strap_string_copy(string, string2));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), buf));
	TEST_ASSERT_TRUE(strap_string_length(string) == SIZE_LARGE - 1);
	return 1;
}

test_t test_string_ncopy_string()
{
	string = strap_string_alloc("first");
	string2 = strap_string_alloc("second");
	TEST_ASSERT_TRUE(strap_string_ncopy(string, string2, 3));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "sec"));
	TEST_ASSERT_TRUE(strap_string_length(string) == 3);
	return 1;
}

test_t test_string_concat_null()
{
	char str[] = "str";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(strap_string_concat(string, NULL));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), str));
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(str));
	return 1;
}

test_t test_string_concat_empty()
{
	char str[] = "first";
	string = strap_string_alloc(str);
	string2 = strap_string_alloc(NULL);
	TEST_ASSERT_TRUE(strap_string_concat(string, string2));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), str));
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(str));
	return 1;
}

test_t test_string_concat_string()
{
	const char *catstr = "firstsecond";
	string = strap_string_alloc("first");
	string2 = strap_string_alloc("second");
	TEST_ASSERT_TRUE(strap_string_concat(string, string2));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), catstr));
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(catstr));
	return 1;
}

test_t test_string_concat_large()
{
	const char *buf = "first";
	char buf2[9994];
	char buf3[SIZE_LARGE];
	memset(buf2, 'a', 9994);
	memset(buf3, 'a', SIZE_LARGE);
	buf2[9993] = 0;
	buf3[SIZE_LARGE - 1] = 0;
	memcpy(buf3, buf, 5);
	string = strap_string_alloc(buf);
	string2 = strap_string_alloc(buf2);
	TEST_ASSERT_TRUE(strap_string_concat(string, string2));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), buf3));
	TEST_ASSERT_TRUE(strap_string_length(string) == SIZE_LARGE - 1);
	return 1;
}

test_t test_string_nconcat_string()
{
	const char *catstr = "firstsec";
	string = strap_string_alloc("first");
	string2 = strap_string_alloc("second");
	TEST_ASSERT_TRUE(strap_string_nconcat(string, string2, 3));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), catstr));
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(catstr));
	return 1;
}

test_t test_string_copy_from_null()
{
	char str[] = "first";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(!strap_string_copy_from(NULL, NULL));
	TEST_ASSERT_TRUE(strap_string_copy_from(string, NULL));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), ""));
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

test_t test_string_copy_from_empty()
{
	char str[] = "first";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(strap_string_copy_from(string, ""));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), ""));
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

test_t test_string_copy_from_string()
{
	char str[] = "first";
	char str2[] = "second";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(strap_string_copy_from(string, str2));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), str2));
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(str2));
	return 1;
}

test_t test_string_copy_from_large()
{
	char str[] = "first";
	char str2[SIZE_LARGE];

	string = strap_string_alloc(str);
	memset(str2, 'a', SIZE_LARGE);
	str2[SIZE_LARGE - 1] = 0;

	TEST_ASSERT_TRUE(strap_string_copy_from(string, str2));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), str2));
	TEST_ASSERT_TRUE(strap_string_length(string) == SIZE_LARGE - 1);
	return 1;
}

test_t test_string_ncopy_from_string()
{
	char str[] = "first";
	char str2[] = "second";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(strap_string_ncopy_from(string, str2, 3));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "sec"));
	TEST_ASSERT_TRUE(strap_string_length(string) == 3);
	return 1;
}

test_t test_string_copy_to_null()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(!strap_string_copy_to(NULL, NULL));
	TEST_ASSERT_TRUE(!strap_string_copy_to(string, NULL));
	return 1;
}

test_t test_string_copy_to_empty()
{
	char str[16];
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_copy_to(string, str));
	TEST_ASSERT_TRUE(!strcmp(str, "str"));
	TEST_ASSERT_TRUE(strlen(str) == 3);
	return 1;
}

test_t test_string_copy_to_string()
{
	char str[16] = "first";
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_copy_to(string, str));
	TEST_ASSERT_TRUE(!strcmp(str, "str"));
	TEST_ASSERT_TRUE(strlen(str) == 3);
	return 1;
}

test_t test_string_ncopy_to_string()
{
	char str[16] = "first";
	string = strap_string_alloc("second");
	TEST_ASSERT_TRUE(strap_string_ncopy_to(string, str, 3));
	TEST_ASSERT_TRUE(!strcmp(str, "sec"));
	TEST_ASSERT_TRUE(strlen(str) == 3);
	return 1;
}

test_t test_string_strcat_null()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(!strap_string_strcat(NULL, NULL));
	TEST_ASSERT_TRUE(strap_string_strcat(string, NULL));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "str"));
	TEST_ASSERT_TRUE(strap_string_length(string) == 3);
	return 1;
}

test_t test_string_strcat_empty()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_strcat(string, ""));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "str"));
	TEST_ASSERT_TRUE(strap_string_length(string) == 3);
	return 1;
}

test_t test_string_strcat_string()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_strcat(string, "ing"));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "string"));
	TEST_ASSERT_TRUE(strap_string_length(string) == 6);
	return 1;
}

test_t test_string_strcat_large()
{
	char buf[9996];
	char buf2[SIZE_LARGE];
	memset(buf, 'a', 9996);
	memset(buf2, 'a', SIZE_LARGE);
	buf[9995] = 0;
	buf2[SIZE_LARGE - 1] = 0;
	memcpy(buf2, "str", 3);

	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_strcat(string, buf));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), buf2));
	TEST_ASSERT_TRUE(strap_string_length(string) == SIZE_LARGE - 1);
	return 1;
}

test_t test_string_nstrcat_string()
{
	string = strap_string_alloc("first");
	TEST_ASSERT_TRUE(strap_string_nstrcat(string, "second", 3));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "firstsec"));
	TEST_ASSERT_TRUE(strap_string_length(string) == 8);
	return 1;
}

test_t test_string_substring_null()
{
	return 2;
}

test_t test_string_substring_invalid()
{
	string = strap_string_alloc("first");
	TEST_ASSERT_TRUE(string2 = strap_string_create_substring(string, 10, 10));
	TEST_ASSERT_TRUE(string3 = strap_string_create_substring(string, 0, 0));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string2), ""));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string3), ""));
	return 1;
}

test_t test_string_substring_valid()
{
	string = strap_string_alloc("motherhood");
	TEST_ASSERT_TRUE(string2 = strap_string_create_substring(string, 1, 5));
	TEST_ASSERT_TRUE(string3 = strap_string_create_substring(string, 6, 999));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string2), "other"));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string3), "hood"));
	return 1;
}

test_t test_string_erase_null()
{
	return 2;
}

test_t test_string_erase_invalid()
{
	char buf[] = "first";
	string = strap_string_alloc(buf);
	TEST_ASSERT_FALSE(strap_string_erase(NULL, 5, 10));
	TEST_ASSERT_TRUE(strap_string_erase(string, 10, 10));
	TEST_ASSERT_TRUE(strap_string_erase(string, 0, 0));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), buf));
	return 1;
}

test_t test_string_erase_valid()
{
	string = strap_string_alloc("string motherhood");
	TEST_ASSERT_TRUE(strap_string_erase(string, 0, 7));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "motherhood"));
	TEST_ASSERT_TRUE(strap_string_erase(string, 6, 10));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "mother"));
	return 1;
}

test_t test_string_trim_null()
{
	return 2;
}

test_t test_string_trim_valid()
{
	string = strap_string_alloc("    this string           ");
	string2 = strap_string_alloc("nospace");
	string3 = strap_string_alloc(" ");
	string4 = strap_string_alloc("");
	TEST_ASSERT_TRUE(strap_string_trim(string));
	TEST_ASSERT_TRUE(strap_string_trim(string2));
	TEST_ASSERT_TRUE(strap_string_trim(string3));
	TEST_ASSERT_TRUE(strap_string_trim(string4));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "this string"));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string2), "nospace"));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string4), ""));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string3), ""));
	return 1;
}

test_t test_string_compare_null()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_compare(NULL, NULL) == 0);
	TEST_ASSERT_TRUE(strap_string_compare(string, NULL) == 1);
	TEST_ASSERT_TRUE(strap_string_compare(NULL, string) == -1);
	return 1;
}

test_t test_string_compare_nonequal()
{
	string = strap_string_alloc("strap");
	string2 = strap_string_alloc("strip");
	TEST_ASSERT_TRUE(strap_string_compare(string, string2) < 0);
	TEST_ASSERT_TRUE(strap_string_compare(string2, string) > 0);
	return 1;
}

test_t test_string_compare_equal()
{
	string = strap_string_alloc("strap");
	string2 = strap_string_clone(string);
	TEST_ASSERT_TRUE(strap_string_compare(string, string2) == 0);
	return 1;
}

test_t test_string_find_null()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_find(NULL, NULL) == STRAP_NO_MATCH);
	TEST_ASSERT_TRUE(strap_string_find(string, NULL) == STRAP_NO_MATCH);
	return 1;
}

test_t test_string_find_no_match()
{
	string = strap_string_alloc("str");
	string2 = strap_string_alloc("a");
	TEST_ASSERT_TRUE(strap_string_find(string, string2) == STRAP_NO_MATCH);
	return 1;
}

test_t test_string_find_match()
{
	string = strap_string_alloc("this is a rather long string");
	string2 = strap_string_alloc("long");
	TEST_ASSERT_TRUE(strap_string_find(string, string2) == 17);
	return 1;
}

test_t test_string_nfind_match()
{
	return 2;
}

test_t test_string_nfind_no_match()
{
	return 2;
}

test_t test_string_split_null()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	string = strap_string_alloc("");
	TEST_ASSERT_TRUE(strap_string_split(NULL, arr, ""));
	TEST_ASSERT_TRUE(!strap_string_split(string, NULL, ""));
	TEST_ASSERT_TRUE(strap_string_split(string, arr, NULL));
	return 1;
}

test_t test_string_split_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	string = strap_string_alloc("");
	TEST_ASSERT_TRUE(strap_string_split(string, arr, ","));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 1);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), ""));
	return 1;
}

test_t test_string_split_comma()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	string = strap_string_alloc("one,two,three,four");
	TEST_ASSERT_TRUE(strap_string_split(string, arr, ","));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 4);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), "one"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), "two"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 2), "three"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 3), "four"));
	return 1;
}

test_t test_string_split_word()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	string = strap_string_alloc("one-w-two-w-three");
	TEST_ASSERT_TRUE(strap_string_split(string, arr, "-w-"));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), "one"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), "two"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 2), "three"));
	return 1;
}

test_t test_string_reverse_null()
{
	return 2;
}

test_t test_string_reverse_valid()
{
	string = strap_string_alloc("this is my string");
	TEST_ASSERT_TRUE(!strap_string_reverse(NULL));
	TEST_ASSERT_TRUE(strap_string_reverse(string));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "gnirts ym si siht"));
	return 1;
}

test_t test_string_shrink_null()
{
	return 2;
}

test_t test_string_shrink_empty()
{
	return 2;
}

test_t test_string_shrink_valid()
{
	return 2;
}

// ----------------------------------------------------------------------------

test_t test_array_clone_null()
{
	TEST_ASSERT_FALSE(strap_array_clone(NULL));
	return 1;
}

test_t test_array_clone_string_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	strap_array_append_cstr(arr, "this");
	strap_array_append_cstr(arr, "is");
	strap_array_append_cstr(arr, "a");
	strap_array_append_cstr(arr, "strap_array_clone");
	strap_array_append_cstr(arr, "function");
	strap_array_append_cstr(arr, "test");
	TEST_ASSERT_TRUE(arr2 = strap_array_clone(arr));
	TEST_ASSERT_TRUE(strap_array_count(arr2) == 6);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 0), "this"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 1), "is"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 2), "a"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 3), "strap_array_clone"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 4), "function"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 5), "test"));
	TEST_ASSERT_FALSE(arr == arr2);
	return 1;
}

test_t test_array_clone_valid_int()
{
	int i;
	arr = strap_array_alloc(STRAP_TYPE_INT);
	for (i = 0; i < STRAP_INIT_CAPACITY*2; i++) {
		strap_array_append_int(arr, i);
	}
	strap_array_clear(arr);
	for (i = 0; i < STRAP_INIT_CAPACITY/2; i++) {
		strap_array_append_int(arr, i);
	}
	TEST_ASSERT_TRUE(arr2 = strap_array_clone(arr));
	TEST_ASSERT_TRUE(strap_array_count(arr2) == STRAP_INIT_CAPACITY/2);
	TEST_ASSERT_TRUE(strap_array_capacity(arr) != strap_array_capacity(arr2));
	TEST_ASSERT_FALSE(arr == arr2);
	return 1;
}

test_t test_array_create_string_null()
{
	TEST_ASSERT_TRUE(!strap_array_create_string(NULL, 1));
	return 1;
}

test_t test_array_create_string_invalid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "one"));
	TEST_ASSERT_TRUE(!strap_array_create_string(arr, 4));
	return 1;
}

test_t test_array_create_string_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "one"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "two"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "three"));
	TEST_ASSERT_TRUE(string = strap_array_create_string(arr, 1));
	TEST_ASSERT_TRUE(!strcmp(strap_string_get_cstr(string), "two"));
	return 1;
}

test_t test_array_append_string_null()
{
	return 2;
}

test_t test_array_append_string_valid()
{
	return 2;
}


test_t test_array_append_cstr_null()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(!strap_array_append_cstr(NULL, NULL));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, NULL));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 0);
	return 1;
}

test_t test_array_append_cstr_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, ""));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, ""));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	return 1;
}

test_t test_array_append_cstr_valid()
{
	int i;
	char *buf[4] = { "first", "second", "third", "fourth" };
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	for (i = 0; i < 4; i++)
		TEST_ASSERT_TRUE(strap_array_append_cstr(arr, buf[i]));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 4);
	for (i = 0; i < 4; i++)
		TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, i), buf[i]));
	return 1;
}

test_t test_array_append_cstr_large_string()
{
	const char *buf = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
		"Vivamus porta orci orci. Donec convallis felis faucibus quam tempor "
		"lobortis. Suspendisse vitae velit ornare, pulvinar mauris ut, tempor nibh. "
		"Cras vitae lacus quis tortor viverra molestie. Etiam aliquam neque risus, "
		"vel mollis est dignissim at. Donec iaculis blandit finibus. Maecenas nec "
		"laoreet lorem, sed pellentesque erat. Nullam lacus dui, ultrices eget "
		"egestas vel, laoreet ut arcu. Aliquam eget ipsum egestas, convallis "
		"leo eu integer. ";
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, buf));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, buf));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 2);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), buf));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), buf));
	return 1;
}

test_t test_array_append_cstr_large_count()
{
	int i;
	char buf[SIZE_LARGE];
	memset(buf, 'a', SIZE_LARGE);
	buf[SIZE_LARGE - 1] = '\0';
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	for (i = 0; i < SIZE_LARGE; i++)
		TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "a"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, buf));
	TEST_ASSERT_TRUE(strap_array_count(arr) == SIZE_LARGE + 1);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, SIZE_LARGE), buf));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), "a"));
	return 1;
}

test_t test_array_insert_cstr_null()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(!strap_array_insert_cstr(NULL, 0, NULL));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, NULL));
	TEST_ASSERT_TRUE(!strap_array_count(arr));
	return 1;
}

test_t test_array_insert_cstr_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, ""));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, ""));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), ""));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), ""));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 2), "third"));
	return 1;
}

// TEMP REMOVEME
extern void prt(StrapArray *arr);

test_t test_array_insert_cstr_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, "third"));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, "one"));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 1, "second"));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 1, "a second long string"));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 10, "last"));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 5);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), "one"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), "a second long string"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 2), "second"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 3), "third"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 4), "last"));
	return 1;
}

test_t test_array_insert_cstr_large()
{
	int i;
	char buf[SIZE_LARGE];
	memset(buf, 'a', SIZE_LARGE);
	buf[SIZE_LARGE - 1] = '\0';
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	for (i = 0; i < SIZE_LARGE; i++)
		TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, "a"));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, buf));
	TEST_ASSERT_TRUE(strap_array_count(arr) == SIZE_LARGE + 1);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), buf));
	return 1;
}

test_t test_array_insert_string_null()
{
	return 2;
}

test_t test_array_insert_string_valid()
{
	return 2;
}

test_t test_array_replace_cstr_null()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(!strap_array_replace_cstr(NULL, 0, NULL));
	TEST_ASSERT_TRUE(strap_array_replace_cstr(arr, 0, NULL));
	TEST_ASSERT_TRUE(!strap_array_count(arr));
	return 1;
}

test_t test_array_replace_cstr_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_replace_cstr(arr, 0, "test"));
	TEST_ASSERT_TRUE(strap_array_replace_cstr(arr, 1, "me"));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 0);
	return 1;
}

test_t test_array_replace_cstr_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "one"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_replace_cstr(arr, 0, "first"));
	TEST_ASSERT_TRUE(strap_array_replace_cstr(arr, 1, ""));
	TEST_ASSERT_TRUE(strap_array_replace_cstr(arr, 2, "3rd"));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), "first"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), ""));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 2), "3rd"));
	return 1;
}

test_t test_array_replace_cstr_equal_length()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "first"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_replace_cstr(arr, 1, "number"));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), "number"));
	return 1;
}

test_t test_array_replace_cstr_large()
{
	char buf[SIZE_LARGE];
	memset(buf, 'a', SIZE_LARGE);
	buf[SIZE_LARGE - 1] = '\0';
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	strap_array_append_cstr(arr, "test");
	TEST_ASSERT_TRUE(strap_array_replace_cstr(arr, 0, buf));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), buf));
	return 1;
}

test_t test_array_replace_string_null()
{
	return 2;
}

test_t test_array_replace_string_valid()
{
	return 2;
}

test_t test_array_find_cstr_null()
{
	TEST_ASSERT_TRUE(strap_array_find_cstr(NULL, NULL) == STRAP_NO_MATCH);
	TEST_ASSERT_TRUE(strap_array_find_cstr(NULL, "test") == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_find_cstr_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_find_cstr(arr, NULL) == STRAP_NO_MATCH);
	TEST_ASSERT_TRUE(strap_array_find_cstr(arr, "test") == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_find_cstr_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "first"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "fourth"));
	TEST_ASSERT_TRUE(strap_array_find_cstr(arr, "test") == STRAP_NO_MATCH);
	TEST_ASSERT_TRUE(strap_array_find_cstr(arr, "third") == 2);
	return 1;
}

test_t test_array_find_string_null()
{
	return 2;
}

test_t test_array_find_string_empty()
{
	return 2;
}

test_t test_array_nfind_cstr_null()
{
	return 2;
}

test_t test_array_nfind_cstr_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "first"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "test"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "fourth"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "test"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "fifth"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "sixth"));
	TEST_ASSERT_TRUE(strap_array_nfind_cstr(arr, "test", 0) == 1);
	TEST_ASSERT_TRUE(strap_array_nfind_cstr(arr, "test", 1) == 4);
	TEST_ASSERT_TRUE(strap_array_nfind_cstr(arr, "test", 2) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_nfind_cstr_no_match()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "first"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "test"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "fourth"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "test"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "fifth"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "sixth"));
	TEST_ASSERT_TRUE(strap_array_nfind_cstr(arr, "something", 1) == STRAP_NO_MATCH);
	TEST_ASSERT_TRUE(strap_array_nfind_cstr(arr, "something", 0) == STRAP_NO_MATCH);
	return 1;
}

int test_array_nfind_string_null()
{
	return 2;
}

int test_array_nfind_string_valid()
{
	return 2;
}






test_t test_array_clear_null()
{
	TEST_ASSERT_TRUE(!strap_array_clear(NULL));
	return 1;
}

test_t test_array_clear_str_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "first"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_clear(arr));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 0);
	return 1;
}

test_t test_array_erase_range_str_first_element()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "first"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_erase_range(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 2);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), "second"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), "third"));
	return 1;
}

test_t test_array_erase_range_str_last_element()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "first"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_erase_range(arr, 2, 5));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 2);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), "first"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), "second"));
	return 1;
}


test_t test_array_erase_range_str_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "first"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "fourth"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "five"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "six"));
	TEST_ASSERT_TRUE(strap_array_erase_range(arr, 2, 3));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), "first"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), "second"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 2), "six"));
	return 1;
}


test_t test_array_erase_range_int_first_element()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 24));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 55));
	TEST_ASSERT_TRUE(strap_array_erase_range(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 2);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 0) == 24);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 1) == 55);
	return 1;
}

test_t test_array_erase_range_int_last_element()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 24));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 55));
	TEST_ASSERT_TRUE(strap_array_erase_range(arr, 2, 5));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 2);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 0) == 13);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 1) == 24);
	return 1;
}

test_t test_array_erase_range_int_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 24));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 55));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 9));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 73));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 6));
	TEST_ASSERT_TRUE(strap_array_erase_range(arr, 2, 3));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 0) == 13);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 1) == 24);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 2) == 6);
	return 1;
}

test_t test_array_erase_range_float_first_element()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 24));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 55));
	TEST_ASSERT_TRUE(strap_array_erase_range(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 2);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 0) == 24);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 1) == 55);
	return 1;
}

test_t test_array_erase_range_float_last_element()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 24));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 55));
	TEST_ASSERT_TRUE(strap_array_erase_range(arr, 2, 5));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 2);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 0) == 13);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 1) == 24);
	return 1;
}

test_t test_array_erase_range_float_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 24));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 55));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 9));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 73));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 6));
	TEST_ASSERT_TRUE(strap_array_erase_range(arr, 2, 3));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 0) == 13);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 1) == 24);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 2) == 6);
	return 1;
}


test_t test_array_reverse_null()
{
	TEST_ASSERT_FALSE(strap_array_reverse(NULL));
	return 1;
}

test_t test_array_reverse_str_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	return 1;
}

test_t test_array_reverse_str_valid_odd()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	strap_array_append_cstr(arr, "reversed");
	strap_array_append_cstr(arr, "was");
	strap_array_append_cstr(arr, "this");
	TEST_ASSERT_TRUE(strap_array_reverse(arr) == arr);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), "this"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), "was"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 2), "reversed"));
	return 1;
}

test_t test_array_reverse_str_valid_even()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	strap_array_append_cstr(arr, "reversed");
	strap_array_append_cstr(arr, "also");
	strap_array_append_cstr(arr, "was");
	strap_array_append_cstr(arr, "this");
	TEST_ASSERT_TRUE(strap_array_reverse(arr) == arr);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), "this"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), "was"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 2), "also"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 3), "reversed"));
	return 1;
}

test_t test_array_reverse_short_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	return 1;
}

test_t test_array_reverse_short_valid_odd()
{
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	strap_array_append_short(arr, 5);
	strap_array_append_short(arr, 13);
	strap_array_append_short(arr, 24);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 0) == 24);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 1) == 13);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 2) == 5);
	return 1;
}

test_t test_array_reverse_short_valid_even()
{
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	strap_array_append_short(arr, 5);
	strap_array_append_short(arr, 13);
	strap_array_append_short(arr, 24);
	strap_array_append_short(arr, 7);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 0) == 7);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 1) == 24);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 2) == 13);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 3) == 5);
	return 1;
}

test_t test_array_reverse_int_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	return 1;
}

test_t test_array_reverse_int_valid_odd()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	strap_array_append_int(arr, 5);
	strap_array_append_int(arr, 13);
	strap_array_append_int(arr, 24);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 0) == 24);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 1) == 13);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 2) == 5);
	return 1;
}

test_t test_array_reverse_int_valid_even()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	strap_array_append_int(arr, 5);
	strap_array_append_int(arr, 13);
	strap_array_append_int(arr, 24);
	strap_array_append_int(arr, 7);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 0) == 7);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 1) == 24);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 2) == 13);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 3) == 5);
	return 1;
}

test_t test_array_reverse_float_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	return 1;
}

test_t test_array_reverse_float_valid_odd()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	strap_array_append_float(arr, 0.03);
	strap_array_append_float(arr, 13);
	strap_array_append_float(arr, 24);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 0) == 24);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 1) == 13);
	TEST_ASSERT_TRUE(FLOAT_EQUALS(strap_array_get_float(arr, 2), 0.03));
	return 1;
}

test_t test_array_reverse_float_valid_even()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	strap_array_append_float(arr, 5);
	strap_array_append_float(arr, 13);
	strap_array_append_float(arr, 24);
	strap_array_append_float(arr, 0.07);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	TEST_ASSERT_TRUE(FLOAT_EQUALS(strap_array_get_float(arr, 0), 0.07));
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 1) == 24);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 2) == 13);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 3) == 5);
	return 1;
}

test_t test_array_reverse_longdouble_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_LONG_DOUBLE);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	return 1;
}

test_t test_array_reverse_longdouble_valid_odd()
{
	arr = strap_array_alloc(STRAP_TYPE_LONG_DOUBLE);
	strap_array_append_longdouble(arr, 0.03);
	strap_array_append_longdouble(arr, 13);
	strap_array_append_longdouble(arr, 24);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	TEST_ASSERT_TRUE(strap_array_get_longdouble(arr, 0) == 24);
	TEST_ASSERT_TRUE(strap_array_get_longdouble(arr, 1) == 13);
	TEST_ASSERT_TRUE(strap_array_get_longdouble(arr, 2) == 0.03);
	return 1;
}

test_t test_array_reverse_longdouble_valid_even()
{
	arr = strap_array_alloc(STRAP_TYPE_LONG_DOUBLE);
	strap_array_append_longdouble(arr, 5);
	strap_array_append_longdouble(arr, 13);
	strap_array_append_longdouble(arr, 24);
	strap_array_append_longdouble(arr, 0.07);
	TEST_ASSERT_TRUE(strap_array_reverse(arr));
	TEST_ASSERT_TRUE(strap_array_get_longdouble(arr, 0) == 0.07);
	TEST_ASSERT_TRUE(strap_array_get_longdouble(arr, 1) == 24);
	TEST_ASSERT_TRUE(strap_array_get_longdouble(arr, 2) == 13);
	TEST_ASSERT_TRUE(strap_array_get_longdouble(arr, 3) == 5);
	return 1;
}


test_t test_array_create_subarray_null()
{
	TEST_ASSERT_FALSE(strap_array_create_subarray(NULL, 5, 12));
	return 1;
}

test_t test_array_create_subarray_str_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_FALSE(strap_array_create_subarray(arr, 0, 0));
	TEST_ASSERT_FALSE(strap_array_create_subarray(arr, 0, 1));
	return 1;
}

test_t test_array_create_subarray_str_invalid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	strap_array_append_cstr(arr, "one");
	strap_array_append_cstr(arr, "two");
	strap_array_append_cstr(arr, "three");
	TEST_ASSERT_FALSE(arr2 = strap_array_create_subarray(arr, 5, 1));
	TEST_ASSERT_FALSE(arr2 = strap_array_create_subarray(arr, 0, 0));
	return 1;
}

test_t test_array_create_subarray_str_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "first"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "fourth"));
	TEST_ASSERT_TRUE(arr2 = strap_array_create_subarray(arr, 1, 2));
	TEST_ASSERT_TRUE(strap_array_count(arr2) == 2);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 0), "second"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 1), "third"));
	return 1;
}

test_t test_array_create_subarray_str_first_element()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "number"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "one"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "three"));
	TEST_ASSERT_TRUE(arr2 = strap_array_create_subarray(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_count(arr2) == 1);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 0), "number"));
	return 1;
}

test_t test_array_create_subarray_str_large_n()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "number"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "one"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "three"));
	TEST_ASSERT_TRUE(arr2 = strap_array_create_subarray(arr, 0, 100));
	TEST_ASSERT_TRUE(strap_array_count(arr2) == 3);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 0), "number"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 1), "one"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr2, 2), "three"));
	return 1;
}

test_t test_array_shrink_null()
{
	TEST_ASSERT_FALSE(strap_array_shrink(NULL));
	return 1;
}

test_t test_array_shrink_str_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_shrink(arr));
	TEST_ASSERT_TRUE(strap_array_capacity(arr) == STRAP_INIT_CAPACITY);
	return 1;
}

test_t test_array_shrink_str_valid()
{
	int i;
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	for (i = 0; i < 128; i++)
		strap_array_append_cstr(arr, "test");
	strap_array_clear(arr);
	TEST_ASSERT_FALSE(strap_array_capacity(arr) == STRAP_INIT_CAPACITY);
	TEST_ASSERT_TRUE(strap_array_shrink(arr));
	TEST_ASSERT_TRUE(strap_array_capacity(arr) == STRAP_INIT_CAPACITY);
	return 1;
}

test_t test_array_sort_null()
{
	return 2;
}

test_t test_array_sort_str_empty()
{
	return 2;
}

test_t test_array_sort_str_ascending()
{
	return 2;
}

test_t test_array_sort_str_descending()
{
	return 2;
}


// ----------------------------------------------------------------------------

test_t test_array_append_char_null()
{
	TEST_ASSERT_FALSE(strap_array_append_char(NULL, 0));
	return 1;
}

test_t test_array_append_char_valid()
{
	int i;
	char buf[5] = { 1, 3, 0, 5, 13 };
	arr = strap_array_alloc(STRAP_TYPE_CHAR);
	for (i = 0; i < 5; i++)
		TEST_ASSERT_TRUE(strap_array_append_char(arr, buf[i]));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 5);
	for (i = 0; i < 5; i++)
		TEST_ASSERT_TRUE(strap_array_get_char(arr, i) == buf[i]);
	return 1;
}

test_t test_array_insert_char_null()
{
	TEST_ASSERT_FALSE(strap_array_insert_char(NULL, 0, 0));
	return 1;
}

test_t test_array_insert_char_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_CHAR);
	TEST_ASSERT_TRUE(strap_array_insert_char(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_insert_char(arr, 0, 2));
	TEST_ASSERT_TRUE(strap_array_insert_char(arr, 0, 3));
	TEST_ASSERT_TRUE(strap_array_insert_char(arr, 1, 4));
	TEST_ASSERT_TRUE(strap_array_insert_char(arr, 0, 5));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 5);
	TEST_ASSERT_TRUE(strap_array_get_char(arr, 0) == 5);
	TEST_ASSERT_TRUE(strap_array_get_char(arr, 1) == 3);
	TEST_ASSERT_TRUE(strap_array_get_char(arr, 2) == 4);
	TEST_ASSERT_TRUE(strap_array_get_char(arr, 3) == 2);
	TEST_ASSERT_TRUE(strap_array_get_char(arr, 4) == 1);
	return 1;
}

test_t test_array_replace_char_null()
{
	TEST_ASSERT_FALSE(strap_array_replace_char(NULL, 0, 23));
	return 1;
}

test_t test_array_replace_char_invalid_index()
{
	arr = strap_array_alloc(STRAP_TYPE_CHAR);
	TEST_ASSERT_TRUE(strap_array_replace_char(arr, 0, 23));
	TEST_ASSERT_TRUE(strap_array_replace_char(arr, 1, 35));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 0);
	return 1;
}

test_t test_array_replace_char_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_CHAR);
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 5));
	TEST_ASSERT_TRUE(strap_array_replace_char(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_replace_char(arr, 1, 13));
	TEST_ASSERT_TRUE(strap_array_replace_char(arr, 2, 25));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(strap_array_get_char(arr, 0) == 1);
	TEST_ASSERT_TRUE(strap_array_get_char(arr, 1) == 13);
	TEST_ASSERT_TRUE(strap_array_get_char(arr, 2) == 25);
	return 1;
}

test_t test_array_find_char_null()
{
	TEST_ASSERT_TRUE(strap_array_find_char(NULL, 55) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_find_char_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_CHAR);
	TEST_ASSERT_TRUE(strap_array_find_char(arr, 5) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_find_char_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_CHAR);
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 23));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 15));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 99));
	TEST_ASSERT_TRUE(strap_array_find_char(arr, 5) == 0);
	TEST_ASSERT_TRUE(strap_array_find_char(arr, 15) == 2);
	return 1;
}

test_t test_array_nfind_char_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_CHAR);
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 1));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 3));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 4));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 99));
	TEST_ASSERT_TRUE(strap_array_nfind_char(arr, 13, 0) == 1);
	TEST_ASSERT_TRUE(strap_array_nfind_char(arr, 13, 1) == 4);
	TEST_ASSERT_TRUE(strap_array_nfind_char(arr, 13, 2) == 5);
	TEST_ASSERT_TRUE(strap_array_nfind_char(arr, 13, 3) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_nfind_char_no_match()
{
	arr = strap_array_alloc(STRAP_TYPE_CHAR);
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 1));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 2));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 3));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 4));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_char(arr, 6));
	TEST_ASSERT_TRUE(strap_array_nfind_char(arr, 13, 0) == STRAP_NO_MATCH);
	TEST_ASSERT_TRUE(strap_array_nfind_char(arr, 13, 1) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_shrink_char_valid()
{
	char i;
	arr = strap_array_alloc(STRAP_TYPE_CHAR);
	for (i = 0; i < 127; i++)
		strap_array_append_char(arr, i);
	strap_array_clear(arr);
	TEST_ASSERT_FALSE(strap_array_capacity(arr) == STRAP_INIT_CAPACITY);
	TEST_ASSERT_TRUE(strap_array_shrink(arr));
	TEST_ASSERT_TRUE(strap_array_capacity(arr) == STRAP_INIT_CAPACITY);
	return 1;
}

// ----------------------------------------------------------------------------

test_t test_array_append_int_null()
{
	TEST_ASSERT_FALSE(strap_array_append_int(NULL, 0));
	return 1;
}

test_t test_array_append_int_valid()
{
	int i;
	int buf[5] = { 1, 3, 0, 5, 13};
	arr = strap_array_alloc(STRAP_TYPE_INT);
	for (i = 0; i < 5; i++)
		TEST_ASSERT_TRUE(strap_array_append_int(arr, buf[i]));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 5);
	for (i = 0; i < 5; i++)
		TEST_ASSERT_TRUE(strap_array_get_int(arr, i) == buf[i]);
	return 1;
}

test_t test_array_insert_int_null()
{
	TEST_ASSERT_FALSE(strap_array_insert_int(NULL, 0, 0));
	return 1;
}

test_t test_array_insert_int_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_insert_int(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_insert_int(arr, 0, 2));
	TEST_ASSERT_TRUE(strap_array_insert_int(arr, 0, 3));
	TEST_ASSERT_TRUE(strap_array_insert_int(arr, 1, 4));
	TEST_ASSERT_TRUE(strap_array_insert_int(arr, 0, 5));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 5);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 0) == 5);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 1) == 3);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 2) == 4);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 3) == 2);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 4) == 1);
	return 1;
}

test_t test_array_replace_int_null()
{
	TEST_ASSERT_FALSE(strap_array_replace_int(NULL, 0, 23));
	return 1;
}

test_t test_array_replace_int_invalid_index()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_replace_int(arr, 0, 23));
	TEST_ASSERT_TRUE(strap_array_replace_int(arr, 1, 35));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 0);
	return 1;
}

test_t test_array_replace_int_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 5));
	TEST_ASSERT_TRUE(strap_array_replace_int(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_replace_int(arr, 1, 13));
	TEST_ASSERT_TRUE(strap_array_replace_int(arr, 2, 25));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 0) == 1);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 1) == 13);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 2) == 25);
	return 1;
}

test_t test_array_find_int_null()
{
	TEST_ASSERT_TRUE(strap_array_find_int(NULL, 55) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_find_int_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_find_int(arr, 5) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_find_int_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 23));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 15));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 99));
	TEST_ASSERT_TRUE(strap_array_find_int(arr, 5) == 0);
	TEST_ASSERT_TRUE(strap_array_find_int(arr, 15) == 2);
	return 1;
}

test_t test_array_nfind_int_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 1));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 3));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 4));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 99));
	TEST_ASSERT_TRUE(strap_array_nfind_int(arr, 13, 0) == 1);
	TEST_ASSERT_TRUE(strap_array_nfind_int(arr, 13, 1) == 4);
	TEST_ASSERT_TRUE(strap_array_nfind_int(arr, 13, 2) == 5);
	TEST_ASSERT_TRUE(strap_array_nfind_int(arr, 13, 3) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_nfind_int_no_match()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 1));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 2));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 3));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 4));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 6));
	TEST_ASSERT_TRUE(strap_array_nfind_int(arr, 13, 0) == STRAP_NO_MATCH);
	TEST_ASSERT_TRUE(strap_array_nfind_int(arr, 13, 1) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_shrink_int_valid()
{
	int i;
	arr = strap_array_alloc(STRAP_TYPE_INT);
	for (i = 0; i < 128; i++)
		strap_array_append_int(arr, i);
	strap_array_clear(arr);
	TEST_ASSERT_FALSE(strap_array_capacity(arr) == STRAP_INIT_CAPACITY);
	TEST_ASSERT_TRUE(strap_array_shrink(arr));
	TEST_ASSERT_TRUE(strap_array_capacity(arr) == STRAP_INIT_CAPACITY);
	return 1;
}

// ----------------------------------------------------------------------------

test_t test_array_append_short_null()
{
	TEST_ASSERT_FALSE(strap_array_append_short(NULL, 0));
	return 1;
}

test_t test_array_append_short_valid()
{
	int i;
	short buf[5] = { 1, 3, 0, 5, 13 };
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	for (i = 0; i < 5; i++)
		TEST_ASSERT_TRUE(strap_array_append_short(arr, buf[i]));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 5);
	for (i = 0; i < 5; i++)
		TEST_ASSERT_TRUE(strap_array_get_short(arr, i) == buf[i]);
	return 1;
}

test_t test_array_insert_short_null()
{
	TEST_ASSERT_FALSE(strap_array_insert_short(NULL, 0, 0));
	return 1;
}

test_t test_array_insert_short_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	TEST_ASSERT_TRUE(strap_array_insert_short(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_insert_short(arr, 0, 2));
	TEST_ASSERT_TRUE(strap_array_insert_short(arr, 0, 3));
	TEST_ASSERT_TRUE(strap_array_insert_short(arr, 1, 4));
	TEST_ASSERT_TRUE(strap_array_insert_short(arr, 0, 5));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 5);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 0) == 5);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 1) == 3);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 2) == 4);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 3) == 2);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 4) == 1);
	return 1;
}

test_t test_array_replace_short_null()
{
	TEST_ASSERT_FALSE(strap_array_replace_short(NULL, 0, 23));
	return 1;
}

test_t test_array_replace_short_invalid_index()
{
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	TEST_ASSERT_TRUE(strap_array_replace_short(arr, 0, 23));
	TEST_ASSERT_TRUE(strap_array_replace_short(arr, 1, 35));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 0);
	return 1;
}

test_t test_array_replace_short_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 5));
	TEST_ASSERT_TRUE(strap_array_replace_short(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_replace_short(arr, 1, 13));
	TEST_ASSERT_TRUE(strap_array_replace_short(arr, 2, 25));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 0) == 1);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 1) == 13);
	TEST_ASSERT_TRUE(strap_array_get_short(arr, 2) == 25);
	return 1;
}

test_t test_array_find_short_null()
{
	TEST_ASSERT_TRUE(strap_array_find_short(NULL, 55) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_find_short_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	TEST_ASSERT_TRUE(strap_array_find_short(arr, 5) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_find_short_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 23));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 15));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 99));
	TEST_ASSERT_TRUE(strap_array_find_short(arr, 5) == 0);
	TEST_ASSERT_TRUE(strap_array_find_short(arr, 15) == 2);
	return 1;
}

test_t test_array_nfind_short_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 1));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 3));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 4));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 99));
	TEST_ASSERT_TRUE(strap_array_nfind_short(arr, 13, 0) == 1);
	TEST_ASSERT_TRUE(strap_array_nfind_short(arr, 13, 1) == 4);
	TEST_ASSERT_TRUE(strap_array_nfind_short(arr, 13, 2) == 5);
	TEST_ASSERT_TRUE(strap_array_nfind_short(arr, 13, 3) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_nfind_short_no_match()
{
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 1));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 2));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 3));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 4));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_short(arr, 6));
	TEST_ASSERT_TRUE(strap_array_nfind_short(arr, 13, 0) == STRAP_NO_MATCH);
	TEST_ASSERT_TRUE(strap_array_nfind_short(arr, 13, 1) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_shrink_short_valid()
{
	short i;
	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	for (i = 0; i < 127; i++)
		strap_array_append_short(arr, i);
	strap_array_clear(arr);
	TEST_ASSERT_FALSE(strap_array_capacity(arr) == STRAP_INIT_CAPACITY);
	TEST_ASSERT_TRUE(strap_array_shrink(arr));
	TEST_ASSERT_TRUE(strap_array_capacity(arr) == STRAP_INIT_CAPACITY);
	return 1;
}


// ----------------------------------------------------------------------------

test_t test_array_append_long_null()
{
	TEST_ASSERT_FALSE(strap_array_append_long(NULL, 0));
	return 1;
}

test_t test_array_append_long_valid()
{
	int i;
	long buf[5] = { 1, 3, 0, 5, 13 };
	arr = strap_array_alloc(STRAP_TYPE_LONG);
	for (i = 0; i < 5; i++)
		TEST_ASSERT_TRUE(strap_array_append_long(arr, buf[i]));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 5);
	for (i = 0; i < 5; i++)
		TEST_ASSERT_TRUE(strap_array_get_long(arr, i) == buf[i]);
	return 1;
}

test_t test_array_insert_long_null()
{
	TEST_ASSERT_FALSE(strap_array_insert_long(NULL, 0, 0));
	return 1;
}

test_t test_array_insert_long_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_LONG);
	TEST_ASSERT_TRUE(strap_array_insert_long(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_insert_long(arr, 0, 2));
	TEST_ASSERT_TRUE(strap_array_insert_long(arr, 0, 3));
	TEST_ASSERT_TRUE(strap_array_insert_long(arr, 1, 4));
	TEST_ASSERT_TRUE(strap_array_insert_long(arr, 0, 5));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 5);
	TEST_ASSERT_TRUE(strap_array_get_long(arr, 0) == 5);
	TEST_ASSERT_TRUE(strap_array_get_long(arr, 1) == 3);
	TEST_ASSERT_TRUE(strap_array_get_long(arr, 2) == 4);
	TEST_ASSERT_TRUE(strap_array_get_long(arr, 3) == 2);
	TEST_ASSERT_TRUE(strap_array_get_long(arr, 4) == 1);
	return 1;
}

test_t test_array_replace_long_null()
{
	TEST_ASSERT_FALSE(strap_array_replace_long(NULL, 0, 23));
	return 1;
}

test_t test_array_replace_long_invalid_index()
{
	arr = strap_array_alloc(STRAP_TYPE_LONG);
	TEST_ASSERT_TRUE(strap_array_replace_long(arr, 0, 23));
	TEST_ASSERT_TRUE(strap_array_replace_long(arr, 1, 35));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 0);
	return 1;
}

test_t test_array_replace_long_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_LONG);
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 5));
	TEST_ASSERT_TRUE(strap_array_replace_long(arr, 0, 1));
	TEST_ASSERT_TRUE(strap_array_replace_long(arr, 1, 13));
	TEST_ASSERT_TRUE(strap_array_replace_long(arr, 2, 25));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(strap_array_get_long(arr, 0) == 1);
	TEST_ASSERT_TRUE(strap_array_get_long(arr, 1) == 13);
	TEST_ASSERT_TRUE(strap_array_get_long(arr, 2) == 25);
	return 1;
}

test_t test_array_find_long_null()
{
	TEST_ASSERT_TRUE(strap_array_find_long(NULL, 55) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_find_long_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_LONG);
	TEST_ASSERT_TRUE(strap_array_find_long(arr, 5) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_find_long_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_LONG);
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 23));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 15));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 99));
	TEST_ASSERT_TRUE(strap_array_find_long(arr, 5) == 0);
	TEST_ASSERT_TRUE(strap_array_find_long(arr, 15) == 2);
	return 1;
}

test_t test_array_nfind_long_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_LONG);
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 1));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 3));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 4));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 99));
	TEST_ASSERT_TRUE(strap_array_nfind_long(arr, 13, 0) == 1);
	TEST_ASSERT_TRUE(strap_array_nfind_long(arr, 13, 1) == 4);
	TEST_ASSERT_TRUE(strap_array_nfind_long(arr, 13, 2) == 5);
	TEST_ASSERT_TRUE(strap_array_nfind_long(arr, 13, 3) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_nfind_long_no_match()
{
	arr = strap_array_alloc(STRAP_TYPE_LONG);
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 1));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 2));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 3));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 4));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_long(arr, 6));
	TEST_ASSERT_TRUE(strap_array_nfind_long(arr, 13, 0) == STRAP_NO_MATCH);
	TEST_ASSERT_TRUE(strap_array_nfind_long(arr, 13, 1) == STRAP_NO_MATCH);
	return 1;
}

test_t test_array_shrink_long_valid()
{
	long i;
	arr = strap_array_alloc(STRAP_TYPE_LONG);
	for (i = 0; i < 127; i++)
		strap_array_append_long(arr, i);
	strap_array_clear(arr);
	TEST_ASSERT_FALSE(strap_array_capacity(arr) == STRAP_INIT_CAPACITY);
	TEST_ASSERT_TRUE(strap_array_shrink(arr));
	TEST_ASSERT_TRUE(strap_array_capacity(arr) == STRAP_INIT_CAPACITY);
	return 1;
}

// ----------------------------------------------------------------------------

test_t test_array_append_float_null()
{
	TEST_ASSERT_TRUE(!strap_array_append_float(NULL, 0));
	return 1;
}

test_t test_array_append_float_valid()
{
	int i;
	float buf[5] = { 1.5, 3.3, 0.01, 5., 13.};
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	for (i = 0; i < 5; i++)
		TEST_ASSERT_TRUE(strap_array_append_float(arr, buf[i]));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 5);
	for (i = 0; i < 5; i++)
		TEST_ASSERT_TRUE(FLOAT_EQUALS(strap_array_get_float(arr, i), buf[i]));
	return 1;
}

test_t test_array_insert_float_null()
{
	return 2;
}

test_t test_array_insert_float_valid()
{
	return 2;
}

test_t test_array_replace_float_null()
{
	return 2;
}

test_t test_array_replace_float_invalid_index()
{
	return 2;
}

test_t test_array_replace_float_valid()
{
	return 2;
}

test_t test_array_find_float_null()
{
	return 2;
}

test_t test_array_find_float_empty()
{
	return 2;
}

test_t test_array_find_float_valid()
{
	return 2;
}

test_t test_array_nfind_float_valid()
{
	return 2;
}

test_t test_array_nfind_float_no_match()
{
	return 2;
}


// --------------------------------------------------------------

test_t test_array_sum_int_null()
{
	TEST_ASSERT_FALSE(strap_array_sum_int(NULL));
	return 1;
}

test_t test_array_sum_long_null()
{
	TEST_ASSERT_FALSE(strap_array_sum_long(NULL));
	return 1;
}

test_t test_array_sum_uint_null()
{
	TEST_ASSERT_FALSE(strap_array_sum_uint(NULL));
	return 1;
}

test_t test_array_sum_ulong_null()
{
	TEST_ASSERT_FALSE(strap_array_sum_ulong(NULL));
	return 1;
}

test_t test_array_sum_double_null()
{
	TEST_ASSERT_FALSE(strap_array_sum_double(NULL));
	return 1;
}

test_t test_array_sum_longdouble_null()
{
	TEST_ASSERT_FALSE(strap_array_sum_longdouble(NULL));
	return 1;
}

test_t test_array_sum_int_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_FALSE(strap_array_sum_int(arr));
	return 1;
}

test_t test_array_sum_long_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_FALSE(strap_array_sum_long(arr));
	return 1;
}

test_t test_array_sum_uint_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_FALSE(strap_array_sum_uint(arr));
	return 1;
}

test_t test_array_sum_ulong_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_FALSE(strap_array_sum_ulong(arr));
	return 1;
}

test_t test_array_sum_double_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	TEST_ASSERT_FALSE(strap_array_sum_double(arr));
	return 1;
}

test_t test_array_sum_longdouble_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	TEST_ASSERT_FALSE(strap_array_sum_longdouble(arr));
	return 1;
}

test_t test_array_sum_int_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	strap_array_append_int(arr, 125);
	strap_array_append_int(arr, -28);
	strap_array_append_int(arr, 1983);
	TEST_ASSERT_TRUE(strap_array_sum_int(arr) == 2080);
	return 1;
}

test_t test_array_sum_long_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	strap_array_append_int(arr, 125);
	strap_array_append_int(arr, -28);
	strap_array_append_int(arr, 1983);
	TEST_ASSERT_TRUE(strap_array_sum_long(arr) == 2080);
	return 1;
}

test_t test_array_sum_uint_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	strap_array_append_int(arr, 125);
	strap_array_append_int(arr, -28);
	strap_array_append_int(arr, 1983);
	TEST_ASSERT_TRUE(strap_array_sum_uint(arr) == 2080);
	return 1;
}

test_t test_array_sum_ulong_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	strap_array_append_int(arr, 125);
	strap_array_append_int(arr, -28);
	strap_array_append_int(arr, 1983);
	TEST_ASSERT_TRUE(strap_array_sum_ulong(arr) == 2080);
	return 1;
}

test_t test_array_sum_double_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	strap_array_append_float(arr, 0.03);
	strap_array_append_float(arr, 28);
	strap_array_append_float(arr, 0.1008);
	TEST_ASSERT_TRUE(FLOAT_EQUALS(strap_array_sum_double(arr), 28.1308));
	return 1;
}

test_t test_array_sum_longdouble_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	strap_array_append_float(arr, 0.03);
	strap_array_append_float(arr, 28);
	strap_array_append_float(arr, 0.1008);
	TEST_ASSERT_TRUE(DOUBLE_EQUALS(strap_array_sum_longdouble(arr), 28.1308));
	return 1;
}


test_t test_array_erase_null()
{
	TEST_ASSERT_FALSE(strap_array_erase(NULL, 0));
	return 1;
}

test_t test_array_erase_str_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_erase(arr, 0));
	return 1;
}

test_t test_array_erase_str_invalid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "first"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_erase(arr, -1));
	TEST_ASSERT_TRUE(strap_array_erase(arr, 5));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	return 1;
}

test_t test_array_erase_str_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "first"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_erase(arr, 1));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 2);
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 0), "first"));
	TEST_ASSERT_TRUE(!strcmp(strap_array_get_cstr(arr, 1), "third"));
	return 1;
}

test_t test_array_erase_int_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_erase(arr, 0));
	return 1;
}

test_t test_array_erase_int_invalid()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 5));
	TEST_ASSERT_TRUE(strap_array_erase(arr, -1));
	TEST_ASSERT_TRUE(strap_array_erase(arr, 5));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	return 1;
}

test_t test_array_erase_int_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_INT);
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 10));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_int(arr, 24));
	TEST_ASSERT_TRUE(strap_array_erase(arr, 1));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 2);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 0) == 10);
	TEST_ASSERT_TRUE(strap_array_get_int(arr, 1) == 24);
	return 1;
}

test_t test_array_erase_float_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	TEST_ASSERT_TRUE(strap_array_erase(arr, 0));
	return 1;
}

test_t test_array_erase_float_invalid()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 5));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 5));
	TEST_ASSERT_TRUE(strap_array_erase(arr, -1));
	TEST_ASSERT_TRUE(strap_array_erase(arr, 5));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	return 1;
}

test_t test_array_erase_float_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 10));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 13));
	TEST_ASSERT_TRUE(strap_array_append_float(arr, 24));
	TEST_ASSERT_TRUE(strap_array_erase(arr, 1));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 2);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 0) == 10);
	TEST_ASSERT_TRUE(strap_array_get_float(arr, 1) == 24);
	return 1;
}

test_t test_array_sprintf_null()
{
	char buf[16];

	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_sprintf(NULL, NULL) == -1);
	TEST_ASSERT_TRUE(strap_array_sprintf(NULL, buf)  == -1);
	TEST_ASSERT_TRUE(strap_array_sprintf(arr, NULL)  == -1);
	return 1;
}

test_t test_array_sprintf_empty()
{
	char buf[16];
	const char *expected_str =  "[]";

	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_sprintf(arr, buf) == (int) strlen(expected_str));
	TEST_ASSERT_TRUE(!strcmp(buf, expected_str));
	return 1;
}

test_t test_array_sprintf_str()
{
	char buf[32];
	const char *expected_str =  "[\"sprintf\", \"testing\", \"str\"]";

	arr = strap_array_alloc(STRAP_TYPE_STRING);
	strap_array_append_cstr(arr, "sprintf");
	strap_array_append_cstr(arr, "testing");
	strap_array_append_cstr(arr, "str");
	TEST_ASSERT_TRUE(strap_array_sprintf(arr, buf) == (int) strlen(expected_str));
	TEST_ASSERT_TRUE(!strcmp(buf, expected_str));
	return 1;
}

test_t test_array_sprintf_char()
{
	char buf[32];
	const char *expected_str =  "[13, 24, 122]";

	arr = strap_array_alloc(STRAP_TYPE_CHAR);
	strap_array_append_char(arr, 13);
	strap_array_append_char(arr, 24);
	strap_array_append_char(arr, 122);
	TEST_ASSERT_TRUE(strap_array_sprintf(arr, buf) == (int) strlen(expected_str));
	TEST_ASSERT_TRUE(!strcmp(buf, expected_str));
	return 1;
}

test_t test_array_sprintf_short()
{
	char buf[32];
	const char *expected_str =  "[13, 24, 122]";

	arr = strap_array_alloc(STRAP_TYPE_SHORT);
	strap_array_append_short(arr, 13);
	strap_array_append_short(arr, 24);
	strap_array_append_short(arr, 122);
	TEST_ASSERT_TRUE(strap_array_sprintf(arr, buf) == (int) strlen(expected_str));
	TEST_ASSERT_TRUE(!strcmp(buf, expected_str));
	return 1;
}

test_t test_array_sprintf_int()
{
	char buf[32];
	const char *expected_str =  "[13, 24, 137]";

	arr = strap_array_alloc(STRAP_TYPE_INT);
	strap_array_append_int(arr, 13);
	strap_array_append_int(arr, 24);
	strap_array_append_int(arr, 137);
	TEST_ASSERT_TRUE(strap_array_sprintf(arr, buf) == (int) strlen(expected_str));
	TEST_ASSERT_TRUE(!strcmp(buf, expected_str));
	return 1;
}

test_t test_array_sprintf_long()
{
	char buf[32];
	const char *expected_str =  "[13, 24, 137]";

	arr = strap_array_alloc(STRAP_TYPE_LONG);
	strap_array_append_long(arr, 13);
	strap_array_append_long(arr, 24);
	strap_array_append_long(arr, 137);
	TEST_ASSERT_TRUE(strap_array_sprintf(arr, buf) == (int) strlen(expected_str));
	TEST_ASSERT_TRUE(!strcmp(buf, expected_str));
	return 1;
}

test_t test_array_sprintf_float()
{
	char buf[32];
	const char *expected_str =  "[0.005, 1.3, 137]";

	arr = strap_array_alloc(STRAP_TYPE_FLOAT);
	strap_array_append_float(arr, 0.005);
	strap_array_append_float(arr, 1.3);
	strap_array_append_float(arr, 137);
	TEST_ASSERT_TRUE(strap_array_sprintf(arr, buf) == (int) strlen(expected_str));
	TEST_ASSERT_TRUE(!strcmp(buf, expected_str));
	return 1;
}

test_t test_array_sprintf_double()
{
	char buf[32];
	const char *expected_str =  "[0.005, 1.3, 137]";

	arr = strap_array_alloc(STRAP_TYPE_DOUBLE);
	strap_array_append_double(arr, 0.005);
	strap_array_append_double(arr, 1.3);
	strap_array_append_double(arr, 137);
	TEST_ASSERT_TRUE(strap_array_sprintf(arr, buf) == (int) strlen(expected_str));
	TEST_ASSERT_TRUE(!strcmp(buf, expected_str));
	return 1;
}

test_t test_array_sprintf_longdouble()
{
	char buf[32];
	const char *expected_str = "[0.005, 1.3, 137]";

	arr = strap_array_alloc(STRAP_TYPE_LONG_DOUBLE);
	strap_array_append_longdouble(arr, 0.005);
	strap_array_append_longdouble(arr, 1.3);
	strap_array_append_longdouble(arr, 137);
	TEST_ASSERT_TRUE(strap_array_sprintf(arr, buf) == (int) strlen(expected_str));
	TEST_ASSERT_TRUE(!strcmp(buf, expected_str));
	return 1;
}







void test_prep()
{
}

void test_cleanup()
{
	strap_string_free(string);
	strap_string_free(string2);
	strap_string_free(string3);
	strap_string_free(string4);
	strap_array_free(arr);
	strap_array_free(arr2);
	string  = NULL;
	string2 = NULL;
	string3 = NULL;
	string4 = NULL;
	arr = NULL;
	arr2 = NULL;
}

int main ()
{
	int res;

	pass_count = 0;
	test_count = 0;
	setbuf(stdout, NULL);
	TEST_BEFORE_EACH(test_prep);
	TEST_AFTER_EACH(test_cleanup);
	puts_h("-- TEST StrapString --");

	TEST_RUN(test_string_alloc_null);
	TEST_RUN(test_string_alloc_empty);
	TEST_RUN(test_string_alloc_string);
	TEST_RUN(test_string_alloc_array);
	TEST_RUN(test_string_alloc_large);

	TEST_RUN(test_string_clone_null);
	TEST_RUN(test_string_clone_empty);
	TEST_RUN(test_string_clone_string);
	TEST_RUN(test_string_clone_large);

	TEST_RUN(test_string_copy_null);
	TEST_RUN(test_string_copy_empty);
	TEST_RUN(test_string_copy_string);
	TEST_RUN(test_string_copy_large);
	TEST_RUN(test_string_ncopy_string);

	TEST_RUN(test_string_concat_null);
	TEST_RUN(test_string_concat_empty);
	TEST_RUN(test_string_concat_string);
	TEST_RUN(test_string_concat_large);
	TEST_RUN(test_string_nconcat_string);

	TEST_RUN(test_string_copy_from_null);
	TEST_RUN(test_string_copy_from_empty);
	TEST_RUN(test_string_copy_from_string);
	TEST_RUN(test_string_copy_from_large);
	TEST_RUN(test_string_ncopy_from_string);

	TEST_RUN(test_string_copy_to_null);
	TEST_RUN(test_string_copy_to_empty);
	TEST_RUN(test_string_copy_to_string);
	TEST_RUN(test_string_ncopy_to_string);

	TEST_RUN(test_string_strcat_null);
	TEST_RUN(test_string_strcat_empty);
	TEST_RUN(test_string_strcat_string);
	TEST_RUN(test_string_strcat_large);
	TEST_RUN(test_string_nstrcat_string);

	TEST_RUN(test_string_substring_null);
	TEST_RUN(test_string_substring_invalid);
	TEST_RUN(test_string_substring_valid);

	TEST_RUN(test_string_erase_null);
	TEST_RUN(test_string_erase_invalid);
	TEST_RUN(test_string_erase_valid);

	TEST_RUN(test_string_trim_null);
	TEST_RUN(test_string_trim_valid);

	TEST_RUN(test_string_compare_null);
	TEST_RUN(test_string_compare_nonequal);
	TEST_RUN(test_string_compare_equal);

	TEST_RUN(test_string_find_null);
	TEST_RUN(test_string_find_no_match);
	TEST_RUN(test_string_find_match);

	TEST_RUN(test_string_nfind_match);
	TEST_RUN(test_string_nfind_no_match);

	TEST_RUN(test_string_split_null);
	TEST_RUN(test_string_split_empty);
	TEST_RUN(test_string_split_comma);
	TEST_RUN(test_string_split_word);

	TEST_RUN(test_string_reverse_null);
	TEST_RUN(test_string_reverse_valid);

	TEST_RUN(test_string_shrink_null);
	TEST_RUN(test_string_shrink_empty);
	TEST_RUN(test_string_shrink_valid);

	/* ARRAY cstr */
	puts_h("\n-- TEST StrapArray (cstr) --");

	TEST_RUN(test_array_clone_null);
	TEST_RUN(test_array_clone_string_valid);
	TEST_RUN(test_array_clone_valid_int);

	TEST_RUN(test_array_create_string_null);
	TEST_RUN(test_array_create_string_invalid);
	TEST_RUN(test_array_create_string_valid);


	TEST_RUN(test_array_append_cstr_null);
	TEST_RUN(test_array_append_cstr_empty);
	TEST_RUN(test_array_append_cstr_valid);
	TEST_RUN(test_array_append_cstr_large_string);
	TEST_RUN(test_array_append_cstr_large_count);

	TEST_RUN(test_array_append_string_null);
	TEST_RUN(test_array_append_string_valid);

	TEST_RUN(test_array_insert_cstr_null);
	TEST_RUN(test_array_insert_cstr_empty);
	TEST_RUN(test_array_insert_cstr_valid);
	TEST_RUN(test_array_insert_cstr_large);

	TEST_RUN(test_array_insert_string_null);
	TEST_RUN(test_array_insert_string_valid);


	TEST_RUN(test_array_replace_cstr_null);
	TEST_RUN(test_array_replace_cstr_empty);
	TEST_RUN(test_array_replace_cstr_valid);
	TEST_RUN(test_array_replace_cstr_equal_length);
	TEST_RUN(test_array_replace_cstr_large);

	TEST_RUN(test_array_replace_string_null);
	TEST_RUN(test_array_replace_string_valid);


	TEST_RUN(test_array_find_cstr_null);
	TEST_RUN(test_array_find_cstr_empty);
	TEST_RUN(test_array_find_cstr_valid);

	TEST_RUN(test_array_find_string_null);
	TEST_RUN(test_array_find_string_empty);


	TEST_RUN(test_array_nfind_cstr_null);
	TEST_RUN(test_array_nfind_cstr_valid);
	TEST_RUN(test_array_nfind_cstr_no_match);

	TEST_RUN(test_array_nfind_string_null);
	TEST_RUN(test_array_nfind_string_valid);


	/* ARRAY char */
	puts_h("\n-- TEST StrapArray (char) --");

	TEST_RUN(test_array_append_char_null);
	TEST_RUN(test_array_append_char_valid);

	TEST_RUN(test_array_insert_char_null);
	TEST_RUN(test_array_insert_char_valid);

	TEST_RUN(test_array_replace_char_null);
	TEST_RUN(test_array_replace_char_invalid_index);
	TEST_RUN(test_array_replace_char_valid);

	TEST_RUN(test_array_find_char_null);
	TEST_RUN(test_array_find_char_empty);
	TEST_RUN(test_array_find_char_valid);

	TEST_RUN(test_array_nfind_char_valid);
	TEST_RUN(test_array_nfind_char_no_match);

	/* ARRAY short */
	puts_h("\n-- TEST StrapArray (short) --");

	TEST_RUN(test_array_append_short_null);
	TEST_RUN(test_array_append_short_valid);

	TEST_RUN(test_array_insert_short_null);
	TEST_RUN(test_array_insert_short_valid);

	TEST_RUN(test_array_replace_short_null);
	TEST_RUN(test_array_replace_short_invalid_index);
	TEST_RUN(test_array_replace_short_valid);

	TEST_RUN(test_array_find_short_null);
	TEST_RUN(test_array_find_short_empty);
	TEST_RUN(test_array_find_short_valid);

	TEST_RUN(test_array_nfind_short_valid);
	TEST_RUN(test_array_nfind_short_no_match);

	/* ARRAY int */
	puts_h("\n-- TEST StrapArray (int) --");

	TEST_RUN(test_array_append_int_null);
	TEST_RUN(test_array_append_int_valid);

	TEST_RUN(test_array_insert_int_null);
	TEST_RUN(test_array_insert_int_valid);

	TEST_RUN(test_array_replace_int_null);
	TEST_RUN(test_array_replace_int_invalid_index);
	TEST_RUN(test_array_replace_int_valid);

	TEST_RUN(test_array_find_int_null);
	TEST_RUN(test_array_find_int_empty);
	TEST_RUN(test_array_find_int_valid);

	TEST_RUN(test_array_nfind_int_valid);
	TEST_RUN(test_array_nfind_int_no_match);

	/* ARRAY long */
	puts_h("\n-- TEST StrapArray (long) --");

	TEST_RUN(test_array_append_long_null);
	TEST_RUN(test_array_append_long_valid);

	TEST_RUN(test_array_insert_long_null);
	TEST_RUN(test_array_insert_long_valid);

	TEST_RUN(test_array_replace_long_null);
	TEST_RUN(test_array_replace_long_invalid_index);
	TEST_RUN(test_array_replace_long_valid);

	TEST_RUN(test_array_find_long_null);
	TEST_RUN(test_array_find_long_empty);
	TEST_RUN(test_array_find_long_valid);

	TEST_RUN(test_array_nfind_long_valid);
	TEST_RUN(test_array_nfind_long_no_match);

	/* ARRAY float */
	puts_h("\n-- TEST StrapArray (float) --");

	TEST_RUN(test_array_append_float_null);
	TEST_RUN(test_array_append_float_valid);

	TEST_RUN(test_array_insert_float_null);
	TEST_RUN(test_array_insert_float_valid);

	TEST_RUN(test_array_replace_float_null);
	TEST_RUN(test_array_replace_float_invalid_index);
	TEST_RUN(test_array_replace_float_valid);

	TEST_RUN(test_array_find_float_null);
	TEST_RUN(test_array_find_float_empty);
	TEST_RUN(test_array_find_float_valid);

	TEST_RUN(test_array_nfind_float_valid);
	TEST_RUN(test_array_nfind_float_no_match);

	/* ARRAY general */
	puts_h("\n-- TEST StrapArray (general) --");

	TEST_RUN(test_array_sum_int_null);
	TEST_RUN(test_array_sum_long_null);
	TEST_RUN(test_array_sum_uint_null);
	TEST_RUN(test_array_sum_ulong_null);
	TEST_RUN(test_array_sum_double_null);
	TEST_RUN(test_array_sum_longdouble_null);

	TEST_RUN(test_array_sum_int_empty);
	TEST_RUN(test_array_sum_long_empty);
	TEST_RUN(test_array_sum_uint_empty);
	TEST_RUN(test_array_sum_ulong_empty);
	TEST_RUN(test_array_sum_double_empty);
	TEST_RUN(test_array_sum_longdouble_empty);

	TEST_RUN(test_array_sum_int_valid);
	TEST_RUN(test_array_sum_long_valid);
	TEST_RUN(test_array_sum_uint_valid);
	TEST_RUN(test_array_sum_ulong_valid);
	TEST_RUN(test_array_sum_double_valid);
	TEST_RUN(test_array_sum_longdouble_valid);


	TEST_RUN(test_array_erase_null);
	TEST_RUN(test_array_erase_str_empty);
	TEST_RUN(test_array_erase_str_invalid);
	TEST_RUN(test_array_erase_str_valid);

	TEST_RUN(test_array_erase_int_empty);
	TEST_RUN(test_array_erase_int_invalid);
	TEST_RUN(test_array_erase_int_valid);

	TEST_RUN(test_array_erase_float_empty);
	TEST_RUN(test_array_erase_float_invalid);
	TEST_RUN(test_array_erase_float_valid);


	TEST_RUN(test_array_erase_range_str_first_element);
	TEST_RUN(test_array_erase_range_str_last_element);
	TEST_RUN(test_array_erase_range_str_valid);

	TEST_RUN(test_array_erase_range_int_first_element);
	TEST_RUN(test_array_erase_range_int_last_element);
	TEST_RUN(test_array_erase_range_int_valid);

	TEST_RUN(test_array_erase_range_float_first_element);
	TEST_RUN(test_array_erase_range_float_last_element);
	TEST_RUN(test_array_erase_range_float_valid);


	TEST_RUN(test_array_reverse_null);
	TEST_RUN(test_array_reverse_str_empty);
	TEST_RUN(test_array_reverse_str_valid_odd);
	TEST_RUN(test_array_reverse_str_valid_even);

	TEST_RUN(test_array_reverse_short_empty);
	TEST_RUN(test_array_reverse_short_valid_odd);
	TEST_RUN(test_array_reverse_short_valid_even);

	TEST_RUN(test_array_reverse_int_empty);
	TEST_RUN(test_array_reverse_int_valid_odd);
	TEST_RUN(test_array_reverse_int_valid_even);

	TEST_RUN(test_array_reverse_float_empty);
	TEST_RUN(test_array_reverse_float_valid_odd);
	TEST_RUN(test_array_reverse_float_valid_even);

	TEST_RUN(test_array_reverse_longdouble_empty);
	TEST_RUN(test_array_reverse_longdouble_valid_odd);
	TEST_RUN(test_array_reverse_longdouble_valid_even);

	// TEST_RUN(test_array_clear_null);
	// TEST_RUN(test_array_clear_str_valid);
	//

	// TEST_RUN(test_array_create_subarray_null);
	// TEST_RUN(test_array_create_subarray_str_invalid);
	// TEST_RUN(test_array_create_subarray_str_valid);
	// TEST_RUN(test_array_create_subarray_str_first_element);
	// TEST_RUN(test_array_create_subarray_str_large_n);
	//
	// TEST_RUN(test_array_sort_null);
	// TEST_RUN(test_array_sort_str_empty);
	// TEST_RUN(test_array_sort_str_ascending);
	// TEST_RUN(test_array_sort_str_descending);
	//


	// TEST_RUN(test_array_shrink_null);
	// TEST_RUN(test_array_shrink_str_empty);
	// TEST_RUN(test_array_shrink_str_valid);

	TEST_RUN(test_array_sprintf_null);
	TEST_RUN(test_array_sprintf_empty);

	TEST_RUN(test_array_sprintf_str);
	TEST_RUN(test_array_sprintf_char);
	TEST_RUN(test_array_sprintf_short);
	TEST_RUN(test_array_sprintf_int);
	TEST_RUN(test_array_sprintf_long);
	TEST_RUN(test_array_sprintf_float);
	TEST_RUN(test_array_sprintf_double);
	TEST_RUN(test_array_sprintf_longdouble);

	puts("---------------------------------");
	printf("%d Tests, %d Passed, %d Failed\n", test_count, pass_count,
				test_count - pass_count);
	return !(pass_count == test_count);
}
