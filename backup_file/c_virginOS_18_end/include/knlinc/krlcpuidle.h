//Aen @2022.04.08
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlcpuidle.h
//description: idle线程头文件

#ifndef _KRLCPUIDLE_H
#define _KRLCPUIDLE_H
void init_krlcpuidle();
void krlcpuidle_start();
thread_t* new_cpuidle_thread();
void new_cpuidle();
void krlcpuidle_main();
#endif // KRLCPUIDLE_H
