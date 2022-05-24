//Aen @2022.04.06
//公众号：技术乱舞
//https://aeneag.xyz
//filename:halinit.c
//description:HAL全局初始化文件
#include "virginostypes.h"
#include "virginosmctrl.h"
void mov32byte(void *from, void *to)
{
    __asm__ __volatile__(
        "vmovups (%0),%%ymm0\n\t"
        "vmovups %%ymm0, (%1)\n\t" ::"r"(from),
        "r"(to)
        :);
    return;
}

void mov64byte(void *from, void *to)
{
    __asm__ __volatile__(
        "vmovups (%0),%%ymm0\n\t"
        "vmovups 32(%0),%%ymm1\n\t"
        "vmovups %%ymm0, (%1)\n\t"
        "vmovups %%ymm1, 32(%1)\n\t" ::"r"(from),
        "r"(to)
        :);
    return;
}

void mov128byte(void *from, void *to)
{
    __asm__ __volatile__(
        "vmovups (%0),%%ymm0\n\t"
        "vmovups 32(%0),%%ymm1\n\t"
        "vmovups 64(%0),%%ymm2\n\t"
        "vmovups 96(%0),%%ymm3\n\t"
        "vmovups %%ymm0, (%1)\n\t"
        "vmovups %%ymm1, 32(%1)\n\t"
        "vmovups %%ymm2, 64(%1)\n\t"
        "vmovups %%ymm3, 96(%1)\n\t" ::"r"(from),
        "r"(to)
        :);
    return;
}

void copy_avx2(void *from, void *to, uint_t count)
{
    void *s = from;
    void *d = to;
    for (uint_t i = 0; i < count; i += 32)
    {
        mov32byte(s, d);
        s += 32;
        d += 32;
    }

    return;
}



void test_avx2()
{
    msadsc_t *smsa = NULL, *dmsa = NULL;
    uint_t relpnr = 0;
    uint_t pages = 1024;
    u64_t stsc=0, etsc=0, astsc=0, ltsc=0;

    smsa = mm_division_pages(&memmgrob, pages, &relpnr, MA_TYPE_KRNL, DMF_RELDIV);
    dmsa = mm_division_pages(&memmgrob, pages, &relpnr, MA_TYPE_KRNL, DMF_RELDIV);

    if (smsa == NULL || dmsa == NULL)
    {
        system_error("test_avx2 err\n");
    }

    void *sadr = (void *)msadsc_ret_vaddr(smsa);
    void *dadr = (void *)msadsc_ret_vaddr(dmsa);
    //hal_memset(sadr, 1, (pages * 0x1000));
    // hal_memset(dadr, 2, (pages * 0x1000));

    for (size_t i = 0; i < 100000; i++)
    {
        hal_wbinvd();

        stsc = x86_rdtsc();                      //开始运行函数时钟数
        copy_avx2(sadr, dadr, (pages * 0x1000)); //avx复制4MB大小的数据
        etsc = x86_rdtsc();                      //结束运行函数时钟数
        astsc += (etsc - stsc);
        //kprint("avx2指令复制:%dKB,需要时钟周期%d\n", (pages * 0x1000) / 1024, etsc - stsc);

        hal_wbinvd();

        stsc = x86_rdtsc();                       //开始运行函数时钟数
        hal_memcpy(sadr, dadr, (pages * 0x1000)); //普通复制4MB大小的数据
        etsc = x86_rdtsc();                       //结束运行函数时钟数
        ltsc += (etsc - stsc);
        //kprint("通用指令复制:%dKB,需要时钟周期%d\n", (pages * 0x1000) / 1024, etsc - stsc);
    }
    kprint("通用指令复制:%dKB,100000次平均需要时钟周期%d\n", (pages * 0x1000) / 1024, ltsc / 100000);
    kprint("AVX指令复制:%dKB,100000次平均需要时钟周期%d\n", (pages * 0x1000) / 1024, astsc / 100000);

    // die(0);
    return;
}

void init_hal()
{
    print_out = 1;
    init_halplaltform();
    move_img2maxpadr(&kmachbsp);
    init_halmm();
    init_halintupt();
    kprint("HAL层初始化成功\n");
    return;
}