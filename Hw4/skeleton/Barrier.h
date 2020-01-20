#ifndef BARRIER_H_
#define BARRIER_H_

#include <semaphore.h>

class Barrier {
public:
    Barrier(unsigned int num_of_threads);
    void wait();
		unsigned int waitingThreads();
    ~Barrier();
		unsigned int numberOfThreads;
		unsigned int threadsCounter;
		sem_t barrierSemaphore;
		sem_t counterSemaphore;
		sem_t flowSemaphore;
};

#endif // BARRIER_H_

