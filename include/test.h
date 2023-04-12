#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <ctype.h>
#include <stdio.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

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
};

static int tests_passed = 0;
static int tests_failed = 0;
static int tests_skipped = 0;

struct test_summary blank_test_summary(void) {
	struct test_summary summary;
	summary.result = TEST_RESULT_NONE;
	summary.test_name = NULL;
	summary.message = NULL;
	summary.line_number = -1;
	return summary;
}

void display_test(struct test_summary summary) {
	switch (summary.result) {
	case TEST_RESULT_NONE:
		assert(0);
	case TEST_RESULT_PASS:
		printf(ANSI_COLOR_GREEN "PASS");
		break;
	case TEST_RESULT_FAIL:
		printf(ANSI_COLOR_RED "FAIL");
		break;
	case TEST_RESULT_SKIP:
		printf(ANSI_COLOR_YELLOW "SKIP");
		break;
	}
	printf(ANSI_COLOR_RESET "\t");
	printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET, summary.test_name);
	if (summary.message != NULL) {
		printf("\t");
		printf("%s", summary.message);
		if (summary.line_number >= 0) {
			printf(" on ");
			printf(ANSI_COLOR_BLUE "line %d" ANSI_COLOR_RESET, summary.line_number);
		}
	}
}

void track_test_summary(struct test_summary summary) {
	switch (summary.result) {
	case TEST_RESULT_NONE:
		assert(0);
	case TEST_RESULT_PASS:
		tests_passed++;
		break;
	case TEST_RESULT_FAIL:
		tests_failed++;
		break;
	case TEST_RESULT_SKIP:
		tests_skipped++;
		break;
	}
}

struct test_summary test_pass(void) {
	struct test_summary summary = blank_test_summary();
	summary.result = TEST_RESULT_PASS;
	return summary;
}

struct test_summary test_fail(char *failure_code, int failure_line) {
	struct test_summary summary = blank_test_summary();
	summary.result = TEST_RESULT_FAIL;
	summary.message = failure_code;
	summary.line_number = failure_line;
	return summary;
}

struct test_summary test_skip(int line_number) {
	struct test_summary summary = blank_test_summary();
	summary.result = TEST_RESULT_SKIP;
	summary.message = "SKIP()";
	summary.line_number = line_number;
	return summary;
}

void tests_run(char *filename, void (*tests_fn)(void)) {
	printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET ":\n\n", filename);
	tests_fn();
	printf("\n");
}

struct test_summary
test_run(char *test_name, struct test_summary (*test_fn)(void)) {
	struct test_summary summary = test_fn();
	summary.test_name = test_name;
	track_test_summary(summary);
	display_test(summary);
	printf("\n");
	return summary;
}

typedef struct test_summary test;

#define TESTS(fn)                                                              \
	{ tests_run(#fn, fn); }
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

int summarize_tests(void) {
	int failed = tests_failed > 0;

	if (failed) {
		printf(ANSI_COLOR_RED "FAIL" ANSI_COLOR_RESET);
	} else {
		printf(ANSI_COLOR_GREEN "PASS" ANSI_COLOR_RESET);
	}

	printf("\t");
	printf("total: %d, ", tests_passed + tests_failed + tests_skipped);
	printf("passed: %d, ", tests_passed);
	printf("failed: %d, ", tests_failed);
	printf("skipped: %d\n", tests_skipped);

	return failed;
}

#endif
