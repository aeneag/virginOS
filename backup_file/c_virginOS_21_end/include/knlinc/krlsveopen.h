//Aen @2022.04.08
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlsveopen.h
//description: 内核服务头文件
#ifndef _KRLSVEOPEN_H
#define _KRLSVEOPEN_H
sysstus_t krlsvetabl_open(uint_t swinr,stkparame_t* stkparv);
hand_t krlsve_open(void* file,uint_t flgs,uint_t stus);
hand_t krlsve_core_open(void* file,uint_t flgs,uint_t stus);
sysstus_t krlsve_open_device(objnode_t* ondep);
#endif // KRLSVEOPEN_H
