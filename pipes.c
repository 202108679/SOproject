#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_TUBENAMELEN 10
#define TOKEN_INCREASE 500

void write_pipe(int n, char tubes[][MAX_TUBENAMELEN], int token){
    int fd = open(tubes[n], O_WRONLY);
    write(fd, &token, sizeof(token));
    close(fd);
}

int read_pipe(int n, char tubes[][MAX_TUBENAMELEN]){
    int token;
    int fd = open(tubes[n], O_RDONLY);
    read(fd, &token, sizeof(token));
    close(fd);
    return token;
}

int main(int argc, char *argv[]){
    
    int n = atoi(argv[1]);
    float prob = atof(argv[2]);

    char tubes[n][MAX_TUBENAMELEN];

    pid_t pid;
    pid_t pids[n];

    for (int i = 1; i <= n; i++)
    {
        snprintf(tubes[i], sizeof(tubes[i]), "pipe%dto%d", i, i+1);
    }

    snprintf(tubes[n-1], sizeof(tubes[n-1]), "pipe%dto1", n);    

    for (int k = 0; k < n; k++)
    {
        mkfifo(tubes[k], 0666);
    }

    int token = 0;

    for (int i = 0; i < n; i++)
    {
        pid = fork();
        if (pid > 0)
        {
            pids[i] = pid;
            if(i == n-1) read_pipe(n-1, tubes);
        }
        else if (pid == 0) //children
        {
            
            token = i == 0 ? token : read_pipe(i-1, tubes);

            if((double)rand() / RAND_MAX <= prob){
                printf("[p%d] lock on token (val = %d)\n", i+1, token); 
                sleep(atoi(argv[3])); 
                printf("[p%d] unlock token\n",i+1, token); 
            }
            
            write_pipe(i,tubes,token+TOKEN_INCREASE);
            
            exit(0);
        }
        else
        {
            int status = 0;

            printf("Failed to fork child %d: ", i);
            for (int    j = 0; j < i; j++)
            {
                printf("Killing %d\n", pids[j]);
                kill(SIGTERM, pids[j]);
            }
            for (int j = 0; j < n; j++)
                unlink(tubes[j]);
            printf("Terminating!\n");
            
        }
    }
}