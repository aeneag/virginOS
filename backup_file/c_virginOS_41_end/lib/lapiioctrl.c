/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: lapiioctrl.c
* description:文件管理API文件
************************************************************************************************/
#include "libtypes.h"
#include "libheads.h"

sysstus_t api_ioctrl(hand_t fhand,buf_t buf,uint_t iocode,uint_t flgs)
{
    sysstus_t rets;
    API_ENTRY_PARE4(INR_FS_IOCTRL,rets,fhand,buf,iocode,flgs);
    return rets;
}