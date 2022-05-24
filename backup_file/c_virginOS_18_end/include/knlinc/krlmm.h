//Aen @2022.04.08
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlmm.h
//description: 内核层内存管理头文件
#ifndef _KRLMM_H
#define _KRLMM_H
void init_krlmm();

adr_t krlnew(size_t mmsize);

bool_t krldelete(adr_t fradr,size_t frsz);
#endif // _KRLMM_H
