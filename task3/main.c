/* 
 * File:   main.c
 * Author: andrew
 *
 * Created on 20 Апрель 2012 г., 2:32
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define PID_MAX 32768

/*
 * 
 */
FILE * fd_urls;
const char url_prefix[] = "http";
struct sigaction sa;
struct sigaction previoussa;

int fd[2];

void sigchld_handler(int signo, siginfo_t *info, void* context) {
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0)
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            write(fd[1], &exit_status, sizeof (exit_status));
            write(fd[1], &(info->si_pid), sizeof (pid_t));
        }
}

int main(int argc, char** argv) {
    char *line = NULL;
    ssize_t line_len;
    int url_number = 0;

    if (argc > 1) {
        fd_urls = fopen(argv[1], "rt");
        if (fd_urls == NULL) {
            fprintf(stderr, "Error in reading file with urls error: %s\n", strerror(errno));
            return 1;
        }
    } else {

        printf(" Please input URLs 1 per line:\n");
        fd_urls = stdin;
    }

    if (pipe(fd) == -1) {
        perror("Failed to create a pipe");
        fclose(fd_urls);
        return 1;
    }

    memset(&sa, 0, sizeof (struct sigaction));
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sigchld_handler;
    sigaction(SIGCHLD, &sa, &previoussa);


    while (getline(&line, &line_len, fd_urls) != -1) {
        if (memcmp(url_prefix, line, 4) == 0) {
            url_number++;
            int p = fork();
            if (p == -1) {
                fprintf(stderr, "can't fork: %s\n", strerror(errno));
            }
            if (p == 0) {
                char filename[10];
                sprintf(filename, "%d", url_number);
                int fd_output = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXO | S_IRWXG);
                if (fd_output == -1) {
                    fprintf(stderr, "Couldn't open %s, %s\n", filename, strerror(errno));
                    continue;
                }
                
                dup2(fd_output, 1);

                int fd_dev_null = open("/dev/null", O_WRONLY | O_CREAT);
                if (fd_dev_null == -1) {
                    fprintf(stderr, "Couldn't open /dev/null, %s\n", filename, strerror(errno));
                    continue;
                }
                dup2(fd_dev_null, 2);
                close(0);
                execlp("curl", "curl", line, NULL);
            }
        }
    }
    if (fd_urls != stdin)
        fclose(fd_urls);
    int i;
    for (i = 0; i < url_number; ++i) {
        int success;
        pid_t pid;
        if (read(fd[0], &success, sizeof (success)) < 0) {
            while(errno==EINTR)
            {
                if (read(fd[0], &success, sizeof (success)) > 0)
                    break;
            }
            if(errno > 0)
            fprintf(stderr, "Couldn't read status %s\n", strerror(errno));
        };
        if (read(fd[0], &pid, sizeof (pid)) < 0);
        {
            while(errno==EINTR)
            {
                if (read(fd[0], &pid, sizeof (pid)) > 0)
                    break;
            }            
            fprintf(stderr, "Couldn't read pid_t %s\n", strerror(errno));
        };
        if (success == 0) {
            printf("Download successful pid: %d\n", pid);
        } else {
            printf("Download failed pid: %d\n", pid);
        }
    }
    return (0);
}

