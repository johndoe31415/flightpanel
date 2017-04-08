#ifndef __XSQUAWKBOX_HPP__
#define __XSQUAWKBOX_HPP__

#include <stdbool.h>
#include <pthread.h>
#include <XPLMDataAccess.h>
#include "fsconnection.hpp"

struct vhf_dataref_t {
	XPLMDataRef frequency_active, frequency_standby;
};

struct nav_dataref_t {
	XPLMDataRef frequency_active, frequency_standby;
};

struct misc_dataref_t {
	XPLMDataRef barometer_setting;
	XPLMDataRef altitude;
	XPLMDataRef ias;
};

struct datarefs_t {
	struct vhf_dataref_t vhf1, vhf2;
	struct nav_dataref_t nav1, nav2;
	struct misc_dataref_t misc;
};

class XSquawkBoxConnection : public FSConnection {
	private:
		bool _loop_running;
		pthread_t _periodic_query_thread;
		struct datarefs_t _datarefs;
	public:
		XSquawkBoxConnection();
		void event_loop();
		void poll_data();
		bool connected(void) const {
			return true;
		}
		void set_quit() {
			_loop_running = false;
		}
		bool is_loop_running() const {
			return _loop_running;
		}
		~XSquawkBoxConnection();
};

#endif
