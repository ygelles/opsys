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
    if(Smash.jobsList.getJobByPid(Smash.fgPid) == nullptr){
      Smash.jobsList.addJob(Smash.fgCmdLine,Smash.fgPid, true);
    } else {
      Smash.jobsList.getJobByPid(Smash.fgPid)->setStopped(true);
    }

    kill(Smash.fgPid,SIGSTOP);
    cout<<"smash: process "<<Smash.fgPid<<" was stopped"<<endl;
  }
}

void ctrlCHandler(int sig_num) {
    cout<<"smash: got ctrl-C"<<endl;
    if(Smash.fgPid != -1){
      kill(Smash.fgPid,SIGKILL);
      cout<<"smash: process "<<Smash.fgPid<<" was killed"<<endl;
    }

}
