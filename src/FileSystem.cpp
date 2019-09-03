//
// Created by Bosmat&Nikita on 08/11/17.
//

#include "../include/Files.h"
#include "../include/FileSystem.h"
#include "../include/GlobalVariables.h"
#include <iostream>
// ----------------- RULE OF 5 ----------------------
//Destructor
FileSystem::~FileSystem() {
    if((verbose == 1) || (verbose == 3)){
        cout << "FileSystem::~FileSystem()" << endl;
    }
    if(rootDirectory != nullptr){
        delete rootDirectory;
        rootDirectory = nullptr;
        workingDirectory = nullptr;
    }

}

//Copy Constructor
FileSystem::FileSystem(const FileSystem &other) : rootDirectory(other.rootDirectory), workingDirectory(other.workingDirectory) {
    if((verbose == 1) || (verbose == 3)){
        cout << "FileSystem::FileSystem(const FileSystem &other)" << endl;
    }

}

//Move Constructor
FileSystem::FileSystem(FileSystem &&other) : rootDirectory(std::move(other.rootDirectory)), workingDirectory(std::move(other.workingDirectory)){
    if((verbose == 1) || (verbose == 3)){
        cout << "FileSystem::FileSystem(FileSystem &&other)" << endl;
    }
}

//Copy Assignment
FileSystem &FileSystem::operator=(const FileSystem &other){
    if((verbose == 1) || (verbose == 3)){
        cout << "FileSystem &FileSystem::operator=(const FileSystem &other)" << endl;
    }
    if(this != &other){
        (*rootDirectory) = *(other.rootDirectory);
        (*workingDirectory) = *(other.workingDirectory);
    }

    return *this;
}

//Move Assignment
FileSystem &FileSystem::operator=(FileSystem &&other) {
    if((verbose == 1) || (verbose == 3)){
        cout << "FileSystem &FileSystem::operator=(FileSystem &&other)" << endl;
    }
    if(this != &other){
        rootDirectory = other.rootDirectory;
        workingDirectory = other.workingDirectory;

        other.rootDirectory = nullptr;
        other.workingDirectory = nullptr;
    }
    return *this;
}
// --------------------------------------------------------------------------------
FileSystem::FileSystem() : rootDirectory(new Directory("/", nullptr)), workingDirectory(std::move(rootDirectory)){
    // workingDirectory = rootDirectory;
}

Directory &FileSystem::getRootDirectory() const {
    return *rootDirectory;
}

Directory &FileSystem::getWorkingDirectory() const {
    return *workingDirectory;
}

void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) {
    workingDirectory = newWorkingDirectory;
}
