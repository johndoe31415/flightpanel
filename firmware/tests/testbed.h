#ifndef __TESTBED_H__
#define __TESTBED_H__

#include <string.h>
#include <stdbool.h>

typedef char* (*failfnc_t)(const void *lhs, const void *rhs);

#define test_fail_if(cond)			if (cond) test_fail(__FILE__, __LINE__, __FUNCTION__, #cond " was true")
#define test_fail_unless(cond)		if (!(cond)) test_fail(__FILE__, __LINE__, __FUNCTION__, #cond " was false")
#define test_assert_str_eq(a, b)	{ const char *_a = (a); const char *_b = (b); if (strcmp(_a, _b)) test_fail_ext(__FILE__, __LINE__, __FUNCTION__, #a " != " #b, testbed_failfnc_str, _a, _b); }
#define test_assert_int_eq(a, b)	{ int _a = (a); int _b = (b); if (_a != _b) test_fail_ext(__FILE__, __LINE__, __FUNCTION__, #a " != " #b, testbed_failfnc_int, &_a, &_b); }

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
bool test_verbose(void);
void debug(const char *msg, ...);
void test_start(int argc, char **argv);
void test_success(void);
void test_fail_ext(const char *file, int line, const char *fncname, const char *reason, failfnc_t failfnc, const void *lhs, const void *rhs);
void test_fail(const char *file, int line, const char *fncname, const char *reason);
char *testbed_failfnc_int(const void *vlhs, const void *vrhs);
char *testbed_failfnc_str(const void *vlhs, const void *vrhs);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
