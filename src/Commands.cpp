//
// Created by Bosmat&Nikita on 08/11/17.
//

#include <iostream>
#include "../include/Commands.h"
#include "../include/FileSystem.h"
#include "../include/GlobalVariables.h"

BaseCommand::BaseCommand(string args) : args(args)  {

}
BaseCommand::~BaseCommand(){}
string BaseCommand::getArgs() {
    return args;
}

Directory *BaseCommand:: getDirectoryPointer(Directory &startPoint, string path, bool toMakeDir) {
    Directory* returnValue= nullptr;

    if(path == ""){
        return &startPoint;
    }
    if (toMakeDir && path.find('/') == string::npos)
        returnValue = &startPoint;
    else {
        size_t indexOfSlash = path.find('/');
        string firstDirOfPath;
        /*
         * In the following lines we determine what is the first directory we go to - To do so, we see if ~PATH~ is a single word or not
         */
        string restOfThePath = "";
        if(indexOfSlash == std::string::npos){
            firstDirOfPath = path;

        } else {
            firstDirOfPath = path.substr(0, indexOfSlash);
            restOfThePath = path.substr(indexOfSlash+1);
        }

        /*
         * In the following lines we, given a request to reach the PARENT, see if it's a legal request - i.e. see if the W.D. is the root directory or not
         */
        if(firstDirOfPath == "..") {
            if(startPoint.getParent() == nullptr) return nullptr;
            else return getDirectoryPointer(*(startPoint.getParent()), restOfThePath, toMakeDir);
        }
        /*
         * In the following line we see that we don't have already a BaseFile with the same name
         */
        for(std::size_t i = 0; i < startPoint.getChildren().size(); i++){
            if(startPoint.getChildren().at(i)->getName() == firstDirOfPath){
                if(startPoint.getChildren().at(i)->isDirectory())
                    return getDirectoryPointer(*((Directory*)(startPoint.getChildren().at(i))), restOfThePath, toMakeDir);
                else {
                    returnValue = nullptr;
                    toMakeDir = false;
                }
            }
        }
    }

    Directory* tempCurrentDir = &startPoint;
    Directory* tempNewDir;
    if(toMakeDir) {
        while(path.find('/') != std::string::npos){
            tempNewDir = new Directory(path.substr(0,path.find('/')), tempCurrentDir);
            tempCurrentDir->addFile(tempNewDir);
            tempCurrentDir = tempNewDir;
            path = path.substr(path.find('/')+1);
        }
        tempNewDir = new Directory(path, tempCurrentDir);
        tempCurrentDir->addFile(tempNewDir);
    }

    return returnValue;
}

bool BaseCommand::canBeRemoved(Directory *dirNotToBeRemoved, Directory *dirWeTryToRemove) {
    if(dirNotToBeRemoved == nullptr){
        return true;
    }
    else if(dirNotToBeRemoved == dirWeTryToRemove){
        return false;
    }
    return canBeRemoved(dirNotToBeRemoved->getParent(), dirWeTryToRemove);
}

BaseFile *BaseCommand :: getBaseFilePointer(FileSystem &fs, string path, bool toRename, string newName, bool toRemove, bool* removed) {
    Directory *tempPtr;
    BaseFile *toCopy = nullptr;
    string fileOrDirToLookFor;
    if (path.at(0) == '/') {
        if (path.find('/') == path.find_last_of('/')) { // CASE 1: "/..."
            tempPtr = &fs.getRootDirectory();
            if(path == "/"){
                toCopy = tempPtr;
            }
            else {
                fileOrDirToLookFor = path.substr(1);
            }

        } else {                                         // CASE 2: "/.../.../..."
            tempPtr = getDirectoryPointer(fs.getRootDirectory(), path.substr(1, path.find_last_of('/')), false);
            fileOrDirToLookFor = path.substr(path.find_last_of('/') + 1);
        }
    }
    else {
        if (path.find('/') == std::string::npos) {       // CASE 3: "..."
            tempPtr = &fs.getWorkingDirectory();
            fileOrDirToLookFor = path;
            if (path == "..") {
                if (&(fs.getWorkingDirectory()) != &(fs.getRootDirectory()) && !toRemove){
                    return fs.getWorkingDirectory().getParent();
                }
                else{
                    if(!toRemove){
                        cout << "No such file or directory" << endl;
                    }
                    else if (toRemove){
                        if((&(fs.getWorkingDirectory()))->getParent() != (&(fs.getRootDirectory()))){
                            toCopy = fs.getWorkingDirectory().getParent();
                        }
                        else{
                            cout << "Can't remove root directory" << endl;
                            return nullptr;
                        }

                    }

                }

            }

        } else {                                          // CASE 4: .../.../.../..."
            tempPtr = getDirectoryPointer(fs.getWorkingDirectory(), path.substr(0, path.find_last_of('/')), false);
            fileOrDirToLookFor = path.substr(path.find_last_of('/') + 1);
        }
    }

    bool found = false;
    if ((tempPtr != nullptr) && (fileOrDirToLookFor != "..")) {
        for (std::size_t i = 0; (i < tempPtr->getChildren().size()) && (!found); i++) {
            if (tempPtr->getChildren().at(i)->getName() == fileOrDirToLookFor) {
                toCopy = tempPtr->getChildren().at(i);
                found = true;
            }
        }
    }
        //the file or directory we want to COPY/INSERT TO/REMOVE/RENAME is not found
    else{
        if ((tempPtr != nullptr) && (fileOrDirToLookFor == "..")){
            toCopy = tempPtr->getParent();
        }
        else{
            toCopy = nullptr;
        }
    }

    //cout << "No such file or directory" << endl;

    if (toRename == true && toCopy!= nullptr) {
        found = false;
        if (tempPtr != nullptr && toCopy->isDirectory()) {
            if (((Directory *) toCopy) != &(fs.getWorkingDirectory()) && ((Directory *) toCopy) != &(fs.getRootDirectory())) {
                for (std::size_t i = 0; (i < tempPtr->getChildren().size()) && (!found); i = i + 1) {
                    if (tempPtr->getChildren().at(i)->getName() == newName) {
                        found = true;
                    }
                }
                if(!found) toCopy->setName(newName);
            } else {
                if(((Directory *) toCopy) == &(fs.getWorkingDirectory())) cout << "Can't rename the working directory" << endl;
                else cout << "Can't rename the root directory" << endl;
            }
        }
        else if(tempPtr != nullptr){
            for (std::size_t i = 0; (i < tempPtr->getChildren().size()) && (!found); i = i + 1) {
                if (tempPtr->getChildren().at(i)->getName() == newName) {
                    found = true;
                }
            }
            if(!found) {
                toCopy->setName(newName);
            }

        }
    }
    if((toRemove) && (tempPtr != nullptr)){
        if (toCopy == nullptr){
            cout << "No such file or directory" << endl;
        }
        else{
            if(toCopy->isDirectory()){
                if(canBeRemoved(&(fs.getWorkingDirectory()), (Directory*) toCopy)){
                    tempPtr->removeFile(toCopy);
                    *removed = true;
                    toCopy = nullptr;
                }
                else{
                    *removed = false;
                }
            }
            else {
                tempPtr->removeFile(toCopy);
                *removed = true;
                toCopy = nullptr;
            }
        }
    }
    return toCopy;
}

// ------------------------------- P W D
PwdCommand::PwdCommand(string args) : BaseCommand(args) {

}

void PwdCommand::execute(FileSystem &fs) {
    cout << fs.getWorkingDirectory().getAbsolutePath() << endl;
}

string PwdCommand::toString() {
    return "pwd";
}

PwdCommand *PwdCommand::makeNew() {
    return this;
}

// ------------------------------- C D
CdCommand::CdCommand(string args) : BaseCommand(args) {

}

void CdCommand::execute(FileSystem &fs) {
    Directory* tempPtr;
    if(getArgs().size() != 0){
        // We should check if the path starts from the root (absolute path) or from the working directory (relative path)

        if(getArgs().at(0)=='/') { // The path we received as an argument is a ~potentially~ ABSOLUTE ONE
            tempPtr = getDirectoryPointer(fs.getRootDirectory(), getArgs().substr(1), false);
            setOrErrorMessage(fs, tempPtr);
        }
        else { // The path we received as an argument is a ~potentially~ RELATIVE ONE
            tempPtr = getDirectoryPointer(fs.getWorkingDirectory(), getArgs(), false);
            setOrErrorMessage(fs, tempPtr);
        }

    }
    tempPtr = nullptr;

}

void CdCommand::setOrErrorMessage(FileSystem &fs, Directory *tempDir) {
    if(tempDir == nullptr){
        cout << "The system cannot find the path specified" << endl;
    }
    else fs.setWorkingDirectory(tempDir);
}

string CdCommand::toString() {
    return "cd";
}

CdCommand *CdCommand::makeNew() {
    return this;
}

// ------------------------------- L S
LsCommand::LsCommand(string args) : BaseCommand(args) {

}

void LsCommand::execute(FileSystem &fs) {
    Directory* tempPtr;
    if(getArgs().size() == 0) printOrErrorMessage(&fs.getWorkingDirectory(), false);
        /*
         * In the following lines we see if we have to sort it by SIZE
         */
    else if((getArgs().size() >= 2) && (getArgs().substr(0,2) == "-s")){
        if(getArgs().size() == 2){
            printOrErrorMessage(&fs.getWorkingDirectory(), true);
        }
        else if (getArgs().at(2)==' '){
            /*
             * The following if checks if the path we received as an argument is a ~potentially~ ABSOLUTE ONE
             */
            if(getArgs().at(3)=='/'){
                printOrErrorMessage(getDirectoryPointer(fs.getRootDirectory() , getArgs().substr(4), false), true);
            }
            else printOrErrorMessage(getDirectoryPointer(fs.getWorkingDirectory(),getArgs().substr(3), false), true);
        }
        else printOrErrorMessage(nullptr, false);
    }
    else{
        // We should check if the path starts from the root (absolute path) or from the working directory (relative path)
        if(getArgs().at(0) == '/') { // The path we received as an argument is a ~potentially~ ABSOLUTE ONE
            tempPtr = getDirectoryPointer(fs.getRootDirectory(), getArgs().substr(1), false);
            printOrErrorMessage(tempPtr, false);
        }
        else { // The path we received as an argument is a ~potentially~ RELATIVE ONE
            tempPtr = getDirectoryPointer(fs.getWorkingDirectory(), getArgs(), false);
            printOrErrorMessage(tempPtr, false);
        }

    }
}

void LsCommand::printOrErrorMessage(Directory *startPoint, bool sortBySize) {
    if(startPoint == nullptr) {
        cout << "The system cannot find the path specified" << endl;
    }
    else{
        if (sortBySize) {
            startPoint->sortBySize();
        }
        else {
            startPoint->sortByName();
        }
        // vector<BaseFile*> tempChildren = startPoint->getChildren();

        for(std::size_t i = 0; i < startPoint->getChildren().size(); i++){
            if(startPoint->getChildren().at(i)->isDirectory()){
                cout << "DIR\t" + startPoint->getChildren().at(i)->getName() + "\t" + std::to_string((startPoint->getChildren().at(i))->getSize()) << endl;
            }
            else{
                cout<< "FILE\t" + startPoint->getChildren().at(i)->getName() + "\t" + std::to_string(startPoint->getChildren().at(i)->getSize())<< endl;
            }
        }

    }
}

string LsCommand::toString() {
    return "ls";
}

LsCommand *LsCommand::makeNew() {
    return this;
}

// ------------------------------- M K D I R
MkdirCommand::MkdirCommand(string args) : BaseCommand(args) {

}

void MkdirCommand::execute(FileSystem &fs) {
    if(!(getArgs().empty())){
        // We should check if the path starts from the root (absolute path) or from the working directory (relative path)

        if(getArgs().at(0)=='/') { // The path we received as an argument is a ~potentially~ ABSOLUTE ONE
            getDirectoryPointer(fs.getRootDirectory(), getArgs().substr(1), true);
        }
        else { // The path we received as an argument is a ~potentially~ RELATIVE ONE
            getDirectoryPointer(fs.getWorkingDirectory(), getArgs(), true);
        }
    }
}

string MkdirCommand::toString() {
    return "mkdir";
}

MkdirCommand *MkdirCommand::makeNew() {
    return this;
}

// ------------------------------- M K F I L E
MkfileCommand::MkfileCommand(string args) : BaseCommand(args) {

}

void MkfileCommand::execute(FileSystem &fs) {
    if (getArgs().size() != 0) { // We see that the PATH isn't empty
        size_t indexOfSlash = getArgs().find_last_of('/');
        if ((indexOfSlash == std::string::npos) || (indexOfSlash == 0)) { // In this case, we don't have a path but a FILE <name/size> to create (in the WORKING DIRECTORY)
            if(indexOfSlash == std::string::npos){
                int size = std::stoi(getArgs().substr(getArgs().find(' ')));
                File* tempFile = new File(getArgs().substr(0, getArgs().find(' ')), size);
                fs.getWorkingDirectory().addFile(tempFile);
            }
            else { // indexOfSlash = 0
                File* tempFile = new File(getArgs().substr(1, getArgs().find(' ')), std::stoi(getArgs().substr(getArgs().find(' '))));
                fs.getRootDirectory().addFile(tempFile);
            }
        }
        else {
            /*
             * We received as an argument a path AND a file <name/size>
             * YESH SLASH
             */
            Directory *tempPtr;
            // We should check if the path starts from the root (absolute path) or from the working directory (relative path)

            if (getArgs().at(0) == '/') { // The path we received as an argument is a ~potentially~ ABSOLUTE ONE
                tempPtr = getDirectoryPointer(fs.getRootDirectory(), getArgs().substr(1, getArgs().find_last_of('/')),
                                              false);
            }
            else { // The path we received as an argument is a ~potentially~ RELATIVE ONE
                tempPtr = getDirectoryPointer(fs.getWorkingDirectory(), getArgs().substr(0, getArgs().find_last_of('/')), false);
            }

            if (tempPtr != nullptr) { // The path we were given is a LEGAL one
                string fileJustName = getArgs().substr(getArgs().find_last_of('/') + 1);
                File* tempFile = new File(fileJustName.substr(0, fileJustName.find(' ')),
                                          std::stoi(fileJustName.substr(fileJustName.find(' ') + 1)));
                tempPtr->addFile(tempFile);
            }
            else cout << "The system cannot find the path specified" << endl; // The path we were given is not a LEGAL one
        }
    }
}

string MkfileCommand::toString() {
    return "mkfile";
}

MkfileCommand *MkfileCommand::makeNew() {
    return this;
}

// ------------------------------- C P
CpCommand::CpCommand(string args) : BaseCommand(args) {

}

void CpCommand:: execute(FileSystem &fs) {
    realCopyExecute(fs, false);
}

bool CpCommand::realCopyExecute(FileSystem &fs, bool abortMission) {
    if (getArgs().find(' ') != std::string::npos) {
        string sourcePath = getArgs().substr(0, getArgs().find(' '));
        string destinationPath = getArgs().substr(getArgs().find(' ') + 1);
        BaseFile *toCopy = getBaseFilePointer(fs, sourcePath, false, "", false, nullptr);
        BaseFile *toInsert = getBaseFilePointer(fs, destinationPath, false, "", false, nullptr);
        BaseFile* toCopyBaseFile = toCopy;
        if (toCopy != nullptr && toInsert != nullptr && toInsert->isDirectory()) {

            if(abortMission) {
                ((Directory*)toInsert)->removeFile(toCopy);
                if(toCopy->isDirectory()){
                    cout << "Can't move directory" << endl;
                    return false;
                }
                else{
                    cout << "Can't move file" << endl;
                    return false;
                }
            }else {
                if(toCopy->isDirectory()){
                    toCopyBaseFile = new Directory(*((Directory*)toCopy));
                    ((Directory*)toCopyBaseFile)->setParent((Directory*)toInsert);
                }
                else{
                    toCopyBaseFile = new File(*((File*)toCopy));
                }
                for (std::size_t i = 0; i <((Directory *)toInsert)->getChildren().size() ; i++) {
                    if ((((Directory *) toInsert)->getChildren().at(i)->getName()) == (toCopyBaseFile->getName())) {
                        if(toCopyBaseFile->isDirectory()){
                            cout << "Can't move directory" << endl;
                            delete toCopyBaseFile;
                            return false;

                        }
                        else{
                            cout << "Can't move file" << endl;
                            delete toCopyBaseFile;
                            return false;
                    }
                }
            }
                ((Directory*)toInsert)->addFile(toCopyBaseFile);
        }
    }
    else {
        cout << "No such file or directory" << endl;
        return false;
    }
}
return true;
}

string CpCommand::toString() {
    return "cp";
}

CpCommand *CpCommand::makeNew() {
    return this;
}
// ------------------------------- M V

MvCommand::MvCommand(string args) : BaseCommand(args) {

}

void MvCommand::execute(FileSystem &fs) {
    CpCommand* tempCpCom = new CpCommand(getArgs());
    if(getArgs().substr(0, getArgs().find(' ')) != getArgs().substr(getArgs().find(' ')+1)){
        if(tempCpCom->realCopyExecute(fs, false)){
            if (getArgs().size() != 0){
                bool* ifRemoved = new bool(false);
                getBaseFilePointer(fs, getArgs().substr(0, getArgs().find(' ')), false, "", true, ifRemoved);
                if((*ifRemoved) == false) {
                    tempCpCom->realCopyExecute(fs, true);
                }
                delete ifRemoved;
            }
        }
    }


    delete tempCpCom;
}

string MvCommand::toString() {
    return "mv";
}

MvCommand *MvCommand::makeNew() {
    return this;
}
// ------------------------------- R E N A M E

/*
 * Consider the extreme case:
 * "Rename / <new name>
 */

RenameCommand::RenameCommand(string args) : BaseCommand(args) {

}

void RenameCommand::execute(FileSystem &fs) {
    if(getArgs().find(' ') != std::string::npos) {
        if(getBaseFilePointer(fs, getArgs().substr(0, getArgs().find(' ')), true, getArgs().substr(getArgs().find(' ')+1), false,
                              nullptr) == nullptr){
            cout << "No such file or directory" << endl;
        }
    }
}

string RenameCommand::toString() {
    return "rename";
}

RenameCommand *RenameCommand::makeNew() {
    return this;
}

// ------------------------------- R E M O V E
RmCommand::RmCommand(string args) : BaseCommand(args) {

}

void RmCommand::execute(FileSystem &fs) {
    if (getArgs().size() != 0){
        bool* ifRemoved = new bool(false);
        getBaseFilePointer(fs, getArgs(), false, "", true, ifRemoved);
        if((*ifRemoved) == false){
            cout << "Can't remove directory" << endl;
        }
        delete ifRemoved;
    }
}


string RmCommand::toString() {
    return "rm";
}

RmCommand *RmCommand::makeNew() {
    return this;
}

// ------------------------------- H I S T O R Y
HistoryCommand::HistoryCommand(string args, const vector<BaseCommand*> &history) : BaseCommand::BaseCommand(args), history(history){
}

void HistoryCommand::execute(FileSystem &fs) {
    for(std::size_t i = 0; i < history.size(); i++){
        cout << i << "\t" + history.at(i)->toString() + " " + (history.at(i))->getArgs() << endl;
    }
}

string HistoryCommand::toString() {
    return "history";
}

HistoryCommand *HistoryCommand::makeNew() {
    return this;
}

// -------------------------------  V E R B O S E
VerboseCommand::VerboseCommand(string args) : BaseCommand(args) {

}

void VerboseCommand::execute(FileSystem &fs) {
    if(getArgs() != "0" && getArgs() != "1" && getArgs() != "2" && getArgs() != "3"){
        cout << "Wrong verbose input" << endl;
    }
    else {
        int tempVerboseValueForNoamTheKing = stoi(getArgs());
        verbose = tempVerboseValueForNoamTheKing;
    }
}

string VerboseCommand::toString() {
    return "verbose";
}

VerboseCommand *VerboseCommand::makeNew() {
    return this;
}

// ------------------------------- E R R O R
ErrorCommand::ErrorCommand(string args) : BaseCommand(args) {

}

void ErrorCommand::execute(FileSystem &fs) {
    cout << getArgs() + ": Unknown command" << endl;
}

string ErrorCommand::toString() {
    return "error";
}

ErrorCommand *ErrorCommand::makeNew() {
    return this;
}

// -------------------------------  E X E C
ExecCommand::ExecCommand(string args, const vector<BaseCommand *> &history): BaseCommand(args), history(history){

}

void ExecCommand::execute(FileSystem &fs) {
    std::size_t  tempVerboseValueForNoamTheKing = stoi(getArgs());
    if((tempVerboseValueForNoamTheKing < 0) || (tempVerboseValueForNoamTheKing >= history.size())){
        cout << "Command not found" << endl;
    }
    else{
        BaseCommand* dejaVuCommand = history.at(tempVerboseValueForNoamTheKing)->makeNew();
        dejaVuCommand->execute(fs);
    }
}


string ExecCommand::toString() {
    return "exec";
}


ExecCommand *ExecCommand::makeNew() {
    return this;
}
// -------------------------------