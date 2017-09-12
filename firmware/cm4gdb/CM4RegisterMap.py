#!/usr/bin/python3
#
#	flightpanel - A Cortex-M4 based USB flight panel for flight simulators.
#	Copyright (C) 2017-2017 Johannes Bauer
#
#	This file is part of flightpanel.
#
#	flightpanel is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; this program is ONLY licensed under
#	version 3 of the License, later versions are explicitly excluded.
#
#	flightpanel is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with flightpanel; if not, write to the Free Software
#	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#	Johannes Bauer <JohannesBauer@gmx.de>

from .RegisterMap import BaseRegisterMap

class SystemControlBlockRegisterMap(BaseRegisterMap):
	SHPR = 0x18		# System Handler Priority Register
	SHCSR = 0x24	# System Handler Control and Status Register

class NVICRegisterMap(BaseRegisterMap):
	ISER = 0
	ISPR = 0x100
	IABR = 0x200
	IP = 0x300

class SystemControlSpaceRegisterMap(BaseRegisterMap):
	SysTick = 0x10
	NVIC = NVICRegisterMap(0x100)
	SCB = SystemControlBlockRegisterMap(0xd00)

class GPIORegisterMap(BaseRegisterMap):
	MODER = 0x0
	OTYPER = 0x04
	OSPEEDR = 0x8
	PUPDR = 0xc
	IDR = 0x10
	ODR = 0x14
	BSRR = 0x18
	LCKR = 0x1c
	AFR1 = 0x20
	AFR2 = 0x24

class SystemControlBlockRegisterMap(BaseRegisterMap):
	SHPR = 0x18			# System Handler Priority Register
	SHCSR = 0x24		# System Handler Control and Status Register

class AHB1RegisterMap(BaseRegisterMap):
	GPIOA = GPIORegisterMap(0x0000)
	GPIOB = GPIORegisterMap(0x0400)
	GPIOC = GPIORegisterMap(0x0800)
	GPIOD = GPIORegisterMap(0x0c00)
	GPIOE = GPIORegisterMap(0x1000)
	GPIOF = GPIORegisterMap(0x1400)
	GPIOG = GPIORegisterMap(0x1800)
	GPIOH = GPIORegisterMap(0x1c00)
	GPIOI = GPIORegisterMap(0x2000)
	GPIOJ = GPIORegisterMap(0x2400)
	GPIOK = GPIORegisterMap(0x2800)
	DMA1 = 0x6000
	DMA1_Streams = 0x6010

class PeripheralRegisterMap(BaseRegisterMap):
	AHB1 = AHB1RegisterMap(0x20000)

class CM4RegisterMap(BaseRegisterMap):
	PERIPH = PeripheralRegisterMap(0x40000000)
	SCS = SystemControlSpaceRegisterMap(0xe000e000)

cm4reg = CM4RegisterMap(0)

if __name__ == "__main__":
	print("0x%x" % (cm4reg.addr))
	print("0x%x" % (cm4reg.PERIPH.addr))
	print("0x%x" % (cm4reg.PERIPH.AHB1.addr))
	print("0x%x" % (cm4reg.PERIPH.AHB1.GPIOC.addr))
	print("0x%x" % (cm4reg.PERIPH.AHB1.GPIOC.ODR.addr))
