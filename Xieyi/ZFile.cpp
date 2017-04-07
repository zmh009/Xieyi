#include "ZFile.h"

ZFile::ZFile()
{

}

ZFile::~ZFile()
{

}

/* 向指定文件追加写入数据，如果文件或文件所在目录不存在则会创建
 * @param    filePath    string,待写入的文件
 * @param    data        string,待写入的数据
 * @param    isBinary    bool,是否以二进制方式写入
 * @return   成功为0，出错为-1
 */
int ZFile::write(const std::__cxx11::string &filePath, const std::__cxx11::string &data, bool isBinary)
{
    cout << "write to " <<filePath <<"("<<data.length()<<")"<< endl;
    if (filePath.empty() || data.empty())
    {
        // 空数据不操作
        return 0;
    }

    // 创建该文件所需的目录
    if (-1 == createDir(filePath))
    {
        return -1;
    }

    // 写入的方式为追加，支持写入二进制数据
    ios_base::openmode mode = ios_base::out | ios_base::app;
    if (isBinary)
    {
        mode |= ios_base::binary;
    }

    mOfstr.open(filePath, mode);
    mOfstr << data;
    mOfstr.close();
    // 判断是否写入成功
    if (!mOfstr)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/* 对一个文件创建其所在的目录,字符'/'前被认为是目录名
 * @param    filePath    string,待创建目录的文件
 * @return   成功为0，出错为-1
 */
int ZFile::createDir(const std::__cxx11::string &filePath)
{
    if (filePath.empty())
    {
        // 空文件不创建
        return 0;
    }

    // 判断该文件所在的目录是否已经存在
    int pos = filePath.find_last_of('/');
    string object = string(filePath.c_str(), pos);
    if (0 == access(object.c_str(), F_OK))
    {
        return 0;
    }

    // 权限为drwxr_xr_x
    int mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    for (size_t i = 1; i < filePath.size(); ++i) { // 跳过i == 0 且该字符为'/'的情况
        if (filePath[i] != '/') { // 跳过为'/'的字符，以获得正确的目录名
            continue;
        }

        // 创建目录
        string part = string(filePath.c_str(), i);
        const char *dir = part.c_str();
        if (-1 == access(dir, F_OK)) {
            // 如果目录不存在则创建
            int ret = mkdir(dir, mode);
            // 如果创建出错则返回-1
            if (ret == -1) {
                return -1;
            }
        }
    }
    return 0;
}
