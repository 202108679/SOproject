#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>



int main(int argc, const char *argv[]) {

    //fork & convertion

    for (int i = 1; i < argc;i++) {

        pid_t pid;
        if (( pid = fork() ) == -1) {           
            fprintf(stderr, "%s: could not fork command: %s\n", argv[0], strerror(errno));
            continue;
        } else 
        if(pid == 0){            
            printf("%d\n", getpid());
            const char* filenameText = argv[i];
            char fname[strlen(filenameText)];
            strcpy(fname, filenameText);
            fname[strlen(fname)-4] = '\0';
            strcat(fname, ".epub");
            printf("[pid%i] convertion of %s in process... \n", getpid(), filenameText);
            execlp("pandoc", "pandoc", filenameText, "-o", fname, (char *)0);
        } 
        else {  
            printf("%d\n", getpid());
            if (waitpid(pid, NULL, 0) == -1) {
                perror("wait");
                return EXIT_FAILURE;
            }                
        }
            //remove(argv[i]);
    }
    
    //memory allocation & zip

    char *zip[argc + 4];
    zip[0] = malloc((strlen("zip")+1)*sizeof(char));
    strcpy(zip[0], "zip");
    zip[1] = malloc((strlen("ebooks.zip")+1)*sizeof(char));
    strcpy(zip[1], "ebooks.zip");

    //to .epub

    for(int k = 0; k < argc - 1; k++){
        char dummy[sizeof(argv[k + 1]+1)] = {}, *var_01;
        strcpy(dummy, argv[k + 1]);
        var_01 = strchr(dummy, '.');
        if(var_01 != 0){*var_01 = 0;}
        strcat(dummy, ".epub");
        zip[k+2] = malloc((strlen(dummy)+1)*sizeof(char));
        strcpy(zip[k+2], dummy);
    }
    
    zip[argc+2] = "--quiet";
    zip[argc+3] = NULL;

    execvp(zip[0], zip);
}