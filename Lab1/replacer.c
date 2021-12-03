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

    // **** Check read access ****
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

    // **** Check write access **** 
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
    d_fd = open(argv[2], O_RDWR);  
    if(d_fd < 0){  
        printf("open() failed with error [%s]\n",strerror(errno));  
        perror("open");
        return 1;  
    }  
    else  
        printf("open() Successful: Destination File Opened for Writing.\n");

    // **** Replace 1s with Ls and Copy to Destination ****
    // Set up buffer to read 100 characters at a time
    char buf[100];
    // Initialize first 100 or less bytes read to bytes_read variable
    int bytes_read = read(s_fd, buf, 100);

    // Run through each byte read and replace any "1"s with "L"s
    for (int i = 0; i < bytes_read; i++)
        if (buf[i] == '1') buf[i] = 'L';
    // Attempt to write, deliver error message if fail
    returnval = write(d_fd, buf, bytes_read);
    if (returnval < 0){
        printf("Error in writing: \n");
        perror("write");
    }
    returnval = write(d_fd, "XYZ", 3);
    if (returnval < 0){
        printf("Error in writing: \n");
        perror("write");
    }

    // Repeat previous process for remaining characters in file:
    // Replace "1"s with "L"s and copy over to destination file
    while(bytes_read > 0){
        bytes_read = read(s_fd, buf, 100);
        if (bytes_read > 0){
            for (int i = 0; i < bytes_read; i++){
                if (buf[i] == '1')
                    buf[i] = 'L';
            }
            // Attempt to write, deliver error message if fail
            returnval = write(d_fd, buf, bytes_read);
            if (returnval < 0){
                printf("Error in writing: \n");
                perror("write");
            }
            returnval = write(d_fd, "XYZ", 3);
            if (returnval < 0){
                printf("Error in writing: \n");
                perror("write");
            }
        }
    }

    // Attempt to close both files, handle any closing erros
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