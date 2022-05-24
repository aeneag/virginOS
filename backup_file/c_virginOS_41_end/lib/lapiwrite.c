/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: lapiwrite.c
* description:文件管理API文件
************************************************************************************************/
#include "libtypes.h"
#include "libheads.h"

sysstus_t api_write(hand_t fhand,buf_t buf,size_t len,uint_t flgs)
{
    sysstus_t rets;
    API_ENTRY_PARE4(INR_FS_WRITE,rets,fhand,buf,len,flgs);
    return rets;
}