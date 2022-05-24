/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: 
* description:
************************************************************************************************/
#include "libc.h"
hand_t exel(void* file,uint_t flgs)
{
    hand_t rethand=api_exel_thread(file,flgs);
    return rethand;
}

void exit()
{
    api_exit_thread();
    return;
}


hand_t pid(void* tname)
{
    hand_t rethand=api_retn_threadhand(tname);
    return rethand;
}

sysstus_t retn_threadstats(hand_t thand,uint_t scode,uint_t data,buf_t buf)
{
    sysstus_t rets=api_retn_threadstats(thand,scode,data,buf);
    return rets;
}

sysstus_t set_threadstats(hand_t thand,uint_t scode,uint_t data,buf_t buf)
{
    sysstus_t rets=api_set_threadstats(thand,scode,data,buf);
    return rets;
}
