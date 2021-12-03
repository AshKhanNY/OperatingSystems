#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>
#include <string.h>  
#include <errno.h>  

int main(int argc, char *argv[])  
{  
    // Initialize file descriptor
    int fd;  

    // Prevents unnecessary number of arguments
    if(2 != argc){  
        printf("\n Usage : \n");  
        return 1;  
    }  
    errno = 0;  

    // Update file descriptor by attempting to open file 
    // in read/write mode
    fd = open(argv[1], O_RDWR|O_CREAT|O_TRUNC, 0664);  

    // Open function failed if file descriptor is negative integer.
    // Print error message to user using the status of errno
    if(fd < 0){  
        printf("\n open() failed with error [%s]\n",strerror(errno));  
        perror("open");
        return 1;  
    } else {  
        // open() succeeded, now one can do read and write operations 
        // on the file since we opened it in read-write mode. Also once
        // done with processing, the file needs to be closed. 
        printf("\n Open() Successful: File Created & Opened for Reading/Writing.\n");  
    }

    // Attempt to close file, store return value in close_val
    int close_val = close(fd);
    // Close function failed if close_val is negative integer.
    // Print error message to user using the status of errno
    if (close_val < 0){
        printf("\n close() failed with error [%s]\n",strerror(errno));  
        perror("close");
        return 1; 
    }
    else  
    {  
        // Successfully closed file.
        printf("\n File Closed Successfully.\n");
    }

    return 0;  
}