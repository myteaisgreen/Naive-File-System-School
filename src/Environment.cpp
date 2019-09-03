//
// Created by Bosmat&Nikita on 08/11/17.
//

#include <iostream>
#include "../include/FileSystem.h"
#include "../include/Commands.h"
#include "../include/Environment.h"


Environment::Environment() : commandsHistory(), fs() {
}

void Environment::start() {
    cout << "/> ";
    string userInput;
    getline(cin, userInput);
    BaseCommand* newCommand;
    while(userInput != "exit"){
        if(((verbose == 2) | (verbose == 3))){
            cout << userInput << endl;
        }
        if(userInput.find("pwd") == 0){
            newCommand = new PwdCommand("");
            ((PwdCommand*)newCommand)->execute(fs);
        }
        else if(userInput.find("cd ") == 0){
            newCommand = new CdCommand(userInput.substr(userInput.find(' ')+1));
            ((CdCommand*)newCommand)->execute(fs);
        }
        else if(userInput.find("ls") == 0){
            if(userInput.find(' ') != std::string::npos)
                newCommand = new LsCommand(userInput.substr(userInput.find(' ')+1));
            else {
                newCommand = new LsCommand("");
            }
            ((LsCommand*)newCommand)->execute(fs);
        }
        else if(userInput.find("mkdir ") == 0){
            newCommand = new MkdirCommand(userInput.substr(userInput.find(' ')+1));
            ((MkdirCommand*)newCommand)->execute(fs);
        }
        else if(userInput.find("mkfile ") == 0){
            newCommand = new MkfileCommand(userInput.substr(userInput.find(' ')+1));
            ((MkfileCommand*)newCommand)->execute(fs);
        }
        else if(userInput.find("cp ") == 0){
            newCommand = new CpCommand(userInput.substr(userInput.find(' ')+1));
            ((CpCommand*)newCommand)->execute(fs);
        }
        else if(userInput.find("mv ") == 0){
            newCommand = new MvCommand(userInput.substr(userInput.find(' ')+1));
            ((MvCommand*)newCommand)->execute(fs);
        }
        else if(userInput.find("rename ") == 0){
            newCommand = new RenameCommand(userInput.substr(userInput.find(' ')+1));
            ((RenameCommand*)newCommand)->execute(fs);
        }
        else if(userInput.find("rm ") == 0){
            newCommand = new RmCommand(userInput.substr(userInput.find(' ')+1));
            ((RmCommand*)newCommand)->execute(fs);
        }
        else if(userInput == "history"){
            newCommand = new HistoryCommand("", commandsHistory);
            ((HistoryCommand*)newCommand)->execute(fs);
        }
        else if(userInput.find("verbose") == 0){
            newCommand = new VerboseCommand(userInput.substr(userInput.find(' ')+1));
            ((VerboseCommand*)newCommand)->execute(fs);
        }
        else if(userInput.find("exec") == 0){
            newCommand = new ExecCommand(userInput.substr(userInput.find(' ')+1), commandsHistory);
            ((ExecCommand*)newCommand)->execute(fs);
        }
        else {
            if (userInput.find(' ') != string::npos) {
                newCommand = new ErrorCommand(userInput.substr(0,userInput.find(' ')));
            } else {
                newCommand = new ErrorCommand(userInput);
            }
            ((ErrorCommand*)newCommand)->execute(fs);
        }

        addToHistory(newCommand);

        cout << fs.getWorkingDirectory().getAbsolutePath() + "> ";
        getline(cin, userInput);
    }
}

FileSystem &Environment::getFileSystem()  {
    return fs;
}

void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

std::vector<BaseCommand*> &Environment:: getHistory() {
    return commandsHistory;
}

// RULE OF 5
//Destructor
Environment::~Environment() {
    if((verbose == 1) || (verbose == 3)){
        cout << "Environment::~Environment()" << endl;
    }
    if (commandsHistory.size() != 0) {
        for (std::vector<BaseCommand*>::iterator commandsHistoryIterator = commandsHistory.begin(); commandsHistoryIterator != commandsHistory.end(); commandsHistoryIterator++) {
            delete *commandsHistoryIterator;
        }
    }

    // delete &commandsHistory;
    // delete &fs;
}

//Copy Constructor
Environment::Environment(const Environment &other) : commandsHistory(), fs(other.fs){
    if((verbose == 1) || (verbose == 3)){
        cout << "Environment::Environment(const Environment &other)" << endl;
    }
    for(std::size_t i = 0; i < other.commandsHistory.size(); i++){
        commandsHistory.push_back(other.commandsHistory.at(i)->makeNew());
    }
}

//Move Constructor
Environment::Environment(Environment &&other) : commandsHistory(other.commandsHistory), fs(std::move(other.fs)) {
    if((verbose == 1) || (verbose == 3)){
        cout << "Environment::Environment(const Environment &other)" << endl;
    }
    for(std::size_t i = 0; i < other.commandsHistory.size(); i++){
        other.commandsHistory.at(i) = nullptr;
    }
}

//Copy Assignment
Environment &Environment::operator=(const Environment &other){
    if((verbose == 1) || (verbose == 3)){
        cout << "Environment &Environment::operator=(const Environment &other)" << endl;
    }
    if(this != &other){
        fs = other.fs;
        for (std::size_t i = 0; i < commandsHistory.size() ; ++i) {
            if(commandsHistory.at(i) != nullptr){
                delete (commandsHistory.at(i));
                commandsHistory.at(i) = nullptr;
            }
            commandsHistory.erase(commandsHistory.begin()+i);
        }
        for(std::size_t i = 0; i < other.commandsHistory.size(); i++){
            commandsHistory.push_back(other.commandsHistory.at(i)->makeNew());
        }
    }
    return *this;
}


Environment &Environment::operator=(Environment &&other) {
    if((verbose == 1) || (verbose == 3)){
        cout << "Environment &Environment::operator=(Environment &&other)" << endl;
    }
    if(this != &other){
        delete &fs;
        for (std::size_t i = 0; i < commandsHistory.size() ; ++i) {
            if(commandsHistory.at(i) != nullptr){
                delete (commandsHistory.at(i));
                commandsHistory.at(i) = nullptr;
            }
            commandsHistory.erase(commandsHistory.begin()+i);
        }
        delete &commandsHistory;
        fs = other.fs;
        commandsHistory = other.commandsHistory;
        delete &(other.commandsHistory);
    }
    return *this;
}

