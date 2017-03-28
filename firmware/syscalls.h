#ifndef __SYSCALLS_H__
#define __SYSCALLS_H__

#include <stdio.h>
#include <sys/stat.h>

struct _reent;

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void *_sbrk(intptr_t increment);
void _exit(int status);
ssize_t _write_r(struct _reent *reent, int fd, const void *data, size_t length);
int _close_r(struct _reent *reent, int fd);
int _fstat_r(struct _reent *reent, int fd, struct stat *stat);
int _isatty_r(struct _reent *reent, int fd);
off_t _lseek_r(struct _reent *reent, int fd, off_t offset, int whence);
ssize_t _read_r(struct _reent *reent, int fd, const void *data, size_t length);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
