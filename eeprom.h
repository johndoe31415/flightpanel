#ifndef __EEPROM_H__
#define __EEPROM_H__

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
bool eeprom_write(uint16_t addr, const void *data, uint16_t length);
bool eeprom_read(uint16_t addr, void *data, uint16_t length);
bool eeprom_dump(int pagecnt);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
