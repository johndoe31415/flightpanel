#include <stdio.h>
#include <stdbool.h>
#include "fault.h"

void fail_assertion(const char *assertion, const char *filename, int lineno) {
	printf("Assertion failed: %s (%s:%d)\n", assertion, filename, lineno);
	while (true);
}

void HardFault_Handler(void) {
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
