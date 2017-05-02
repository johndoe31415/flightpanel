#ifndef __ARBITER_HPP__
#define __ARBITER_HPP__

#include "fsconnection.hpp"
#include "fpconnection.hpp"

class Arbiter {
	private:
		FSConnection *_fs_connection;
		FPConnection *_fp_connection;
		struct instrument_data_t _last_fs_data, _last_fp_data;

		struct instrument_data_t _put_fs_data, _put_fp_data;
		struct component_selection_t _put_fs_selection, _put_fp_selection;

		void arbitrate(const struct instrument_data_t &new_fs_data, const struct instrument_data_t &new_fp_data);
		template<typename T> void arbitrate_value(const struct instrument_data_t &new_fs_data, const struct instrument_data_t &new_fp_data, const struct arbitration_t &entry);
	public:
		Arbiter(FSConnection *fs_connection, FPConnection *fp_connection);
		void run();
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
