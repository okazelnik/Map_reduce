//
// Created by sophie_c on 4/26/16.
//

#include "MapReduceClient.h"
#include "MapReduceFramework.h"
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <setjmp.h>
#include <unistd.h>
#include "string.h"
#include <queue>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>


#define FIRST_ELEMENT 0
#define SUCCESS 0
#define FIRST_FOLDER 1
#define SUBSTRING 0

using namespace std;

string subString;

typedef std::list<v2Base*> V2_LIST;

// the directory we are looking the files in.
class DirToSearch : public k1Base{
public:
    string path;
    DirToSearch(string file): path(file){};
    bool operator<(const k1Base &other) const {
        return true;
    };
    ~DirToSearch(){};
};

// the value of the directory, doesn't matter because we didn't use it.
class DirValue : public v1Base{
public:
    string name;
    DirValue(string val): name(val){};

};

// the file which includes the correct substring (in the example is "os").
class FileSubStr : public k2Base{
public:
    string name;
    FileSubStr(string fileName): name(fileName){};
    bool operator<(const k2Base &other) const {
        FileSubStr* src = (FileSubStr*) this;
        FileSubStr* toCheck = (FileSubStr*) &other;
        if (toCheck->name < src->name){
            return true;
        }

        return false;
    }
    ~FileSubStr(){};
};

// the num of times the substring file appears (in our example 1 for
// each pair)
class NumOfApperance : public v2Base{
public:
    int num;
    NumOfApperance(int val): num(val){};
    ~NumOfApperance(){};

};

// the key will be the same has in k2Base.
class exFileName : public k3Base{
public:
    string name;
    exFileName(string fileName): name(fileName){};
    bool operator<(const k3Base &other) const{
        exFileName* src = (exFileName*) this;
        exFileName* toCheck = (exFileName*) &other;
        if (toCheck->name < src->name){
            return true;
        }

        return false;
    }
};

//after the shuffle we will see the number of appearances of the file.
class ApperanceOfFileName : public v3Base{
public:

    int num;
    ApperanceOfFileName(int val): num(val){};
    ~ApperanceOfFileName(){};

};

// map function of search.
class Search : public MapReduceBase{
public:

    void Map(const k1Base *const key, const v1Base *const value) const{

        // creating a dir object.
        DIR *pDir;
        struct dirent *entry;

        // initialize the path and the substring to check.
        string path = ((DirToSearch*)key)->path;
        //string subString = "os";
        string checkSubStr = ((DirValue*)value)->name;

        // checking if the dir can be open and seeking for the files in it.
        if ((pDir = opendir(path.c_str()))){
            while((entry = readdir(pDir))){

                if (strcmp(entry->d_name, ".") != SUCCESS && strcmp(entry->d_name, "..") != SUCCESS){

                    if (path.find(checkSubStr) != std::string::npos){

                        // creating a new pair of (fileName.numOfApperance).
                        NumOfApperance* apperanceOfFile = new NumOfApperance(1);
                        FileSubStr* k2 = new FileSubStr(entry->d_name);
                        Emit2(k2,apperanceOfFile);
                    }
                }
            }

            closedir(pDir);

        }

        delete key;
        delete value;
    }

    void Reduce(const k2Base *const key, const V2_LIST &values) const{

        string fileName = ((FileSubStr*) key)-> name;
        int apperances = 0;

        // sum of values in each key (fileName) we have.
        for (v2Base* val: values){

            apperances += ((NumOfApperance*) val)->num;

        }

        exFileName* returnFileName = new exFileName(fileName);
        ApperanceOfFileName* apperanceOfFile =  new ApperanceOfFileName(apperances);
        Emit3(returnFileName, apperanceOfFile);

        for (v2Base* val: values){

            delete val;
        }
    }
};

/**
 * method which prints out to the user the names of the files which
 * answer the demand of a substring file name.
 */
void printNames(OUT_ITEMS_LIST& checkList){

    // pointers to the begin and the end of the list.
    auto start = checkList.begin();
    //auto end = checkList.end();

    int sizeOfList = checkList.size();
    int amount = 0;

    for (int i = 0; i < sizeOfList; i++){

        exFileName* tempFileName = (exFileName*) (*start).first;
        ApperanceOfFileName* tempAmount = (ApperanceOfFileName*) (*start).second;
        amount = tempAmount-> num;

        for (int j = 0; j < amount; j++){

            cout << (tempFileName-> name) << " ";
        }
        cout << endl;
        start++;
    }
}


// the main function
int main(int argc, char *argv[]){

    // Initialize the relevant variables.
    int numOfFolders = 0;
    subString =  argv[SUBSTRING];
    struct stat sb;
    Search searchObj;
    IN_ITEMS_LIST inputList;
    OUT_ITEMS_LIST outputList;

    if (argc < 2){
        //no output will be printed
        return 0;
    }

    // creating an input list of pairs (directoryName, dirValue)
    // the dirValue dosen't matter and its like a null.
    for (int i = FIRST_FOLDER; i < (argc - 1); i++){

        if (stat(argv[i], &sb) == SUCCESS && S_ISDIR(sb.st_mode)){

            numOfFolders++;
            DirToSearch* dirName = new DirToSearch(argv[i]);
            DirValue* dirVal = new DirValue(subString);
            inputList.push_back(IN_ITEM(dirName,dirVal));
        }
    }

    // Run the framework
    outputList = runMapReduceFramework(searchObj, inputList, numOfFolders);

    // prints the items that were found by the map reduce framework
    printNames(outputList);

    // deletes the items for the output list.
    for (OUT_ITEM idx : outputList){

        delete idx.first;
        delete idx.second;
    }

    inputList.clear();
    outputList.clear();

    return 0;
}


