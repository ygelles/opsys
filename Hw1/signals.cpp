#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"
#include <unistd.h>
#include "defines.h"

using namespace std;

void ctrlZHandler(int sig_num) {
  cout<<endl<<"Bye"<<endl;
  kill(getpid(),SIGSTOP);
	// TODO: Add your implementation
}

void ctrlCHandler(int sig_num) {
  static int counter=0;
  counter++;
  if(counter>=3){
    kill(getpid(),SIGKILL);
  }
  cout<<endl<<(3-counter)<<" more to exit"<<endl;
  // TODO: Add your implementation
}
