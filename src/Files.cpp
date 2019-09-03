//
// Created by Bosmat&Nikita on 08/11/17.
//
#include <algorithm>
#include <iostream>
#include "../include/Files.h"
#include "../include/GlobalVariables.h"
// ------------------------------------------------------------------

BaseFile::BaseFile(string name):name(name) {

}

BaseFile::~BaseFile() {}

string BaseFile::getName() const {
    return name;
}

void BaseFile::setName(string newName) {
    name = newName;
}

File::File(string name, int size): BaseFile(name), size(size) {
}

int File::getSize() {
    return size;
}
/*
 * CONSTRUCTOR
 */
bool File::isDirectory() {
    return false;
}

// File::~File() {}

// ------------------------------------------------------------------

// Constructor
Directory::Directory(string name, Directory *parent): BaseFile(name), children(), parent(parent) {
    if((verbose == 1) || (verbose == 3)){
        cout << "Directory::Directory(string name, Directory *parent)" << endl;
    }

}
// -------------------------- RULE OF 5 ----------------------------
// Destructor
Directory::~Directory() {
    if((verbose == 1) || (verbose == 3)){
        cout << "Directory::~Directory()" << endl;
    }


    if (children.size() != 0) {
        for (std::vector<BaseFile *>::iterator childrenIterator = children.begin(); childrenIterator != children.end(); childrenIterator++) {
            if ((*childrenIterator) != nullptr) {
                BaseFile* tempPtr = *childrenIterator;
                delete tempPtr;
                (*childrenIterator) = nullptr;
            }
        }
    }
    // delete parent; - We shouldn't delete because the default destructor will be deployed

    children.clear();
    // delete &children;

    parent = nullptr;

}

// Copy Constructor

Directory::Directory(Directory &other) : BaseFile(other.getName()), children(), parent(other.getParent()) {
    if((verbose == 1) || (verbose == 3)){
        cout << "Directory::Directory(Directory &other)" << endl;
    }
    for(std::size_t i = 0; i < other.getChildren().size(); i++){
        if ((other.getChildren().at(i))->isDirectory()){
            BaseFile* tempNewDir = new Directory(*(Directory*)(other.getChildren().at(i)));
            ((Directory*) tempNewDir)->setParent(this);
            this->children.push_back(tempNewDir);
        }
        else{
            BaseFile* tempNewFile = new File(*(File*)(other.getChildren().at(i)));
            this->children.push_back(tempNewFile);
        }
    }
}

//Move Constructor

Directory::Directory(Directory &&other): BaseFile(other.getName()), children(other.getChildren()), parent(other.getParent()){
    if((verbose == 1) || (verbose == 3)){
        cout << "Directory::Directory(Directory &&other)" << endl;
    }
    if (this != &other){
        for(std::size_t i = 0; i < other.getChildren().size(); i++){
            if(this->children.at(i) != nullptr && this->children.at(i)->isDirectory()){
                ((Directory*) (this->children.at(i)))->setParent(this);
            }
            other.getChildren().at(i) = nullptr;
        }
        other.setParent(nullptr);
    }
}

//Copy Assignment
Directory &Directory::operator=(const Directory &other){
    if((verbose == 1) || (verbose == 3)){
        cout << "Directory &Directory::operator=(const Directory &other)" << endl;
    }
    this->setName(other.getName());
    this->setParent(other.getParent());
    if(this != &other){
        for(std::size_t i = 0; i < children.size(); i++){
            if((children.at(i) != nullptr)){
                delete children.at(i);
                children.at(i) = nullptr;
            }
            children.erase(children.begin()+i);
        }
    }

    for(std::size_t i = 0; i < other.children.size(); i++){
        if ((other.children.at(i))->isDirectory()){
            BaseFile* tempNewDir = new Directory(*(Directory*)(other.children.at(i)));
            ((Directory*) tempNewDir)->setParent(this);
            this->children.push_back(tempNewDir);
        }
        else{
            BaseFile* tempNewFile = new File(*(File*)(other.children.at(i)));
            this->children.push_back(tempNewFile);
        }
    }


    return *this;
}

//Move Assignment
Directory &Directory::operator=(Directory &&other) {
    if((verbose == 1) || (verbose == 3)){
        cout << "Directory &Directory::operator=(Directory &&other)" << endl;
    }
    this->setName(other.getName());
    this->setParent(other.getParent());
    if(this != &other){
        for(std::size_t i = 0; i < children.size(); i++){
            if((children.at(i) != nullptr)){
                delete children.at(i);
                children.at(i) = nullptr;
            }
            children.erase(children.begin()+i);
        }
    }

    for(std::size_t i = 0; i < other.getChildren().size(); i++){
        if(this->children.at(i)->isDirectory()){
            ((Directory*) (this->children.at(i)))->setParent(this);
        }
        other.getChildren().at(i) = nullptr;
    }
    other.setParent(nullptr);

    return *this;
}
// -------------------------------------------------------------
bool Directory::isDirectory() {
    return true;
} // InstanceOf alternative
Directory *Directory::getParent() const {
    return parent;
}
void Directory:: setParent(Directory *newParent) {
    parent = newParent;
}
void Directory:: addFile(BaseFile *file) {
    if(file != nullptr){
        for (std::size_t i = 0; i <children.size() ; i++) {
            if (children.at(i)->getName() == file->getName()) {
                if (file->isDirectory()) {
                    cout << "The directory already exists" << endl;
                }
                else {
                    cout << "File already exists" << endl;
                }
                delete file;
                return;
            }
        }
        children.push_back(file);
    }
} // CHANGE TO SUPPORT A CHECK FOR DUPLICATES
void Directory:: removeFile(string name) {
    for (std::size_t i = 0; i < children.size(); ++i) {
        if (children.at(i)->getName() == name) {
            delete children.at(i);
            children.erase(children.begin() + i);
            return;
        }
    }
}
void Directory:: removeFile(BaseFile *file) {
    for (std::size_t i = 0; i < children.size(); ++i) {
        if(children.at(i) == file){
            delete children.at(i);
            children.erase(children.begin()+i);
            return;
        }
    }
}
bool Directory:: compareByNames(BaseFile *a, BaseFile *b) {
    return ((a->getName()) < (b->getName()));
}
void Directory:: sortByName() {
    std::sort(children.begin(),children.end(), compareByNames);
}
bool Directory:: compareBySize(BaseFile *a, BaseFile *b) {
    if(a->getSize() == b->getSize()){
        return ((a->getName()) < (b->getName()));
    }
    else{
        return ( (a->getSize()) < (b->getSize()) );
    }
}
void Directory:: sortBySize() {
    std::sort(children.begin(), children.end(), compareBySize);
}
vector<BaseFile *> Directory::getChildren() {
    return children;
}
int Directory::getSize() {
    if(children.size()==0) return 0;
    int totalSize = 0;
    for(std::vector<BaseFile*>::iterator childrenIterator = children.begin() ; childrenIterator != children.end(); childrenIterator++){
        totalSize = totalSize + ((*childrenIterator)->getSize());
    }
    return totalSize;
}
string Directory::getAbsolutePath() {
    string absolutePath = "";
    //check
    if(this->parent == nullptr)
        absolutePath ="/";
    else if((this->parent)->getParent() == nullptr)
        absolutePath = "/"+this->getName();
    else
        absolutePath = parent->getAbsolutePath()+"/"+this->getName();
    return absolutePath;
}
