#ifndef __INSTRUMENTS_H__
#define __INSTRUMENTS_H__

#define VHF_BASE_FREQUENCY			118000
#define VHF_DETENT					25
#define VHF_BANDWIDTH_DETENTS		(19 * 40)

#define NAV_BASE_FREQUENCY			108000
#define NAV_DETENT					50
#define NAV_BANDWIDTH_DETENTS		(10 * 20)

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
struct instrument_state;
struct flight_data;
void hid_tick(void);
void instruments_idle_loop(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
