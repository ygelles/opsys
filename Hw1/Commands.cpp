#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"
#include "defines.h"
using namespace std;

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif



int _parseCommandLine(const char* cmd_line,char** args) {
    FUNC_ENTRY()
    stringstream check1(cmd_line);
    string intermediate;
    int i = 0;
    while(getline(check1, intermediate, ' '))
    {
        args[i] = (char*)malloc(intermediate.length()+1);
        memset(args[i], 0, intermediate.length()+1);
        strcpy(args[i], intermediate.c_str());
        args[++i] = NULL;
    }
    return i;

    FUNC_EXIT()
}

bool _isBackgroundComamnd(const char* cmd_line) {
    const string whitespace = " \t\n";
    const string str(cmd_line);
    return str[str.find_last_not_of(whitespace)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
    const string whitespace = " \t\n";
    const string str(cmd_line);
    // find last character other than spaces
    size_t idx = str.find_last_not_of(whitespace);
    // if all characters are spaces then return
    if (idx == string::npos) {
        return;
    }
    // if the command line does not end with & then return
    if (cmd_line[idx] != '&') {
        return;
    }
    // replace the & (background sign) with space and then remove all tailing spaces.
    cmd_line[idx] = ' ';
    // truncate the command line string up to the last non-space character
    cmd_line[str.find_last_not_of(whitespace, idx-1) + 1] = 0;
}


// TODO: Add your implementation for classes in Commands.h 
Command::Command(const char* cmd_line){
    stringstream command = stringstream(cmd_line);
    args = vector<string>();
    string intermediate;
    while(getline(command, intermediate, ' ')){
        if(intermediate.empty()){ continue; }
        args.push_back(intermediate);
    }
    cmd=args.at(0);
    args.erase(args.begin());



    debug("command: "<<cmd<<endl);
    for (unsigned int i = 0; i <args.size() ; ++i) {
        debug("arg "<<i<<":"<<args[i]<<endl);
    }

}



SmallShell::SmallShell() {
// TODO: add your implementation

}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line) {
    stringstream command = stringstream(cmd_line);
    string cmd;

    while(getline(command, cmd, ' ')){
        if(cmd.empty()){ continue; }// FIXME:NEED TO IGNORE TABS??
        break;
    }

    if(cmd=="pwd"){
        return new GetCurrDirCommand(cmd_line);
    }
    if(cmd=="cd"){
        return new ChangeDirCommand(cmd_line,lastVisited);
    }
    if(cmd=="history"){
        return new HistoryCommand(cmd_line,cmdHist);
    }


    return nullptr;
}


void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here
    // for example:
    cmdHist.addRecord(cmd_line);
    Command* cmd = CreateCommand(cmd_line);
    cmd->execute();
    delete(cmd);
    //
    // Command cmd= Command(cmd_line);
    // cmd->execute();
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}

void GetCurrDirCommand::execute() {
    cout<<::getcwd(nullptr,INT32_MAX)<<endl;
}

void ChangeDirCommand::execute() {
    if(args.size()>1){
        errorMsg("cd", "too many arguments");
    } else{
        if(args[0]=="-"){
            if(lastPwd.empty()){
                errorMsg("cd", "OLDPWD not set");
            }else{
                string current=getcwd(nullptr,INT32_MAX);//current pwd
                if(chdir(lastPwd.c_str())!=0){
                    errorMsgSys("chdir");
                }else{
                    lastPwd=current;
                }
            }
        } else {
            string current=getcwd(nullptr,INT32_MAX);//current pwd
            if (chdir(args[0].c_str()) != 0) {
                errorMsgSys("chdir");
            } else {
                lastPwd=current;
            }
        }
    }
}

void HistoryCommand::execute() {
    cmdHist.printHistory();
}
