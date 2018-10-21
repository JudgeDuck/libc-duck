#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "syscall.h"

#ifndef NULL
#define NULL 0
#endif

/*namespace judgeduck {
	extern void *malloc_start;
}*/

static long brk = 0;
static long malloc_pos = 0;

static void * do_malloc(size_t size) {
	if (size > 0x7fffffffu) {
		return NULL;
	}
	if (!brk) {
		brk = __syscall(SYS_brk, 0);
		brk += -brk & (PAGE_SIZE - 1);
		malloc_pos = brk;
	}
	void *ret = (void *) malloc_pos;
	malloc_pos += size;
	if (malloc_pos > brk) {
		long tmp = malloc_pos;
		tmp += -tmp & (PAGE_SIZE - 1);
		if (__syscall(SYS_brk, tmp) != tmp) {
			return NULL;
		}
		brk = tmp;
	}
	return ret;
	
	/*if (malloc_curpos == NULL) {
		malloc_curpos = judgeduck::malloc_start;
	}
	if (size > 0x7fffffffu) {
		return NULL;
	}
	void *ret = malloc_curpos;
	malloc_curpos = (char *) malloc_curpos + size;
	return ret;*/
	//return NULL;
}

void * malloc(size_t size) {
	if (size == 0) {
		return NULL;
	} else {
		return do_malloc(size);
	}
}

void * calloc(size_t n, size_t size) {
	if (n * size == 0) {
		return NULL;
	} else {
		void *ret = do_malloc(n * size);
		if (ret) {
			memset(ret, 0, n * size);
		}
		return ret;
	}
}

void * realloc(void *p, size_t size) {
	if (p == NULL) {
		return malloc(size);
	} else if (size == 0) {
		free(p);
		return NULL;
	} else {
		return NULL;
	}
}

void free(void *p) {
	return;
}
