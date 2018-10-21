#include <elf.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include "syscall.h"
#include "atomic.h"
#include "libc.h"
#include "pthread_impl.h"

__thread int dummy_duck_957e81a1d6064bf60bbead450ccc3287;

__attribute__((__weak__, __visibility__("hidden")))
extern void (*const __init_array_start)(void), (*const __init_array_end)(void);

int __dso_handle() {return 0;}

#define AUX_CNT 38

int init_tp(void *p)
{
	pthread_t td = p;
	td->self = td;
	libc.can_do_threads = 0;  // No threads on duck
	td->detach_state = DT_JOINABLE;
	td->tid = 0;
	td->detach_state = 0;
	td->locale = &libc.global_locale;
	td->robust_list.head = &td->robust_list.head;
	return 0;
}

static void init_libc(char **envp, char *pn)
{
	size_t i, *auxv, aux[AUX_CNT] = { 0 };
	__environ = envp;
	for (i=0; envp[i]; i++);
	libc.auxv = auxv = (void *)(envp+i+1);
	for (i=0; auxv[i]; i+=2) if (auxv[i]<AUX_CNT) aux[auxv[i]] = auxv[i+1];
	__hwcap = 0;
	__sysinfo = 0;
	libc.page_size = PAGE_SIZE;

	if (!pn) pn = "";
	__progname = __progname_full = pn;
	for (i=0; pn[i]; i++) if (pn[i]=='/') __progname = pn+i+1;

	init_tp(__pthread_self());

	libc.secure = 1;
}

static void libc_start_init(void)
{
	uintptr_t a = (uintptr_t)&__init_array_start;
	for (; a<(uintptr_t)&__init_array_end; a+=sizeof(void(*)()))
		(*(void (**)(void))a)();
}

static void libstdduck_init()
{
	char *envp[2] = {0, 0};

	init_libc(envp, "");
	libc_start_init();
}

void _Z15libstdduck_initv()
{
	libstdduck_init();
}
