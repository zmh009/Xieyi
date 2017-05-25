#include "ZBeheviorResponse.h"

ZBeheviorResponse::ZBeheviorResponse()
{

}

int ZBeheviorResponse::record(const std::__cxx11::string &information, const std::__cxx11::string &filePath)
{
    ofstream ofile;
    ofile.open(filePath, std::ios_base::app);
    if (!(ofile << information))
    {
        return -1;
    }
    ofile.close();
    return 0;
}

int ZBeheviorResponse::refuseByIp(const std::__cxx11::string &ip)
{
    return 0;
}
