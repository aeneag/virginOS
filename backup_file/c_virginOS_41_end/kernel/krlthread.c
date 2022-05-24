//Aen @2022.04.20
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlthread.c
//description: 线程管理文件

#include "virginostypes.h"
#include "virginosmctrl.h"
void micrstk_t_init(micrstk_t *initp)
{
    for (uint_t i = 0; i < MICRSTK_MAX; i++)
    {
        initp->msk_val[i] = 0;
    }
    return;
}
//进程机器上下文初始化
void context_t_init(context_t *initp)
{

    initp->ctx_nextrip = 0;
    initp->ctx_nextrsp = 0;
    initp->ctx_nexttss = &x64tss[hal_retn_cpuid()];
    return;
}
//返回进程id
uint_t krlretn_thread_id(thread_t *tdp)
{
    return (uint_t)tdp;
}
/************************************************************************************************
* @2022/05/04
* Aen
* https://aeneag.xyz
* description:初始化thread_t结构体
*     
*   
*  返回 thread_t
**************************************************************************************************/
void thread_t_init(thread_t *initp)
{
    krlspinlock_init(&initp->td_lock);
    list_init(&initp->td_list);
    initp->td_flgs = TDFLAG_FREE;
    initp->td_stus = TDSTUS_NEW;
    initp->td_cpuid = hal_retn_cpuid();//使用了一个cpu
    initp->td_id = krlretn_thread_id(initp);//使用地址做id
    initp->td_tick = 0;
    initp->td_sumtick = 0;
    initp->td_privilege = PRILG_USR;
    initp->td_priority = PRITY_MIN;
    initp->td_runmode = 0;
    initp->td_krlstktop = NULL;
    initp->td_krlstkstart = NULL;
    initp->td_usrstktop = NULL;
    initp->td_usrstkstart = NULL;
    initp->td_mmdsc = &initmmadrsdsc;
    initp->td_resdsc = NULL;
    initp->td_privtep = NULL;
    initp->td_extdatap = NULL;
    initp->td_appfilenm = NULL;
    initp->td_appfilenmlen = 0;
    context_t_init(&initp->td_context);
    for (uint_t hand = 0; hand < TD_HAND_MAX; hand++)
    {
        initp->td_handtbl[hand] = NULL;
    }
    krlmemset((void*)initp->td_name, 0, THREAD_NAME_MAX);
    return;
}
/************************************************************************************************
* @2022/05/04
* Aen
* https://aeneag.xyz
* description:建立thread_t结构体的实例变量
*     
*   
*  返回 thread_t
**************************************************************************************************/
thread_t *krlnew_thread_dsc()
{

    thread_t *rettdp = (thread_t *)(krlnew((size_t)(sizeof(thread_t))));
    if (rettdp == NULL)
    {
        return NULL;
    }
    //初始化进程
    thread_t_init(rettdp);
    return rettdp;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description: 更新 时钟 数
*     
*   
*  返回 thread_t
**************************************************************************************************/
void krlthd_inc_tick(thread_t *thdp)
{

    cpuflg_t cpuflg;
    krlspinlock_cli(&thdp->td_lock, &cpuflg);
    thdp->td_tick++;
    thdp->td_sumtick++;
    if (thdp->td_tick > TDRUN_TICK)
    {
        thdp->td_tick = 0;
        krlsched_set_schedflgs();
    }
    krlspinunlock_sti(&thdp->td_lock, &cpuflg);
    return;
}
uint_t krlthread_sumtick(thread_t* thdp)
{
    if(NULL == thdp)
    {
        return 0;
    }
    return thdp->td_sumtick;
}

uint_t krlthd_curr_sumtick()
{
    return krlthread_sumtick(krlsched_retn_currthread());
}

hand_t krlthd_retn_nullhand(thread_t *thdp)
{
    cpuflg_t cpuflg;
    hand_t rethd = NO_HAND;
    krlspinlock_cli(&thdp->td_lock, &cpuflg);
    for (uint_t hand = 0; hand < TD_HAND_MAX; hand++)
    {
        if (thdp->td_handtbl[hand] == NULL)
        {
            rethd = (hand_t)hand;
            goto retn_step;
        }
    }
    rethd = NO_HAND;
retn_step:
    krlspinunlock_sti(&thdp->td_lock, &cpuflg);
    return rethd;
}

hand_t krlthd_add_objnode(thread_t *thdp, objnode_t *ondp)
{
    cpuflg_t cpuflg;
    hand_t rethd = NO_HAND;
    krlspinlock_cli(&thdp->td_lock, &cpuflg);
    for (uint_t hand = 0; hand < TD_HAND_MAX; hand++)
    {
        if (thdp->td_handtbl[hand] == NULL)
        {
            rethd = (hand_t)hand;
            goto next_step;
        }
    }
    rethd = NO_HAND;
    goto retn_step;

next_step:
    thdp->td_handtbl[rethd] = ondp;
retn_step:
    krlspinunlock_sti(&thdp->td_lock, &cpuflg);
    return rethd;
}

hand_t krlthd_del_objnode(thread_t *thdp, hand_t hand)
{
    if ((hand >= TD_HAND_MAX) || (hand <= NO_HAND))
    {
        return NO_HAND;
    }

    cpuflg_t cpuflg;
    hand_t rethd = NO_HAND;
    krlspinlock_cli(&thdp->td_lock, &cpuflg);

    if (thdp->td_handtbl[hand] == NULL)
    {
        rethd = NO_HAND;
        goto retn_step;
    }
    thdp->td_handtbl[hand] = NULL;
    rethd = hand;
retn_step:
    krlspinunlock_sti(&thdp->td_lock, &cpuflg);
    return rethd;
}

objnode_t *krlthd_retn_objnode(thread_t *thdp, hand_t hand)
{
    if ((hand >= TD_HAND_MAX) || (hand <= NO_HAND))
    {
        return NULL;
    }

    cpuflg_t cpuflg;
    objnode_t *retondp = NULL;
    krlspinlock_cli(&thdp->td_lock, &cpuflg);

    if (thdp->td_handtbl[hand] == NULL)
    {
        retondp = NULL;
        goto retn_step;
    }
    retondp = thdp->td_handtbl[hand];
retn_step:
    krlspinunlock_sti(&thdp->td_lock, &cpuflg);
    return retondp;
}
/************************************************************************************************
* @2022/05/04
* Aen
* https://aeneag.xyz
* description: 内核栈
*     thread_t *thdp, void *runadr, uint_t cpuflags
* 
*  返回 
**************************************************************************************************/
void krlthread_kernstack_init(thread_t *thdp, void *runadr, uint_t cpuflags)
{
         
    //处理栈顶16字节对齐
    thdp->td_krlstktop &= (~0xf);
    thdp->td_usrstktop &= (~0xf);
    //内核栈顶减去intstkregs_t结构的大小
    intstkregs_t *arp = (intstkregs_t *)(thdp->td_krlstktop - sizeof(intstkregs_t));

    hal_memset((void*)arp, 0, sizeof(intstkregs_t));

    //rip寄存器的值设为程序运行首地址
    arp->r_rip_old = (uint_t)runadr;
    
    //cs寄存器的值设为内核代码段选择子
    arp->r_cs_old = K_CS_IDX;
    arp->r_rflgs = cpuflags;
    //返回进程的内核栈
    arp->r_rsp_old = thdp->td_krlstktop;
    arp->r_ss_old = 0;
   //其它段寄存器的值设为内核数据段选择子
    arp->r_ds = K_DS_IDX;
    arp->r_es = K_DS_IDX;
    arp->r_fs = K_DS_IDX;
    arp->r_gs = K_DS_IDX;
    //设置进程下一次运行的地址为runadr
    thdp->td_context.ctx_nextrip = (uint_t)runadr;
    //设置进程下一次运行的栈地址为arp
    thdp->td_context.ctx_nextrsp = (uint_t)arp;

    return;
}
/************************************************************************************************
* @2022/05/04
* Aen
* https://aeneag.xyz
* description: 用户栈
*     thread_t *thdp, void *runadr, uint_t cpuflags
* 
*  返回 
**************************************************************************************************/
void krlthread_userstack_init(thread_t *thdp, void *runadr, uint_t cpuflags)
{
    thdp->td_krlstktop &= (~0xf);
    thdp->td_usrstktop &= (~0xf);
    intstkregs_t *arp = (intstkregs_t *)(thdp->td_krlstktop - sizeof(intstkregs_t));
    hal_memset((void*)arp, 0, sizeof(intstkregs_t));
    
    arp->r_rip_old = (uint_t)runadr;
    arp->r_cs_old = U_CS_IDX;
    arp->r_rflgs = cpuflags;
    arp->r_rsp_old = thdp->td_usrstktop;
    arp->r_ss_old = U_DS_IDX;
   
    arp->r_ds = U_DS_IDX;
    arp->r_es = U_DS_IDX;
    arp->r_fs = U_DS_IDX;
    arp->r_gs = U_DS_IDX;
 
    thdp->td_context.ctx_nextrip = (uint_t)runadr;
    thdp->td_context.ctx_nextrsp = (uint_t)arp;

    return;
}
char_t* thread_name(thread_t* thread, char_t* name)
{
    uint_t len = 0;
    adr_t vadr = NULL;
    if(NULL == thread)
    {
        return NULL;
    }
    if(NULL == name)
    {
        krlstrcpy("unkown thread", thread->td_name);
        thread->td_appfilenm = thread->td_name;
        thread->td_appfilenmlen = THREAD_NAME_MAX;
        return thread->td_appfilenm;
    }
    len = krlstrlen(name);
    if(len < (THREAD_NAME_MAX - 1))
    {
        krlstrcpy(name, thread->td_name);
        thread->td_appfilenm = thread->td_name;
        thread->td_appfilenmlen = THREAD_NAME_MAX;
        return thread->td_appfilenm;
    }
    vadr = krlnew(((size_t)len + 1));
    if(NULL == vadr)
    {
        return NULL;
    }
    krlmemset((void*)vadr, 0, len + 1);
    krlstrcpy(name, (char_t*)vadr);
    thread->td_appfilenm = (char_t*)vadr;
    thread->td_appfilenmlen = (uint_t)(len + 1);    
    return thread->td_appfilenm;
}

/************************************************************************************************
* @2022/05/04
* Aen
* https://aeneag.xyz
* description:创建 普通 进程 接口 核心函数
*     void *filerun,   uint_t flg, uint_t prilg, uint_t prity, size_t usrstksz,   size_t krlstksz
*  应用程序启动运行的地址、创建标志、   进程权限        进程优先级、     进程的应用程序栈大小   内核栈大小 
*  与内核态进程相似，只不过多了用户栈 
*  返回 thread_t
**************************************************************************************************/
thread_t *krlnew_user_thread_core(char_t* name, void *filerun, uint_t flg, uint_t prilg, uint_t prity, size_t usrstksz, size_t krlstksz)
{
    thread_t *ret_td = NULL;
    bool_t acs = FALSE;
    adr_t usrstkadr = NULL, usrstktop = NULL, krlstkadr = NULL;
    mmadrsdsc_t* mm; 
    mm = new_mmadrsdsc();
    if(NULL == mm)
    {
        return NULL;
    }
    if(NULL == mm->msd_virmemadrs.vs_stackkmvdsc)
    {
        del_mmadrsdsc(mm);
        return NULL;
    }

    usrstkadr = mm->msd_virmemadrs.vs_stackkmvdsc->kva_start;
    usrstktop = mm->msd_virmemadrs.vs_stackkmvdsc->kva_end;
    if(USER_VIRTUAL_ADDRESS_END != usrstktop)
    {
        del_mmadrsdsc(mm);
        return NULL;
    }

    if((usrstktop - usrstkadr) < usrstksz)
    {
        del_mmadrsdsc(mm);
        return NULL;
    }

    krlstkadr = krlnew(krlstksz);
    if (krlstkadr == NULL)
    {
        del_mmadrsdsc(mm);
        return NULL;
    }
    ret_td = krlnew_thread_dsc();
    if (ret_td == NULL)
    {
        acs = krldelete(krlstkadr, krlstksz);
        acs = del_mmadrsdsc(mm);
        if (acs == FALSE)
        {
            return NULL;
        }
        return NULL;
    }
    thread_name(ret_td, name);
    ret_td->td_mmdsc = mm;
    mm->msd_thread = ret_td;
    ret_td->td_privilege = prilg;
    ret_td->td_priority = prity;

    ret_td->td_krlstktop = krlstkadr + (adr_t)(krlstksz - 1);
    ret_td->td_krlstkstart = krlstkadr;
    ret_td->td_usrstktop = usrstktop;
    ret_td->td_usrstkstart = usrstkadr;

    krlthread_userstack_init(ret_td, filerun, UMOD_EFLAGS);

    krlschdclass_add_thread(ret_td);
    return ret_td;
}
/************************************************************************************************
* @2022/05/04
* Aen
* https://aeneag.xyz
* description:创建 内核态 进程 接口 核心函数
*     void *filerun,   uint_t flg, uint_t prilg, uint_t prity, size_t usrstksz,   size_t krlstksz
*  应用程序启动运行的地址、创建标志、   进程权限        进程优先级、     进程的应用程序栈大小   内核栈大小  
*  返回 thread_t
**************************************************************************************************/
thread_t *krlnew_kern_thread_core(char_t* name, void *filerun, uint_t flg, uint_t prilg, uint_t prity, size_t usrstksz, size_t krlstksz)
{
    thread_t *ret_td = NULL;
    bool_t acs = FALSE;
    adr_t krlstkadr = NULL;
    //分配内核栈空间
    krlstkadr = krlnew(krlstksz);
    if (krlstkadr == NULL)
    {
        return NULL;
    }
    //建立thread_t结构体的实例变量
    ret_td = krlnew_thread_dsc();
    if (ret_td == NULL)
    {//创建失败必须要释放之前的栈空间
        acs = krldelete(krlstkadr, krlstksz);
        if (acs == FALSE)
        {
            return NULL;
        }
        return NULL;
    }
    thread_name(ret_td, name);
    //设置进程权限
    ret_td->td_privilege = prilg;
    //设置进程优先级
    ret_td->td_priority = prity;
    //设置进程的内核栈顶和内核栈开始地址
    ret_td->td_krlstktop = krlstkadr + (adr_t)(krlstksz - 1);
    ret_td->td_krlstkstart = krlstkadr;

    //初始化进程的内核栈
    krlthread_kernstack_init(ret_td, filerun, KMOD_EFLAGS);
    //加入进程调度系统
    krlschdclass_add_thread(ret_td);
    //返回进程指针
    return ret_td;
}
/************************************************************************************************
* @2022/05/04
* Aen
* https://aeneag.xyz
* description:创建进程接口函数
*     void *filerun,   uint_t flg, uint_t prilg, uint_t prity, size_t usrstksz,   size_t krlstksz
*  应用程序启动运行的地址、创建标志、   进程权限        进程优先级、     进程的应用程序栈大小   内核栈大小  
*  返回 thread_t
**************************************************************************************************/
thread_t *krlnew_thread(char_t* name, void *filerun, uint_t flg, uint_t prilg, uint_t prity, size_t usrstksz, size_t krlstksz)
{
    size_t tustksz = usrstksz, tkstksz = krlstksz;
    //对参数进行检查，不合乎要求就返回NULL表示创建失败
    if (filerun == NULL || usrstksz > DAFT_TDUSRSTKSZ || krlstksz > DAFT_TDKRLSTKSZ)
    {
        return NULL;
    }
    // 判断优先级
    if ((prilg != PRILG_USR && prilg != PRILG_SYS) || (prity >= PRITY_MAX))
    {
        return NULL;
    }
    //进程应用程序栈大小检查，大于默认大小则使用默认大小
    if (usrstksz < DAFT_TDUSRSTKSZ)
    {
        tustksz = DAFT_TDUSRSTKSZ;
    }
    //进程内核栈大小检查，大于默认大小则使用默认大小
    if (krlstksz < DAFT_TDKRLSTKSZ)
    {
        tkstksz = DAFT_TDKRLSTKSZ;
    }
    //是否建立内核进程
    if (KERNTHREAD_FLG == flg)
    {
        return krlnew_kern_thread_core(name, filerun, flg, prilg, prity, tustksz, tkstksz);
    }
    //是否建立普通进程
    else if (USERTHREAD_FLG == flg)
    {
        return krlnew_user_thread_core(name, filerun, flg, prilg, prity, tustksz, tkstksz);
    }
    return NULL;
}

thread_t* krlthread_execvl(thread_t* thread, char_t* filename)
{
    u64_t retadr = 0, filelen = 0;
    adr_t vadr = 0;
    if(NULL == thread || NULL == filename)
    {
        return NULL;
    }
    get_file_rvadrandsz(filename, &kmachbsp, &retadr, &filelen);
    if(NULL == retadr || 0 == filelen)
    {
        return NULL;
    }
    if(thread->td_mmdsc == &initmmadrsdsc)
    {
        return NULL;
    }
    if((0x100000+filelen) >= THREAD_HEAPADR_START)
    {
        return NULL;
    }
    vadr = kvma_initdefault_virmemadrs(thread->td_mmdsc, APPRUN_START_VITRUALADDR, filelen, KMV_BIN_TYPE);
    if(NULL == vadr)
    {
        return NULL;
    }
    
    return thread;

}