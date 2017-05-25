#ifndef ZSQL_H
#define ZSQL_H

#include <string>
#include <map>
#include <list>
#include <mysql/mysql.h>
//#include <QtSql/QSqlDatabase>
//#include <QtSql/QSqlQuery>
//#include <QSqlError>
//#include <QSqlRecord>
using std::string;
using std::map;
using std::list;

//test
#include <QDebug>

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
//    int insert(const string &table, const map<string, valueType> &insertData);
    int insert(const string &sql);
    int binaryEscape(const string &src, string &dst);

//    int select(const string &table, const std::__cxx11::string &condition = "1");
    int select(const string &sql);
    int select(rowValueType &value);
    int fieldCount();
    bool selectEnd(){return mSelectEnd;}
//    int getByName(const string &Name, string &value);
//    int getByName(const string &Name);
//    bool next(){return mQuery.next();}

    string getError(){return mError;}
private:
//    QSqlDatabase mMySQLDb;
//    QSqlQuery mQuery;
    MYSQL mMysqlDb;
    MYSQL_RES *mResult;
    string mError;
    bool mSelectEnd;
};

#endif // ZSQL_H
