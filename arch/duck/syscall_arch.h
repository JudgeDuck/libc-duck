#include "syscall_duck.h"

static inline long __syscall0(long n) {
	return __syscall_duck(n, 0, 0, 0, 0, 0, 0);
}

static inline long __syscall1(long n, long a1) {
	return __syscall_duck(n, a1, 0, 0, 0, 0, 0);
}

static inline long __syscall2(long n, long a1, long a2) {
	return __syscall_duck(n, a1, a2, 0, 0, 0, 0);
}

static inline long __syscall3(long n, long a1, long a2, long a3) {
	return __syscall_duck(n, a1, a2, a3, 0, 0, 0);
}

static inline long __syscall4(long n, long a1, long a2, long a3, long a4) {
	return __syscall_duck(n, a1, a2, a3, a4, 0, 0);
}

static inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5) {
	return __syscall_duck(n, a1, a2, a3, a4, a5, 0);
}

static inline long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6) {
	return __syscall_duck(n, a1, a2, a3, a4, a5, a6);
}

#define __SYSCALL_LL_E(x) \
((union { long long ll; long l[2]; }){ .ll = x }).l[0], \
((union { long long ll; long l[2]; }){ .ll = x }).l[1]
#define __SYSCALL_LL_O(x) __SYSCALL_LL_E((x))

#define VDSO_USEFUL
#define VDSO_CGT_SYM "__vdso_clock_gettime"
#define VDSO_CGT_VER "LINUX_2.6"

#define SYSCALL_USE_SOCKETCALL
