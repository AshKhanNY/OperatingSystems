/* ----- ASSIGNMENT 2 (SINGLE THREAD) -----
Develop a multithreaded app that can find the integer 
in the range 1 to 10000 that has the largest number 
of divisors. It should print that integer as well as 
the time spent to finish the calculation. You can 
use any language (e.g. C, Java, or Python), and Any 
API library.

This is the single (like me, hah) threaded version.

>>> HOW TO COMPILE AND RUN <<<

gcc -o single single.c -lm -lpthread
./single

Note: The reason why we do this is because we need access
to the math and pthread library directly (as far as I know...)
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#define MAX 10000
#define THREADS 1
int num_largest_divisors;

// Initialize "Range" data type
struct Range_s {
	int from;
	int to;
};
typedef struct Range_s Range;

// Initialize countDivisors helper function
int countDivisors(int n);
// Initialize numOfDivisors thread function
void *numOfDivisors(void *arg);

int main(int argc, char const *argv[])
{
	printf("Executing single threaded process...\n");
	// Initialize global variable, to be used across threads
	num_largest_divisors = 0;
	int i; // Initialze count variable

	// Set up multiple ranges and start threads for each
	pthread_t tid;
	Range range_array[THREADS];
	for (i = 0; i < THREADS; ++i){
		Range temp = {MAX/THREADS * i, MAX/THREADS * (i + 1)};
		range_array[i] = temp;
	}

	pthread_t theads[THREADS]; // Setting up array of threads
	clock_t start, end; // Setting up clocks to determine processing time
	start = clock();
	pthread_attr_t attr; 
    pthread_attr_init(&attr);
	// Running multiple threads and joining them after they've completed
	for (i = 0; i < THREADS; ++i) pthread_create(&theads[i], &attr, numOfDivisors, (void *)&range_array[i]);
	for (i = 0; i < THREADS; ++i) pthread_join(theads[i], NULL);
	end = clock();
	// Calculate how much time passed in seconds
	double time_elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

	// Print number with largest number of divisors
	printf("Integer w/ most divisors: %d\n", num_largest_divisors);
	printf("Time taken to process with single thread: %f seconds\n", time_elapsed);
	return 0;
}

// Helper function to count number of divisors for a number using
// sqrt of the number
int countDivisors(int n){
	int i, count = 0;
	for (i = 1; i <= n; ++i){
		if (n % i == 0)
			count = (n / i == i) ? count + 1 : count + 2;
	}
	return count;
}

// Thread function to keep track of number within a particular
// range that has the highest number of divisors
void *numOfDivisors(void *arg){
	Range *r = (Range *)arg;
	int div, i, max_int = 1, max_div = 0;
	for (i = r->from; i <= r->to; i += 2){
		div = countDivisors(i);
		if (div > max_div){
			max_int = i;
			max_div = div;
		}
	}
	num_largest_divisors = fmax(num_largest_divisors, max_int);
	pthread_exit(0);
}