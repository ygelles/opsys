#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <wait.h>
#include <signal.h>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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

void errorMsgSys(const string &sys) {
    ostringstream os;
    os << "smash error: " << sys << " failed";
    perror(os.str().c_str());
}

void errorMsg(const string &fn, const string &msg) {
    ostringstream os;
    os << "smash error: " << fn << ": " << msg;
    perror(os.str().c_str());
}

int _parseCommandLine(const char *cmd_line, char **args) {
    FUNC_ENTRY()
    stringstream check1(cmd_line);
    string intermediate;
    int i = 0;
    while (getline(check1, intermediate, ' ')) {
        args[i] = (char *) malloc(intermediate.length() + 1);
        memset(args[i], 0, intermediate.length() + 1);
        strcpy(args[i], intermediate.c_str());
        args[++i] = NULL;
    }
    return i;

    FUNC_EXIT()
}

bool _isBackgroundComamnd(const char *cmd_line) {
    const string whitespace = " \t\n";
    const string str(cmd_line);
    return str[str.find_last_not_of(whitespace)] == '&';
}

void _removeBackgroundSign(char *cmd_line) {
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
    cmd_line[str.find_last_not_of(whitespace, idx - 1) + 1] = 0;
}


Command::Command(const char *cmd_line) {
    stringstream command = stringstream(cmd_line);
    args = vector<string>();
    string intermediate;
    while (getline(command, intermediate, ' ')) {
        if (intermediate.empty()) { continue; }
        args.push_back(intermediate);
    }
    cmd = args.at(0);
    args.erase(args.begin());
    cmdLine = string(cmd_line);


}


SmallShell::SmallShell() {

}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command *SmallShell::CreateCommand(const char *cmd_line) {
    stringstream command = stringstream(cmd_line);
    string cmd;

    while (getline(command, cmd, ' ')) {
        if (cmd.empty()) { continue; }// FIXME:NEED TO IGNORE TABS??
        break;
    }
    string commandLine = string(cmd_line);
    if (commandLine.find('|') < commandLine.size()) {
        return new PipeCommand(cmd_line);
    }
    if (commandLine.find('>') < commandLine.size()) {
        return new RedirectionCommand(cmd_line);
    }
    if (cmd == "pwd") {
        return new GetCurrDirCommand(cmd_line);
    } else if (cmd == "cd") {
        return new ChangeDirCommand(cmd_line, lastVisited);
    } else if (cmd == "history") {
        return new HistoryCommand(cmd_line, cmdHist);
    } else if (cmd == "jobs") {
        return new JobsCommand(cmd_line);
    } else if (cmd == "kill") {
        return new KillCommand(cmd_line);
    } else if (cmd == "showpid") {
        return new ShowPidCommand(cmd_line);
    } else if (cmd == "quit") {
        return new QuitCommand(cmd_line);
    } else if (cmd == "fg") {
        return new ForegroundCommand(cmd_line);
    } else if (cmd == "bg") {
        return new BackgroundCommand(cmd_line);
    } else if (cmd == "cp") {
        return new CopyCommand(cmd_line);
    } else {
        return new ExternalCommand(cmd_line);
    }


    return nullptr;
}


void SmallShell::executeCommand(const char *cmd_line) {
    cmdHist.addRecord(cmd_line);
    Command *cmd = CreateCommand(cmd_line);
    cmd->execute();
    delete (cmd);
    Smash.jobsList.removeFinishedJobs();
}

void GetCurrDirCommand::execute() {
    cout << ::getcwd(nullptr, INT32_MAX) << endl;
}

void ChangeDirCommand::execute() {
    if (args.size() > 1) {
        cerr<<"smash error: cd: too many arguments"<<endl;
    } else {
        if (args[0] == "-") {
            if (lastPwd.empty()) {
                cerr<<"smash error: cd: OLDPWD not set"<<endl;
            } else {
                string current = getcwd(nullptr, INT32_MAX);//current pwd
                if (chdir(lastPwd.c_str()) != 0) {
                    errorMsgSys("chdir");
                } else {
                    lastPwd = current;
                }
            }
        } else {
            string current = getcwd(nullptr, INT32_MAX);//current pwd
            if (chdir(args[0].c_str()) != 0) {
                errorMsgSys("chdir");
            } else {
                lastPwd = current;
            }
        }
    }
}

void HistoryCommand::execute() {
    cmdHist.printHistory();
}

void ExternalCommand::execute() {
    string tempString = cmdLine;
    if (!cmdLine.empty() && cmdLine.at(cmdLine.size() - 1) == '&') {
        tempString.pop_back();
    }
    pid_t tempPid = vfork();
    if (tempPid < 0) {
        errorMsgSys("fork");
    }
    if (tempPid == 0) {
        setpgrp();
        /*if(tempString.find('*')<tempString.size() || tempString.find('?')<tempString.size()){
            execlp("/bin/bash", "bash", "-c", tempString.c_str(), NULL);
            exit(0);
        }else{
            execlp(cmd.c_str(), tempString.c_str(), NULL);
            exit(0);
        }*/
        execlp("/bin/bash", "bash", "-c", tempString.c_str(), NULL);
        exit(0);
    } else {
        if (!cmdLine.empty() && cmdLine.at(cmdLine.size() - 1) == '&') {
            Smash.jobsList.addJob(cmdLine, tempPid);
        } else {
            Smash.fgPid = tempPid;
            Smash.fgCmdLine = cmdLine;
            waitpid(tempPid, nullptr, WUNTRACED);
            Smash.fgPid = -1;
            Smash.fgCmdLine = "";
        }
    }
}


void JobsList::addJob(string cmd, pid_t pid, bool isStopped) {
    removeFinishedJobs();
    jobsCounter++;
    jobs.push_back(JobEntry(cmd, jobsCounter, pid, time(nullptr), isStopped));
}

void JobsList::printJobsList() {
    removeFinishedJobs();       //added: removing finished jobs before printing
    for (unsigned int i = 0; i < jobs.size(); ++i) {
        jobs.at(i).print();
    }
}

void JobsList::killAllJobs() {
    removeFinishedJobs();
    cout << "smash: sending SIGKILL signal to " << jobs.size() << " jobs:" << endl;
    for (unsigned int i = 0; i < jobs.size(); ++i) {
        cout << jobs.at(i).getProcessId() << ": " << jobs.at(i).getCmdLine() << endl;
        kill(jobs.at(i).getProcessId(), SIGKILL);
    }
}

void JobsList::removeFinishedJobs() {
    unsigned int maxJobId=0;
    for (JobEntry &job: this->jobs) {
        if (waitpid(job.getProcessId(), nullptr, WNOHANG) == 0) {
            if(job.getJobId()>maxJobId){
                maxJobId=job.getJobId();
            }
        }else{
            removeJobById(job.getJobId());
        }
    }
    jobsCounter=maxJobId;
}

JobsList::JobEntry *JobsList::getJobById(int jobId) {
    //cerr<<"this is the id that was sent:"<<jobId<<endl;//FIXME!!!!!!!!!!!!!!!!!!!!
    for (JobEntry &job: this->jobs) {
        if (job.getJobId() == (unsigned int) jobId) {
            return &job;
        }
    }
    return nullptr;
}

JobsList::JobEntry *JobsList::getJobByPid(pid_t pid) {
    for (JobEntry &job: this->jobs) {
        if (job.getProcessId() == pid) {
            return &job;
        }
    }
    return nullptr;
}

void JobsList::removeJobById(int jobId) {
    int i = 0;
    for (JobEntry &job: jobs) {
        if (job.getJobId() == (unsigned int) jobId) {
            jobs.erase(jobs.begin() + i);
            return;
        }
        i++;
    }
}

JobsList::JobEntry *JobsList::getLastJob() {
  for (unsigned int i = jobs.size(); i > 0; --i) {
    if (jobs.at(i - 1).visible) {
      return &jobs.at(i - 1);
    }
  }
  return nullptr;
}

JobsList::JobEntry *JobsList::getLastStoppedJob() {
    for (unsigned int i = jobs.size(); i > 0; --i) {
        if (jobs.at(i - 1).getStopped() && jobs.at(i - 1).visible) {
            return &jobs.at(i - 1);
        }
    }
    return nullptr;
}

void JobsList::updateJobsCounter() {

}

void JobsCommand::execute() {
    Smash.jobsList.removeFinishedJobs();
    Smash.jobsList.printJobsList();
}

unsigned int JobsList::JobEntry::getJobId() const {
    return jobId;
}

pid_t JobsList::JobEntry::getProcessId() const {
    return processId;
}

string JobsList::JobEntry::getCmdLine() const {
    return cmd;
}

const bool JobsList::JobEntry::getStopped() const {
    return (stopped != "");
}

void JobsList::JobEntry::setStopped(bool isStopped) {
    this->stopped = (isStopped) ? " (stopped)" : "";
}

bool isNumber(string s) {
    for (unsigned int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == 0)
            return false;

    return true;
}

void KillCommand::execute() {
    Smash.jobsList.removeFinishedJobs();
    string tempString = cmdLine;
    if (args.size() <= 2) {//right number of args
        if ((args[0].at(0)) == '-') {//right format -> has '-'
            if (isNumber(args[0].substr(1))) {// right format -> first arg is a number
                if (isNumber(args[1])) {      //right format second arg is number
                    SmallShell &smash = Smash;
                    int jobId = atoi(args[1].c_str());
                    if (smash.jobsList.getJobById(jobId) != nullptr) {//jobExist
                        int jobPid = smash.jobsList.getJobById(jobId)->getProcessId();
                      //  cerr<<"pid is:::::::::::::"<<jobPid<<endl;//FIXME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                        int sig = atoi(args[0].substr(1).c_str());
                        if (kill(jobPid, sig) == 0) {//if kill success
                            cout << "signal number " << sig << " was sent to pid " << jobPid << endl;
                            return;
                        } else {//kill failed
                            errorMsgSys("kill");
                            return;
                        }
                    } else {
                        cerr <<"smash error: kill: job-id " << jobId << " does not exist"<<endl;
                        return;
                    }
                }

            }
        }
    }
    cerr <<"smash error: kill: invalid arguments"<<endl;
    return;
}

void ShowPidCommand::execute() {
    cout << "smash pid is " << getpid() << endl;
}

void QuitCommand::execute() {
    Smash.jobsList.removeFinishedJobs();
    if (!args.empty()) {
        if (args[0] == "kill") {
            Smash.jobsList.killAllJobs();
        }
    }
    exit(0);
}

void ForegroundCommand::execute() {
    Smash.jobsList.removeFinishedJobs();
    pid_t tempPid = -1;
    string tempCmdLine = "";
    if (args.size() != 1) {
        if (args.empty()) {
            if (Smash.jobsList.getLastJob() == nullptr) {
                cerr << "smash error: fg: jobs list is empty" << endl;
                return;
            } else {
                tempPid = Smash.jobsList.getLastJob()->getProcessId();
                tempCmdLine = Smash.jobsList.getLastJob()->getCmdLine();

               Smash.jobsList.getLastJob()->resetTime();
                Smash.jobsList.getLastJob()->visible = false;
            }
        } else {
            cerr << "smash error: fg: invalid arguments" << endl;
            return;
        }
    } else if (!isNumber(args[0])) {
        cerr << "smash error: fg: invalid arguments" << endl;
        return;
    } else if (Smash.jobsList.getJobById(atoi(args[0].c_str())) == nullptr) {
        cerr << "smash error: fg: job-id " << args[0] << " does not exist" << endl;
        return;
    } else if (!Smash.jobsList.getJobById(atoi(args[0].c_str()))->visible) {
        cerr << "smash error: fg: job-id " << args[0] << " does not exist" << endl;
        return;
    } else {
        tempPid = Smash.jobsList.getJobById(atoi(args[0].c_str()))->getProcessId();
        tempCmdLine = Smash.jobsList.getJobById(atoi(args[0].c_str()))->getCmdLine();
        Smash.jobsList.getJobById(atoi(args[0].c_str()))->visible = false;
        Smash.jobsList.getJobById(atoi(args[0].c_str()))->resetTime();
    }

    cout<<tempCmdLine<<" : "<<tempPid<<endl;
    kill(tempPid, SIGCONT);
    Smash.fgPid = tempPid;
    Smash.fgCmdLine = tempCmdLine;
    waitpid(tempPid, nullptr, WUNTRACED);
    Smash.fgPid = -1;
    Smash.fgCmdLine = "";

}

void BackgroundCommand::execute() {
    Smash.jobsList.removeFinishedJobs();
    pid_t tempPid = -1;
    string tempCmdLine = "";
    if (args.size() != 1) {
        if (args.empty()) {
            if (Smash.jobsList.getLastStoppedJob() == nullptr) {
                cerr << "smash error: bg: there is no stopped jobs to resume" << endl;
                return;
            } else {
                tempPid = Smash.jobsList.getLastStoppedJob()->getProcessId();
                tempCmdLine = Smash.jobsList.getLastStoppedJob()->getCmdLine();
                Smash.jobsList.getLastStoppedJob()->setStopped(false);
                Smash.jobsList.getLastStoppedJob()->resetTime();
            }
        } else {
            cerr << "smash error: bg: invalid arguments" << endl;
            return;
        }
    } else if (!isNumber(args[0])) {
        cerr << "smash error: bg: invalid arguments" << endl;
        return;
    } else if (Smash.jobsList.getJobById(atoi(args[0].c_str())) == nullptr) {
      cerr << "smash error: bg: job-id " << args[0] << " does not exist"
           << endl;
      return;
    } else if (!Smash.jobsList.getJobById(atoi(args[0].c_str()))->visible) {
        cerr << "smash error: bg: job-id " << args[0] << " does not exist" << endl;
        return;
    } else if (!Smash.jobsList.getJobById(atoi(args[0].c_str()))->getStopped()) {
        cerr << "smash error: bg: job-id " << args[0] << " is already running in the background" << endl;
        return;
    } else {
        tempPid = Smash.jobsList.getJobById(atoi(args[0].c_str()))->getProcessId();
        tempCmdLine = Smash.jobsList.getJobById(atoi(args[0].c_str()))->getCmdLine();
        Smash.jobsList.getJobById(atoi(args[0].c_str()))->setStopped(false);
        Smash.jobsList.getJobById(atoi(args[0].c_str()))->resetTime();
    }
    cout<<tempCmdLine<<" : "<<tempPid<<endl;
    kill(tempPid, SIGCONT);
}

inline bool exists(const string &name) {
    return (access(name.c_str(), F_OK) != -1);
}

void CopyCommand::execute() {
    string src = args[0];
    string dst = args[1];
    string buf;
    char charRead;
    int numOfBytes;
    int srcFd = open(src.c_str(), O_RDONLY);
    int dstFd;
    if (srcFd != -1) {    //opening src succeeded

        numOfBytes = read(srcFd, &charRead, 1);
        while (numOfBytes > 0) {
            buf.push_back(charRead);
            numOfBytes = read(srcFd, &charRead, 1);
        }
        if (numOfBytes == 0) {//reading src succeeded
            if ((exists(dst) && remove(dst.c_str()) != -1) || (!exists(dst))) {//removing dst succeeded
                dstFd = open(dst.c_str(), O_CREAT | O_WRONLY, 0644);
                if (dstFd != -1) {  //opening dst succeeded
                    if (write(dstFd, buf.c_str(), buf.size()) != -1) {//writing to dst succeeded
                        cout << "smash: " << src << " was copied to " << dst << endl;
                    }//writing to dst failed
                } else {
                    errorMsgSys("open");
                }//opening dst failed
            }//removing dst failed
        }//reading src failed
    } else { //open src failed
        errorMsgSys("open");
    }
}


void RedirectionCommand::execute() {
    int rediSpot = cmdLine.find('>');
    string cmdToExec = cmdLine.substr(0, rediSpot);
    string fileToRedi = cmdLine.substr(rediSpot + 1, cmdLine.size() - rediSpot - 1);
    pid_t pid = fork();
    if (pid == 0) // child process
    {
        if (!fileToRedi.empty() && fileToRedi.at(fileToRedi.size() - 1) == '&') {
            fileToRedi.pop_back();
        }
        close(1);
        if (fileToRedi.at(0) == '>') {
            fileToRedi.erase((fileToRedi.begin()));
            open(fileToRedi.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        } else {
            open(fileToRedi.c_str(), O_WRONLY | O_CREAT, 0644);
        }

        Smash.getInstance().executeCommand(cmdToExec.c_str());
        exit(0);//exit shell in son process

    } else if (pid > 0) {   // parent process
        if (!cmdLine.empty() && cmdLine.at(cmdLine.size() - 1) == '&') {
            Smash.jobsList.addJob(cmdLine, pid);
        } else {
            Smash.fgPid = pid;
            Smash.fgCmdLine = cmdLine;
            waitpid(pid, nullptr, WUNTRACED);
            Smash.fgPid = -1;
            Smash.fgCmdLine = "";
        }
    }

}

void PipeCommand::execute() {
    int fds[2];
    int pipeOut = 1;
    if (pipe(fds) < 0) {
        exit(1);//FIXME:perror
    }
    size_t pipeSpot = cmdLine.find('|');
    string cmd1 = cmdLine.substr(0, pipeSpot);
    string cmd2 = cmdLine.substr(pipeSpot + 1, cmdLine.size() - pipeSpot - 1);
    if (cmd2.at(0) == '&') {
        pipeOut = 2;
        cmd2.erase(cmd2.begin());
    }
    if (!cmd2.empty() && cmd2.at(cmd2.size() - 1) == '&') {
        cmd2.pop_back();
    }
    //FIXME:BG
    int pid1 = fork();
    if (pid1 < 0) {
        exit(1);//FIXME:perror
    }
    if (pid1 == 0) {//son1-writer
        dup2(fds[1], pipeOut);//stdout is writing edge
        close(fds[0]);//close reading edge
        close(fds[1]);//close writing edge(the stdout is connected to pipe now)
        Smash.getInstance().executeCommand(cmd1.c_str());
        exit(0);//exit shell in son process
    }
    int pid2 = fork();
    if (pid2 < 0) {
        exit(1);//FIXME:perror
    }
    if (pid2 == 0) {//son2-reader
        dup2(fds[0], 0);//stdin is reading edge
        close(fds[0]);//close reading edge(the stdin is connected to pipe now)
        close(fds[1]);//close writing edge
        Smash.getInstance().executeCommand(cmd2.c_str());
        exit(0);//exit shell in son process
    }
    //parent process
    close(fds[0]);
    close(fds[1]);
    if (!cmdLine.empty() && cmdLine.at(cmdLine.size() - 1) == '&') {
        Smash.jobsList.addJob(cmdLine, pid2);
    } else {
        Smash.fgPid = pid2;
        Smash.fgCmdLine = cmdLine;
        waitpid(pid2, nullptr, WUNTRACED);
        Smash.fgPid = -1;
        Smash.fgCmdLine = "";
    }

}



