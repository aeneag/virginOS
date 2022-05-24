//Aen @2022.04.06
//公众号：技术乱舞
//https://aeneag.xyz
//filename:spinlock.h
//description:自旋锁头文件
#ifndef _SPINLOCK_T_H
#define _SPINLOCK_T_H



typedef struct
{
	 volatile u32_t lock;
} spinlock_t;
#endif
