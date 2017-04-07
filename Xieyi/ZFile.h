#ifndef ZFILE_H
#define ZFILE_H

extern "C"{
#include <sys/stat.h>
#include <unistd.h>
}

#include <string>
#include <fstream>
using std::string;
using std::ofstream;
using std::ios_base;

//-- test
#include <iostream>
using std::cout;
using std::endl;

class ZFile
{
public:
    ZFile();
    ~ZFile();

    int write(const string &filePath, const string &data, bool isBinary);

private:
    int createDir(const string &filePath);
    // bool IsExist();
    ofstream mOfstr;
};

#endif // ZFILE_H
