#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>

#define T 1000
#define THREADS 4

/**
 * Approximates pi using the monte carlo method
 * places output in the supplied pointer
 *
 * Does not have a return value. The result is placed in the parameter so that this thread need not worry about memory management
 *
 * If the result were returned, we would have to manually get storage on the heap to be stored indefinitely (when the thread is rejoined)
 *
 * pointer must be at least 4 bytes (a float)
 */
void *pi(void* param) {
	float num_inside = 0.0;
	float num_outside = 0.0;
	// C<99 compliant
	int i=0;
	for(i; i<T; i++) {
		float x = ((float) rand() / (float) RAND_MAX);
		float y = ((float) rand() / (float) RAND_MAX);
		if((x*x + y*y)<1.0)
			num_inside++;
		else num_outside++;
	}
	// manually typecast
	float* output = (float *) param;
	// put result in parameter
	*output = num_inside / num_outside;
	// we need not explicitly return, we're not returning a value
}

/**
 * Might as well define an average function here
 * ps Java declares [] with the type name, not the variable name.
 */
float f_average(float nums[], int nums_size) {
	float sum;
	int i=0;
	for(i; i<nums_size; i++)
		sum += nums[i];
	return sum / nums_size;
}


int main(int argc, char** argv) {
	// Seed rng
	srand(time(NULL));
	pthread_t thread[THREADS];
	float result[THREADS];
	int i=0;
	for(i; i<THREADS; i++) {
		int did_thread = pthread_create(&thread[i], NULL, pi, &result[i]);
		assert(!did_thread);
	}
	// start all threads before waiting for them to complete
	i=0;
	for(i; i<THREADS; i++) {
		// didn't return anything, so I don't care about the second parameter
		int did_join = pthread_join(thread[i], NULL);
		assert(!did_join);
	}
	// compute the average
	// as a side note, we can't just pass the array.
	// passing arrays as function parameters is equivalent to passing a pointer.
	// the sizeof() MACRO is a compile-time calculation that gets replaced with a constant.
	// sizeof works in the same scope, but knowing that array parameters are pointers reveals
	// there's no way to know the size of an array across a function call.
	// So I have to include the size as a parameter
	int size = sizeof(result)/sizeof(result[0]);
	float average = f_average(result, size);
	printf("%f\n", average);
	return 0;
}
