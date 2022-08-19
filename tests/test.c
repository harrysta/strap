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

int test_alloc()
{
	TEST_ASSERT_TRUE(arr_str != NULL);
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == 0);
	return 1;
}

int test_get_empty()
{
	TEST_ASSERT_TRUE(strap_array_get_str(arr_str, 0) == NULL);
	TEST_ASSERT_TRUE(strap_array_get_str(arr_str, 9999) == NULL);
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == 0);
	return 1;
}

int test_append_string()
{
	char buf[8];

	// assert count
	TEST_ASSERT_TRUE(!strap_array_append_str(arr_str, "first"));
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == 1);
	strap_array_append_str(arr_str, "second");
	strap_array_append_str(arr_str, "third");
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == 3);

	// assert string content
	TEST_ASSERT_TRUE(strcmp(strap_array_get_str(arr_str, 0), "first") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_str(arr_str, 1), "second") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_str(arr_str, 2), "third") == 0);
	TEST_ASSERT_TRUE(strap_array_get_str(arr_str, 2) != NULL);
	TEST_ASSERT_TRUE(strap_array_get_str(arr_str, 3) == NULL);
	return 1;
}

int test_big_array()
{
	size_t i;
	size_t size = 99999;
	char buf[6] = "atest";

	for (i = 0; i < size; i++) {
		TEST_ASSERT_TRUE(!strap_array_append_str(arr_str, buf));
		TEST_ASSERT_TRUE(strcmp(strap_array_get_str(arr_str, i), buf) == 0);
		buf[0] = (((buf[0] - 'a') + 1) % 26) + 'a';
	}
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == size);
	return 1;
}

int test_array_blank_values()
{
	char buf[8];

	strap_array_append_str(arr_str, NULL);
	strap_array_append_str(arr_str, "");
	strap_array_append_str(arr_str, "second");
	strap_array_append_str(arr_str, "");
	strap_array_append_str(arr_str, "fourth");
	TEST_ASSERT_TRUE(strap_array_count(arr_str) == 4);

	TEST_ASSERT_TRUE(strcmp(strap_array_get_str(arr_str, 0), "") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_str(arr_str, 1), "second") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_str(arr_str, 2), "") == 0);
	TEST_ASSERT_TRUE(strcmp(strap_array_get_str(arr_str, 3), "fourth") == 0);
	return 1;
}

void test_prep()
{
	arr_str = strap_array_alloc(STRAP_TYPE_STRING);
}

void test_cleanup()
{
	strap_array_free(arr_str);
}

int main ()
{
	pass_count = 0;
	test_count = 0;
	setbuf(stdout, NULL);
	TEST_BEFORE_EACH(test_prep);
	TEST_AFTER_EACH(test_cleanup);
	puts("-- START --");
	TEST_RUN(test_alloc);
	TEST_RUN(test_get_empty);
	TEST_RUN(test_append_string);
	TEST_RUN(test_array_blank_values);
	TEST_RUN(test_big_array);
	puts("---------------------------------");
	printf("%d Tests, %d Passed, %d Failed\n", test_count, pass_count,
				test_count - pass_count);
	return !(pass_count == test_count);
}
