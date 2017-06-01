#ifndef ZSQL_H
#define ZSQL_H

#include <string>
#include <map>
#include <list>
#include <mysql/mysql.h>

using std::string;
using std::map;
using std::list;

class ZSQL
{
public:
    typedef MYSQL_ROW rowValueType;
public:
    ZSQL();
    ~ZSQL();
    int connect(const string &user,
                const string &password,
                const string &dbName,
                const string &hostName = "localhost", short port = 3306);
    void close();
    int insert(const string &sql);
    int binaryEscape(const string &src, string &dst);

    int select(const string &sql);
    int select(rowValueType &value);
    int fieldCount();
    bool selectEnd(){return mSelectEnd;}

    string getError(){return mError;}
private:
    MYSQL mMysqlDb;
    MYSQL_RES *mResult;
    string mError;
    bool mSelectEnd;
};

#endif // ZSQL_H
