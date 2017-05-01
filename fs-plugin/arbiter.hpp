#ifndef __ARBITER_HPP__
#define __ARBITER_HPP__

#include "fsconnection.hpp"
#include "fpconnection.hpp"

class Arbiter {
	private:
		FSConnection *_fs_connection;
		FPConnection *_fp_connection;

	public:
		Arbiter(FSConnection *fs_connection, FPConnection *fp_connection);
		void run();
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
