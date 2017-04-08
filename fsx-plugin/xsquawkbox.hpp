#ifndef __XSQUAWKBOX_HPP__
#define __XSQUAWKBOX_HPP__

#include <stdbool.h>
#include <pthread.h>
#include "fsconnection.hpp"

class XSquawkBoxConnection : public FSConnection {
	private:
		bool _loop_running;
		pthread_t _periodic_query_thread;

	public:
		XSquawkBoxConnection();
		void event_loop();
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
