#define STM32F40_41xxx
#define STM32F407xx
#define HSE_VALUE			((uint32_t)8000000)
//#define assert_param(x)

void fail_assertion(const char *reason, const char *filename, int lineno);
#define assert_param(x)		if (!(x)) fail_assertion(#x, __FILE__, __LINE__)
