#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <libgen.h>
#include "testbed.h"

static const char *testname;
static FILE *debug_log = NULL;

static void write_summary_file(bool success) {
	FILE *f = fopen("tests.log", "a");
	if (!f) {
		fprintf(stderr, "Failed to write summary file.\n");
		exit(EXIT_FAILURE);
	}
	fprintf(f, "%s %d\n", testname, success ? 1 : 0);
	fclose(f);
}

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

static void test_syntax(const char *pgmname) {
	fprintf(stderr, "%s (--verbose) (--summary)\n", pgmname);
	fprintf(stderr, "\n");
	fprintf(stderr, "-v, --verbose     Increase verbosity during test.\n");
	fprintf(stderr, "-s, --summary     Only print a test summary, don't do any testing.\n");
}

static void print_headline(const char *text) {
	const int total_length = 80;
	const int dbl_bar_len = total_length - strlen(text);
	const int bar_len = dbl_bar_len / 2;

	for (int i = 0; i < bar_len; i++) {
		fprintf(stderr, "=");
	}
	fprintf(stderr, " %s ", text);
	for (int i = 0; i < dbl_bar_len - bar_len; i++) {
		fprintf(stderr, "=");
	}
	fprintf(stderr, "\n");
}

static int test_print_summary(void) {
	FILE *f = fopen("tests.log", "r");
	if (!f) {
		fprintf(stderr, "Failed to read summary file.\n");
		exit(EXIT_FAILURE);
	}
	int conducted_test_cnt = 0;
	int failed_test_cnt = 0;
	char line[256];
	print_headline("TEST SUMMARY");
	while (fgets(line, sizeof(line) - 1, f)) {
		line[sizeof(line) - 1] = 0;
		int l = strlen(line);
		if (l && (line[l - 1] == '\r')) {
			line[--l] = 0;
		}
		if (l && (line[l - 1] == '\n')) {
			line[--l] = 0;
		}

		char *testname = strtok(line, " ");
		int test_success = atoi(strtok(NULL, " "));
		conducted_test_cnt++;
		if (!test_success) {
			fprintf(stderr, "FAILED: %s\n", testname);
			failed_test_cnt++;
		}
	}
	fclose(f);
	const int successful_test_cnt = conducted_test_cnt - failed_test_cnt;
	fprintf(stderr, "%d tests: %d PASS, %d FAIL\n", conducted_test_cnt, successful_test_cnt, failed_test_cnt);
	if (failed_test_cnt == 0) {
		print_headline("EVERYTHING OK");
	} else {
		print_headline("FAILED TESTS");
	}
	return (failed_test_cnt > 0) ? 1 : 0;
}

void test_start(int argc, char **argv) {
	const char *new_testname = basename(argv[0]);

	struct option long_options[] = {
		{"verbose", no_argument, 0, 'v' },
		{"summary", no_argument, 0, 's' },
		{ 0 }
	};

	int opt;
	while ((opt = getopt_long(argc, argv, "vs", long_options, NULL)) != -1) {
		switch (opt) {
			case 'v':
				debug_log = stderr;
				break;
			case 's':
				exit(test_print_summary());
				break;
			default:
				fprintf(stderr, "Unexpected option -- %d.\n", opt);
				test_syntax(argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	if (optind != argc) {
		fprintf(stderr, "Unexpected excess argument.\n");
		test_syntax(argv[0]);
		exit(EXIT_FAILURE);
	}
	testname = new_testname;
}

void test_success(void) {
	fprintf(stderr, "PASS: %s\n", testname);
	write_summary_file(true);
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
	write_summary_file(false);
	return result;
}

