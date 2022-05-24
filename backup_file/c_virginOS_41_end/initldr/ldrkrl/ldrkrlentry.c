//Aen @2022.04.01
//公众号：技术乱舞
//https://aeneag.xyz
//filename:ldrkrlentry.c
//description: 二级引导器主函数

#include "cmctl.h"

extern idtr_t IDT_PTR;


void ldrkrl_entry()
{
    init_curs();
    close_curs();
    clear_screen(VGADP_DFVL);
    init_bstartparm();
    return;
}



void kerror(char_t* kestr)
{
    kprint("INITKLDR DIE ERROR:%s\n",kestr);
    for(;;);
    return;
}


void __attribute__((optimize("O0"))) die(u32_t dt)
{

    u32_t dttt=dt,dtt=dt;
    if(dt==0)
    {
        for(;;);
    }

    for(u32_t i=0;i<dt;i++)
    {
        for(u32_t j=0;j<dtt;j++)
        {
            for(u32_t k=0;k<dttt;k++)
            {
                ;
            }
        }
    }



    return;
}

