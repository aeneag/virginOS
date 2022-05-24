/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: 
* description:
************************************************************************************************/
#ifndef LIBMM_H
#define LIBMM_H
void* mallocblk(size_t blksz);
sysstus_t mfreeblk(void* fradr,size_t blksz);
#endif

