#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char *line = NULL;
    size_t bufsize = 0;
    pid_t pid, wpid;

    while (1) {
        printf(">>> ");
        getline(&line, &bufsize, stdin);
        line[strlen(line) - 1] = '\0';  
        
        if (strcmp(line, "exit") == 0) break; 


        pid = fork();
        if (pid == 0) {  
            //child
            char *argz[] = { line, NULL }; 
            if (execvp(line, argz) == -1) perror("nsh");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {  
            //Parent
            while ((wpid = waitpid(pid, NULL, 0)) > 0);
        } else { 
            perror("Fork failed");
        }
        wpid = waitpid(pid, NULL, 0);
    }
    free(line); 
    return 0;
}
