#include <ctest.h>

#include <stdio.h>

static int tests_passed  = 0;
static int tests_failed  = 0;
static int tests_skipped = 0;

static struct test_summary blank_test_summary(void) {
	struct test_summary summary;
	summary.result      = TEST_RESULT_NONE;
	summary.test_name   = NULL;
	summary.message     = NULL;
	summary.line_number = -1;
	return summary;
}

struct test_summary test_pass(void) {
	struct test_summary summary = blank_test_summary();
	summary.result              = TEST_RESULT_PASS;
	return summary;
}

struct test_summary test_fail(char *failed_code, int failed_line) {
	struct test_summary summary = blank_test_summary();
	summary.result              = TEST_RESULT_FAIL;
	summary.message             = failed_code;
	summary.line_number         = failed_line;
	return summary;
}

struct test_summary test_skip(int line_number) {
	struct test_summary summary = blank_test_summary();
	summary.result              = TEST_RESULT_SKIP;
	summary.message             = "SKIP()";
	summary.line_number         = line_number;
	return summary;
}

struct test_summary
test_run(char *test_name, struct test_summary (*test_fn)(void)) {
	struct test_summary summary = test_fn();
	summary.test_name           = test_name;

	switch (summary.result) {
	case TEST_RESULT_NONE:
		assert(0);
	case TEST_RESULT_PASS:
		tests_passed++;
		printf(ANSI_COLOR_GREEN "PASS");
		break;
	case TEST_RESULT_FAIL:
		tests_failed++;
		printf(ANSI_COLOR_RED "FAIL");
		break;
	case TEST_RESULT_SKIP:
		tests_skipped++;
		printf(ANSI_COLOR_YELLOW "SKIP");
		break;
	}

	printf(
		ANSI_COLOR_RESET "\t" ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET,
		summary.test_name
	);

	if (summary.message != NULL) {
		printf("\t%s", summary.message);
		if (summary.line_number >= 0) {
			printf(
				" on " ANSI_COLOR_BLUE "line %d" ANSI_COLOR_RESET, summary.line_number
			);
		}
	}

	printf("\n");

	return summary;
}

int tests_summarize(void) {
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
