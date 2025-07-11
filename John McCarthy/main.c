#include <sys/prctl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <seccomp.h>
#include <time.h>
#include <signal.h>
#define buf_size 250

void handle_alarm(int sig) {
    exit(0);
}

void setup() {
	signal(SIGALRM, handle_alarm);
	setbuf(stdout, NULL);
	setbuf(stdin, NULL);
	setbuf(stderr, NULL);
}

void banner() {
    printf("Accessing high-performance compute resources...\n");
    printf("Session runtime is restricted to 15 seconds due to infrastructure load.\n");
}

pid_t pid;
void start_timer() {
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) {
        alarm(15);
        return;
    }

    if (prctl(PR_SET_PDEATHSIG, SIGKILL) == -1) {
        perror("prctl");
        exit(1);
    }

    for (int i = 0; i < 15; i++) {
        printf("[Timer] %d seconds of computation remaining...\n", 15 - i);
        sleep(1);
    }

    printf("[Timer] Time limit exceeded.\n");
    printf("[System] Your isolated environment is now terminating.\n");

    exit(0);
}

scmp_filter_ctx ctx;
void setup_seccomp() {
	ctx = seccomp_init(SCMP_ACT_KILL);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(chdir), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(chroot), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(brk), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(sigreturn), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(lseek), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigprocmask), 0);
    seccomp_load(ctx);
}

void *buf;
void read_buf() {
    buf = mmap(NULL, buf_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (buf == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("Input your program size\n");
    int program_size = 0;
    scanf("%d%*c", &program_size);
    if (program_size > buf_size) {
        program_size = buf_size;
    }
    buf = buf + 0x1000 - program_size;

    printf("Input your program\n");
    ssize_t n = read(0, buf, buf_size);
    if (n <= 0) {
        perror("read");
        exit(1);
    }
}

int main(void) {
	setup();
	banner();
    read_buf();
    start_timer();
    setup_seccomp();
    ((void(*)())buf)();

    while(1) {
        sleep(1);
    }
	return 0;
}
