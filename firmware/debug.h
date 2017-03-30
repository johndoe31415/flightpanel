#ifndef __DEBUG_H__
#define __DEBUG_H__

#define emit(value)			__asm__ __volatile__(".word " #value)

#define bkpt(value)			__asm__ __volatile__("bkpt #" #value)

#endif
