#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strap.h"

#define ERROR_MESSAGE_SIZE 512

#define TEST_BEFORE_EACH(a) test_before_each = a
#define TEST_AFTER_EACH(a) test_after_each = a

#define TEST_RUN(a)              \
do {                             \
	test_before_each();            \
	printf("%s: ", #a);            \
	if ((a())) {                   \
		printf("PASS\n");            \
		pass_count++;                \
	}                              \
	else {                         \
		printf("FAILED\n");          \
		printf("%s", error_message); \
	}                              \
	test_count++;                  \
	test_after_each();             \
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

static int pass_count;
static int test_count;
static char error_message[ERROR_MESSAGE_SIZE];
static void (*test_before_each)(void);
static void (*test_after_each)(void);

void test_prep();
void test_cleanup();

StrapArray *arr;
StrapString *string, *string2, *string3, *string4;

int test_string_alloc_null()
{
	string = strap_string_alloc(NULL);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

int test_string_alloc_empty()
{
	string = strap_string_alloc("");
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

int test_string_alloc_string()
{
	char str[] = "string";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), str) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(str));
	return 1;
}

int test_string_alloc_array()
{
	size_t size = 8;
	char buf[size];

	memset(buf, 'a', size);
	buf[7] = '\0'; /* always required for c strings */

	string = strap_string_alloc(buf);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "aaaaaaa") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == size - 1);
	return 1;
}

int test_string_alloc_large()
{
	size_t size = 9999;
	char buf[size];
	memset(buf, 'a', size);
	buf[size - 1] = 0;
	string = strap_string_alloc(buf);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), buf) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == size - 1);
	return 1;
}

int test_string_clone_null()
{
	string = strap_string_clone(NULL);
	TEST_ASSERT_TRUE(!string);
	return 1;
}

int test_string_clone_empty()
{
	string2 = strap_string_alloc("");
	string = strap_string_clone(string2);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == strap_string_length(string2));
	return 1;
}

int test_string_clone_string()
{
	string2 = strap_string_alloc("first");
	string = strap_string_clone(string2);
	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "first") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == strap_string_length(string2));
	return 1;
}


int test_string_clone_large()
{
	size_t size = 9999;
	char buf[size];

	memset(buf, 'a', size);
	buf[size - 1] = 0;
	string2 = strap_string_alloc(buf);
	string = strap_string_clone(string2);

	TEST_ASSERT_TRUE(string);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), buf) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == size - 1);
	return 1;
}
int test_string_copy_null()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_copy(string, NULL));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

int test_string_copy_empty()
{
	string = strap_string_alloc("first");
	string2 = strap_string_alloc(NULL);
	TEST_ASSERT_TRUE(strap_string_copy(string, string2));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

int test_string_copy_string()
{
	string = strap_string_alloc("first");
	string2 = strap_string_alloc("second");
	TEST_ASSERT_TRUE(strap_string_copy(string, string2));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "second") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 6);
	return 1;
}

int test_string_copy_large()
{
	size_t size = 9999;
	char buf[size];
	memset(buf, 'a', size);
	buf[size - 1] = 0;
	string = strap_string_alloc("first");
	string2 = strap_string_alloc(buf);
	TEST_ASSERT_TRUE(strap_string_copy(string, string2));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), buf) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == size - 1);
	return 1;
}

int test_string_ncopy_string()
{
	string = strap_string_alloc("first");
	string2 = strap_string_alloc("second");
	TEST_ASSERT_TRUE(strap_string_ncopy(string, string2, 3));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "sec") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 3);
	return 1;
}

int test_string_concat_null()
{
	char str[] = "str";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(strap_string_concat(string, NULL));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), str) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(str));
	return 1;
}

int test_string_concat_empty()
{
	char str[] = "first";
	string = strap_string_alloc(str);
	string2 = strap_string_alloc(NULL);
	TEST_ASSERT_TRUE(strap_string_concat(string, string2));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), str) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(str));
	return 1;
}

int test_string_concat_string()
{
	string = strap_string_alloc("first");
	string2 = strap_string_alloc("second");
	char catstr[] = "firstsecond";
	TEST_ASSERT_TRUE(strap_string_concat(string, string2));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), catstr) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(catstr));
	return 1;
}

int test_string_concat_large()
{
	size_t size = 9994;
	size_t catsize = 9999;
	char buf[] = "first";
	char buf2[size];
	char buf3[catsize];
	memset(buf2, 'a', size);
	memset(buf3, 'a', catsize);
	buf2[size - 1] = 0;
	buf3[catsize - 1] = 0;
	memcpy(buf3, buf, 5);
	string = strap_string_alloc(buf);
	string2 = strap_string_alloc(buf2);
	TEST_ASSERT_TRUE(strap_string_concat(string, string2));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), buf3) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == catsize - 1);
	return 1;
}

int test_string_nconcat_string()
{
	string = strap_string_alloc("first");
	string2 = strap_string_alloc("second");
	char catstr[] = "firstsec";
	TEST_ASSERT_TRUE(strap_string_nconcat(string, string2, 3));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), catstr) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(catstr));
	return 1;
}

int test_string_copy_from_null()
{
	char str[] = "first";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(!strap_string_copy_from(NULL, NULL));
	TEST_ASSERT_TRUE(strap_string_copy_from(string, NULL));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

int test_string_copy_from_empty()
{
	char str[] = "first";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(strap_string_copy_from(string, ""));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 0);
	return 1;
}

int test_string_copy_from_string()
{
	char str[] = "first";
	char str2[] = "second";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(strap_string_copy_from(string, str2));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), str2) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == strlen(str2));
	return 1;
}

int test_string_copy_from_large()
{
	size_t size = 9999;
	char str[] = "first";
	char str2[size];

	string = strap_string_alloc(str);
	memset(str2, 'a', size);
	str2[size - 1] = 0;

	TEST_ASSERT_TRUE(strap_string_copy_from(string, str2));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), str2) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == size - 1);
	return 1;
}

int test_string_ncopy_from_string()
{
	char str[] = "first";
	char str2[] = "second";
	string = strap_string_alloc(str);
	TEST_ASSERT_TRUE(strap_string_ncopy_from(string, str2, 3));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "sec") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 3);
	return 1;
}

int test_string_copy_to_null()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(!strap_string_copy_to(NULL, NULL));
	TEST_ASSERT_TRUE(!strap_string_copy_to(string, NULL));
	return 1;
}

int test_string_copy_to_empty()
{
	char str[16];
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_copy_to(string, str));
	TEST_ASSERT_TRUE(strcmp(str, "str") == 0);
	TEST_ASSERT_TRUE(strlen(str) == 3);
	return 1;
}

int test_string_copy_to_string()
{
	char str[16] = "first";
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_copy_to(string, str));
	TEST_ASSERT_TRUE(strcmp(str, "str") == 0);
	TEST_ASSERT_TRUE(strlen(str) == 3);
	return 1;
}

int test_string_ncopy_to_string()
{
	char str[16] = "first";
	string = strap_string_alloc("second");
	TEST_ASSERT_TRUE(strap_string_ncopy_to(string, str, 3));
	TEST_ASSERT_TRUE(strcmp(str, "sec") == 0);
	TEST_ASSERT_TRUE(strlen(str) == 3);
	return 1;
}

int test_string_strcat_null()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(!strap_string_strcat(NULL, NULL));
	TEST_ASSERT_TRUE(strap_string_strcat(string, NULL));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "str") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 3);
	return 1;
}

int test_string_strcat_empty()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_strcat(string, ""));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "str") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 3);
	return 1;
}

int test_string_strcat_string()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_strcat(string, "ing"));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "string") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 6);
	return 1;
}

int test_string_strcat_large()
{
	size_t size = 9996;
	size_t catsize = 9999;
	char buf[size];
	char buf2[catsize];
	memset(buf, 'a', size);
	memset(buf2, 'a', catsize);
	buf[size - 1] = 0;
	buf2[catsize - 1] = 0;
	memcpy(buf2, "str", 3);

	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_strcat(string, buf));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), buf2) == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == catsize - 1);
	return 1;
}

int test_string_nstrcat_string()
{
	string = strap_string_alloc("first");
	TEST_ASSERT_TRUE(strap_string_nstrcat(string, "second", 3));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "firstsec") == 0);
	TEST_ASSERT_TRUE(strap_string_length(string) == 8);
	return 1;
}

int test_string_substring_invalid()
{
	string = strap_string_alloc("first");
	TEST_ASSERT_TRUE(string2 = strap_string_create_substring(string, 10, 10));
	TEST_ASSERT_TRUE(string3 = strap_string_create_substring(string, 0, 0));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string2), "") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string3), "") == 0);
	return 1;
}

int test_string_substring_valid()
{
	string = strap_string_alloc("motherhood");
	TEST_ASSERT_TRUE(string2 = strap_string_create_substring(string, 1, 5));
	TEST_ASSERT_TRUE(string3 = strap_string_create_substring(string, 6, 999));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string2), "other") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string3), "hood") == 0);
	return 1;
}

int test_string_erase_invalid()
{
	char buf[] = "first";
	string = strap_string_alloc(buf);
	TEST_ASSERT_TRUE(!strap_string_erase(NULL, 5, 10));
	TEST_ASSERT_TRUE(strap_string_erase(string, 10, 10));
	TEST_ASSERT_TRUE(strap_string_erase(string, 0, 0));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), buf) == 0);
	return 1;
}

int test_string_erase_valid()
{
	string = strap_string_alloc("string motherhood");
	TEST_ASSERT_TRUE(strap_string_erase(string, 0, 7));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "motherhood") == 0);
	TEST_ASSERT_TRUE(strap_string_erase(string, 6, 10));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "mother") == 0);
	return 1;
}

int test_string_trim()
{
	string = strap_string_alloc("    this string           ");
	string2 = strap_string_alloc("nospace");
	string3 = strap_string_alloc(" ");
	string4 = strap_string_alloc("");
	TEST_ASSERT_TRUE(strap_string_trim(string));
	TEST_ASSERT_TRUE(strap_string_trim(string2));
	TEST_ASSERT_TRUE(strap_string_trim(string3));
	TEST_ASSERT_TRUE(strap_string_trim(string4));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "this string") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string2), "nospace") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string4), "") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string3), "") == 0);
	return 1;
}

int test_string_compare_null()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_compare(NULL, NULL) == 0);
	TEST_ASSERT_TRUE(strap_string_compare(string, NULL) == 1);
	TEST_ASSERT_TRUE(strap_string_compare(NULL, string) == -1);
	return 1;
}

int test_string_compare_nonequal()
{
	string = strap_string_alloc("strap");
	string2 = strap_string_alloc("strip");
	TEST_ASSERT_TRUE(strap_string_compare(string, string2) < 0);
	TEST_ASSERT_TRUE(strap_string_compare(string2, string) > 0);
	return 1;
}

int test_string_compare_equal()
{
	string = strap_string_alloc("strap");
	string2 = strap_string_clone(string);
	TEST_ASSERT_TRUE(strap_string_compare(string, string2) == 0);
	return 1;
}

int test_string_find_null()
{
	string = strap_string_alloc("str");
	TEST_ASSERT_TRUE(strap_string_find(NULL, NULL) == -1);
	TEST_ASSERT_TRUE(strap_string_find(string, NULL) == -1);
	return 1;
}

int test_string_find_no_match()
{
	string = strap_string_alloc("str");
	string2 = strap_string_alloc("a");
	TEST_ASSERT_TRUE(strap_string_find(string, string2) == -1);
	return 1;
}

int test_string_find_match()
{
	string = strap_string_alloc("this is a rather long string");
	string2 = strap_string_alloc("long");
	TEST_ASSERT_TRUE(strap_string_find(string, string2) == 17);
	return 1;
}

int test_string_split_null()
{
	return 1;
}

int test_string_split_empty()
{
	return 1;
}

int test_string_split_comma()
{
	return 1;
}

int test_string_split_word()
{
	return 1;
}

int test_string_reverse()
{
	string = strap_string_alloc("this is my string");
	TEST_ASSERT_TRUE(!strap_string_reverse(NULL));
	TEST_ASSERT_TRUE(strap_string_reverse(string));
	TEST_ASSERT_TRUE(strcmp(strap_string_get_cstr(string), "gnirts ym si siht") == 0);
	return 1;
}

// ----------------------------------------------------------------------------

int test_array_alloc()
{
	TEST_ASSERT_TRUE(!(arr = strap_array_alloc(STRAP_TYPE_COUNT)));
	TEST_ASSERT_TRUE(arr = strap_array_alloc(STRAP_TYPE_STRING));
	TEST_ASSERT_TRUE(!strap_array_count(arr));
	return 1;
}

int test_array_get_cstr_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(!strap_array_get_cstr(arr, 0));
	TEST_ASSERT_TRUE(!strap_array_get_cstr(arr, 9999));
	TEST_ASSERT_TRUE(!strap_array_count(arr));
	return 1;
}

int test_array_append_cstr_null()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(!strap_array_append_cstr(NULL, NULL));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, NULL));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 0);
	return 1;
}

int test_array_append_cstr_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, ""));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "second"));
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, ""));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	return 1;
}

int test_array_append_cstr_valid()
{
	int i;
	char *buf[4] = { "first", "second", "third", "fourth" };
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	for (i = 0; i < 4; i++)
		TEST_ASSERT_TRUE(strap_array_append_cstr(arr, buf[i]));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 4);
	puts("");
	strap_array_printf(arr);
	for (i = 0; i < 4; i++)
		TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr, i), buf[i]) == 0);
	return 1;
}

int test_array_append_cstr_large()
{
	int i;
	size_t size = 9999;
	char buf[size];
	memset(buf, 'a', size);
	buf[size - 1] = '\0';
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	puts("");
	for (i = 0; i < size; i++) {
	looping:
		// logd(g_count);
		TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "a"));
	}
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, buf));
	TEST_ASSERT_TRUE(strap_array_count(arr) == size + 1);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr, size), buf) == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr, 0), "a") == 0);
	// strap_array_printf(arr);
	return 1;
}

int test_array_insert_cstr_null()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(!strap_array_insert_cstr(arr, 0, NULL));
	TEST_ASSERT_TRUE(!strap_array_count(arr));
	return 1;
}

int test_array_insert_cstr_empty()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, ""));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, ""));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 3);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr, 0), "") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr, 1), "") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr, 2), "third") == 0);
	return 1;
}

int test_array_insert_cstr_valid()
{
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	TEST_ASSERT_TRUE(strap_array_append_cstr(arr, "third"));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, "first"));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 1, "second"));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 10, "last"));
	TEST_ASSERT_TRUE(strap_array_count(arr) == 4);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr, 0), "first") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr, 1), "second") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr, 2), "third") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr, 3), "last") == 0);
	return 1;
}

int test_array_insert_cstr_large()
{
	int i;
	size_t size = 9999;
	char buf[size];
	memset(buf, 'a', size);
	buf[size - 1] = '\0';
	arr = strap_array_alloc(STRAP_TYPE_STRING);
	for (i = 0; i < size; i++)
		TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, "a"));
	TEST_ASSERT_TRUE(strap_array_insert_cstr(arr, 0, buf));
	TEST_ASSERT_TRUE(strap_array_count(arr) == size + 1);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr, 0), buf) == 0);
	return 1;
}

int test_array_replace_cstr_null()
{
	return 1;
}

int test_array_replace_cstr_empty()
{
	return 1;
}

int test_array_replace_cstr_valid()
{
	return 1;
}

int test_array_replace_cstr_large()
{
	return 1;
}

int test_array_find_cstr_null()
{
	return 1;
}

int test_array_find_cstr_empty()
{
	return 1;
}

int test_array_find_cstr_valid()
{
	return 1;
}

int test_array_find_cstr_large()
{
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
	string  = NULL;
	string2 = NULL;
	string3 = NULL;
	string4 = NULL;
	arr = NULL;
}

int main ()
{
	pass_count = 0;
	test_count = 0;
	setbuf(stdout, NULL);
	TEST_BEFORE_EACH(test_prep);
	TEST_AFTER_EACH(test_cleanup);
	puts("-- START --");

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

	TEST_RUN(test_string_substring_invalid);
	TEST_RUN(test_string_substring_valid);

	TEST_RUN(test_string_erase_invalid);
	TEST_RUN(test_string_erase_valid);

	TEST_RUN(test_string_trim);

	TEST_RUN(test_string_compare_null);
	TEST_RUN(test_string_compare_nonequal);
	TEST_RUN(test_string_compare_equal);

	TEST_RUN(test_string_find_null);
	TEST_RUN(test_string_find_no_match);
	TEST_RUN(test_string_find_match);

	TEST_RUN(test_string_split_null);
	TEST_RUN(test_string_split_empty);
	TEST_RUN(test_string_split_comma);
	TEST_RUN(test_string_split_word);

	TEST_RUN(test_string_reverse);

	// ARRAY cstr

	TEST_RUN(test_array_alloc);
	TEST_RUN(test_array_get_cstr_empty);

	TEST_RUN(test_array_append_cstr_null);
	TEST_RUN(test_array_append_cstr_empty);
	TEST_RUN(test_array_append_cstr_valid);
	TEST_RUN(test_array_append_cstr_large);

	// TEST_RUN(test_array_insert_cstr_null);
	// TEST_RUN(test_array_insert_cstr_empty);
	// TEST_RUN(test_array_insert_cstr_valid);
	// TEST_RUN(test_array_insert_cstr_large);
 //
	// TEST_RUN(test_array_replace_cstr_null);
	// TEST_RUN(test_array_replace_cstr_empty);
	// TEST_RUN(test_array_replace_cstr_valid);
	// TEST_RUN(test_array_replace_cstr_large);
 //
	// TEST_RUN(test_array_find_cstr_null);
	// TEST_RUN(test_array_find_cstr_empty);
	// TEST_RUN(test_array_find_cstr_valid);
	// TEST_RUN(test_array_find_cstr_large);
 //
	puts("---------------------------------");
	printf("%d Tests, %d Passed, %d Failed\n", test_count, pass_count,
				test_count - pass_count);
	return !(pass_count == test_count);
}
