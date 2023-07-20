/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/stat.h>

#define MAX_LINE 80 /* 80 chars per line, per command */


// int amp_check(char *args[], int size)
// {
//     int amp = 0;

//     if(*args[size-1]=='&'){
//         args[size-1] = NULL;
//         amp = 1;
//         size--;
//     }
//     return amp;
// }

// void gen_exec()



void execonly(char *args[], int len){
    
    pid_t child_pid;

    int amp_flag = 0;
    int l = 0; 

    if(*args[len-1] == '@')
    {
      
        printf("error");
    }
    else if(*args[len-1] == '&')
    {
        args[len-1] = NULL;
        l++;
        amp_flag=1;
        len--;
    }

    child_pid = fork();
    
    if(child_pid > 0){ 

    if (amp_flag == 0)
        {
            wait(NULL);
            l++;
        }
        return;
    }
    else if(child_pid == 0) {
        if(len > 2){
        
            if(strcmp(args[len- 2], "<" ) == 0){ 
                args[len - 2] = NULL;
                
                int in = open(args[len - 1], O_RDONLY);
                
                dup2(in, STDIN_FILENO);
                
                close(in);
            }
            else if(strcmp(args[len - 2], ">" ) == 0){
                args[len - 2] = NULL;

               if((args[len - 2]) != NULL){
                    printf("\n");
               }
                int out = open(args[len - 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                
                dup2(out, STDOUT_FILENO);
               
                close(out);
            }  
        }
        
        if (execvp(args[0], args) < 0){
            printf("\nFailed");
        }
        exit(1);

    }

}
    
    
 

 
void execforpipe(char *args[], char *argsPipe[], int flag){
    int fd[2];
    pid_t pid;

     
    if(pipe(fd) < 0){    
        return;
    }
    
    pid = fork();
    if(pid > 0){
        
        pid = fork();

        if(pid > 0){
            int status = 0;

            close(fd[0]);
            printf(" ");
            close(fd[1]);

            if(flag==0)
                waitpid(pid, &status, 0);
            return;
        }

        else if(pid == 0){
            dup2(fd[0], STDIN_FILENO);

            close(fd[1]);
            printf(" ");

            close(fd[0]);
            if(execvp(argsPipe[0], argsPipe) < 0){

                printf("\nFailed");
            }
            
            exit(1);
        }
        
        else {
         
            return;
        }
    }
    else if(pid == 0){
        dup2(fd[1], STDOUT_FILENO);
        
        close(fd[0]);

	printf("");
         
        close(fd[1]);
        
        if(execvp(args[0],args) < 0){
            printf("\nFailed");
        }
        exit(1);
    }
    
    else{

        return;
    }
}

void exec(char *args[], int lc)
{
    int ampersand=0; 
    int nemp = 0;
    int pipe1;

    int i = 0;
    for(i = 0; i < lc; i++){
        if(strcmp(args[i], "|") == 0)
        {
            pipe1 = i;
            nemp = 1;
        }
    }

    if (nemp == 0){

        pipe1 = -1;
    }

    if(pipe1 == -1){
        execonly(args, lc);
        return;
    } else {

	printf("knsfjkdn\n");
	int x = 0;
        char *arguments_p[MAX_LINE / 2 + 1];
        

        for(x = 0; x < lc - pipe1 - 1; x++)
        {
            printf("");
            arguments_p[x] = strdup(args[pipe1 + x +1]);
        }
	printf("litun");
       

        arguments_p[x] = NULL; 
	printf("this0\n");

        if(*arguments_p[x-1] == '@')
        {
            printf("error");
        }
        else if(*arguments_p[x-1] == '&')
        {
            ampersand = 1;
            arguments_p[x-1] = NULL; 
        }   
	printf("this\n");

        args[pipe1] = NULL;
        
        execforpipe(args, arguments_p,ampersand);
        return;
    }
}


int main(void)
{
    char History[100];
    History[0] = '\0';
    char *args[MAX_LINE/2 + 1]; /* command line (of 80) has max of 40 arguments */
    int should_run = 1;
    
        
        while (should_run)
        {  
            int pipe1 = 0;
            int c = 0;
            int sizeofinput = 0; 
            printf("osh>");
            int counter = 0;
            
            char inp[100];
            int last_ind = 0;
	    fflush(stdout);
	//	printf("dcddc");
            fgets(inp,MAX_LINE,stdin);

            // last_ind = strlen(inp)-1;
            // inp[last_ind] = '\0';
printf("dcddc");
            inp[strlen(inp)-1] = '\0';
	printf("dcddc");
            if(strcmp(inp, "exit") == 0)
            {
		printf("Heldfdrjskn");
                should_run = 0;
                // return 0;
            }
            else if(strcmp(inp, "!!") == 0)
            {
                if(strlen(History) == 0)
                {
                    printf("No commands in history\n");
                    // return -1;
                }
                else if(strlen(History) != 0)
                {
                    printf("%s\n", History);
                    strcpy(inp, History);
                }
                else
                {
                    printf("No commands in history\n");
                    // return -1;
                }
            }
		
            if(strlen(inp) > 0)
                {
                strcpy(History, inp);
                //parsing the command
        
                char *tok = strtok(inp," ");
                while(tok != NULL)
                {
                    args[counter++] = tok;
                    printf(" ");
                    tok = strtok(NULL, " ");
                }
                args[counter] = tok;

                exec(args, counter);
                //end parsing
                //args and counter
                // checkign if the command contains a pipe if yes it will return the position
        


                
                }


        
            /**
             * After reading user input, the steps are:
             * (1) fork a child process
             * (2) the child process will invoke execvp()
             * (3) if command includes &, parent and child will run concurrently
             */
        }
    
    return 0;
}

 

 
