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

class ZFile
{
public:
    ZFile();
    ~ZFile();

    int write(const string &filePath, const string &data, bool isBinary);
    string getError() const;

private:
    int createDir(const string &filePath);
    ofstream mOfstr;

    string mError;
};

#endif // ZFILE_H
