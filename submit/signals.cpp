#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"
#include <unistd.h>
#include "defines.h"

using namespace std;
void errorMsgSys2(const string &sys) {
    ostringstream os;
    os << "smash error: " << sys << " failed";
    perror(os.str().c_str());
}
void ctrlZHandler(int sig_num) {
  cout<<"smash: got ctrl-Z"<<endl;
  if(Smash.fgPid != -1){
    if(Smash.jobsList.getJobByPid(Smash.fgPid) == nullptr){
      Smash.jobsList.addJob(Smash.fgCmdLine,Smash.fgPid, true);
    } else {
      Smash.jobsList.getJobByPid(Smash.fgPid)->setStopped(true);
      Smash.jobsList.getJobByPid(Smash.fgPid)->visible = true;
    }

    if(kill(Smash.fgPid,SIGSTOP)==0) {
        cout << "smash: process " << Smash.fgPid << " was stopped" << endl;
    }else{
        errorMsgSys2("kill");
    }
}
}

void ctrlCHandler(int sig_num) {
    cout<<"smash: got ctrl-C"<<endl;
    if(Smash.fgPid != -1){
      if(kill(Smash.fgPid,SIGKILL)==0) {
          Smash.jobsList.removeFinishedJobs();
          cout << "smash: process " << Smash.fgPid << " was killed" << endl;
      } else{
          errorMsgSys2("kill");
      }
    }
#ifdef DEBUG_MODE
    else{
        exit(0);
    }
#endif
}
