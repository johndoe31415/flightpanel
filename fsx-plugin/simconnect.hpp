#ifndef __SIMCONNECT_HPP__
#define __SIMCONNECT_HPP__

#include <stdbool.h>
#include <pthread.h>
#include <windows.h>
#include "SimConnect.h"
#include "fsconnection.hpp"

class SimConnectConnection : public FSConnection {
	private:
		HANDLE _simconnect_handle;
		bool _loop_running;
		pthread_t _periodic_query_thread;

	public:
		SimConnectConnection();
		void event_loop();
		HANDLE get_simconnect_handle(void) const {
			return _simconnect_handle;
		}
		bool connected(void) const {
			return get_simconnect_handle() != NULL;
		}
		void set_quit() {
			_loop_running = false;
		}
		bool is_loop_running() const {
			return _loop_running;
		}
		~SimConnectConnection();
};

#endif
