#ifndef __INTTYPES_EMULATION__
#define __INTTYPES_EMULATION__

#if defined(VARIANT_WINDOWS)

#define PRId64			"I64d"
#define PRIu64			"I64u"
#define PRIi64			"I64i"
#define PRIx64			"I64x"

#elif defined(VARIANT_LINUX)

#include <inttypes.h>

#endif

#endif
