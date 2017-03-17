#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "rs232.h"

extern uint8_t _ebss;
static uint8_t *current_break = &_ebss;
struct _reent;

void *_sbrk(intptr_t increment) {
	void *retval = current_break;
	current_break += increment;
	return retval;
}

void _exit(int status) {
	while (true);
}

ssize_t _write_r(struct _reent *reent, int fd, const void *data, size_t length) {
	if (fd == STDOUT_FILENO) {
		for (size_t i = 0; i < length; i++) {
			char c = ((char*)data)[i];
			if (c == '\n') {
				rs232_transmitchar('\r');
			}
			rs232_transmitchar(c);
		}
		return length;
	}
	return 0;
}

int _close_r(struct _reent *reent, int fd) {
	return 0;
}

int _fstat_r(struct _reent *reent, int fd, struct stat *stat) {
	return -1;
}

int _isatty_r(struct _reent *reent, int fd) {
	return 0;
}

off_t _lseek_r(struct _reent *reent, int fd, off_t offset, int whence) {
	return 0;
}

ssize_t _read_r(struct _reent *reent, int fd, const void *data, size_t length) {
	return 0;
}

