#include <stdio.h>  /* IO */
#include <stdlib.h> /* exit, EXIT_FAILURE, EXIT_SUCCESS */
#include <unistd.h> /* fork, dup2, close, execve */
#include <wait.h>   /* wait */
#include <fcntl.h>  /* open */

int main(int argc, char *argv[], char *envp[]) {
    if (argc < 3) {
        puts("Wrong arguments given. Usage: 'launch <prog> <file>'.");
        exit(EXIT_FAILURE);
    }
    char *progname = argv[1];
    char *filename = argv[2];

    pid_t pid;
    pid = fork();
    /* Error */
    if (pid == -1) {
        perror("Error occurred while trying to fork process.");
        exit(EXIT_FAILURE);
    }
    /* Child */
    else if (pid == 0) { 
        int f;
        /* Redirecting stdout to file */
        if ((f = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH)) == -1) {
            perror("Unable to open new IO stream using given file name in child process.");
            exit(EXIT_FAILURE);
        }
        if (dup2(f, 1) == -1) {
            perror("Unable to redirect output in child process.");
            exit(EXIT_FAILURE);
        }
        close(f);
        /* argv + 2 => launch <prog> <file> <args> = prog <args> */
        argv[2] = progname;
        execve(progname, argv + 2, envp);
        /* In case exec did not start new prog in this process */
        perror("Failed launching program.");
        exit(EXIT_FAILURE);
    }
    /* Parent */
    else {
        /* pid var. here contains PID of child process */
        int res; /* Status code */
        pid_t cpid;
        cpid = wait(&res);
        if (cpid == pid) {
            printf("Child process with PID %d ended with return code %d.\n", pid, res);
        }
        else {
            /* It shouldn't be possible, but anyways */
            puts("Wrong child process ended.");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}