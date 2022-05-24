/************************************************************************************************
* Aen @2022.05.20
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: lapimm.h
* description:内存管理API头文件
************************************************************************************************/
#ifndef _LAPIMM_H
#define _LAPIMM_H
void* api_mallocblk(size_t blksz);
sysstus_t api_mfreeblk(void* fradr,size_t blksz);
void test_api();
#endif // LAPIMM_H
