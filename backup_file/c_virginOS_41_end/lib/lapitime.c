/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: lapitime.c
* description:文件管理API文件
************************************************************************************************/
#include "libtypes.h"
#include "libheads.h"


sysstus_t api_time(buf_t ttime)
{
    
    sysstus_t rets;
    API_ENTRY_PARE1(INR_TIME,rets,ttime);
    return rets;
}

sysstus_t api_tick(uint_t id)
{
    
    sysstus_t rets;
    API_ENTRY_PARE1(INR_TD_TICK,rets,id);
    return rets;
}