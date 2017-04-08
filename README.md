flightpanel
===========

This project is a flight panel that I'm building. It's a USB device that's
based on the STM32F4-DISCOVERY board (around $20) which features the
STM32F407VG Cortex-M4 processor. From the PC side it shall be detected as a
joystick and give the user radio panel controls (i.e., VHF1, VHF2, NAV1, NAV2
frequencies, AP and transponder functionality).  The idea is that you plug the
device in via USB, it gets detected as a HID device (joystick), you have some
plugin glue-code that connects your flight simulator to that "joystick" to
control all your radios and AP in real hardware. Currently Microsoft FSX and
X-Plane 11 are supported (via SimConnect or XSquawkBox, respectively).

Note: This is currently not functional! It's just very early development
releases. Also, some of the Cortex-M4 code for STM32F4 might be interesting to
someone.

Building the microcontroller firmware
=====================================
To build the firmware, you first need the following dependencies:

  * [STM32 F4 DSP and Standard Peripheral Library (STSW-STM32065)](http://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html)
  * [STM32Cube Embedded Software (STM32CubeF4)](http://www.st.com/en/embedded-software/stm32cube-embedded-software.html)

I've used the Standard Library v1.8.0 and Cube FW v1.15.0 with the following MD5SUMs:

```
c876e4a7fe8e499a47d3854fb86305f4  en.stm32f4_dsp_stdperiph_lib.zip
d3535e7ae5782cb83df6e48a52688810  en.stm32cubef4.zip
```

After downloading, copy the ZIP files into the firmware/ subdirectory and
execute the bootstraping script. It will copy some files, patch them slightly
and create all symlinks you need for building properly.

```
$ ./bootstrap.sh
```

Then, simply "make" the ext-st/ and cube/ subdirectories and you should be able
to build the firmware of flightpanel.


Building the flight simulator backend 
=====================================
For this, you also need some external dependencies that I cannot include here.
For FSX, you need the SimConnect SDK. This is included in the Deluxe version of
FSX or the necessary files can also be downloaded as part of the SP1 SDK, which
Microsoft distributes as a ZIP file. To build, you only need SimConnect.h and
SimConnect.lib. The DLL that is required to run should always be installed with
every version of FSX.

To build with X-Plane 11, you need XSquawkBox (the X-Plane SDK). Download the
latest version (SDK v2).

Links:

	* [Microsoft FSX: SimConnect SDK](http://www.fsdeveloper.com/wiki/index.php?title=SDK_Installation_(FSX))
	* [X-Plane: XSquawkBox SDK](http://www.xsquawkbox.net/xpsdk/mediawiki/Download)

Place the plugin files in the plugin/sdk-simconnect or plugin/xsquawkbox
subdirectory (depending if you want to build for FSX or for X-Plane). Then,
bootstrap hidapi by calling

```
$ ./bootstrap-hidapi.py
```

This will compile hidapi for Linux and also cross-compile the library for
Windows (make sure you have the i686-w64-mingw32-gcc installed for this). It's
used for USB communication with the flight panel. Then, decide for one variant:
linux-xplane, linux-emulator and windows-fsx are the possible options.  Set the
VARIANT environment variable appropriately. Also remember that if you want to
build the FSX plugin to have the MinGW cross-compiler ready. Then, simply build
using make:

```
$ export VARIANT=linux-xplane
$ make
```

