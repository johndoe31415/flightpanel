flightpanel
===========

This project is a flight panel that I'm building. It's a USB device that's
based on the STM32F4-DISCOVERY board (around $20) which features the
STM32F407VG Cortex-M4 processor. From the PC side it shall be detected as a
joystick and give the user radio panel controls (i.e., VHF1, VHF2, NAV1, NAV2
frequencies, AP and transponder functionality).  The idea is that you plug the
device in via USB, it gets detected as a HID device (joystick), you have some
plugin glue-code that connects your favorite flight simulator to that
"joystick" and can now control all your radios and AP in real hardware.

Note: This is currently not functional! It's just very early development
releases. Also, some of the Cortex-M4 code for STM32F4 might be interesting to
someone.

