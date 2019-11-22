#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <string>
#include <queue>
#include <deque>
#include <iomanip>

using namespace std;
#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define HISTORY_MAX_RECORDS (50)

class Command {

protected:
    string cmd; //FIXME may be not use
    vector<string> args;

public:
    Command(const char *cmd_line);

    virtual ~Command() = default;

    virtual void execute() = 0;
    //virtual void prepare();
    //virtual void cleanup();
    // TODO: Add your extra methods if needed
};

class BuiltInCommand : public Command {
public:
    BuiltInCommand(const char *cmd_line) : Command(cmd_line) {}

    virtual ~BuiltInCommand() {}
};

class ExternalCommand : public Command {
public:
    ExternalCommand(const char *cmd_line);

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
    ShowPidCommand(const char *cmd_line);

    virtual ~ShowPidCommand() {}

    void execute() override;
};

class JobsList;

class QuitCommand : public BuiltInCommand {
// TODO: Add your data members public:
    QuitCommand(const char *cmd_line, JobsList *jobs);

    virtual ~QuitCommand() {}

    void execute() override;
};


//a data structure for FIFO
template<typename T, int max, typename Container=std::deque<T>>
class Fifo : public std::queue<T, Container> {
public:

    void push(const T &value) {
        if (this->size() == max) {
            this->c.pop_front();
        }
        queue<T, Container>::push(value);
    }

    void print() {
        if (this->empty()) {
            return;
        }
        for (unsigned int i = 0; i < this->size(); ++i) {//fixme:finishhhhhh
            const T temp = T(this->c.front());   //make a copy of the first
            cout << temp;                      //print it
            this->c.pop_front();            //delete the first
            this->push(temp);                     //push the copy to the end
        }
    }
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
        // TODO: Add your data members
    };
    // TODO: Add your data members
public:
    JobsList();

    ~JobsList();

    void addJob(Command *cmd, bool isStopped = false);

    void printJobsList();

    void killAllJobs();

    void removeFinishedJobs();

    JobEntry *getJobById(int jobId);

    void removeJobById(int jobId);

    JobEntry *getLastJob(int *lastJobId);

    JobEntry *getLastStoppedJob(int *jobId);
    // TODO: Add extra methods or modify exisitng ones as needed
};

class JobsCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    JobsCommand(const char *cmd_line, JobsList *jobs);

    virtual ~JobsCommand() {}

    void execute() override;
};

class KillCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    KillCommand(const char *cmd_line, JobsList *jobs);

    virtual ~KillCommand() {}

    void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    ForegroundCommand(const char *cmd_line, JobsList *jobs);

    virtual ~ForegroundCommand() {}

    void execute() override;
};

class BackgroundCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    BackgroundCommand(const char *cmd_line, JobsList *jobs);

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
