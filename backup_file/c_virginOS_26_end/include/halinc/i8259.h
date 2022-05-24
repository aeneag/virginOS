//Aen @2022.04.05
//公众号：技术乱舞
//https://aeneag.xyz
//filename:i8259.h
//description:i8259中断控制器头文件
#ifndef _I8259_H
#define _I8259_H
void init_i8259();
void i8259_send_eoi();
void i8259_enabled_line(u32_t line);
void i8259_disable_line(u32_t line);
void i8259_save_disableline(u64_t* svline,u32_t line);
void i8259_rest_enabledline(u64_t* svline,u32_t line);
#endif
