#ifndef __TESTBED_H__
#define __TESTBED_H__

#define test_fail_if(cond)		if (cond) test_fail(#cond " was true")
#define test_fail_unless(cond)	if (!(cond)) test_fail(#cond " was false")

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void debug(const char *msg, ...);
void test_start(int argc, char **argv);
void test_success(void);
void test_fail(const char *reason);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
