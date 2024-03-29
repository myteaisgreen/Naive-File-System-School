#ifndef FILES_H_
#define FILES_H_

#include <string>
#include <vector>

using namespace std;

class BaseFile {

private:
    string name;

public:
    BaseFile(string name);
    string getName() const;
    void setName(string newName);
    virtual int getSize() = 0;
    virtual ~BaseFile() = 0;
    virtual bool isDirectory() = 0;
};

class File : public BaseFile {
private:
    int size;
public:
    File(string name, int size); // Constructor
    int getSize(); // Return the size of the file
    virtual bool isDirectory();
    // virtual ~File();
};

class Directory : public BaseFile {
private:
    vector<BaseFile*> children;
    Directory *parent;
    static bool compareByNames(BaseFile *a, BaseFile *b);
    static bool compareBySize(BaseFile *a, BaseFile *b);
public:
    virtual bool isDirectory();
// RULE OF 5
//Destructor
    virtual ~Directory();
//Copy Constructor
    Directory(Directory &other);
//Move Constructor
    Directory(Directory &&other);
//Copy Assignment
    Directory& operator =(const Directory &other);
//Move Assignment
    Directory& operator =(Directory &&other);

    Directory(string name, Directory *parent); // Constructor
    Directory *getParent() const; // Return a pointer to the parent of this directory
    void setParent(Directory *newParent); // Change the parent of this directory
    void addFile(BaseFile* file); // Add the file to children
    void removeFile(string name); // Remove the file with the specified name from children
    void removeFile(BaseFile* file); // Remove the file from children
    void sortByName(); // Sort children by name alphabetically (not recursively)
    void sortBySize(); // Sort children by size (not recursively)
    vector<BaseFile*> getChildren(); // Return children
    int getSize(); // Return the size of the directory (recursively)
    string getAbsolutePath();  //Return the path from the root to this

};

#endif