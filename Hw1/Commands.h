#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <string>
#include <iomanip>
#include <time.h>
#include "defines.h"
#include "fifo50.h"

using namespace std;
#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define HISTORY_MAX_RECORDS (50)

class JobsList;
class Command {

protected:
    string cmdLine;
    string cmd; //FIXME may be not use
    vector<string> args;
    int inputFile;
    int outputFile;
    int errorFile;

public:
    Command(const char *cmd_line,int inputFile= 0,int outputFile= 1, int errorFile= 2);

    virtual ~Command() = default;

    virtual void execute() = 0;
    virtual void prepare();
    virtual void cleanup();
};

class BuiltInCommand : public Command {
public:
    BuiltInCommand(const char *cmd_line) : Command(cmd_line) {}

    virtual ~BuiltInCommand() {}
};

class ExternalCommand : public Command {
public:
    ExternalCommand(const char *cmd_line): Command(cmd_line) {}

    virtual ~ExternalCommand() {}

    void execute() override;
};

class PipeCommand : public Command {
    // TODO: Add your data members
public:
    PipeCommand(const char *cmd_line);

    virtual ~PipeCommand() {}

    void execute() override;
};

class RedirectionCommand : public Command {
    // TODO: Add your data members
public:
    explicit RedirectionCommand(const char *cmd_line);

    virtual ~RedirectionCommand() {}

    void execute() override;
    //void prepare() override;
    //void cleanup() override;
};

class ChangeDirCommand : public BuiltInCommand {//CD
// TODO: Add your data members public:

public:
    string &lastPwd;    //gets the cu

    ChangeDirCommand(const char *cmd_line, string &lastPwd) : BuiltInCommand(cmd_line), lastPwd(lastPwd) {}

    virtual ~ChangeDirCommand() = default;

    void execute() override;
};

class GetCurrDirCommand : public BuiltInCommand {//PWD
public:
    GetCurrDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}

    virtual ~GetCurrDirCommand() {}

    void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
public:
    ShowPidCommand(const char *cmd_line):BuiltInCommand(cmd_line){}

    virtual ~ShowPidCommand() {}

    void execute() override;
};



class QuitCommand : public BuiltInCommand {
public:
    QuitCommand(const char *cmd_line):BuiltInCommand(cmd_line){}

    virtual ~QuitCommand() {}

    void execute() override;
};


//The class of the history log
class CommandsHistory {
private:
    int numOfCommands = 0;
protected:

    class CommandHistoryEntry {
    private:
        string cmdLine;
        int timeStamp;
    public:
        CommandHistoryEntry(const char *cmd_line, int time) : cmdLine(string(cmd_line)), timeStamp(time) {}

        void setTimeStamp(int t) {
            timeStamp = t;
        }

        friend ostream &operator<<(ostream &os, const CommandHistoryEntry &cmdE) {
            os << right << std::setw(5) << cmdE.timeStamp << " " << cmdE.cmdLine << endl;
            return os;
        }

        int getTimeStamp() const {
            return timeStamp;
        }

        const string &getCmd() const {
            return cmdLine;
        }
    };

    // TODO: Add your data members
    Fifo<CommandHistoryEntry, HISTORY_MAX_RECORDS> cmdHistory;

public:

    void printHistory() {
        cmdHistory.print();
    }

    /**
     * adding entry to the commands history logs if its different from the previous command or
     * updating the previous command's time stamp if the current one is the same
     * @param cmd_line
     */
    void addRecord(const char *cmd_line) {
        CommandHistoryEntry cmdEnt(cmd_line, ++numOfCommands);
        if (!cmdHistory.empty()) {
            if (cmdHistory.back().getCmd() == cmdEnt.getCmd()) {
                cmdHistory.back().setTimeStamp(cmdEnt.getTimeStamp());
                return;
            }
        }
        cmdHistory.push(cmdEnt);


    }

};

//the actual command(history) class
class HistoryCommand : public BuiltInCommand {
    CommandsHistory cmdHist;
public:
    HistoryCommand(const char *cmd_line, CommandsHistory &history) : BuiltInCommand(cmd_line), cmdHist(history) {}

    virtual ~HistoryCommand() = default;

    void execute() override;
};

class JobsList {
public:
    class JobEntry {
        string cmd;
        unsigned int jobId;
        pid_t processId;
        time_t startTime;
        string stopped;


    public:
        JobEntry(string cmd,unsigned int jobId,pid_t processId,time_t startTime,bool isStopped):
        cmd(cmd),jobId(jobId),processId(processId),startTime(startTime){
          if(isStopped){
            stopped=" (stopped)";
          } else {
            stopped="";
          }
        }
       void print(){
          cout<<"["<<jobId<<"] "<<cmd<<" : "<<processId<<" ";
          cout<<(int)difftime(time(nullptr),startTime)<<" secs"<<stopped<<endl;
        }

        const bool getStopped() const;

        void setStopped(bool isStopped);

        unsigned int getJobId() const;

        pid_t getProcessId() const;

        string getCmdLine() const;
    };
    vector<JobEntry> jobs;
    unsigned int jobsCounter;
public:
    JobsList():jobs(vector<JobEntry>()),jobsCounter(0){}

    ~JobsList()= default;

    void addJob(string cmd, pid_t pid,bool isStopped= false);

    void printJobsList();

    void killAllJobs();

    void removeFinishedJobs();

    JobEntry* getJobById(int jobId);

    void removeJobById(int jobId);

    JobEntry *getLastJob();

    JobEntry *getLastStoppedJob();

    JobEntry* getJobByPid(pid_t pid);
};

class JobsCommand : public BuiltInCommand {
public:
    JobsCommand(const char *cmd_line):BuiltInCommand(cmd_line){}

    virtual ~JobsCommand() {}

    void execute() override;
};

class KillCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    KillCommand(const char *cmd_line):BuiltInCommand(cmd_line){}

    virtual ~KillCommand() {}

    void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    ForegroundCommand(const char *cmd_line):BuiltInCommand(cmd_line){}

    virtual ~ForegroundCommand() {}

    void execute() override;
};

class BackgroundCommand : public BuiltInCommand {
public:
    BackgroundCommand(const char *cmd_line):BuiltInCommand(cmd_line){}

    virtual ~BackgroundCommand() {}

    void execute() override;
};

class CopyCommand : public BuiltInCommand {
public:
    CopyCommand(const char *cmd_line);

    virtual ~CopyCommand() {}

    void execute() override;
};


class SmallShell {
private:
    // TODO: Add your data members
    CommandsHistory cmdHist;        //commands log
    string lastVisited;             // last visited address

    SmallShell();

public:

    JobsList jobsList;
    pid_t fgPid;
    string fgCmdLine;


    Command *CreateCommand(const char *cmd_line);

    SmallShell(SmallShell const &) = delete; // disable copy ctor
    void operator=(SmallShell const &) = delete; // disable = operator
    static SmallShell &getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    ~SmallShell();

    void executeCommand(const char *cmd_line);
    // TODO: add extra methods as needed
};


#endif //SMASH_COMMAND_H_
