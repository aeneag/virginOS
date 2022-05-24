/************************************************************************************************
* Aen @2022.05.07
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: krldevice.c
* description:设备管理文件
************************************************************************************************/
#include "virginostypes.h"
#include "virginosmctrl.h"

/************************************************************************************************
* @2022/05/07
* Aen
* https://aeneag.xyz
* description:初始化设备链表
*     
*            
*  返回 null
**************************************************************************************************/
void devtlst_t_init(devtlst_t *initp, uint_t dtype)
{
    initp->dtl_type = dtype;
    initp->dtl_nr = 0;
    list_init(&initp->dtl_list);

    return;
}
/************************************************************************************************
* @2022/05/07
* Aen
* https://aeneag.xyz
* description:初始化全局设备表
*     
*            
*  返回 null
**************************************************************************************************/
void devtable_t_init(devtable_t *initp)
{
    list_init(&initp->devt_list);
    krlspinlock_init(&initp->devt_lock);
    list_init(&initp->devt_devlist);
    list_init(&initp->devt_drvlist);
    initp->devt_devnr = 0;
    initp->devt_drvnr = 0;
    for (uint_t t = 0; t < DEVICE_MAX; t++)
    {
        devtlst_t_init(&initp->devt_devclsl[t], t);
    }
    return;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:初始化设备 id
*     
*            
*  返回 null
**************************************************************************************************/
void devid_t_init(devid_t *initp, uint_t mty, uint_t sty, uint_t nr)
{
    initp->dev_mtype = mty;
    initp->dev_stype = sty;
    initp->dev_nr = nr;
    return;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:初始化设备实体变量
*     
*            
*  返回 null
**************************************************************************************************/
void device_t_init(device_t *initp)
{
    list_init(&initp->dev_list);
    list_init(&initp->dev_indrvlst);
    list_init(&initp->dev_intbllst);
    krlspinlock_init(&initp->dev_lock);
    initp->dev_count = 0;
    krlsem_t_init(&initp->dev_sem);
    initp->dev_stus = 0;
    initp->dev_flgs = 0;
    devid_t_init(&initp->dev_id, 0, 0, 0);
    initp->dev_intlnenr = 0;
    list_init(&initp->dev_intserlst);
    list_init(&initp->dev_rqlist);
    initp->dev_rqlnr = 0;
    krlsem_t_init(&initp->dev_waitints);
    initp->dev_drv = NULL;
    initp->dev_attrb = NULL;
    initp->dev_privdata = NULL;
    initp->dev_userdata = NULL;
    initp->dev_extdata = NULL;
    initp->dev_name = NULL;

    return;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:初始化驱动 id
*     
*            
*  返回 null
**************************************************************************************************/
void krlretn_driverid(driver_t *dverp)
{
    dverp->drv_id = (uint_t)dverp;
    return;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:驱动程序初始化
*     
*            
*  返回 null
**************************************************************************************************/
void driver_t_init(driver_t *initp)
{
    krlspinlock_init(&initp->drv_lock);
    list_init(&initp->drv_list);
    initp->drv_stuts = 0;
    initp->drv_flg = 0;
    krlretn_driverid(initp);
    initp->drv_count = 0;
    krlsem_t_init(&initp->drv_sem);
    initp->drv_safedsc = NULL;
    initp->drv_attrb = NULL;
    initp->drv_privdata = NULL;

    for (uint_t dsi = 0; dsi < IOIF_CODE_MAX; dsi++)
    {
        initp->drv_dipfun[dsi] = drv_defalt_func;
    }
    list_init(&initp->drv_alldevlist);
    initp->drv_entry = NULL;
    initp->drv_exit = NULL;
    initp->drv_userdata = NULL;
    initp->drv_extdata = NULL;
    initp->drv_name = NULL;
    return;
}
/************************************************************************************************
* @2022/05/07
* Aen
* https://aeneag.xyz
* description:初始化设备表
*     
*            
*  返回 null
**************************************************************************************************/
void init_krldevice()
{
    devtable_t_init(&osdevtable);
    kprint("设备管理初始化成功\n");
    return;
}

/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:运行驱动程序函数
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t krlrun_driverentry(drventyexit_t drventry)
{
    driver_t *drvp = new_driver_dsc();//建立driver_t实例变量
    if (drvp == NULL)
    {
        return DFCERRSTUS;
    }
    if (drventry(drvp, 0, NULL) == DFCERRSTUS)//运行驱动程序入口函数
    {
        return DFCERRSTUS;
    }
    // kprint("  drivers \n");
    if (krldriver_add_system(drvp) == DFCERRSTUS)//把驱动程序加入系统
    {
        return DFCERRSTUS;
    }
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:初始化驱动程序
*     
*            
*  返回 null
**************************************************************************************************/
void init_krldriver()
{
//遍历驱动程序表中的每个驱动程序入口函数
// kprint("设备驱动初始化start\n");
    for (uint_t ei = 0; osdrvetytabl[ei] != NULL; ei++)
    {//运行一个驱动程序入口
        if (krlrun_driverentry(osdrvetytabl[ei]) == DFCERRSTUS)
        {
            hal_sysdie("init driver err");
        }
    }
    kprint("设备驱动初始化成功\n");
    return;
}

/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:删除驱动实体变量
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t del_driver_dsc(driver_t *drvp)
{
    if (krldelete((adr_t)drvp, sizeof(driver_t)) == FALSE)
    {
        return DFCERRSTUS;
    }
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:新建驱动实体变量
*     
*            
*  返回 null
**************************************************************************************************/
driver_t *new_driver_dsc()
{
    driver_t *dp = (driver_t *)krlnew(sizeof(driver_t));
    if (dp == NULL)
    {
        return NULL;
    }
    driver_t_init(dp);//初始化驱动

    return dp;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:删除设备实体变量
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t del_device_dsc(device_t *devp)
{
    if (krldelete((adr_t)devp, sizeof(device_t)) == FALSE)
    {
        return DFCERRSTUS;
    }
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:新建设备实体变量
*     
*            
*  返回 null
**************************************************************************************************/
device_t *new_device_dsc()
{
    device_t *dp = (device_t *)krlnew(sizeof(device_t));
    if (dp == NULL)
    {
        return NULL;
    }
    device_t_init(dp);

    return dp;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description: 驱动功能函数，用于初始化
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t drv_defalt_func(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description: 比较两个设备id 信息是否匹配
*     
*            
*  返回 null
**************************************************************************************************/
bool_t krlcmp_devid(devid_t *sdidp, devid_t *cdidp)
{
    if (sdidp->dev_mtype != cdidp->dev_mtype)
    {
        return FALSE;
    }
    if (sdidp->dev_stype != cdidp->dev_stype)
    {
        return FALSE;
    }
    if (sdidp->dev_nr != cdidp->dev_nr)
    {
        return FALSE;
    }
    return TRUE;
}

drvstus_t krlnew_devid(devid_t *devid)
{
    device_t *findevp;
    drvstus_t rets = DFCERRSTUS;
    cpuflg_t cpufg;
    list_h_t *lstp;
    devtable_t *dtbp = &osdevtable;
    uint_t devmty = devid->dev_mtype;
    uint_t devidnr = 0;
    if (devmty >= DEVICE_MAX)
    {
        return DFCERRSTUS;
    }

    krlspinlock_cli(&dtbp->devt_lock, &cpufg);
    if (devmty != dtbp->devt_devclsl[devmty].dtl_type)
    {
        rets = DFCERRSTUS;
        goto return_step;
    }
    if (list_is_empty(&dtbp->devt_devclsl[devmty].dtl_list) == TRUE)
    {
        rets = DFCOKSTUS;
        devid->dev_nr = 0;
        goto return_step;
    }
    list_for_each(lstp, &dtbp->devt_devclsl[devmty].dtl_list)
    {
        findevp = list_entry(lstp, device_t, dev_intbllst);
        if (findevp->dev_id.dev_nr > devidnr)
        {
            devidnr = findevp->dev_id.dev_nr;

        }
    }
    devid->dev_nr = devidnr++;
    rets = DFCOKSTUS;
return_step:
    krlspinunlock_sti(&dtbp->devt_lock, &cpufg);
    return rets;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description: 驱动程序加载到内核
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t krldriver_add_system(driver_t *drvp)
{
    cpuflg_t cpufg;
    devtable_t *dtbp = &osdevtable;
    krlspinlock_cli(&dtbp->devt_lock, &cpufg);
    list_add(&drvp->drv_list, &dtbp->devt_drvlist);
    dtbp->devt_drvnr++;
    krlspinunlock_sti(&dtbp->devt_lock, &cpufg);
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description: 设备与驱动建立联系
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t krldev_add_driver(device_t *devp, driver_t *drvp)
{
    list_h_t *lst;
    device_t *fdevp;
    if (devp == NULL || drvp == NULL)
    {
        return DFCERRSTUS;
    }
    list_for_each(lst, &drvp->drv_alldevlist)//遍历这个驱动上所有设备
    {
        fdevp = list_entry(lst, device_t, dev_indrvlst);
        if (krlcmp_devid(&devp->dev_id, &fdevp->dev_id) == TRUE)//比较设备ID有相同的则返回错误
        {
            return DFCERRSTUS;
        }
    }

    list_add(&devp->dev_indrvlst, &drvp->drv_alldevlist);//将设备挂载到驱动上
    devp->dev_drv = drvp;//让设备中dev_drv字段指向管理自己的驱动
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description: 向内核注册设备  其实就是把 设备挂载到设备表中去
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t krlnew_device(device_t *devp)
{
    device_t *findevp;
    drvstus_t rets = DFCERRSTUS;
    cpuflg_t cpufg;
    list_h_t *lstp;
    devtable_t *dtbp = &osdevtable;
    uint_t devmty = devp->dev_id.dev_mtype;
    if (devp == NULL)
    {
        return DFCERRSTUS;
    }
    if (devp->dev_drv == NULL)
    {
        return DFCERRSTUS;
    }
    if (devmty >= DEVICE_MAX)
    {
        return DFCERRSTUS;
    }

    krlspinlock_cli(&dtbp->devt_lock, &cpufg);
    if (devmty != dtbp->devt_devclsl[devmty].dtl_type)
    {
        rets = DFCERRSTUS;
        goto return_step;
    }
    
    //设备id不能重复
    list_for_each(lstp, &dtbp->devt_devclsl[devmty].dtl_list)
    {
        findevp = list_entry(lstp, device_t, dev_intbllst);
        if (krlcmp_devid(&devp->dev_id, &findevp->dev_id) == TRUE)
        {
            rets = DFCERRSTUS;
            goto return_step;
        }
    }
    //先把设备加入设备表的全局设备链表
    list_add(&devp->dev_intbllst, &dtbp->devt_devclsl[devmty].dtl_list);
    //将设备加入对应设备类型的链表中
    list_add(&devp->dev_list, &dtbp->devt_devlist);
    dtbp->devt_devclsl[devmty].dtl_nr++;//设备计数加一
    dtbp->devt_devnr++;//总的设备数加一
    rets = DFCOKSTUS;
return_step:
        krlspinunlock_sti(&dtbp->devt_lock, &cpufg);
    return rets;
}

drvstus_t krldev_inc_devcount(device_t *devp)
{

    if (devp->dev_count >= (~0UL))
    {
        return DFCERRSTUS;
    }
    cpuflg_t cpufg;
    hal_spinlock_saveflg_cli(&devp->dev_lock, &cpufg);
    devp->dev_count++;
    hal_spinunlock_restflg_sti(&devp->dev_lock, &cpufg);
    return DFCOKSTUS;
}

drvstus_t krldev_dec_devcount(device_t *devp)
{

    if (devp->dev_count < (1))
    {
        return DFCERRSTUS;
    }
    cpuflg_t cpufg;
    hal_spinlock_saveflg_cli(&devp->dev_lock, &cpufg);
    devp->dev_count--;
    hal_spinunlock_restflg_sti(&devp->dev_lock, &cpufg);
    return DFCOKSTUS;
}

drvstus_t krldev_add_request(device_t *devp, objnode_t *request)
{
    cpuflg_t cpufg;
    objnode_t *np = (objnode_t *)request;
    krlspinlock_cli(&devp->dev_lock, &cpufg);
    list_add_tail(&np->on_list, &devp->dev_rqlist);
    devp->dev_rqlnr++;
    krlspinunlock_sti(&devp->dev_lock, &cpufg);
    return DFCOKSTUS;
}

drvstus_t krldev_complete_request(device_t *devp, objnode_t *request)
{
    if (devp == NULL || request == NULL)
    {
        return DFCERRSTUS;
    }
    if (devp->dev_rqlnr < 1)
    {
        hal_sysdie("krldev_complete_request err devp->dev_rqlnr<1");
    }
    cpuflg_t cpufg;
    krlspinlock_cli(&devp->dev_lock, &cpufg);
    list_del(&request->on_list);
    devp->dev_rqlnr--;
    krlspinunlock_sti(&devp->dev_lock, &cpufg);
    krlsem_up(&request->on_complesem);
    return DFCOKSTUS;
}

drvstus_t krldev_retn_request(device_t *devp, uint_t iocode, objnode_t **retreq)
{
    if (retreq == NULL || iocode >= IOIF_CODE_MAX)
    {
        return DFCERRSTUS;
    }
    cpuflg_t cpufg;
    objnode_t *np;
    list_h_t *list;
    drvstus_t rets = DFCERRSTUS;
    krlspinlock_cli(&devp->dev_lock, &cpufg);
    list_for_each(list, &devp->dev_rqlist)
    {
        np = list_entry(list, objnode_t, on_list);
        if (np->on_opercode == (sint_t)iocode)
        {
            *retreq = np;
            rets = DFCOKSTUS;
            goto return_step;
        }
    }
    rets = DFCERRSTUS;
    *retreq = NULL;
return_step:
    krlspinunlock_sti(&devp->dev_lock, &cpufg);
    return rets;
}

drvstus_t krldev_wait_request(device_t *devp, objnode_t *request)
{
    if (devp == NULL || request == NULL)
    {
        return DFCERRSTUS;
    }
    krlsem_down(&request->on_complesem);
    return DFCOKSTUS;
}

void krldev_wait_intupt(device_t *devp)
{
    return;
}

void krldev_up_intupt(device_t *devp)
{
    return;
}

drvstus_t krldev_retn_rqueparm(void *request, buf_t *retbuf, uint_t *retcops, uint_t *retlen, uint_t *retioclde, uint_t *retbufcops, size_t *retbufsz)
{
    objnode_t *ondep = (objnode_t *)request;
    if (ondep == NULL)
    {
        return DFCERRSTUS;
    }
    if (retbuf != NULL)
    {
        *retbuf = ondep->on_buf;
    }
    if (retcops != NULL)
    {
        *retcops = ondep->on_currops;
    }
    if (retlen != NULL)
    {
        *retlen = ondep->on_len;
    }
    if (retioclde != NULL)
    {
        *retioclde = ondep->on_ioctrd;
    }
    if (retbufcops != NULL)
    {
        *retbufcops = ondep->on_bufcurops;
    }
    if (retbufsz != NULL)
    {
        *retbufsz = ondep->on_bufsz;
    }
    return DFCOKSTUS;
}

device_t *krlonidfl_retn_device(void *dfname, uint_t flgs)
{
    device_t *findevp;
    cpuflg_t cpufg;
    list_h_t *lstp;
    devtable_t *dtbp = &osdevtable;

    if (dfname == NULL || flgs != DIDFIL_IDN)
    {
        return NULL;
    }
    devid_t *didp = (devid_t *)dfname;
    uint_t devmty = didp->dev_mtype;
    if (devmty >= DEVICE_MAX)
    {
        return NULL;
    }
    krlspinlock_cli(&dtbp->devt_lock, &cpufg);
    if (devmty != dtbp->devt_devclsl[devmty].dtl_type)
    {
        findevp = NULL;
        goto return_step;
    }
    list_for_each(lstp, &dtbp->devt_devclsl[devmty].dtl_list)
    {
        findevp = list_entry(lstp, device_t, dev_intbllst);
        if (krlcmp_devid(didp, &findevp->dev_id) == TRUE)
        {
           // findevp = findevp;
            goto return_step;
        }
    }

    findevp = NULL;
return_step:
    krlspinunlock_sti(&dtbp->devt_lock, &cpufg);
    return findevp;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:中断回调接口函数
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t krlnew_devhandle(device_t *devp, intflthandle_t handle, uint_t phyiline)
{
    intserdsc_t *sdp = krladd_irqhandle(devp, handle, phyiline);//调用内核层中断框架接口函数
    if (sdp == NULL)
    {

        return DFCERRSTUS;
    }
    cpuflg_t cpufg;
    krlspinlock_cli(&devp->dev_lock, &cpufg);

    list_add(&sdp->s_indevlst, &devp->dev_intserlst);
    devp->dev_intlnenr++;
    krlspinunlock_sti(&devp->dev_lock, &cpufg);
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:发送设备IO
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t krldev_io(objnode_t *nodep)
{
    device_t *devp = (device_t *)(nodep->on_objadr);//获取设备对象
    if ((nodep->on_objtype != OBJN_TY_DEV && nodep->on_objtype != OBJN_TY_FIL) || nodep->on_objadr == NULL)
    {//检查操作对象类型是不是文件或者设备，对象地址是不是为空
        return DFCERRSTUS;
    }//检查IO操作码是不是合乎要求
    if (nodep->on_opercode < 0 || nodep->on_opercode >= IOIF_CODE_MAX)
    {
        return DFCERRSTUS;
    }
    return krldev_call_driver(devp, nodep->on_opercode, 0, 0, NULL, nodep);
}
/************************************************************************************************
* @2022/05/08
* Aen
* https://aeneag.xyz
* description:调用设备驱动
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t krldev_call_driver(device_t *devp, uint_t iocode, uint_t val1, uint_t val2, void *p1, void *p2)
{
    driver_t *drvp = NULL;
    if (devp == NULL || iocode >= IOIF_CODE_MAX)
    {
        return DFCERRSTUS;
    }
    drvp = devp->dev_drv;
    if (drvp == NULL)//检查设备和IO操作码
    {
        return DFCERRSTUS;
    }
    //用IO操作码为索引调用驱动程序功能分派函数数组中的函数
    return drvp->drv_dipfun[iocode](devp, p2);
}
