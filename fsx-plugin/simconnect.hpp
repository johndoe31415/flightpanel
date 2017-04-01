#ifndef __SIMCONNECT_HPP__
#define __SIMCONNECT_HPP__

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void simconnect_instrument_to_abstract(const struct simconnect_instrument_data_t *in, struct instrument_data_t *out);
void simconnect_event_loop(void *hSimConnect);
void* simconnect_init(void);
void simconnect_close(void *hSimConnect);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
