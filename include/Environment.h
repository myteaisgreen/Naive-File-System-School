#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Files.h"
#include "Commands.h"
#include "GlobalVariables.h"
#include <string>
#include <vector>

using namespace std;

class Environment {
private:
	vector<BaseCommand*> commandsHistory;
	FileSystem fs;

public:

    // RULE OF 5
//Destructor
    virtual ~Environment();
//Copy Constructor
    Environment(const Environment &other);
//Move Constructor
    Environment(Environment &&other);
//Copy Assignment
    Environment& operator =(const Environment &other);
//Move Assignment
    Environment& operator =(Environment &&other);
	Environment();
	void start();
	FileSystem& getFileSystem() ; // Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	vector<BaseCommand*>& getHistory(); // Return a reference to the history of commands
};

#endif