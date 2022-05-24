/************************************************************************************************
* Aen @2022.05.19
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: krlservice.c
* description:内核服务文件
************************************************************************************************/
#include "virginostypes.h"
#include "virginosmctrl.h"

sysstus_t krlservice(uint_t inr, void* sframe)
{
    if(INR_MAX <= inr)
    {
        return SYSSTUSERR;
    }
    if(NULL == osservicetab[inr])
    {
        return SYSSTUSERR;
    }
    return osservicetab[inr](inr, (stkparame_t*)sframe);
}