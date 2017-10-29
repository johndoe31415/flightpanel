/**
 *	flightpanel - A Cortex-M4 based USB flight panel for flight simulators.
 *	Copyright (C) 2017-2017 Johannes Bauer
 *
 *	This file is part of flightpanel.
 *
 *	flightpanel is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	flightpanel is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with flightpanel; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <pthread.h>
#include "osdeps.hpp"

class Lock {
	private:
		pthread_mutex_t _mutex;

	public:
		Lock() {
			pthread_mutex_init(&_mutex, NULL);
		}

		void lock() {
			//fprintf(stderr, "lock %p\n", &_mutex);
			pthread_mutex_lock(&_mutex);
		}

		void unlock() {
			//fprintf(stderr, "unlock %p\n", &_mutex);
			pthread_mutex_unlock(&_mutex);
		}

		~Lock() {
			pthread_mutex_destroy(&_mutex);
		}
};

class Event {
	private:
		pthread_mutex_t _mutex;
		pthread_cond_t _cond;
		bool _state;

	public:
		Event() {
			pthread_mutex_init(&_mutex, NULL);
			pthread_cond_init(&_cond, NULL);
			_state = false;
		}

		void set() {
			_state = true;
			pthread_cond_broadcast(&_cond);
		}

		void reset() {
			_state = false;
		}

		bool wait(unsigned int timeout_millis = 2000) {
			/* Separate millseconds and seconds */
			unsigned int timeout_seconds = timeout_millis / 1000;
			timeout_millis = timeout_millis % 1000;

			/* Add timeout to current time */
			struct timespec timeout;
			now(&timeout);

			timeout.tv_sec += timeout_seconds;
            timeout.tv_nsec += timeout_millis * 1000000;

            /* Wrap around nanoseconds */
            timeout.tv_sec += timeout.tv_nsec / 1000000000;
            timeout.tv_nsec = timeout.tv_nsec % 1000000000;

			bool success = true;
			pthread_mutex_lock(&_mutex);
			while (!_state) {
				if (pthread_cond_timedwait(&_cond, &_mutex, &timeout)) {
					/* Timeout */
					success = false;
					break;
				}
			}
			pthread_mutex_unlock(&_mutex);
			return success;
		}

		~Event() {
			pthread_cond_destroy(&_cond);
			pthread_mutex_destroy(&_mutex);
		}

};

class LockGuard {
	private:
		Lock &_lock;
		const char *_hint;

	public:
		LockGuard(Lock &lock, const char *hint = NULL) : _lock(lock), _hint(hint) {
			if (_hint) {
//				fprintf(stderr, "%s LOCK\n", _hint);
			}
			_lock.lock();
		}

		~LockGuard() {
			if (_hint) {
//				fprintf(stderr, "%s unlock\n", _hint);
			}
			_lock.unlock();
		}
};

class Thread {
	template<typename T> friend void *thread_trampoline(void *ctx);

	private:
		pthread_t _thread;
		bool _thread_running;
		unsigned int _thread_interval_millis;
		virtual void thread_action() = 0;
		void thread_function();

	protected:
		const bool thread_running() const {
			return _thread_running;
		}

	public:
		Thread(unsigned int thread_interval_millis);
		void start();
		void stop();
		virtual ~Thread();
};

#endif
