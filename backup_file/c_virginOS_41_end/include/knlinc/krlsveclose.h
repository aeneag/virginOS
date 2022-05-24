//Aen @2022.04.08
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlsveclose.h
//description: 内核服务头文件
#ifndef _KRLSVECLOSE_H
#define _KRLSVECLOSE_H
sysstus_t krlsvetabl_close(uint_t inr,stkparame_t* stkparv);
sysstus_t krlsve_close(hand_t fhand);
sysstus_t krlsve_core_close(hand_t fhand);
sysstus_t krlsve_close_device(objnode_t* ondep);
#endif // KRLSVECLOSE_H
