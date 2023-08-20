#include <signal.h> // sigaction, sigemptyset, sigaction
#include <errno.h> // errno
#include <stdio.h> // null, perror
#include <sys/wait.h> // WNOHANG
#include <stdlib.h> // exit
#include "childProcess.h"

/**
 * Lida da o sinal que o sistema envia quando os processos filhos param ou terminal
*/
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    /** Aguarda até que o processo filho morra */
    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


/**
 * Lida com os sinais recebidos pelo sistema operacional quando um processo filho encerra
*/
void handleChildProcessTermination() {
    struct sigaction signalAction;

    signalAction.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&signalAction.sa_mask);
    signalAction.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &signalAction, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}
