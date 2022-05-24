/************************************************************************************************
* Aen @2022.05.08
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: krlintupt.c
* description:内核层中断处理文件
************************************************************************************************/
#include "virginostypes.h"
#include "virginosmctrl.h"
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description: 
*     
*            
*  返回 null
**************************************************************************************************/
intserdsc_t *krladd_irqhandle(void *device, intflthandle_t handle, uint_t phyiline)
{

    if (device == NULL || handle == NULL)
    {
        return NULL;
    }
    //根据设备中断线返回对应中断异常描述符
    intfltdsc_t *intp = hal_retn_intfltdsc(phyiline);
    if (intp == NULL)
    {
        return NULL;
    }
    intserdsc_t *serdscp = (intserdsc_t *)krlnew(sizeof(intserdsc_t));//建立一个intserdsc_t结构体实例变量
    if (serdscp == NULL)
    {
        return NULL;
    }

    //初始化intserdsc_t结构体实例变量，并把设备指针和回调函数放入其中
    intserdsc_t_init(serdscp, 0, intp, device, handle);
    //把intserdsc_t结构体实例变量挂载到中断异常描述符结构中
    if (hal_add_ihandle(intp, serdscp) == FALSE)
    {

        if (krldelete((adr_t)serdscp, sizeof(intserdsc_t)) == FALSE)
        {
            hal_sysdie("krladd_irqhandle ERR");
        }
        return NULL;
    }

    return serdscp;
}


drvstus_t krlenable_intline(uint_t ifdnr)
{

    return hal_enable_intline(ifdnr);
}

drvstus_t krldisable_intline(uint_t ifdnr)
{

    return hal_disable_intline(ifdnr);
}
