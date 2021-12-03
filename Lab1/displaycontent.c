#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>
#include <string.h>  
#include <errno.h>  

int main(int argc, char *argv[])  
{  
    // Initialze file descriptor and file path variable
    int fd;
    char* filepath = argv[1];  
    
    // Check read access  
    int returnval = access(filepath, R_OK);  
    if (returnval == 0)  
        printf ("%s can be read.\n", filepath);  
    else  
    {  
        // If returnval is not 0, we check errno to see what
        // exactly is causing an issue. 
        if (errno == ENOENT)  
            printf ("%s does not exist.\n", filepath);  
        else if (errno == EACCES)  
            printf ("%s is not readable.\n", filepath);  
        return 0;  
    }  

    // Update file descriptor by attempting to open file 
    // in read/write mode
    fd = open(argv[1], O_RDONLY);  

    // Open function failed if file descriptor is negative integer.
    // Print error message to user using the status of errno
    if(fd < 0){  
        printf("open() failed with error [%s]\n",strerror(errno));  
        perror("open");
        return 1;  
    }  
    else  
    {  
        printf("open() Successful: File Opened for Reading.\n");
    }

    /* Read file code: */
    // Initialize buffer to store file characters, and
    // bytes_read to store number of bytes read.
    char* buf;
    int bytes_read;

    // Run while loop over entire file until all bytes are read
    // (end of file can be approached when bytes_read is NOT 1)
    while(1){
        bytes_read = read(fd, &buf, 1);
        if (bytes_read != 1) break;
        // Write each character to stdout 
        write(1, &buf, bytes_read);
    }


    // Attempt to close file, store return value in close_val
    int close_val = close(fd);
    // Close function failed if close_val is negative integer.
    // Print error message to user using the status of errno
    if (close_val < 0){
        printf("close() failed with error [%s]\n",strerror(errno));  
        perror("close");
        return 1; 
    }
    else  
    {  
        // Successfully closed file.
        printf("File Closed Successfully.\n");
    }

    return 0;  
}