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

Building
========
To build this, you first need the following dependencies:

  * STM32 F4 DSP and Standard Peripheral Library 
    http://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html
  * STM32 CubeF4 
    http://www.st.com/en/embedded-software/stm32cube-embedded-software.html

I've used the Standard Library v1.8.0 and Cube FW v1.15.0 with the following MD5SUMs:

c876e4a7fe8e499a47d3854fb86305f4  en.stm32f4_dsp_stdperiph_lib.zip
d3535e7ae5782cb83df6e48a52688810  en.stm32cubef4.zip

After downloading, copy the Stdperipheral library to the ext-st/ subdirectory
and the Cube library to cube/. First build ext-st:

$ cd ext-st
$ make setup
$ make

The "make setup" step is important, it will create necessary symlinks.
Afterwards, do the same for the cube library:

$ cd ../cube
$ make setup
$ make

Then you should be able to build flightpanel:

$ cd ..
$ make
