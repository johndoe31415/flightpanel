#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "testbed.h"

static const char *testname;
static FILE *debug_log = NULL;

bool test_verbose(void) {
	return debug_log != NULL;
}

void debug(const char *msg, ...) {
	if (debug_log != NULL) {
		va_list ap;
		va_start(ap, msg);
		vfprintf(debug_log, msg, ap);
		va_end(ap);
	}
}

void test_start(int argc, char **argv) {
	const char *new_testname = argv[0];
	if ((argc == 2) && (!strcmp(argv[1], "-v"))) {
		debug_log = stderr;
	}
	testname = new_testname;
}

void test_success(void) {
	fprintf(stderr, "PASS: %s\n", testname);
	exit(EXIT_SUCCESS);
}

void test_fail_ext(const char *file, int line, const char *reason, failfnc_t failfnc, const void *lhs, const void *rhs) {
	fprintf(stderr, "FAILED %s:%d: %s (%s)\n", file, line, testname, reason);
	if (failfnc != NULL) {
		char *extended_reason = failfnc(lhs, rhs);
		fprintf(stderr, "   %s\n", extended_reason);
		free(extended_reason);
	}
	exit(EXIT_FAILURE);
}

void test_fail(const char *file, int line, const char *reason) {
	test_fail_ext(file, line, reason, NULL, NULL, NULL);
}

char *testbed_failfnc_str(const void *vlhs, const void *vrhs) {
	const char *lhs = (const char*)vlhs;
	const char *rhs = (const char*)vrhs;
	char *result = calloc(1, strlen(lhs) + strlen(rhs) + 32);
	sprintf(result, "LHS = \"%s\", RHS = \"%s\"", lhs, rhs);
	return result;
}
