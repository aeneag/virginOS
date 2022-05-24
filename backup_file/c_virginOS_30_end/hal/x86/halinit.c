//Aen @2022.04.06
//公众号：技术乱舞
//https://aeneag.xyz
//filename:halinit.c
//description:HAL全局初始化文件
#include "virginostypes.h"
#include "virginosmctrl.h"

void init_hal()
{

    init_halplaltform();
    move_img2maxpadr(&kmachbsp);
    init_halmm();
    init_halintupt();
    return;
}