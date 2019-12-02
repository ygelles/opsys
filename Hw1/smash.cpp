#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "Commands.h"
#include "signals.h"
#include "defines.h"

int main(int argc, char* argv[]) {
  if(signal(SIGTSTP , ctrlZHandler)==SIG_ERR) {
    perror("smash error: failed to set ctrl-Z handler");
  }
  if(signal(SIGINT , ctrlCHandler)==SIG_ERR) {
    perror("smash error: failed to set ctrl-C handler");
  }

  SmallShell& smash = SmallShell::getInstance();
  smash.fgPid=-1;
  smash.fgCmdLine="";
  while(true) {
    std::cout << "smash> ";
    std::string cmd_line;
    std::getline(std::cin, cmd_line);
    cmd_line.push_back(0);
    if(!cmd_line.empty() && cmd_line.find_first_not_of(" ") != cmd_line.size() -1 ) {
      cmd_line.pop_back();
      smash.executeCommand(cmd_line.c_str());

    }
  }
  return 0;
}