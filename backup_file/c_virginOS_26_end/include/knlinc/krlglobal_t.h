//Aen @2022.04.08
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlglobal_t.h
//description: kernel全局数据结构头文件
#ifndef _KRLGLOBAL_T_H
#define _KRLGLOBAL_T_H
#define KRL_DEFGLOB_VARIABLE(vartype,varname) \
KEXTERN  __attribute__((section(".data"))) vartype varname
#endif // KRLGLOBAL_T_H
