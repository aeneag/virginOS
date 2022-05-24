/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: lapilseek.c
* description:文件管理API文件
************************************************************************************************/
#include "libtypes.h"
#include "libheads.h"

sysstus_t api_lseek(hand_t fhand,uint_t lofset,uint_t flgs)
{
    sysstus_t rets;
    API_ENTRY_PARE3(INR_FS_LSEEK,rets,fhand,lofset,flgs);
    return rets;
}