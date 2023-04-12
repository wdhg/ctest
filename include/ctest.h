#ifndef CTEST_H
#define CTEST_H

#include <assert.h>
#include <ctype.h>
#include <stdio.h>

#define ct_ANSI_COLOR_RED "\x1b[31m"
#define ct_ANSI_COLOR_GREEN "\x1b[32m"
#define ct_ANSI_COLOR_YELLOW "\x1b[33m"
#define ct_ANSI_COLOR_BLUE "\x1b[34m"
#define ct_ANSI_COLOR_RESET "\x1b[0m"

enum ct_test_result {
	TEST_RESULT_NONE,
	TEST_RESULT_PASS,
	TEST_RESULT_FAIL,
	TEST_RESULT_SKIP
};

struct ct_test_summary {
	enum ct_test_result result;
	char *test_name;
	char *message;
	int line_number;
};

int ct_tests_passed = 0;
int ct_tests_failed = 0;
int ct_tests_skipped = 0;

struct ct_test_summary ct_blank_test_summary(void) {
	struct ct_test_summary summary;
	summary.result = TEST_RESULT_NONE;
	summary.test_name = NULL;
	summary.message = NULL;
	summary.line_number = -1;
	return summary;
}

void ct_display_test(struct ct_test_summary summary) {
	switch (summary.result) {
	case TEST_RESULT_NONE:
		assert(0);
	case TEST_RESULT_PASS:
		printf(ct_ANSI_COLOR_GREEN "PASS");
		break;
	case TEST_RESULT_FAIL:
		printf(ct_ANSI_COLOR_RED "FAIL");
		break;
	case TEST_RESULT_SKIP:
		printf(ct_ANSI_COLOR_YELLOW "SKIP");
		break;
	}
	printf(ct_ANSI_COLOR_RESET "\t");
	printf(ct_ANSI_COLOR_YELLOW "%s" ct_ANSI_COLOR_RESET, summary.test_name);
	if (summary.message != NULL) {
		printf("\t");
		printf("%s", summary.message);
		if (summary.line_number >= 0) {
			printf(" on ");
			printf(
					ct_ANSI_COLOR_BLUE "line %d" ct_ANSI_COLOR_RESET, summary.line_number
			);
		}
	}
}

void ct_track_test_summary(struct ct_test_summary summary) {
	switch (summary.result) {
	case TEST_RESULT_NONE:
		assert(0);
	case TEST_RESULT_PASS:
		ct_tests_passed++;
		break;
	case TEST_RESULT_FAIL:
		ct_tests_failed++;
		break;
	case TEST_RESULT_SKIP:
		ct_tests_skipped++;
		break;
	}
}

struct ct_test_summary ct_test_pass(void) {
	struct ct_test_summary summary = ct_blank_test_summary();
	summary.result = TEST_RESULT_PASS;
	return summary;
}

struct ct_test_summary ct_test_fail(char *failure_code, int failure_line) {
	struct ct_test_summary summary = ct_blank_test_summary();
	summary.result = TEST_RESULT_FAIL;
	summary.message = failure_code;
	summary.line_number = failure_line;
	return summary;
}

struct ct_test_summary ct_test_skip(int line_number) {
	struct ct_test_summary summary = ct_blank_test_summary();
	summary.result = TEST_RESULT_SKIP;
	summary.message = "SKIP()";
	summary.line_number = line_number;
	return summary;
}

void ct_tests_run(char *filename, void (*tests_fn)(void)) {
	printf(ct_ANSI_COLOR_BLUE "%s" ct_ANSI_COLOR_RESET ":\n\n", filename);
	tests_fn();
	printf("\n");
}

struct ct_test_summary
ct_test_run(char *test_name, struct ct_test_summary (*test_fn)(void)) {
	struct ct_test_summary summary = test_fn();
	summary.test_name = test_name;
	ct_track_test_summary(summary);
	ct_display_test(summary);
	printf("\n");
	return summary;
}

typedef struct ct_test_summary test;

#define TESTS(fn)                                                              \
	{ ct_tests_run(#fn, fn); }
#define TEST(fn)                                                               \
	{ ct_test_run(#fn, fn); }
#define PASS()                                                                 \
	{ return ct_test_pass(); }
#define FAIL()                                                                 \
	{ return ct_test_fail("FAIL()", __LINE__); }
#define SKIP(fn)                                                               \
	{ return ct_test_skip(__LINE__); }
#define EXPECT(val)                                                            \
	{                                                                            \
		if (!(val)) {                                                              \
			return ct_test_fail("EXPECT(" #val ")", __LINE__);                       \
		}                                                                          \
	}

int ct_summarize_tests(void) {
	int failed = ct_tests_failed > 0;

	if (failed) {
		printf(ct_ANSI_COLOR_RED "FAIL" ct_ANSI_COLOR_RESET);
	} else {
		printf(ct_ANSI_COLOR_GREEN "PASS" ct_ANSI_COLOR_RESET);
	}

	printf("\t");
	printf("total: %d, ", ct_tests_passed + ct_tests_failed + ct_tests_skipped);
	printf("passed: %d, ", ct_tests_passed);
	printf("failed: %d, ", ct_tests_failed);
	printf("skipped: %d\n", ct_tests_skipped);

	return failed;
}

#endif
