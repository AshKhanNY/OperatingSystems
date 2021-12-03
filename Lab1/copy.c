#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>
#include <string.h>  
#include <errno.h>  

int main(int argc, char *argv[])  
{  
    // Prevents unneccessary number of arguments
    if (argc != 3) {
        printf("You need 2 arguments only (source and destination)\n");
        return 1;
    }

    // Set file descriptors for source and destination files
    int s_fd, d_fd;
    // Set buffers for source and destination files
    char* source_f = argv[1];  
    char* destin_f = argv[2];

    // **** Check read access of source ****
    int returnval = access(source_f, R_OK);  
    if (returnval == 0)  
        printf ("%s can be read.\n", source_f);  
    else  
    {  
        // If file cannot be accessed, print error (based on errno)
        if (errno == ENOENT)  
            printf ("%s does not exist.\n", source_f);  
        else if (errno == EACCES)  
            printf ("%s is not readable.\n", source_f);  
        return 0;  
    }  

    // Attempt to open source file for reading purposes
    s_fd = open(argv[1], O_RDONLY);  
    if(s_fd < 0){  
        printf("open() failed with error [%s]\n",strerror(errno));  
        perror("open"); 
        return 1;  
    }  
    else  
        printf("open() Successful: Source File Opened for Reading.\n");

    // **** Check write access of destination **** 
    returnval = access(destin_f, W_OK);  
    if (returnval == 0)  
        printf ("%s can be written.\n", destin_f);  
    else  
    {  
        // If file cannot be accessed, print error (based on errno)
        if (errno == ENOENT)  
            printf ("%s does not exist.\n", destin_f);  
        else if (errno == EACCES)  
            printf ("%s is not readable.\n", destin_f);  
        return 0;  
    }  

    // Attempt to open destination file for writing
    d_fd = open(argv[2], O_WRONLY);  
    if(d_fd < 0){  
        printf("open() failed with error [%s]\n",strerror(errno));
        perror("open");  
        return 1;  
    }  
    else  
        printf("open() Successful: Destination File Opened for Writing.\n");

    // **** Copy from Source to Destination ****
    // Set up buffer and bytes_read variable
    char* buf;
    int bytes_read;
    // Copy one byte at a time from source to destination
    while(1){
        bytes_read = read(s_fd, &buf, 1);
        if (bytes_read != 1) break;
        write(d_fd, &buf, bytes_read);
    }

    // Attempt to close source and destination files 
    int close_val = close(s_fd);
    if (close_val < 0){
        printf("close() failed with error [%s]\n",strerror(errno)); 
        perror("close"); 
        return 1; 
    }
    else  
        printf("Source File Closed Successfully.\n");

    close_val = close(d_fd);
    if (close_val < 0){
        printf("close() failed with error [%s]\n",strerror(errno)); 
        perror("close"); 
        return 1; 
    }
    else  
        printf("Destination File Closed Successfully.\n");

    return 0; 
}