//Aen @2022.04.08
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlinit.c
//description: kernel全局初始化文件
#include "virginostypes.h"
#include "virginosmctrl.h"
void init_krl()
{
    init_krlmm();
	// init_krldevice();
    // init_krldriver();
	// init_krlsched();
    // init_ktime();
    // init_thread();
    // init_task();
    // init_krlcpuidle();

    // hal_enable_irqfiq();
   
    die(0);
    return;
}
