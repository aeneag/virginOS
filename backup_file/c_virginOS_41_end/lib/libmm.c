/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: 
* description:
************************************************************************************************/
#include "libc.h"
void* mallocblk(size_t blksz)
{
    void* retadr=api_mallocblk(blksz);
    return retadr;
}

sysstus_t mfreeblk(void* fradr,size_t blksz)
{
    sysstus_t retstus=api_mfreeblk(fradr,blksz);

    return retstus;
}

