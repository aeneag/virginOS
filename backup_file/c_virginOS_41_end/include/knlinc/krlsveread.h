//Aen @2022.04.08
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlsveread.h
//description: 内核服务头文件
#ifndef _KRLSVEREAD_H
#define _KRLSVEREAD_H
sysstus_t krlsvetabl_read(uint_t inr,stkparame_t* stkparv);
sysstus_t krlsve_read(hand_t fhand,buf_t buf,size_t len,uint_t flgs);
sysstus_t krlsve_core_read(hand_t fhand,buf_t buf,size_t len,uint_t flgs);
sysstus_t krlsve_read_device(objnode_t* ondep);
#endif // KRLSVEREAD_H
