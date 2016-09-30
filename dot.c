#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

struct dot_params {
	int *u;
	int *v;
	int sizes;
};

/**
 * Computes the dot product of two arbitrarily sized arrays
 * Supply arguments in one dot_params struct containing two equally sized arrays of sizes length.
 * Returns a pointer to an int storing the result.
 * This pointer must be freed
 */
void *dot(void *param) {
	// note: can unwrap the pointer like this:
	// struct type name = *((struct type *) param)
	// first cast the param void pointer to a struct type pointer, then dereference it
	// instead we'll leave it as a pointer
	struct dot_params *vectors = (struct dot_params *) param;
	int i=0;
	int *sum = malloc(sizeof(int));
	for(i; i<vectors->sizes; i++) {
		*sum += vectors->u[i] * vectors->v[i];
	}
	return sum;
}

int main() {
	int u[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	int v[] = { 9, 10, 11, 12, 13, 14, 15, 16};
	int size = 8;
	// Since we're dealing with pointers, and arrays are pointers,
	// just pass the array names into the struct.
	struct dot_params first = { u, v, 4 };
	// We can even cut the array in half for free by simply adding to the
	// initial array address
	struct dot_params second = { u+4, v+4, 4 };
	// allocate two thread numbers
	pthread_t first_thread;
	pthread_t second_thread;
	// make the thread
	int did_thread_a = pthread_create(&first_thread, NULL, dot, &first);
	// assure ourselves that we made the thread
	assert(!did_thread_a);
	int did_thread_b = pthread_create(&second_thread, NULL, dot, &second);
	assert(!did_thread_b);
	// now to get the results
	// make a pointer to an int
	// who cares what address I get, the address will get assigned to the address of the malloc'd memory in dot(). Which I definitely have access to.
	// PS that's why pthread_join gets a double pointer.
	// C can't handle multiple return values (who can?) so by convention
	// the status of the method is formally returned, while the result
	// of the thread is passed indirectly through a pointer.
	// Well dot() returns a pointer, so that pointer needs to be returned indirectly by another pointer.
	int *first_result;
	// typecast to void** pointer to hush the compiler. I wrote both methods so I know the size is sufficient.
	int did_join_a = pthread_join(first_thread, (void **) &first_result);
	// assert that joining was successful
	assert(!did_join_a);
	int *second_result;
	int did_join_b = pthread_join(second_thread, (void **) &second_result);
	assert(!did_join_b);
	printf("Dot: %d\n", *first_result + *second_result);
	// Free the memory allocated by dot(), which is pointed to by first_result and second_result. We got those addresses through pthread_join.
	// That memory would probably be freed automatically when we exit, but just in case (and to conform to proper coding practice)
	free(second_result);
	free(first_result);
	return 0;
}
