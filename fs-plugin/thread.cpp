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

#include <stdexcept>
#include <stdio.h>
#include <unistd.h>
#include "thread.hpp"

template<typename T> void *thread_trampoline(void *ctx) {
	T* t = (T*)ctx;
	t->thread_function();
	return NULL;
}

Thread::Thread(unsigned int thread_interval_millis) : _thread_running(false), _thread_interval_millis(thread_interval_millis) {
}

void Thread::start() {
	if (_thread_running) {
		throw std::runtime_error("Thread already running, cannot start again.");
	}
	_thread_running = true;
	pthread_create(&_thread, NULL, thread_trampoline<Thread>, this);
}

void Thread::thread_function() {
	while (_thread_running) {
		thread_action();
		sleep_millis(_thread_interval_millis);
	}
}

void Thread::stop() {
	if (_thread_running) {
		_thread_running = false;
		pthread_join(_thread, NULL);
	}
}

Thread::~Thread() {
	stop();
}
