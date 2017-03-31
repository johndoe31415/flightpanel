#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

//#include <stm32f4xx_irq.h>
#include <misc.h>
#include <core_cm4.h>
#include "fault.h"

static uint32_t* get_sp(void) {
  register uint32_t sp __asm__ ("sp");
  return (uint32_t*)sp;
}

static uint32_t* get_lr(void) {
  register uint32_t lr __asm__ ("lr");
  return (uint32_t*)lr;
}

void fail_assertion(const char *assertion, const char *filename, int lineno) {
	printf("Assertion failed: %s (%s:%d)\n", assertion, filename, lineno);
	while (true);
}

void HardFault_Handler(void) {
	uint32_t *sp = get_sp();
	uint32_t *lr = get_lr();
	printf("\n--> Device hard fault, SP = %p, LR = %p\n", sp, lr);
	for (int i = 0; i < 8; i++) {
		printf("IABR%d %08x\n", i, NVIC->IABR[i]);
	}
	for (int i = 0; i < 32; i += 4) {
		printf("%p ", sp + i);
		for (int j = 0; j < 4; j++) {
			printf("%08x ", sp[i + j]);
		}
		printf("\n");
	}
	while (true);
}

void BusFault_Handler(void) {
//      printf("Device hard fault, stopped: Vecttbl=%d Force=%d Dbg=%d\r\n", uSYSCTL.HFSR().VECTTBL() ? 1 : 0, uSYSCTL.HFSR().FORCED() ? 1 : 0, uSYSCTL.HFSR().DEBUGEVT() ? 1 : 0);
        while (true);
}

void MPUFault_Handler(void) {
//      printf("Device MPU fault, stopped.\r\n");
        while (true);
}

void UsgFault_Handler(void) {
//      printf("Device Usg fault, stopped.\r\n");
        while (true);
}
