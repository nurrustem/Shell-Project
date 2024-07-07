#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

/*SHELL PROJECT by: Nur Rustempašić, Nedžla Šehović and Nejira Subašić
STRUCTURE:
1.INITIALISATION
2.LOOP
3.IN-BUILT COMMANDS
  a.MYLS
  b.MYPWD
  c.MYGREP
  d.MYCD
  e.COMMAND NOT SUPPORTED  

*/


int main(int argc, char *argv[], char *envp[]){
    
    /*...INITIALISATION...
    We initialise the tmp array which is used to store the input command*/
    char tmp[100];
    int c = 0;
    int index = 0;
    
    /*This will set the first character to null when we start, intialising and clearing the array before taking input. 
    We use this instead of bzero because tmp[0] = '\0' is standard C and it works better with checking if 
    the command input is supported later on.*/
    tmp[0] = '\0'; 

    /*...WHILE LOOP...
    This loop is used to read new commands again and again.*/
    while(c!=EOF){
        c = getchar(); 
        

    /*...IN-BUILT COMMANDS...
    Now we will be checking for the supported built-in commands with if else.
    We use strcmp to compare the string in the input to our command.
    Then we spawn a child process which executes the command.
    Parent waits until it is executed and output is returned.*/
        switch (c){
            case ' ':
            case '\n':
            tmp[index] = '\0';//this null-terminates the string
            
            /*...MYLS...
            Simply spawns child process and executes command.*/       
            if (strcmp(tmp, "myls")==0){ 
                pid_t pid = fork();

                if (pid == 0) { 
                    execlp("ls", "ls", (char *)NULL);
                    perror("exec");
                    exit(EXIT_FAILURE);
                }
                else if(pid>0){
                    wait(NULL);
                }
                else {
                    perror("fork");
                }
            }
            /*...MYPWD...
            Simply spawns child process and executes command.*/  
            else if (strcmp(tmp, "mypwd")==0){
                 pid_t pid = fork();
                 if (pid == 0) {
                    execlp("pwd", "pwd", (char *)NULL);
                    perror("exec");
                    exit(EXIT_FAILURE);
                }
                else if(pid>0){
                    wait(NULL);
                }
                else {
                    perror("fork");
                }
            }

            /*...MYGREP...
            User enters pattern, so we need to store it and use it in our execution.*/ 
            else if (strcmp(tmp, "mygrep")==0){
                //the user can input a pattern after using mygrep command
                char pattern[100];
                scanf("%s", pattern);
                printf("Mygrep is done executing");

                pid_t pid = fork();
                if (pid == 0) {
                    //it takes the pattern as input too
                    execlp("grep", "grep", pattern, "test.txt", (char *)NULL);
                    perror("exec");
                    exit(EXIT_FAILURE);
                }
                else if(pid>0){
                    wait(NULL);
                }
                else {
                    perror("fork");
                }
            }

            /*...MYCD...
            Since it is executed in current process we dont need fork.
            We can simply change directory using chdir.*/ 
            else if (strcmp(tmp, "mycd")==0){
                //
                scanf("%s", tmp);
                printf("Changing directory to: %s\n", tmp);

                if (chdir(tmp) != 0) {
                        perror("mycd");
                    } 
                    else {
                        printf("Directory changed \n");
                    }
            }
            
            /*...COMMAND NOT SUPPORTED...
            This is where we check our tmp[0] we implemented in the beginning of the loop.
            If it is unsupported command, it displays an error message.*/
            else{ 
                if(tmp[0] != '\0'){
                printf("This command is not supported: %s\n", tmp);
            }
            }

            //this resets the tmp and index for the next command
            tmp[0] = '\0';
            index = 0;
            break;


            default:
            strncat(tmp, (char*)&c, 1); //this appends the character to tmp
            index++;
            break;
        }
        
    }
    return 0;
}
