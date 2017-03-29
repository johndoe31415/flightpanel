#ifndef __PINMAP_H__
#define __PINMAP_H__

#include <stm32f4xx_gpio.h>

// PA0: UserButton (IN_DN)
#define UserButton_GPIO GPIOA
#define UserButton_PinSource GPIO_PinSource0
#define UserButton_Pin GPIO_Pin_0
#define UserButton_SetHIGH() UserButton_GPIO->BSRRL = UserButton_Pin
#define UserButton_SetLOW() UserButton_GPIO->BSRRH = UserButton_Pin
#define UserButton_SetTo(value) if (value) { UserButton_SetHIGH(); } else { UserButton_SetLOW(); }

// PA4: Audio_WS (?)
#define Audio_WS_GPIO GPIOA
#define Audio_WS_PinSource GPIO_PinSource4
#define Audio_WS_Pin GPIO_Pin_4
#define Audio_WS_SetHIGH() Audio_WS_GPIO->BSRRL = Audio_WS_Pin
#define Audio_WS_SetLOW() Audio_WS_GPIO->BSRRH = Audio_WS_Pin
#define Audio_WS_SetTo(value) if (value) { Audio_WS_SetHIGH(); } else { Audio_WS_SetLOW(); }

// PA5: MEMS_SCK (OUT)
#define MEMS_SCK_GPIO GPIOA
#define MEMS_SCK_PinSource GPIO_PinSource5
#define MEMS_SCK_Pin GPIO_Pin_5
#define MEMS_SCK_SetHIGH() MEMS_SCK_GPIO->BSRRL = MEMS_SCK_Pin
#define MEMS_SCK_SetLOW() MEMS_SCK_GPIO->BSRRH = MEMS_SCK_Pin
#define MEMS_SCK_SetTo(value) if (value) { MEMS_SCK_SetHIGH(); } else { MEMS_SCK_SetLOW(); }

// PA6: MEMS_MISO (IN)
#define MEMS_MISO_GPIO GPIOA
#define MEMS_MISO_PinSource GPIO_PinSource6
#define MEMS_MISO_Pin GPIO_Pin_6
#define MEMS_MISO_SetHIGH() MEMS_MISO_GPIO->BSRRL = MEMS_MISO_Pin
#define MEMS_MISO_SetLOW() MEMS_MISO_GPIO->BSRRH = MEMS_MISO_Pin
#define MEMS_MISO_SetTo(value) if (value) { MEMS_MISO_SetHIGH(); } else { MEMS_MISO_SetLOW(); }

// PA7: MEMS_MOSI (OUT)
#define MEMS_MOSI_GPIO GPIOA
#define MEMS_MOSI_PinSource GPIO_PinSource7
#define MEMS_MOSI_Pin GPIO_Pin_7
#define MEMS_MOSI_SetHIGH() MEMS_MOSI_GPIO->BSRRL = MEMS_MOSI_Pin
#define MEMS_MOSI_SetLOW() MEMS_MOSI_GPIO->BSRRH = MEMS_MOSI_Pin
#define MEMS_MOSI_SetTo(value) if (value) { MEMS_MOSI_SetHIGH(); } else { MEMS_MOSI_SetLOW(); }

// PA9: USB_OTG_VBUS (IN)
#define USB_OTG_VBUS_GPIO GPIOA
#define USB_OTG_VBUS_PinSource GPIO_PinSource9
#define USB_OTG_VBUS_Pin GPIO_Pin_9
#define USB_OTG_VBUS_SetHIGH() USB_OTG_VBUS_GPIO->BSRRL = USB_OTG_VBUS_Pin
#define USB_OTG_VBUS_SetLOW() USB_OTG_VBUS_GPIO->BSRRH = USB_OTG_VBUS_Pin
#define USB_OTG_VBUS_SetTo(value) if (value) { USB_OTG_VBUS_SetHIGH(); } else { USB_OTG_VBUS_SetLOW(); }

// PA10: USB_OTG_ID (IN)
#define USB_OTG_ID_GPIO GPIOA
#define USB_OTG_ID_PinSource GPIO_PinSource10
#define USB_OTG_ID_Pin GPIO_Pin_10
#define USB_OTG_ID_SetHIGH() USB_OTG_ID_GPIO->BSRRL = USB_OTG_ID_Pin
#define USB_OTG_ID_SetLOW() USB_OTG_ID_GPIO->BSRRH = USB_OTG_ID_Pin
#define USB_OTG_ID_SetTo(value) if (value) { USB_OTG_ID_SetHIGH(); } else { USB_OTG_ID_SetLOW(); }

// PA11: USB_OTG_M (IO)
#define USB_OTG_M_GPIO GPIOA
#define USB_OTG_M_PinSource GPIO_PinSource11
#define USB_OTG_M_Pin GPIO_Pin_11
#define USB_OTG_M_SetHIGH() USB_OTG_M_GPIO->BSRRL = USB_OTG_M_Pin
#define USB_OTG_M_SetLOW() USB_OTG_M_GPIO->BSRRH = USB_OTG_M_Pin
#define USB_OTG_M_SetTo(value) if (value) { USB_OTG_M_SetHIGH(); } else { USB_OTG_M_SetLOW(); }

// PA12: USB_OTG_P (IO)
#define USB_OTG_P_GPIO GPIOA
#define USB_OTG_P_PinSource GPIO_PinSource12
#define USB_OTG_P_Pin GPIO_Pin_12
#define USB_OTG_P_SetHIGH() USB_OTG_P_GPIO->BSRRL = USB_OTG_P_Pin
#define USB_OTG_P_SetLOW() USB_OTG_P_GPIO->BSRRH = USB_OTG_P_Pin
#define USB_OTG_P_SetTo(value) if (value) { USB_OTG_P_SetHIGH(); } else { USB_OTG_P_SetLOW(); }

// PB0: Display_RESET (OUT)
#define Display_RESET_GPIO GPIOB
#define Display_RESET_PinSource GPIO_PinSource0
#define Display_RESET_Pin GPIO_Pin_0
#define Display_RESET_SetHIGH() Display_RESET_GPIO->BSRRL = Display_RESET_Pin
#define Display_RESET_SetLOW() Display_RESET_GPIO->BSRRH = Display_RESET_Pin
#define Display_RESET_SetTo(value) if (value) { Display_RESET_SetHIGH(); } else { Display_RESET_SetLOW(); }

// PB6: Audio_SCL (IO)
#define Audio_SCL_GPIO GPIOB
#define Audio_SCL_PinSource GPIO_PinSource6
#define Audio_SCL_Pin GPIO_Pin_6
#define Audio_SCL_SetHIGH() Audio_SCL_GPIO->BSRRL = Audio_SCL_Pin
#define Audio_SCL_SetLOW() Audio_SCL_GPIO->BSRRH = Audio_SCL_Pin
#define Audio_SCL_SetTo(value) if (value) { Audio_SCL_SetHIGH(); } else { Audio_SCL_SetLOW(); }
// PB6: EEPROM_SCL (IO)
#define EEPROM_SCL_GPIO GPIOB
#define EEPROM_SCL_PinSource GPIO_PinSource6
#define EEPROM_SCL_Pin GPIO_Pin_6
#define EEPROM_SCL_SetHIGH() EEPROM_SCL_GPIO->BSRRL = EEPROM_SCL_Pin
#define EEPROM_SCL_SetLOW() EEPROM_SCL_GPIO->BSRRH = EEPROM_SCL_Pin
#define EEPROM_SCL_SetTo(value) if (value) { EEPROM_SCL_SetHIGH(); } else { EEPROM_SCL_SetLOW(); }

// PB9: Audio_SDA (IO)
#define Audio_SDA_GPIO GPIOB
#define Audio_SDA_PinSource GPIO_PinSource9
#define Audio_SDA_Pin GPIO_Pin_9
#define Audio_SDA_SetHIGH() Audio_SDA_GPIO->BSRRL = Audio_SDA_Pin
#define Audio_SDA_SetLOW() Audio_SDA_GPIO->BSRRH = Audio_SDA_Pin
#define Audio_SDA_SetTo(value) if (value) { Audio_SDA_SetHIGH(); } else { Audio_SDA_SetLOW(); }
// PB9: EEPROM_SDA (IO)
#define EEPROM_SDA_GPIO GPIOB
#define EEPROM_SDA_PinSource GPIO_PinSource9
#define EEPROM_SDA_Pin GPIO_Pin_9
#define EEPROM_SDA_SetHIGH() EEPROM_SDA_GPIO->BSRRL = EEPROM_SDA_Pin
#define EEPROM_SDA_SetLOW() EEPROM_SDA_GPIO->BSRRH = EEPROM_SDA_Pin
#define EEPROM_SDA_SetTo(value) if (value) { EEPROM_SDA_SetHIGH(); } else { EEPROM_SDA_SetLOW(); }

// PB10: Audio_CLK_IN (OUT)
#define Audio_CLK_IN_GPIO GPIOB
#define Audio_CLK_IN_PinSource GPIO_PinSource10
#define Audio_CLK_IN_Pin GPIO_Pin_10
#define Audio_CLK_IN_SetHIGH() Audio_CLK_IN_GPIO->BSRRL = Audio_CLK_IN_Pin
#define Audio_CLK_IN_SetLOW() Audio_CLK_IN_GPIO->BSRRH = Audio_CLK_IN_Pin
#define Audio_CLK_IN_SetTo(value) if (value) { Audio_CLK_IN_SetHIGH(); } else { Audio_CLK_IN_SetLOW(); }

// PB12: Display_DC (OUT)
#define Display_DC_GPIO GPIOB
#define Display_DC_PinSource GPIO_PinSource12
#define Display_DC_Pin GPIO_Pin_12
#define Display_DC_SetHIGH() Display_DC_GPIO->BSRRL = Display_DC_Pin
#define Display_DC_SetLOW() Display_DC_GPIO->BSRRH = Display_DC_Pin
#define Display_DC_SetTo(value) if (value) { Display_DC_SetHIGH(); } else { Display_DC_SetLOW(); }

// PB13: Display_SCK (OUT)
#define Display_SCK_GPIO GPIOB
#define Display_SCK_PinSource GPIO_PinSource13
#define Display_SCK_Pin GPIO_Pin_13
#define Display_SCK_SetHIGH() Display_SCK_GPIO->BSRRL = Display_SCK_Pin
#define Display_SCK_SetLOW() Display_SCK_GPIO->BSRRH = Display_SCK_Pin
#define Display_SCK_SetTo(value) if (value) { Display_SCK_SetHIGH(); } else { Display_SCK_SetLOW(); }

// PB14: Display_MISO (IN)
#define Display_MISO_GPIO GPIOB
#define Display_MISO_PinSource GPIO_PinSource14
#define Display_MISO_Pin GPIO_Pin_14
#define Display_MISO_SetHIGH() Display_MISO_GPIO->BSRRL = Display_MISO_Pin
#define Display_MISO_SetLOW() Display_MISO_GPIO->BSRRH = Display_MISO_Pin
#define Display_MISO_SetTo(value) if (value) { Display_MISO_SetHIGH(); } else { Display_MISO_SetLOW(); }

// PB15: Display_MOSI (OUT)
#define Display_MOSI_GPIO GPIOB
#define Display_MOSI_PinSource GPIO_PinSource15
#define Display_MOSI_Pin GPIO_Pin_15
#define Display_MOSI_SetHIGH() Display_MOSI_GPIO->BSRRL = Display_MOSI_Pin
#define Display_MOSI_SetLOW() Display_MOSI_GPIO->BSRRH = Display_MOSI_Pin
#define Display_MOSI_SetTo(value) if (value) { Display_MOSI_SetHIGH(); } else { Display_MOSI_SetLOW(); }

// PC0: USB_PowerSwitchOn (OUT)
#define USB_PowerSwitchOn_GPIO GPIOC
#define USB_PowerSwitchOn_PinSource GPIO_PinSource0
#define USB_PowerSwitchOn_Pin GPIO_Pin_0
#define USB_PowerSwitchOn_SetHIGH() USB_PowerSwitchOn_GPIO->BSRRL = USB_PowerSwitchOn_Pin
#define USB_PowerSwitchOn_SetLOW() USB_PowerSwitchOn_GPIO->BSRRH = USB_PowerSwitchOn_Pin
#define USB_PowerSwitchOn_SetTo(value) if (value) { USB_PowerSwitchOn_SetHIGH(); } else { USB_PowerSwitchOn_SetLOW(); }

// PC3: AudioPDM_OUT (IN)
#define AudioPDM_OUT_GPIO GPIOC
#define AudioPDM_OUT_PinSource GPIO_PinSource3
#define AudioPDM_OUT_Pin GPIO_Pin_3
#define AudioPDM_OUT_SetHIGH() AudioPDM_OUT_GPIO->BSRRL = AudioPDM_OUT_Pin
#define AudioPDM_OUT_SetLOW() AudioPDM_OUT_GPIO->BSRRH = AudioPDM_OUT_Pin
#define AudioPDM_OUT_SetTo(value) if (value) { AudioPDM_OUT_SetHIGH(); } else { AudioPDM_OUT_SetLOW(); }

// PC7: Audio_MCK (?)
#define Audio_MCK_GPIO GPIOC
#define Audio_MCK_PinSource GPIO_PinSource7
#define Audio_MCK_Pin GPIO_Pin_7
#define Audio_MCK_SetHIGH() Audio_MCK_GPIO->BSRRL = Audio_MCK_Pin
#define Audio_MCK_SetLOW() Audio_MCK_GPIO->BSRRH = Audio_MCK_Pin
#define Audio_MCK_SetTo(value) if (value) { Audio_MCK_SetHIGH(); } else { Audio_MCK_SetLOW(); }

// PC10: Audio_SCK (?)
#define Audio_SCK_GPIO GPIOC
#define Audio_SCK_PinSource GPIO_PinSource10
#define Audio_SCK_Pin GPIO_Pin_10
#define Audio_SCK_SetHIGH() Audio_SCK_GPIO->BSRRL = Audio_SCK_Pin
#define Audio_SCK_SetLOW() Audio_SCK_GPIO->BSRRH = Audio_SCK_Pin
#define Audio_SCK_SetTo(value) if (value) { Audio_SCK_SetHIGH(); } else { Audio_SCK_SetLOW(); }

// PC12: Audio_SD (?)
#define Audio_SD_GPIO GPIOC
#define Audio_SD_PinSource GPIO_PinSource12
#define Audio_SD_Pin GPIO_Pin_12
#define Audio_SD_SetHIGH() Audio_SD_GPIO->BSRRL = Audio_SD_Pin
#define Audio_SD_SetLOW() Audio_SD_GPIO->BSRRH = Audio_SD_Pin
#define Audio_SD_SetTo(value) if (value) { Audio_SD_SetHIGH(); } else { Audio_SD_SetLOW(); }

// PD1: Dbg1 (OUT)
#define Dbg1_GPIO GPIOD
#define Dbg1_PinSource GPIO_PinSource1
#define Dbg1_Pin GPIO_Pin_1
#define Dbg1_SetHIGH() Dbg1_GPIO->BSRRL = Dbg1_Pin
#define Dbg1_SetLOW() Dbg1_GPIO->BSRRH = Dbg1_Pin
#define Dbg1_SetTo(value) if (value) { Dbg1_SetHIGH(); } else { Dbg1_SetLOW(); }

// PD2: Dbg2 (OUT)
#define Dbg2_GPIO GPIOD
#define Dbg2_PinSource GPIO_PinSource2
#define Dbg2_Pin GPIO_Pin_2
#define Dbg2_SetHIGH() Dbg2_GPIO->BSRRL = Dbg2_Pin
#define Dbg2_SetLOW() Dbg2_GPIO->BSRRH = Dbg2_Pin
#define Dbg2_SetTo(value) if (value) { Dbg2_SetHIGH(); } else { Dbg2_SetLOW(); }

// PD3: Dbg3 (OUT)
#define Dbg3_GPIO GPIOD
#define Dbg3_PinSource GPIO_PinSource3
#define Dbg3_Pin GPIO_Pin_3
#define Dbg3_SetHIGH() Dbg3_GPIO->BSRRL = Dbg3_Pin
#define Dbg3_SetLOW() Dbg3_GPIO->BSRRH = Dbg3_Pin
#define Dbg3_SetTo(value) if (value) { Dbg3_SetHIGH(); } else { Dbg3_SetLOW(); }

// PD4: AudioReset (OUT)
#define AudioReset_GPIO GPIOD
#define AudioReset_PinSource GPIO_PinSource4
#define AudioReset_Pin GPIO_Pin_4
#define AudioReset_SetHIGH() AudioReset_GPIO->BSRRL = AudioReset_Pin
#define AudioReset_SetLOW() AudioReset_GPIO->BSRRH = AudioReset_Pin
#define AudioReset_SetTo(value) if (value) { AudioReset_SetHIGH(); } else { AudioReset_SetLOW(); }
// PD4: Dbg4 (OUT)
#define Dbg4_GPIO GPIOD
#define Dbg4_PinSource GPIO_PinSource4
#define Dbg4_Pin GPIO_Pin_4
#define Dbg4_SetHIGH() Dbg4_GPIO->BSRRL = Dbg4_Pin
#define Dbg4_SetLOW() Dbg4_GPIO->BSRRH = Dbg4_Pin
#define Dbg4_SetTo(value) if (value) { Dbg4_SetHIGH(); } else { Dbg4_SetLOW(); }

// PD5: USB_FS_OverCurrent (IN)
#define USB_FS_OverCurrent_GPIO GPIOD
#define USB_FS_OverCurrent_PinSource GPIO_PinSource5
#define USB_FS_OverCurrent_Pin GPIO_Pin_5
#define USB_FS_OverCurrent_SetHIGH() USB_FS_OverCurrent_GPIO->BSRRL = USB_FS_OverCurrent_Pin
#define USB_FS_OverCurrent_SetLOW() USB_FS_OverCurrent_GPIO->BSRRH = USB_FS_OverCurrent_Pin
#define USB_FS_OverCurrent_SetTo(value) if (value) { USB_FS_OverCurrent_SetHIGH(); } else { USB_FS_OverCurrent_SetLOW(); }

// PD9: Rotary1_Btn (IN_UP)
#define Rotary1_Btn_GPIO GPIOD
#define Rotary1_Btn_PinSource GPIO_PinSource9
#define Rotary1_Btn_Pin GPIO_Pin_9
#define Rotary1_Btn_SetHIGH() Rotary1_Btn_GPIO->BSRRL = Rotary1_Btn_Pin
#define Rotary1_Btn_SetLOW() Rotary1_Btn_GPIO->BSRRH = Rotary1_Btn_Pin
#define Rotary1_Btn_SetTo(value) if (value) { Rotary1_Btn_SetHIGH(); } else { Rotary1_Btn_SetLOW(); }

// PD10: Rotary1_A (IN_UP)
#define Rotary1_A_GPIO GPIOD
#define Rotary1_A_PinSource GPIO_PinSource10
#define Rotary1_A_Pin GPIO_Pin_10
#define Rotary1_A_SetHIGH() Rotary1_A_GPIO->BSRRL = Rotary1_A_Pin
#define Rotary1_A_SetLOW() Rotary1_A_GPIO->BSRRH = Rotary1_A_Pin
#define Rotary1_A_SetTo(value) if (value) { Rotary1_A_SetHIGH(); } else { Rotary1_A_SetLOW(); }

// PD11: Rotary1_B (IN_UP)
#define Rotary1_B_GPIO GPIOD
#define Rotary1_B_PinSource GPIO_PinSource11
#define Rotary1_B_Pin GPIO_Pin_11
#define Rotary1_B_SetHIGH() Rotary1_B_GPIO->BSRRL = Rotary1_B_Pin
#define Rotary1_B_SetLOW() Rotary1_B_GPIO->BSRRH = Rotary1_B_Pin
#define Rotary1_B_SetTo(value) if (value) { Rotary1_B_SetHIGH(); } else { Rotary1_B_SetLOW(); }

// PD12: LEDGreen (OUT)
#define LEDGreen_GPIO GPIOD
#define LEDGreen_PinSource GPIO_PinSource12
#define LEDGreen_Pin GPIO_Pin_12
#define LEDGreen_SetHIGH() LEDGreen_GPIO->BSRRL = LEDGreen_Pin
#define LEDGreen_SetLOW() LEDGreen_GPIO->BSRRH = LEDGreen_Pin
#define LEDGreen_SetTo(value) if (value) { LEDGreen_SetHIGH(); } else { LEDGreen_SetLOW(); }

// PD13: LEDOrange (OUT)
#define LEDOrange_GPIO GPIOD
#define LEDOrange_PinSource GPIO_PinSource13
#define LEDOrange_Pin GPIO_Pin_13
#define LEDOrange_SetHIGH() LEDOrange_GPIO->BSRRL = LEDOrange_Pin
#define LEDOrange_SetLOW() LEDOrange_GPIO->BSRRH = LEDOrange_Pin
#define LEDOrange_SetTo(value) if (value) { LEDOrange_SetHIGH(); } else { LEDOrange_SetLOW(); }

// PD14: LEDRed (OUT)
#define LEDRed_GPIO GPIOD
#define LEDRed_PinSource GPIO_PinSource14
#define LEDRed_Pin GPIO_Pin_14
#define LEDRed_SetHIGH() LEDRed_GPIO->BSRRL = LEDRed_Pin
#define LEDRed_SetLOW() LEDRed_GPIO->BSRRH = LEDRed_Pin
#define LEDRed_SetTo(value) if (value) { LEDRed_SetHIGH(); } else { LEDRed_SetLOW(); }

// PD15: LEDBlue (OUT)
#define LEDBlue_GPIO GPIOD
#define LEDBlue_PinSource GPIO_PinSource15
#define LEDBlue_Pin GPIO_Pin_15
#define LEDBlue_SetHIGH() LEDBlue_GPIO->BSRRL = LEDBlue_Pin
#define LEDBlue_SetLOW() LEDBlue_GPIO->BSRRH = LEDBlue_Pin
#define LEDBlue_SetTo(value) if (value) { LEDBlue_SetHIGH(); } else { LEDBlue_SetLOW(); }

// PE0: MEMS_INT1 (IN)
#define MEMS_INT1_GPIO GPIOE
#define MEMS_INT1_PinSource GPIO_PinSource0
#define MEMS_INT1_Pin GPIO_Pin_0
#define MEMS_INT1_SetHIGH() MEMS_INT1_GPIO->BSRRL = MEMS_INT1_Pin
#define MEMS_INT1_SetLOW() MEMS_INT1_GPIO->BSRRH = MEMS_INT1_Pin
#define MEMS_INT1_SetTo(value) if (value) { MEMS_INT1_SetHIGH(); } else { MEMS_INT1_SetLOW(); }

// PE1: MEMS_INT2 (IN)
#define MEMS_INT2_GPIO GPIOE
#define MEMS_INT2_PinSource GPIO_PinSource1
#define MEMS_INT2_Pin GPIO_Pin_1
#define MEMS_INT2_SetHIGH() MEMS_INT2_GPIO->BSRRL = MEMS_INT2_Pin
#define MEMS_INT2_SetLOW() MEMS_INT2_GPIO->BSRRH = MEMS_INT2_Pin
#define MEMS_INT2_SetTo(value) if (value) { MEMS_INT2_SetHIGH(); } else { MEMS_INT2_SetLOW(); }

// PE3: MEMS_I2C_SPI (OUT)
#define MEMS_I2C_SPI_GPIO GPIOE
#define MEMS_I2C_SPI_PinSource GPIO_PinSource3
#define MEMS_I2C_SPI_Pin GPIO_Pin_3
#define MEMS_I2C_SPI_SetHIGH() MEMS_I2C_SPI_GPIO->BSRRL = MEMS_I2C_SPI_Pin
#define MEMS_I2C_SPI_SetLOW() MEMS_I2C_SPI_GPIO->BSRRH = MEMS_I2C_SPI_Pin
#define MEMS_I2C_SPI_SetTo(value) if (value) { MEMS_I2C_SPI_SetHIGH(); } else { MEMS_I2C_SPI_SetLOW(); }

#endif
