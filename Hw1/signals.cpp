#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"
#include <unistd.h>
#include "defines.h"

using namespace std;

void ctrlZHandler(int sig_num) {
  cout<<"smash: got ctrl-Z"<<endl;
  if(Smash.fgPid != -1){
    Smash.jobsList.addJob(Smash.fgCmdLine,Smash.fgPid, true);
    kill(Smash.fgPid,SIGSTOP);
    cout<<"smash: process "<<getpid()<<" was stopped"<<endl;
  }
}

void ctrlCHandler(int sig_num) {
    cout<<"smash: got ctrl-C"<<endl;
    if(Smash.fgPid != -1){
      kill(Smash.fgPid,SIGKILL);
      cout<<"smash: process "<<getpid()<<" was killed"<<endl;
    }

}
