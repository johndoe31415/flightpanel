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
		void randomize(struct adf_data_t &adf);
		void randomize(struct dme_data_t &dme);
		void randomize(struct light_data_t &lights);
		void randomize(struct ap_data_t &ap);
		void randomize(struct transponder_data_t &xpdr);
		void randomize(struct misc_data_t &misc);

	public:
		EmulatedConnection();
		bool connected(void) const {
			return true;
		}
		void event_loop();
		~EmulatedConnection();
};

#endif
