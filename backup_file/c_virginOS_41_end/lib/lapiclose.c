/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: lapiclose.c
* description:文件管理API文件
************************************************************************************************/
#include "libtypes.h"
#include "libheads.h"

sysstus_t api_close(hand_t fhand)
{

    sysstus_t rets;
    API_ENTRY_PARE1(INR_FS_CLOSE,rets,fhand);
    return rets;
}