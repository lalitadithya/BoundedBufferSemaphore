/***
 * Bounded buffer problem solved using semaphores
 * @anchor Lalit Adithya
 * @date 18th October, 2016
 * @version 1.0
 * @see Figure 6.9, 6.10 for psuedo code (Operating System Concepts (9th Edition) - Silberschatz, Galvin, and Gagne)
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 100

/***
 * bounded buffer to store the elements
 */
long double *buffer;

/***
 * The required semaphores
 */
sem_t mutex_semaphore, empty_semaphore, full_semaphore;

/**
 * Method to simulate a long running process synomous to "prodcing" an item
 * @param number a random integer
 * @return a really long number
 */
long double produce_item(int number) {
    return (number == 0) ? 1 : number * produce_item(number - 1);
}

/***
 * The producer function
 * @return NULL
 */
void *producer() {
    int buffer_index = 0;
    printf("Producer thread started\n");

    do {
        // produce the item to be stored in the buffer
        long double item = produce_item(buffer_index);

        // decrement the empty semaphore
        sem_wait(&empty_semaphore);

        // acquire the lock
        sem_wait(&mutex_semaphore);

        buffer[buffer_index] = item;
        printf("Produced %d\n", buffer_index);
        buffer_index = (buffer_index + 1);

        // release the lock
        sem_post(&mutex_semaphore);

        // increment the full semaphore
        sem_post(&full_semaphore);
    } while (buffer_index < MAX_BUFFER_SIZE);

    return NULL;
}

/***
 * The consumer function
 * @return NULL
 */
void *consumer() {
    int buffer_index = 0;
    printf("Consumer thread started\n");

    do {
        // decrement the full semaphore
        sem_wait(&full_semaphore);

        // acquire the lock
        sem_wait(&mutex_semaphore);

        printf("Consumed %d\n", buffer_index);
        buffer_index = (buffer_index + 1);

        // release the lock
        sem_post(&mutex_semaphore);

        // increment the empty semaphore
        sem_post(&empty_semaphore);
    } while (buffer_index < MAX_BUFFER_SIZE);

    return NULL;
}

/***
 * Main function
 * @return error code
 */
int main() {
    int error_code;
    pthread_t producer_thread, consumer_thread;
    pthread_attr_t producer_attr, consumer_attr;

    // dynamically allocate memory for buffer and check if allocation was successful
    buffer = (long double *) malloc(sizeof(long double) * MAX_BUFFER_SIZE);
    if (buffer == NULL) {
        printf("Could not allocate memory for buffer\n");
        exit(EXIT_FAILURE);
    }

    // initialize the mutex semaphore check if the initialization was successful
    error_code = sem_init(&mutex_semaphore, 0, 1);
    if (error_code != 0) {
        printf("Could not initialize mutex semaphore, error code = %d\n", errno);
    }

    // initialize the full semaphore check if the initialization was successful
    error_code = sem_init(&full_semaphore, 0, 0);
    if (error_code != 0) {
        printf("Could not initialize full semaphore, error code = %d\n", errno);
    }

    // initialize the empty semaphore check if the initialization was successful
    error_code = sem_init(&empty_semaphore, 0, MAX_BUFFER_SIZE);
    if (error_code != 0) {
        printf("Could not initialize empty semaphore, error code = %d\n", error_code);
    }

    // initialize the attributes for the producer thread and check if the initialization was successful
    error_code = pthread_attr_init(&producer_attr);
    if (error_code != 0) {
        printf("Could not initialize producer thread attributes, error code = %d\n", error_code);
        exit(EXIT_FAILURE);
    }

    // initialize the attributes for the consumer thread and check if the initialization was successful
    error_code = pthread_attr_init(&consumer_attr);
    if (error_code != 0) {
        printf("Could not initialize consumer thread attributes, error code = %d\n", error_code);
        exit(EXIT_FAILURE);
    }

    // create and start the consumer thread and check if the creation and starting of thread was successful
    error_code = pthread_create(&consumer_thread, &consumer_attr, consumer, NULL);
    if (error_code != 0) {
        printf("Could not create consumer thread, error code = %d\n", error_code);
        exit(EXIT_FAILURE);
    }

    // create and start the producer thread and check if the creation and starting of thread was successful
    error_code = pthread_create(&producer_thread, &producer_attr, producer, NULL);
    if (error_code != 0) {
        printf("Could not create producer thread, error code = %d\n", error_code);
        exit(EXIT_FAILURE);
    }

    // wait for the producer thread to finish
    error_code = pthread_join(producer_thread, NULL);
    if (error_code != 0) {
        printf("Could not join with producer thread, error code = %d\n", error_code);
        exit(EXIT_FAILURE);
    }

    // wait for the consumer thread to finish
    error_code = pthread_join(consumer_thread, NULL);
    if (error_code != 0) {
        printf("Could not join with consumer thread, error code = %d\n", error_code);
        exit(EXIT_FAILURE);
    }

    // destroy the attributes for the producer thread and check if it was successful
    error_code = pthread_attr_destroy(&producer_attr);
    if (error_code != 0) {
        printf("Could not destroy producer thread attributes, error code = %d\n", error_code);
    }

    // destroy the attributes for the consumer thread and check if it was successful
    error_code = pthread_attr_destroy(&consumer_attr);
    if (error_code != 0) {
        printf("Could not destroy consumer thread attributes, error code = %d\n", error_code);
    }

    // deallocate the memory allocated for the buffer
    free(buffer);

    // destroy the mutex semaphore and check if the destruction was successful
    error_code = sem_destroy(&mutex_semaphore);
    if (error_code != 0) {
        printf("Could not destroy mutex semaphore, error code = %d", errno);
    }

    // destroy the full semaphore and check if the destruction was successful
    error_code = sem_destroy(&full_semaphore);
    if (error_code != 0) {
        printf("Could not destroy mutex semaphore, error code = %d", errno);
    }

    // destroy the empty semaphore and check if the destruction was successful
    error_code = sem_destroy(&empty_semaphore);
    if (error_code != 0) {
        printf("Could not destroy mutex semaphore, error code = %d", errno);
    }

    return 0;
}