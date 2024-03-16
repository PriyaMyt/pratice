#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

static void sig(int signo) {
    printf("nano.c:sig : In function sig\n");
    if (signo == SIGINT) {
        if (signo == SIGINT) {
            printf("^C\n");
        }
        struct timespec rem;
        if (nanosleep(&rem, &rem) == -1) {
            if (errno == EINTR) {
                printf("nanosleep interrupted: rem time: %.9f\n",
                       (double)rem.tv_sec + (double)rem.tv_nsec / 1000000000.0);
            } else {
                perror("nanosleep");
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2 || argv[1][0] < '0' || argv[1][0] > '1') {
        printf("Usage: %s option=[0|1]\n", argv[0]);
        printf(" 0 : uses the sleep(3) function\n");
        printf(" 1 : uses the nanosleep(2) syscall\n");
        return 1;
    }

    signal(SIGINT, sig);  // Register signal handler for SIGINT

    if (argv[1][0] == '0') {
        // Use sleep(3)
        sleep(10);
    } else {
        // Use nanosleep(2)
        struct timespec t = {.tv_sec = 10, .tv_nsec = 0};
        while (nanosleep(&t, &t) == -1 && errno == EINTR);
    }

    return 0;
}
