/************************************************************************************************
* Aen @2022.05.05
* 公众号：技术乱舞
* https://aeneag.xyz
* filename:krlcpuidle.c
* description:idle线程头文件
************************************************************************************************/
#include "virginostypes.h"
#include "virginosmctrl.h"

void thread_a_main()
{
    uint_t i = 0;
    for (;; i++)
    {
        kprint("进程A运行:%x\n", i);
        // krlschedul();
        die(200);
    }
    return;
}

void thread_b_main()
{
    uint_t i = 0;
    for (;; i++)
    {
        kprint("进程B运行:%x\n", i);
        // krlschedul();
        die(200);
    }
    return;
}

void init_ab_thread()
{
    krlnew_thread("kernelthread-a", (void*)thread_a_main, KERNTHREAD_FLG, 
                PRILG_SYS, PRITY_MIN, DAFT_TDUSRSTKSZ, DAFT_TDKRLSTKSZ);
    krlnew_thread("kernelthread-b", (void*)thread_b_main, KERNTHREAD_FLG, 
                PRILG_SYS, PRITY_MIN, DAFT_TDUSRSTKSZ, DAFT_TDKRLSTKSZ);
    return;
}
void init_user_thread()
{
    thread_t *t = NULL;
    t = krlnew_thread("oneuser.app", (void *)APPRUN_START_VITRUALADDR, USERTHREAD_FLG,
                      PRILG_USR, PRITY_MIN, DAFT_TDUSRSTKSZ, DAFT_TDKRLSTKSZ);
    t = krlthread_execvl(t, "oneuser.app");
    if (NULL != t)
    {
        kprint("oneuser.app进程建立成功:%x\n", (uint_t)t);
    }
    return;
}
void init_krlcpuidle()
{
    new_cpuidle();
    // kprint("     okkkkkkkkk----2----------\n");
    // die(0x400);
    //init_ab_thread();
    init_user_thread();
    kprint("空转进程初始化成功\n");
    print_out = 0;
    krlcpuidle_start();
    return;
}

void krlcpuidle_start()
{

    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];

    thread_t *tdp = schdap->sda_cpuidle;
    tdp->td_context.ctx_nexttss = &x64tss[cpuid];
    x64tss[cpuid].rsp0 = tdp->td_krlstktop;
    tdp->td_context.ctx_nexttss->rsp0 = tdp->td_krlstktop;
    tdp->td_stus = TDSTUS_RUN;
    retnfrom_first_sched(tdp);

    return;
}
thread_t *new_cpuidle_thread()
{

    thread_t *ret_td = NULL;
    bool_t acs = FALSE;
    adr_t krlstkadr = NULL;
    uint_t cpuid = hal_retn_cpuid();
    schdata_t *schdap = &osschedcls.scls_schda[cpuid];
    
    krlstkadr = krlnew(DAFT_TDKRLSTKSZ);
        
    if (krlstkadr == NULL)
    {
        return NULL;
    }
    ret_td = krlnew_thread_dsc();
  
    if (ret_td == NULL)
    {
        acs = krldelete(krlstkadr, DAFT_TDKRLSTKSZ);
        if (acs == FALSE)
        {
            return NULL;
        }
        return NULL;
    }
    thread_name(ret_td, "cpuidle-thread");
    ret_td->td_privilege = PRILG_SYS;
    ret_td->td_priority = PRITY_MIN;

    ret_td->td_krlstktop = krlstkadr + (adr_t)(DAFT_TDKRLSTKSZ - 1);
    ret_td->td_krlstkstart = krlstkadr;
    //   kprint("     okkkkkkkkk----aen----------%x\n",ret_td);
    // die(0x400);
    krlthread_kernstack_init(ret_td, (void *)krlcpuidle_main, KMOD_EFLAGS);

    schdap->sda_cpuidle = ret_td;
    schdap->sda_currtd = ret_td;

    return ret_td;
}

void new_cpuidle()
{

    thread_t *thp = new_cpuidle_thread();

    if (thp == NULL)
    {
        hal_sysdie("newcpuilde err");
    }
    kprint("CPUIDLETASK: %x\n", (uint_t)thp);
    return;
}

void krlcpuidle_main()
{
    uint_t i = 0;
    for (;; i++)
    {
       // kprint("空转进程运行:%x\n", i);
        krlschedul();
    }
    return;
}
