#define _GNU_SOURCE
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <ucontext.h>
#include <sys/mman.h>

static struct sigaction old_sigaction;

void signal_handler(int signum, siginfo_t *siginfo, void *context) {
    // 输出一些基本的调试信息
    fprintf(stderr, "Caught signal %d\n", signum);
    fprintf(stderr, "Faulting address: %p\n", siginfo->si_addr);

    // 试图获取一些关于上下文的信息，这部分是可选的
    ucontext_t *uc = (ucontext_t *)context;
    #if defined(__x86_64__) // 仅适用于x86_64架构
    fprintf(stderr, "Instruction pointer: %p\n", (void*)uc->uc_mcontext.gregs[REG_RIP]);
    #endif

    // 打印一条信息并尝试以非零状态退出
    fprintf(stderr, "Unable to continue execution after segmentation fault.\n");
    
    // 这里调用旧的信号处理函数是可选的，取决于你是否想要调用默认的处理程序
    // 如果你不希望程序终止，可以注释掉下面的代码行
    // if (old_sigaction.sa_handler) {
    //     old_sigaction.sa_handler(signum);
    // }

    // _exit(1); // 直接退出程序，避免继续执行可能导致的不确定行为
}

__attribute__((constructor))
void init() {

    struct sigaction new_sigaction;
    memset(&new_sigaction, 0, sizeof(new_sigaction));
    memset(&old_sigaction, 0, sizeof(old_sigaction));
    new_sigaction.sa_sigaction = signal_handler;
    new_sigaction.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
    
    sigemptyset(&new_sigaction.sa_mask);

    sigaction(SIGSEGV, &new_sigaction, &old_sigaction);
    // sigaction(SIGTRAP, &new_sigaction, &old_sigaction);
}