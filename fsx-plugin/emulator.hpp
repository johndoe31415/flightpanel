#ifndef __EMULATOR_HPP__
#define __EMULATOR_HPP__

#include <stdbool.h>
#include <pthread.h>
#include "fsconnection.hpp"

class EmulatedConnection : public FSConnection {
	private:
		bool _loop_running;
		struct instrument_data_t _instr;

		void randomize(struct vhf_data_t &vhf);
		void randomize(struct nav_data_t &nav);

	public:
		EmulatedConnection();
		void event_loop();
		~EmulatedConnection();
};

#endif
