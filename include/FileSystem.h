#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Files.h"
#include "GlobalVariables.h"
#include <iostream>


class FileSystem {
private:
	Directory* rootDirectory;
	Directory* workingDirectory;
public:
    // RULE OF 5
//Destructor
    virtual ~FileSystem();
//Copy Constructor
    FileSystem(const FileSystem &other);
//Move Constructor
    FileSystem(FileSystem &&other);
//Copy Assignment
    FileSystem& operator =(const FileSystem &other);
//Move Assignment
    FileSystem& operator =(FileSystem &&other);
	FileSystem();
	Directory& getRootDirectory() const; // Return reference to the root directory
	Directory& getWorkingDirectory() const; // Return reference to the working directory
	void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system


};


#endif
