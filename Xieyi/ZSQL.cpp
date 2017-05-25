#include "ZSQL.h"

ZSQL::ZSQL()
{
    mysql_init(&mMysqlDb);
    mResult = nullptr;
    mSelectEnd = true;
}

ZSQL::~ZSQL()
{
    if (mResult != nullptr)
    {
        mysql_free_result(mResult);
    }
}

int ZSQL::connect(const std::__cxx11::string &user, const std::__cxx11::string &password, const std::__cxx11::string &dbName, const std::__cxx11::string &hostName, short port)
{
    if ( nullptr == mysql_real_connect(&mMysqlDb,
                                       hostName.c_str(),
                                       user.c_str(),
                                       password.c_str(),
                                       dbName.c_str(),
                                       port,
                                       nullptr, 0))
    {
        mError = "connect to " + hostName + ":" + std::to_string(port) + "("+ user +"@"+ dbName +") error";
        return -1;
    }
    return 0;
//    mMySQLDb = QSqlDatabase::addDatabase("QMYSQL");
//    mMySQLDb.setHostName(QString(hostName.c_str()));
//    mMySQLDb.setPort(port);
//    mMySQLDb.setDatabaseName(QString(dbName.c_str()));
//    mMySQLDb.setUserName(QString(user.c_str()));
//    mMySQLDb.setPassword(QString(password.c_str()));
//    if (mMySQLDb.open())
//    {
//        return 0;
//    }
//    else
//    {
//        mError = "connect to " + hostName + ":" + std::to_string(port) + "("+ user +"@"+ dbName +") error";
//        return -1;
//    }

}

void ZSQL::close()
{
    mysql_close(&mMysqlDb);
}

//int ZSQL::insert(const string &table, const map<std::__cxx11::string, ZSQL::valueType> &insertData)
//{
//    if (table.empty())
//    {
//        mError = "table is empty";
//        return -1;
//    }
//    else if (insertData.empty())
//    {
//        mError = "insert data is empty";
//        return -1;
//    }

//    // 拼接SQL插入的语句
//    string prepareSql = "insert " + table + "(";
//    for (const auto& each: insertData)
//    {
//        prepareSql += each.first + ",";
//    }
//    prepareSql.pop_back(); // 去掉最后的逗号
//    prepareSql += ") value("; // 此处prepareSQL类似insert table (column1,column2,...) value(

//    // 添加占位符
//    for (size_t i = 0; i < insertData.size(); ++i)
//    {
//        prepareSql += "?,";
//    }
//    // 修改SQL语句格式，去掉最后的逗号并改为右括号
//    prepareSql.back() = ')';  // 此处prepareSQL类似insert table (column1,column2,...) value(?,?,...)

//    // 添加插入的值
//    for (const auto& each: insertData)
//    {
//        mQuery.addBindValue(each.second);
//    }

//    // 语句执行
//    if (!mQuery.exec())
//    {
//        mError = mQuery.lastError().text().toStdString();
//        return -1;
//    }
//    return 0;
//}

int ZSQL::insert(const std::__cxx11::string &sql)
{
    if (0 != mysql_real_query(&mMysqlDb, sql.c_str(), sql.length()))
    {
        mError = mysql_error(&mMysqlDb);
        return -1;
    }
    return 0;
}

int ZSQL::binaryEscape(const std::__cxx11::string &src, std::__cxx11::string &dst)
{
    if (src.empty())
    {
        return 0;
    }

    int maxEscapeLength = src.length() * 2 + 1; // 每个字符都需要转义且有终止符
    char *escapeResult = new char[maxEscapeLength];
    int escapeLength = mysql_real_escape_string(&mMysqlDb, escapeResult, src.c_str(), src.length());
    if (escapeLength > 0)
    {
        dst = escapeResult;
        delete escapeResult;
        return 0;
    }
    else
    {
        mError = "escape error";
        delete escapeResult;
        return -1;
    }
}

int ZSQL::select(const std::__cxx11::string &sql)
{
    if ( 0 != mysql_query(&mMysqlDb, sql.c_str()))
    {
        mError = mysql_error(&mMysqlDb);
        return -1;
    }
    mError = mysql_error(&mMysqlDb);
    if (mResult != nullptr)
    {
        mysql_free_result(mResult);
    }
    mResult = mysql_store_result(&mMysqlDb);

    // 返回查询结果的记录个数
    int count = mysql_num_rows(mResult);
    mSelectEnd = (count == 0); // 无记录则不可查询
    return count;
}

int ZSQL::select(rowValueType &value)
{
    value = mysql_fetch_row(mResult);
    return 0;
}

int ZSQL::fieldCount()
{
    return mysql_num_fields(mResult);
}

//int ZSQL::getByName(const std::__cxx11::string &Name)
//{
//    mysql_fe
//}

//int ZSQL::select(const std::__cxx11::string &table, const string &condition)
//{
//    if (table.empty())
//    {
//        mError = "table is empty";
//        return -1;
//    }
//    // 拼接SQL查询的语句
//    string sql = "select * from " + table + "where " + condition;
//    if (!mQuery.exec(sql.c_str()))
//    {
//        mError = mQuery.lastError().text().toStdString();
//        return -1;
//    }
//    return mQuery.size();
//}

//int ZSQL::getByName(const std::__cxx11::string &Name, std::__cxx11::string &value)
//{
//    if (Name.empty())
//    {
//        mError = "select vaild column";
//        return -1;
//    }

//    QVariant result = mQuery.value(Name.c_str());
//    if (result.isValid())
//    {
//        value = result.toString().toStdString();
//        return 0;
//    }
//    else
//    {
//        mError = "select vaild column";
//        return -1;
//    }
//}
