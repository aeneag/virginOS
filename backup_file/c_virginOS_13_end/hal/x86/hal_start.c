//Aen @2022.04.01
//公众号：技术乱舞
//https://aeneag.xyz
//filename:hal_start.c
//description:开始入口文件
#include "virginostypes.h"
#include "virginosmctrl.h"

void hal_start()
{
    init_hal();
    init_krl();
    for(;;);
    return;
}

