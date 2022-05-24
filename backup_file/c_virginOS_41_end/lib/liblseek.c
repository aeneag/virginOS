/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: 
* description:
************************************************************************************************/
#include "libc.h"
sysstus_t lseek(hand_t fhand,uint_t lofset,uint_t flgs)
{
    sysstus_t rets=api_lseek(fhand,lofset,flgs);
    return rets;
}