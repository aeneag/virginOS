/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: lapimm.c
* description:内存管理API文件
************************************************************************************************/
#include "libtypes.h"
#include "libheads.h"

void* api_mallocblk(size_t blksz)
{
    void* retadr;
    API_ENTRY_PARE1(INR_MM_ALLOC,retadr,blksz);
    return retadr;
}

sysstus_t api_mfreeblk(void* fradr,size_t blksz)
{
    sysstus_t retstus;
    API_ENTRY_PARE2(INR_MM_FREE,retstus,fradr,blksz);
    return retstus;
}