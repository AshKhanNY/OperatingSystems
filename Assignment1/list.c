#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <string.h>  
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>  
#include <errno.h>  

int main(int argc, char* argv[])
{
	// Prevents unneccessary number of arguments
    if (argc != 1) {
        printf("You don't need additional arguments.\n");
        return 1;
    }

    // Set up pipe for two child processes
    int p[2];
    if (pipe(p) < 0){
        printf("pipe() failed with error:\n");
        perror("pipe");
        return 1;  
    }

    // Set up two child processes
    pid_t c1, c2;
    (c1 = fork()) && (c2 = fork()); // Creates 2 children at same time

    if (c1 < 0 || c2 < 0){ // Check if there was an error while forking
        printf("There was an error while forking.\n");
        perror("fork");
        return 1;
    }

    // Initialize two variables for error handling and status
    int e, status;
    if (c1 == 0) {
        /* Child 1 code goes here */
        // This child is responsible for performing the "ls -F"
        // function and passing the output to the next child
        dup2(p[1], STDOUT_FILENO); // Sets up writing end of pipe
        // Close both ends of the pipe to ensure unnecessary waiting
        close(p[0]);
        close(p[1]);
        // Execute command "ls -F"
        e = execl("/bin/ls", "ls", "-F", NULL);
        if (e < 0){ // Handles error if execl fails
            printf("Error when executing \"ls -F\":");
            perror("execl");
            return 1;
        }
        // Exit early after child finishes process
        exit(0);

    } else if (c2 == 0) {
        /* Child 2 code goes here */
        // This child is responsible for taking the input from first child,
        // which is the result of performing "ls -F" and using it w/ the
        // command "nl" so it can output all file contents in numbered list
        dup2(p[0], STDIN_FILENO); // Sets up reading end of pipe
        // Close both ends of the pipe to ensure unnecessary waiting
        close(p[1]);
        close(p[0]);
        // Execute command "nl" on the output of "ls -F"
        e = execl("/bin/nl", "nl", NULL);
        if (e < 0){ // Handles error if execl fails
            printf("Error when executing \"nl\":");
            perror("execl");
            return 1;
        }
        // Exit early after child finishes process
        exit(0);
    } else {
        /* Parent code goes here */
        pid_t pid;
        // Close pipes to avoid standby
        close(p[0]);
        close(p[1]);
        printf("In the parent process, waiting on children...\n");
        while ((pid = wait(&status)) >= 0){ // Wait for all children to finish
            printf("Child w/ pid %d has finished.\n", pid);
            fflush(stdout);
        }
        printf("Parent finished processing.\n");
    }
    return 0;
}