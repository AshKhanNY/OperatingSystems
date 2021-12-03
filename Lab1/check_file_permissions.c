#include <stdio.h>  
#include <unistd.h>  
#include <errno.h>  

int main (int argc, char* argv[])  
{  
	// Stores the filepath
	char* filepath = argv[1]; 

	int returnval;  

	// Check file existence  
	returnval = access (filepath, F_OK);  
	if (returnval == 0)  
		printf ("\n %s exists\n", filepath);  
	else  
	{
		// If returnval is not 0, we check errno to see what
		// exactly is causing an issue.  
		if (errno == ENOENT)  
			printf ("%s does not exist\n", filepath);  
		else if (errno == EACCES)  
			printf ("%s is not accessible\n", filepath);  
		return 0;  
	}  

	// Check read access  
	returnval = access (filepath, R_OK);  
	if (returnval == 0)  
		printf ("\n %s can be read\n", filepath);  
	else  
	{  
		// If returnval is not 0, we check errno to see what
		// exactly is causing an issue.  
		if (errno == ENOENT)  
			printf ("%s does not exist\n", filepath);  
		else if (errno == EACCES)  
			printf ("%s is not accessible\n", filepath);  
		return 0;  
	}  


	// Check write access  
	returnval = access (filepath, W_OK);  
	if (returnval == 0)  
		printf ("\n %s can be written\n", filepath);  
	else  
	{  
		// If returnval is not 0, we check errno to see what
		// exactly is causing an issue.  
		if (errno == ENOENT)  
			printf ("%s does not exist\n", filepath);  
		else if (errno == EACCES)  
			printf ("%s is not accessible\n", filepath);  
		return 0;  
	}  

	 
	return 0;  
} 