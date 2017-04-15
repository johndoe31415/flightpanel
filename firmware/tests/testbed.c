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
	fprintf(stderr, "Testing: %s\n", new_testname);
	if ((argc == 2) && (!strcmp(argv[1], "-v"))) {
		debug_log = stderr;
	}
	testname = new_testname;
}

void test_success(void) {
	fprintf(stderr, "PASS: %s\n", testname);
	exit(EXIT_SUCCESS);
}

void test_fail(const char *reason) {
	fprintf(stderr, "FAIL: %s (%s)\n", testname, reason);
	exit(EXIT_FAILURE);
}
