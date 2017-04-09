#ifndef __FPCONNECTION_HPP__
#define __FPCONNECTION_HPP__

#define USB_VID		0x0483
#define USB_PID		0x572b

struct fpconnection_t {
	bool running;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
struct fpconnection_t* flightpanel_init(void);
void flightpanel_close(struct fpconnection_t* context);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
