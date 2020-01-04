//
// Created by yuvalgelles on 25/12/2019.
//

#ifndef HW2_KILL_WRAPPER_H
#define HW2_KILL_WRAPPER_H


#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int kill(pid_t pid, int sig) {
  printf("welcome to our kill wrapper!\n");
 // printf("pid=%d,sig=%d\n",pid,sig);//FIXME remove
  int __res;
  __asm__(
  "syscall;"
  : "=a" (__res)
  : "0" (62), "D" (pid), "S" (sig)
  : "memory");
  if ((__res) < 0) {
    errno = (-__res);
    return -1;
  }
  return __res;
}


#endif //HW2_KILL_WRAPPER_H
