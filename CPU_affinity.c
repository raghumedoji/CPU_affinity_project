#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

// Thread function that will run on a specific core
void* thread_function(void* arg) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    
    //adds CPU core 0 to the set, meaning that you are selecting core 0 
    CPU_SET((intptr_t)arg, &cpuset); // Set affinity to the core specified by arg

    pthread_t thread = pthread_self();
    // Set the CPU affinity for the current thread
    if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np");
        return NULL;
    }

    // Thread work
    printf("Thread running on core %ld\n", (intptr_t)arg);
    // Perform computation or task
    return NULL;
}

int main() {
    pthread_t threads[4];
    for (int i = 0; i < 4; i++) {
        // Create threads with each one bound to a specific core
        if (pthread_create(&threads[i], NULL, thread_function, (void*)(intptr_t)i) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    return EXIT_SUCCESS;
}
