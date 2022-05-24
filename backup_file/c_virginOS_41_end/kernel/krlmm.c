//Aen @2022.04.23
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlmm.c
//description: 内核层内存管理文件
#include "virginostypes.h"
#include "virginosmctrl.h"

void init_krlmm()
{
    init_krlpagempol();
    init_kvirmemadrs();
    return;
}

adr_t krlnew(size_t mmsize)
{
    if (mmsize == MALCSZ_MIN || mmsize > MALCSZ_MAX)
    {
        return NULL;
    }
    return kmempool_new(mmsize);
}

bool_t krldelete(adr_t fradr, size_t frsz)
{
    if (fradr == NULL || frsz == MALCSZ_MIN || frsz > MALCSZ_MAX)
    {
        return FALSE;
    }
    return kmempool_delete(fradr, frsz);
}
