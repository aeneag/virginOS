//Aen @2022.04.05
//公众号：技术乱舞
//https://aeneag.xyz
//filename:halglobal_t.h
//description:HAL全局数据结构头文件
#ifndef _HALGLOBAL_T_H
#define _HALGLOBAL_T_H
#define HAL_DEFGLOB_VARIABLE(vartype,varname) \
EXTERN  __attribute__((section(".data"))) vartype varname

#endif // HALGLOBAL_T_H
