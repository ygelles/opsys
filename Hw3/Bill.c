#include <unistd.h>
#include "kill_wrapper.h"
#include <stdio.h>
#include <errno.h>

int main(){
  printf("welcome to Bill process. pid - %d\n" , getpid());
  int res;

  //SIGKILL:
  res=kill(getpid(),9);
  if (res == -EPERM){
    printf("Bill avoided SIGKILL!!!\n");
  }

  //SIGSTOP:
  res=kill(getpid(),19);
  printf("supposed to get here after SIGCONT. errno - %d\n", errno);

  return 0;
}