#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

volatile sig_atomic_t interrupted = 0;

void sigint_handler(int signum) {
    interrupted = 1;
}

void sleep_using_sleep() {
    printf("Using sleep(3) function.\n");
    sleep(5);
}

void sleep_using_nanosleep() {
    printf("Using nanosleep(2) syscall.\n");
    struct timespec req, rem;
    req.tv_sec = 5;
    req.tv_nsec = 0;

    while (nanosleep(&req, &rem) && !interrupted) {
        printf("nanosleep interrupted: rem time: %ld.%09ld seconds\n", rem.tv_sec, rem.tv_nsec);
        req = rem;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s option=[0|1]\n0 : uses the sleep(3) function\n1 : uses the nanosleep(2) syscall\n", argv[0]);
        return 1;
    }

    int option = atoi(argv[1]);

    // Registering signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, sigint_handler);

    switch (option) {
        case 0:
            sleep_using_sleep();
            break;
        case 1:
            sleep_using_nanosleep();
            break;
        default:
            printf("Invalid option. Please choose 0 or 1.\n");
            return 1;
    }

    if (!interrupted) {
        printf("Sleep completed.\n");
    } else {
        printf("Sleep interrupted.\n");
    }

    return 0;
}
