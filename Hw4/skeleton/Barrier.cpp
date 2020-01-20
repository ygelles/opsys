//
// Created by elia on 08-Jan-20.
//
#include <iostream>
#include "Barrier.h"
using namespace std;
Barrier::Barrier(unsigned int num_of_threads) {
  numberOfThreads=num_of_threads;
  threadsCounter=0;
  sem_init(&barrierSemaphore,0,0);
  sem_init(&counterSemaphore,0,1);
  sem_init(&flowSemaphore,0,1);
}
Barrier::~Barrier() {
  sem_destroy(&barrierSemaphore);
  sem_destroy(&counterSemaphore);
  sem_destroy(&flowSemaphore);
}
void Barrier::wait() {
  sem_wait(&flowSemaphore);
  sem_wait(&counterSemaphore);
      threadsCounter++;
    if(threadsCounter == numberOfThreads){
      sem_post(&barrierSemaphore);
    }else {
      sem_post(&flowSemaphore);
    }
  sem_post(&counterSemaphore);

  sem_wait(&barrierSemaphore);
  sem_post(&barrierSemaphore);

  sem_wait(&counterSemaphore);
    threadsCounter--;
    if(threadsCounter == 0){
      sem_wait(&barrierSemaphore);
      sem_post(&flowSemaphore);
    }
  sem_post(&counterSemaphore);
}
unsigned int Barrier::waitingThreads() {
  unsigned int toReturn;
  sem_wait(&counterSemaphore);
  toReturn = threadsCounter;
  sem_post(&counterSemaphore);
  return toReturn;
}
