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

void sigchld_handler(int signo, siginfo_t *info, void* context)
{
	int status;    
	while(waitpid(-1,&status,WNOHANG)>0)        
    if(WIFEXITED(status))
	{
		 if(WEXITSTATUS(status) == 0)
			printf("precess with id: %d succefully finished",info->si_pid);
         else
             printf("precess with id: %d finished with error %d",info->si_pid,WEXITSTATUS(status));			
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
                int fd_output = open(filename, O_WRONLY | O_CREAT);
                if (fd_output == -1) {
                    fprintf(stderr, "Couldn't open %s, %s\n", filename, strerror(errno));
                    continue;
                }


                int fd_dev_null = open("/dev/null", O_WRONLY | O_CREAT);
                if (fd_dev_null == -1) {
                    fprintf(stderr, "Couldn't open /dev/null, %s\n", filename, strerror(errno));
                    continue;
                }
                dup2(fd_dev_null, 2);
                execlp("curl", "curl", line, NULL);
            }
        }
    }
    if(fd_urls != stdin)
        fclose(fd_urls);
    return (0);
}

