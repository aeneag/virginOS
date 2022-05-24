/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: 
* description:
************************************************************************************************/
#include "libc.h"
sysstus_t ioctrl(hand_t fhand,buf_t buf,uint_t iocode,uint_t flgs)
{
    sysstus_t rets=api_ioctrl(fhand,buf,iocode,flgs);
    return rets;
}