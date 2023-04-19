#ifndef CTEST_H
#define CTEST_H

#include <assert.h>
#include <stdio.h>

#define ANSI_COLOR_RED    "\x1b[31m"
#define ANSI_COLOR_GREEN  "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE   "\x1b[34m"
#define ANSI_COLOR_RESET  "\x1b[0m"

enum test_result {
	TEST_RESULT_NONE,
	TEST_RESULT_PASS,
	TEST_RESULT_FAIL,
	TEST_RESULT_SKIP
};

struct test_summary {
	enum test_result result;
	char *test_name;
	char *message;
	int line_number;
	int a;
};

struct test_summary
test_run(char *test_name, struct test_summary (*test_fn)(void));
struct test_summary test_pass(void);
struct test_summary test_fail(char *failed_code, int line_number);
struct test_summary test_skip(int line_number);
int tests_summarize(void);

typedef struct test_summary test;

#define TESTS(fn)                                                              \
	{                                                                            \
		printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET ":\n\n", filename);           \
		tests_fn();                                                                \
		printf("\n");                                                              \
	}
#define TEST(fn)                                                               \
	{ test_run(#fn, fn); }
#define PASS()                                                                 \
	{ return test_pass(); }
#define FAIL()                                                                 \
	{ return test_fail("FAIL()", __LINE__); }
#define SKIP(fn)                                                               \
	{ return test_skip(__LINE__); }
#define EXPECT(val)                                                            \
	{                                                                            \
		if (!(val)) {                                                              \
			return test_fail("EXPECT(" #val ")", __LINE__);                          \
		}                                                                          \
	}

#endif
