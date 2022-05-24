//Aen @2022.04.08
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlspinlock.h
//description: kernel自旋锁头文件
#ifndef _KRLSPINLOCK_H
#define _KRLSPINLOCK_H
void krlspinlock_init(spinlock_t* lock);
void krlspinlock_lock(spinlock_t* lock);
void krlspinlock_unlock(spinlock_t* lock);
void krlspinlock_cli(spinlock_t* lock,cpuflg_t* cpuflg);
void krlspinunlock_sti(spinlock_t* lock,cpuflg_t* cpuflg);
#endif