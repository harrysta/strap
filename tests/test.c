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

StrapArray *arr_str;
StrapString *string, *string2, *string3;

int test_array_str_alloc()
{
	TEST_ASSERT_TRUE(arr_str != NULL);
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == 0);
	return 1;
}

int test_array_get_str_empty()
{
	TEST_ASSERT_TRUE(strap_array_get_cstr(arr_str, 0) == NULL);
	TEST_ASSERT_TRUE(strap_array_get_cstr(arr_str, 9999) == NULL);
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == 0);
	return 1;
}

int test_array_append_cstr()
{
	char buf[8];

	// assert count
	TEST_ASSERT_TRUE(!strap_array_append_cstr(arr_str, "first"));
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == 1);
	strap_array_append_cstr(arr_str, "second");
	strap_array_append_cstr(arr_str, "third");
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == 3);

	// assert string content
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr_str, 0), "first") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr_str, 1), "second") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr_str, 2), "third") == 0);
	TEST_ASSERT_TRUE(strap_array_get_cstr(arr_str, 2) != NULL);
	TEST_ASSERT_TRUE(strap_array_get_cstr(arr_str, 3) == NULL);
	return 1;
}

int test_array_append_large()
{
	size_t i;
	size_t size = 99999;
	char buf[6] = "atest";

	for (i = 0; i < size; i++) {
		TEST_ASSERT_TRUE(!strap_array_append_cstr(arr_str, buf));
		TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr_str, i), buf) == 0);
		buf[0] = (((buf[0] - 'a') + 1) % 26) + 'a';
	}
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == size);
	return 1;
}

int test_array_blank_values()
{
	char buf[8];

	strap_array_append_cstr(arr_str, NULL);
	strap_array_append_cstr(arr_str, "");
	strap_array_append_cstr(arr_str, "second");
	strap_array_append_cstr(arr_str, "");
	strap_array_append_cstr(arr_str, "fourth");
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == 4);

	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr_str, 0), "") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr_str, 1), "second") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr_str, 2), "") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_cstr(arr_str, 3), "fourth") == 0);
	return 1;
}

// ----------------------------------------------------------------

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

int test_string_copy_to_null()
{
	return 0;
}

int test_string_copy_to_empty()
{
	return 0;
}
int test_string_copy_to_string()
{
	return 0;
}

int test_string_copy_to_large()
{
	return 0;
}

int test_string_strcat_null()
{
	return 0;
}

int test_string_strcat_empty()
{
	return 0;
}

int test_string_strcat_string()
{
	return 0;
}

int test_string_strcat_large()
{
	return 0;
}


void test_prep()
{
	arr_str = strap_array_alloc(STRAP_TYPE_STRING);
}

void test_cleanup()
{
	strap_array_free(arr_str);
	free(string);
	free(string2);
	free(string3);
	arr_str = NULL;
	string = NULL;
	string2 = NULL;
	string3 = NULL;
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

	TEST_RUN(test_string_concat_null);
	TEST_RUN(test_string_concat_empty);
	TEST_RUN(test_string_concat_string);
	TEST_RUN(test_string_concat_large);

	TEST_RUN(test_string_copy_from_null);
	TEST_RUN(test_string_copy_from_empty);
	TEST_RUN(test_string_copy_from_string);
	TEST_RUN(test_string_copy_from_large);

	TEST_RUN(test_string_copy_to_null);
	TEST_RUN(test_string_copy_to_empty);
	TEST_RUN(test_string_copy_to_string);
	TEST_RUN(test_string_copy_to_large);

	TEST_RUN(test_string_strcat_null);
	TEST_RUN(test_string_strcat_empty);
	TEST_RUN(test_string_strcat_string);
	TEST_RUN(test_string_strcat_large);

	TEST_RUN(test_array_str_alloc);
	TEST_RUN(test_array_get_str_empty);
	TEST_RUN(test_array_append_cstr);
	TEST_RUN(test_array_blank_values);
	TEST_RUN(test_array_append_large);
	puts("---------------------------------");
	printf("%d Tests, %d Passed, %d Failed\n", test_count, pass_count,
				test_count - pass_count);
	return !(pass_count == test_count);
}
