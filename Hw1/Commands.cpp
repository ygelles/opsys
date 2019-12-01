#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <wait.h>
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


void errorMsgSys(const string& sys){
    ostringstream os;
    os<<"smash error: "<< sys<< ": "<<"failed";
    perror(os.str().c_str());
}
void errorMsg(const string& fn,const string& msg){
    ostringstream os;
    os<<"smash error: "<< fn<< ": "<< msg;
    perror(os.str().c_str());
}
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
    cmdLine=string(cmd_line);


    debug("command: "<<cmd<<endl);
    for (unsigned int i = 0; i <args.size() ; ++i) {
        debug("arg "<<i<<":"<<args[i]<<endl);
    }

}



SmallShell::SmallShell() {

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
    else if(cmd=="cd"){
        return new ChangeDirCommand(cmd_line,lastVisited);
    }
    else if(cmd=="history"){
        return new HistoryCommand(cmd_line,cmdHist);
    }
    else if(cmd=="jobs"){
        return new JobsCommand(cmd_line);
    }
    else if(cmd=="kill"){
        return new KillCommand(cmd_line);
    }
    else {
        return new ExternalCommand(cmd_line);
    }


    return nullptr;
}


void SmallShell::executeCommand(const char *cmd_line) {

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

void ExternalCommand::execute() {
    string tempString = cmdLine;
    if(!cmdLine.empty() && cmdLine.at(cmdLine.size()-1) == '&') {
        tempString.pop_back();
    }
    pid_t tempPid=fork();
    if(tempPid<0){debug("error");}
    if(tempPid==0){

        execl("/bin/bash","bash","-c",tempString.c_str(),NULL);
    }
    else {
        if(!cmdLine.empty() && cmdLine.at(cmdLine.size()-1) == '&') {
            SmallShell& smash =SmallShell::getInstance();
            smash.jobsList.addJob(cmdLine,tempPid);
        } else {
            waitpid(tempPid, nullptr, 0);
        }
    }
}


void JobsList::addJob(string cmd,pid_t pid,bool isStopped) {
    jobsCounter++;
    jobs.push_back(JobEntry(cmd,jobsCounter,pid,time(nullptr),isStopped));
}

void JobsList::printJobsList() {
    removeFinishedJobs();       //added: removing finished jobs before printing
    for (unsigned int i = 0; i < jobs.size(); ++i) {
        jobs.at(i).print();
    }
}

void JobsList::killAllJobs() {

}

void JobsList::removeFinishedJobs() {
    for(JobEntry& job: this->jobs ) {
        if(waitpid(job.getProcessId(), nullptr,WNOHANG)>0){
            removeJobById(job.getJobId());
        }
    }
}

JobsList::JobEntry* JobsList::getJobById(int jobId) {
    for(JobEntry& job: this->jobs ) {
        if(job.getJobId()==(unsigned int)jobId){
            return &job;
        }
    }
    return nullptr;
}

void JobsList::removeJobById(int jobId) {
    int i=0;
    for(JobEntry& job: jobs ) {
        if(job.getJobId()==(unsigned int)jobId){
            jobs.erase(jobs.begin()+i);
            return;
        }
        i++;
    }
}

JobsList::JobEntry *JobsList::getLastJob(int *lastJobId) {
    return nullptr;
}

JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId) {
    return nullptr;
}

void JobsCommand::execute() {
    SmallShell& smash =SmallShell::getInstance();
    smash.jobsList.printJobsList();
}

unsigned int JobsList::JobEntry::getJobId() const {
    return jobId;
}

pid_t JobsList::JobEntry::getProcessId() const {
    return processId;
}

bool isNumber(string s)
{
    for (unsigned int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == 0)
            return false;

    return true;
}

void KillCommand::execute() {
    string tempString = cmdLine;
    if (args.size()<=2){//right number of args
        if((args[0].at(0))=='-'){//right format -> has '-'
            if(isNumber(args[0].substr(1))){// right format -> first arg is a number
                if(isNumber(args[1])){      //right format second arg is number
                    SmallShell& smash =SmallShell::getInstance();
                    int jobId = atoi(args[1].c_str());
                    if(smash.jobsList.getJobById(jobId)!= nullptr){//jobExist
                        int jobPid=smash.jobsList.getJobById(jobId)->getProcessId();
                        int sig=atoi(args[0].substr(1).c_str());
                        if(kill(jobPid,sig)==0){//if kill success
                            cout<<"signal number "<<sig<<" was sent to pid "<<jobPid<<endl;
                            return;
                        } else{//kill failed
                             errorMsgSys("kill");
                             return;
                        }
                    } else{
                        ostringstream error;
                        error<<"job-id "<<jobId<< " does not exist";
                        string msg=error.str();
                        errorMsg("kill",msg);
                        return;
                    }
                }

            }
        }
    }
    errorMsg("kill","invalid arguments");
    return;
}
