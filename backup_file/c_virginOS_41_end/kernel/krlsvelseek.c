/************************************************************************************************
* Aen @2022.05.21
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: krlsvelseek.c
* description:内核服务文件
************************************************************************************************/
#include "virginostypes.h"
#include "virginosmctrl.h"
sysstus_t krlsvetabl_lseek(uint_t inr, stkparame_t *stkparv)
{
    if (inr != INR_FS_LSEEK)
    {
        return SYSSTUSERR;
    }
    return krlsve_lseek((hand_t)stkparv->parmv1, (uint_t)stkparv->parmv2,
                        (uint_t)stkparv->parmv3);
}

sysstus_t krlsve_lseek(hand_t fhand, uint_t lofset, uint_t flgs)
{
    return SYSSTUSERR;
}

sysstus_t krlsve_core_lseek(hand_t fhand, uint_t lofset, uint_t flgs)
{
    return SYSSTUSERR;
}
