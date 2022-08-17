#include <stdio.h>
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
StrapNode *node1;

int test_alloc()
{
	TEST_ASSERT_TRUE(arr_str != NULL);
	TEST_ASSERT_TRUE(strap_count(arr_str) == 0);
	return 1;
}

int test_get_empty()
{
	TEST_ASSERT_TRUE(strap_get(arr_str, 0) == NULL);
	TEST_ASSERT_TRUE(strap_get(arr_str, 9999) == NULL);
	return 1;
}

int test_append_string()
{
	char buf[8];

	// assert count
	strap_append_string(arr_str, "first");
	TEST_ASSERT_TRUE(strap_count(arr_str) == 1);
	strap_append_string(arr_str, "second");
	strap_append_string(arr_str, "third");
	TEST_ASSERT_TRUE(strap_count(arr_str) == 3);
	TEST_ASSERT_TRUE(strap_get(arr_str, 2) != NULL);
	TEST_ASSERT_TRUE(strap_get(arr_str, 3) == NULL);

	// assert string content
	strap_node_strcpy(strap_get(arr_str, 0), buf);
	TEST_ASSERT_TRUE(strcmp(buf, "first") == 0);
	strap_node_strcpy(strap_get(arr_str, 1), buf);
	TEST_ASSERT_TRUE(strcmp(buf, "second") == 0);
	strap_node_strcpy(strap_get(arr_str, 2), buf);
	TEST_ASSERT_TRUE(strcmp(buf, "third") == 0);
	return 1;
}

int test_node_strcpy_blank()
{
	char buf[8];

	strap_append_string(arr_str, NULL);
	strap_append_string(arr_str, "");
	strap_append_string(arr_str, "second");
	strap_append_string(arr_str, "");
	strap_append_string(arr_str, "fourth");

	TEST_ASSERT_TRUE(strap_count(arr_str) == 4);
	strap_node_strcpy(strap_get(arr_str, 0), buf);
	TEST_ASSERT_TRUE(strcmp(buf, "") == 0);
	strap_node_strcpy(strap_get(arr_str, 1), buf);
	TEST_ASSERT_TRUE(strcmp(buf, "second") == 0);
	strap_node_strcpy(strap_get(arr_str, 2), buf);
	TEST_ASSERT_TRUE(strcmp(buf, "") == 0);
	strap_node_strcpy(strap_get(arr_str, 3), buf);
	TEST_ASSERT_TRUE(strcmp(buf, "fourth") == 0);
	return 1;
}

int test_node_strcpy_big()
{
	char buf[512], buf2[512];

	memset(buf, 'a', 511);
	buf[511] = '\0';
	strap_append_string(arr_str, buf);
	strap_append_string(arr_str, buf);

	strap_node_strcpy(strap_get(arr_str, 0), buf2);
	TEST_ASSERT_TRUE(strcmp(buf, buf2) == 0);
	strap_node_strcpy(strap_get(arr_str, 1), buf2);
	TEST_ASSERT_TRUE(strcmp(buf, buf2) == 0);
	TEST_ASSERT_TRUE(strap_count(arr_str) == 2);

	return 1;
}

void test_prep()
{
	arr_str = strap_alloc(STRAP_STRING);
}

void test_cleanup()
{
	strap_free(arr_str);
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
	TEST_RUN(test_node_strcpy_blank);
	TEST_RUN(test_node_strcpy_big);
	puts("---------------------------------");
	printf("%d Tests, %d Passed, %d Failed\n", test_count, pass_count,
				test_count - pass_count);
	return !(pass_count == test_count);
}
