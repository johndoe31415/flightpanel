set $RAM_START = 0x20000000
set $RAM_SIZE = 0x10000

define dumpmem
	dump binary memory memory.bin $RAM_START ($RAM_START + $RAM_SIZE)
end
