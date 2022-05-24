/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: 
* description:
************************************************************************************************/
#include "libc.h"
hand_t open(void* file,uint_t flgs,uint_t stus)
{
    hand_t rethand=api_open(file,flgs,stus);
    
    return rethand;
}