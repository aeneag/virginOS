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

    init_krldevice();//初始化设备表
    init_krldriver();//加载程序驱动
    init_krlsched();//初始化进程调度器
    init_krlcpuidle();//初始化空转进程
    die(0);
    return;
}
