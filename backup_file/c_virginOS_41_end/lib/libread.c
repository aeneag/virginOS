/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: 
* description:
************************************************************************************************/
#include "libc.h"
sysstus_t read(hand_t fhand,buf_t buf,size_t len,uint_t flgs)
{
    sysstus_t rets=api_read(fhand,buf,len,flgs);
    return rets;
}
