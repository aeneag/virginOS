//Aen @2022.04.08
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlsvelseek.h
//description: 内核服务头文件
#ifndef _KRLSVELSEEK_H
#define _KRLSVELSEEK_H
sysstus_t krlsvetabl_lseek(uint_t inr,stkparame_t* stkparv);
sysstus_t krlsve_lseek(hand_t fhand,uint_t lofset,uint_t flgs);
sysstus_t krlsve_core_lseek(hand_t fhand,uint_t lofset,uint_t flgs);
#endif // KRLSVELSEEK_H
