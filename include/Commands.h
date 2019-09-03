#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <string>
#include "FileSystem.h"
#include "GlobalVariables.h"


using namespace std;
class BaseCommand {
private:
    string args;

public:
    BaseCommand(string args);
    string getArgs();
    virtual void execute(FileSystem & fs) = 0;
    virtual string toString() = 0;
    virtual ~BaseCommand() = 0;
    /*
     * The following function returns the pointer for the directory
     * at the end of the path (if LEGAL)
     * Applies for: Cd, ls,
     */
    Directory* getDirectoryPointer(Directory &startPoint, string path, bool toMakeDir);

    /*
     * The following function is used for COPY, MOVE.
     * The following function returns the pointer of the one-before-last directory (for SOURCE DESTINATION path)
     */

    BaseFile* getBaseFilePointer(FileSystem &fs, string path, bool toRename,string newName, bool toRemove, bool* removed);
    virtual BaseCommand* makeNew() = 0;
    bool canBeRemoved(Directory* dirNotToBeRemoved, Directory* dirWeTryRemove);
};

class PwdCommand : public BaseCommand {
private:
public:
    PwdCommand(string args);
    void execute(FileSystem & fs); // Every derived class should implement this function according to the document (pdf)
    virtual string toString();
    virtual PwdCommand* makeNew();
};

class CdCommand : public BaseCommand {
private:
    void setOrErrorMessage(FileSystem &fs, Directory *tempDir);
public:
    CdCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual CdCommand* makeNew();
    //  Directory* getDirectoryPointer(Directory &startPoint, string path);
};

class LsCommand : public BaseCommand {
private:
    void printOrErrorMessage(Directory *startPoint, bool sortBySize);
public:
    LsCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual LsCommand* makeNew();
};

class MkdirCommand : public BaseCommand {
private:
public:
    MkdirCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual MkdirCommand* makeNew();

};

class MkfileCommand : public BaseCommand {
private:
public:
    MkfileCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual MkfileCommand* makeNew();
};

class CpCommand : public BaseCommand {
private:
public:
    CpCommand(string args);
    void execute(FileSystem & fs);
    bool realCopyExecute(FileSystem & fs, bool abortMission);
    string toString();
    virtual CpCommand* makeNew();
};

class MvCommand : public BaseCommand {
private:
public:
    MvCommand(string args);
    void execute(FileSystem & fs);
    string toString();

    virtual MvCommand* makeNew();
};

class RenameCommand : public BaseCommand {
private:
public:
    RenameCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual RenameCommand* makeNew();
};

class RmCommand : public BaseCommand {
private:
public:
    RmCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual RmCommand* makeNew();
};

class HistoryCommand : public BaseCommand {
private:
    const vector<BaseCommand *> & history;
public:
    HistoryCommand(string args, const vector<BaseCommand*> & history);
    void execute(FileSystem & fs);
    string toString();
    virtual HistoryCommand* makeNew();
};

class VerboseCommand : public BaseCommand {
private:
public:
    VerboseCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual VerboseCommand* makeNew();
};

class ErrorCommand : public BaseCommand {
private:
public:
    ErrorCommand(string args);
    void execute(FileSystem & fs);
    string toString();
    virtual ErrorCommand* makeNew();
};

class ExecCommand : public BaseCommand {
private:
    const vector<BaseCommand *> & history;
public:
    ExecCommand(string args, const vector<BaseCommand *> & history);
    void execute(FileSystem & fs);
    string toString();
    virtual ExecCommand* makeNew();
};

#endif
