#include "syscall_duck.h"
#include "syscall.h"

#include <string.h>
#include <sys/uio.h>
#include <sys/stat.h>

// import from judgeduck

#define stdin_content _ZN9judgeduck13stdin_contentE
#define stdin_size _ZN9judgeduck10stdin_sizeE

#define stdout_content _ZN9judgeduck14stdout_contentE
#define stdout_size _ZN9judgeduck11stdout_sizeE
#define stdout_max_size _ZN9judgeduck15stdout_max_sizeE

#define malloc_start _ZN9judgeduck12malloc_startE

#define contestant_done _ZN9judgeduck15contestant_doneE

extern void sys_quit_judge();
extern unsigned * volatile contestant_done;

extern char *stdin_content;
extern int stdin_size;

extern char *stdout_content;
extern int stdout_size;
extern int stdout_max_size;

static int stdin_pos;
static int stdout_pos;

extern void *malloc_start;

static void *duck_brk(void *addr) {
	if (!addr) {
		return malloc_start;
	}
	if (addr > malloc_start) {
		malloc_start = addr;
	}
	return malloc_start;
}

static size_t duck_read(int fd, char *buf, size_t len) {
	if (fd != 0) {
		return 0;
	}
	int ret = stdin_size - stdin_pos;
	if ((size_t) ret > len) {
		ret = len;
	}
	memcpy(buf, stdin_content + stdin_pos, ret);
	stdin_pos += ret;
	return ret;
}

static size_t duck_readv(int fd, const struct iovec *iov, int cnt) {
	size_t ret = 0;
	for (int i = 0; i < cnt; i++) {
		size_t tmp = duck_read(fd, iov[i].iov_base, iov[i].iov_len);
		if (tmp <= iov[i].iov_len) {
			ret += tmp;
		}
		if (tmp != iov[i].iov_len) {
			break;
		}
	}
	return ret;
}

static size_t duck_write(int fd, char *buf, size_t len) {
	if (fd != 1) {
		return 0;
	}
	int ret = stdout_max_size - stdout_pos;
	if ((size_t) ret > len) {
		ret = len;
	}
	memcpy(stdout_content + stdout_pos, buf, ret);
	stdout_pos += ret;
	if (stdout_pos > stdout_size) {
		stdout_size = stdout_pos;
	}
	return ret;
}

static size_t duck_writev(int fd, const struct iovec *iov, int cnt) {
	size_t ret = 0;
	for (int i = 0; i < cnt; i++) {
		size_t tmp = duck_write(fd, iov[i].iov_base, iov[i].iov_len);
		if (tmp <= iov[i].iov_len) {
			ret += tmp;
		}
		if (tmp != iov[i].iov_len) {
			break;
		}
	}
	return ret;
}

static int duck_fstat(int fd, struct stat *st) {
	memset(st, 0, sizeof(struct stat));
	if (fd == 0) {
		st->st_mode = S_IFREG | 0644;
		st->st_size = stdin_size;
		st->st_blksize = 4096;
		st->st_blocks = (stdin_size + 4095) / 4096;
		return 0;
	}
	if (fd == 1) {
		st->st_mode = S_IFREG | 0644;
		st->st_size = stdout_size;
		st->st_blksize = 4096;
		st->st_blocks = (stdout_size + 4095) / 4096;
		return 0;
	}
	return -1;
}

long __syscall_duck(long n, long a1, long a2, long a3, long a4, long a5, long a6) {
	switch (n) {
		case SYS_brk:
			return (long) duck_brk((void *) a1);
		case SYS_read:
			return duck_read((int) a1, (char *) a2, (int) a3);
		case SYS_readv:
			return duck_readv((int) a1, (const struct iovec *) a2, (int) a3);
		case SYS_write:
			return duck_write((int) a1, (char *) a2, (int) a3);
		case SYS_writev:
			return duck_writev((int) a1, (const struct iovec *) a2, (int) a3);
		case SYS_ioctl:
			return 0;
		case SYS_fstat:
			return duck_fstat((int) a1, (struct stat *) a2);
		case SYS_exit:
			*contestant_done = 1;
			sys_quit_judge();
			while (1);
		default:
			return -1;
	}
}
