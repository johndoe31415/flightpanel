#!/usr/bin/python3
import gdb
class DebugCortexMIRQs (gdb.Command):
	"Debugging of Cortex-M IRQs and their priorities."

	_UINT32_T = gdb.lookup_type("uint32_t")
	_VECTORS = {
		0: "Reset_Handler",
		1: "NMI_Handler",
		2: "HardFault_Handler",
		3: "MemManage_Handler",
		4: "BusFault_Handler",
		5: "UsageFault_Handler",
		10: "SVC_Handler",
		11: "DebugMon_Handler",
		13: "PendSV_Handler",
		14: "SysTick_Handler",
		15: "WWDG_IRQHandler",
		16: "PVD_IRQHandler",
		17: "TAMP_STAMP_IRQHandler",
		18: "RTC_WKUP_IRQHandler",
		19: "FLASH_IRQHandler",
		20: "RCC_IRQHandler",
		21: "EXTI0_IRQHandler",
		22: "EXTI1_IRQHandler",
		23: "EXTI2_IRQHandler",
		24: "EXTI3_IRQHandler",
		25: "EXTI4_IRQHandler",
		26: "DMA1_Stream0_IRQHandler",
		27: "DMA1_Stream1_IRQHandler",
		28: "DMA1_Stream2_IRQHandler",
		29: "DMA1_Stream3_IRQHandler",
		30: "DMA1_Stream4_IRQHandler",
		31: "DMA1_Stream5_IRQHandler",
		32: "DMA1_Stream6_IRQHandler",
		33: "ADC_IRQHandler",
		34: "CAN1_TX_IRQHandler",
		35: "CAN1_RX0_IRQHandler",
		36: "CAN1_RX1_IRQHandler",
		37: "CAN1_SCE_IRQHandler",
		38: "EXTI9_5_IRQHandler",
		39: "TIM1_BRK_TIM9_IRQHandler",
		40: "TIM1_UP_TIM10_IRQHandler",
		41: "TIM1_TRG_COM_TIM11_IRQHandler",
		42: "TIM1_CC_IRQHandler",
		43: "TIM2_IRQHandler",
		44: "TIM3_IRQHandler",
		45: "TIM4_IRQHandler",
		46: "I2C1_EV_IRQHandler",
		47: "I2C1_ER_IRQHandler",
		48: "I2C2_EV_IRQHandler",
		49: "I2C2_ER_IRQHandler",
		50: "SPI1_IRQHandler",
		51: "SPI2_IRQHandler",
		52: "USART1_IRQHandler",
		53: "USART2_IRQHandler",
		54: "USART3_IRQHandler",
		55: "EXTI15_10_IRQHandler",
		56: "RTC_Alarm_IRQHandler",
		57: "OTG_FS_WKUP_IRQHandler",
		58: "TIM8_BRK_TIM12_IRQHandler",
		59: "TIM8_UP_TIM13_IRQHandler",
		60: "TIM8_TRG_COM_TIM14_IRQHandler",
		61: "TIM8_CC_IRQHandler",
		62: "DMA1_Stream7_IRQHandler",
		63: "FSMC_IRQHandler",
		64: "SDIO_IRQHandler",
		65: "TIM5_IRQHandler",
		66: "SPI3_IRQHandler",
		67: "UART4_IRQHandler",
		68: "UART5_IRQHandler",
		69: "TIM6_DAC_IRQHandler",
		70: "TIM7_IRQHandler",
		71: "DMA2_Stream0_IRQHandler",
		72: "DMA2_Stream1_IRQHandler",
		73: "DMA2_Stream2_IRQHandler",
		74: "DMA2_Stream3_IRQHandler",
		75: "DMA2_Stream4_IRQHandler",
		76: "ETH_IRQHandler",
		77: "ETH_WKUP_IRQHandler",
		78: "CAN2_TX_IRQHandler",
		79: "CAN2_RX0_IRQHandler",
		80: "CAN2_RX1_IRQHandler",
		81: "CAN2_SCE_IRQHandler",
		82: "OTG_FS_IRQHandler",
		83: "DMA2_Stream5_IRQHandler",
		84: "DMA2_Stream6_IRQHandler",
		85: "DMA2_Stream7_IRQHandler",
		86: "USART6_IRQHandler",
		87: "I2C3_EV_IRQHandler",
		88: "I2C3_ER_IRQHandler",
		89: "OTG_HS_EP1_OUT_IRQHandler",
		90: "OTG_HS_EP1_IN_IRQHandler",
		91: "OTG_HS_WKUP_IRQHandler",
		92: "OTG_HS_IRQHandler",
		93: "DCMI_IRQHandler",
		94: "CRYP_IRQHandler",
		95: "HASH_RNG_IRQHandler",
		96: "FPU_IRQHandler",
	}

	def __init__ (self):
		super(DebugCortexMIRQs, self).__init__("cm", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

	def _irq_name(self, irqno):
		return self._VECTORS.get(irqno, "IRQ%d" % (irqno))

	@classmethod
	def _str_to_uint32(cls, s):
		return sum(ord(char) << (8 * i) for (i, char) in enumerate(s))

	@classmethod
	def _str_to_uint32_array(cls, s):
		return [ cls._str_to_uint32(s[i : i + 4]) for i in range(0, len(s), 4) ]

	@classmethod
	def _decode_bits(cls, words, bitlen):
		assert((32 % bitlen) == 0)
		bitmask = (1 << bitlen) - 1
		values_per_word = 32 / bitlen
		result = { }
		for (i, word) in enumerate(words):
			for j in range(values_per_word):
				value_no = (values_per_word * i) + j
				value = (word >> (bitlen * j)) & bitmask
				result[value_no] = value
		return result

	def invoke(self, arg, from_tty):
		SCB_addr = 0xe000e000 + 0xd00		# System Control Block
		SHPR_addr =	SCB_addr + 0x18			# System Handler Priority Register
		SHCSR_addr = SCB_addr + 0x24		# System Handler Control and Status Register

		SHPR = self._decode_bits(self._str_to_uint32_array(gdb.selected_inferior().read_memory(SHPR_addr, 4 * 3)), 8)
		SHCSR = self._decode_bits(self._str_to_uint32_array(gdb.selected_inferior().read_memory(SHCSR_addr, 4)), 1)
		relevant_irqs = set([ irq_no - 3 for irq_no in 3, 4, 5, 10, 11, 13, 14 ])

		SHCSR_bits = {
			0:	"MEMFAULTACT",
			1:	"BUSFAULTACT",
			3:	"USGFAULTACT",
			7:	"SVCALLACT",
			8:	"MONITORACT",
			10:	"PENDSVACT",
			11:	"SYSTICKACT",
			12:	"USGFAULTPENDED",
			13: "MEMFAULTPENDED",
			14:	"BUSFAULTPENDED",
			15:	"SVCALLPENDED",
			16: "MEMFAULTENA",
			17: "BUSFAULTENA",
			18: "USGFAULTENA",
		}

		for base_irq_no in sorted(relevant_irqs):
			irq_no = base_irq_no + 3
			name = self._irq_name(irq_no)
			line = "%-25s " % (name)
			line += "Prio %d" % (SHPR[base_irq_no])
			print(line)
		flags = [ name for (bitno, name) in sorted(SHCSR_bits.items()) if SHCSR[bitno] ]
		print("SHCSR flags: %s" % (" ".join(flags)))

		NVIC_addr = 0xe000e000 + 0x0100
		NVIC_ISER_addr = NVIC_addr + 0x0
		NVIC_ISPR_addr = NVIC_addr + 0x100
		NVIC_IABR_addr = NVIC_addr + 0x200
		NVIC_IP_addr = NVIC_addr + 0x300
		irq_offset = 15

		ISER = self._decode_bits(self._str_to_uint32_array(gdb.selected_inferior().read_memory(NVIC_addr + 0x0, 4 * 8)), 1)
		ISPR = self._decode_bits(self._str_to_uint32_array(gdb.selected_inferior().read_memory(NVIC_addr + 0x100, 4 * 8)), 1)
		IABR = self._decode_bits(self._str_to_uint32_array(gdb.selected_inferior().read_memory(NVIC_addr + 0x200, 4 * 8)), 1)
		IP = self._decode_bits(self._str_to_uint32_array(gdb.selected_inferior().read_memory(NVIC_addr + 0x300, 240)), 8)

		irqs_enabled = set(irq_no for (irq_no, enabled) in ISER.items() if enabled)
		irqs_pending = set(irq_no for (irq_no, pending) in ISPR.items() if pending)
		irqs_active = set(irq_no for (irq_no, active) in IABR.items() if active)

		relevant_irqs = irqs_enabled | irqs_pending | irqs_active
		for base_irq_no in sorted(relevant_irqs):
			irq_no = base_irq_no + 15
			name = self._irq_name(irq_no)
			line = "%-25s " % (name)
			line += "%-15s" % (["", "enabled"][base_irq_no in irqs_enabled])
			line += "%-15s" % (["", "pending"][base_irq_no in irqs_pending])
			line += "%-15s" % (["", "ACTIVE"][base_irq_no in irqs_active])
			line += "Prio %d" % (IP[base_irq_no])
			print(line)

DebugCortexMIRQs()
