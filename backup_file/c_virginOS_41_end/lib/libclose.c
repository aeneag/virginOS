/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: 
* description:
************************************************************************************************/
#include "libc.h"

sysstus_t close(hand_t fhand)
{

    sysstus_t rets=api_close(fhand);
    return rets;
}
