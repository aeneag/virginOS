/************************************************************************************************
* Aen @2022.05.11
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: drvrfs.c
* description:内存文件系统文件
************************************************************************************************/
#include "virginostypes.h"
#include "virginosmctrl.h"
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:/初始化rfsdevext_t结构
*     
*            
*  返回 null
**************************************************************************************************/
void rfsdevext_t_init(rfsdevext_t *initp)
{
    krlspinlock_init(&initp->rde_lock);
    list_init(&initp->rde_list);
    initp->rde_flg = 0;
    initp->rde_stus = 0;
    initp->rde_mstart = NULL;
    initp->rde_msize = 0;
    initp->rde_ext = NULL;
    return;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:文件目录初始化
*     
*            
*  返回 null
**************************************************************************************************/
void rfsdir_t_init(rfsdir_t *initp)
{
    initp->rdr_stus = 0;
    initp->rdr_type = RDR_NUL_TYPE;
    initp->rdr_blknr = 0;
    for (uint_t i = 0; i < DR_NM_MAX; i++)
    {
        initp->rdr_name[i] = 0;
    }
    return;
}

void filblks_t_init(filblks_t *initp)
{
    initp->fb_blkstart = 0;
    initp->fb_blknr = 0;
    return;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:超级块init
*     
*            
*  返回 null
**************************************************************************************************/
void rfssublk_t_init(rfssublk_t *initp)
{
    krlspinlock_init(&initp->rsb_lock);
    initp->rsb_mgic = 0x142422;
    initp->rsb_vec = 1;
    initp->rsb_flg = 0;
    initp->rsb_stus = 0;
    initp->rsb_sz = sizeof(rfssublk_t);
    initp->rsb_sblksz = 1;
    initp->rsb_dblksz = FSYS_ALCBLKSZ;
    initp->rsb_bmpbks = 1;
    initp->rsb_bmpbknr = 0;
    initp->rsb_fsysallblk = 0;
    rfsdir_t_init(&initp->rsb_rootdir);
    return;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:文件管理头init
*     
*            
*  返回 null
**************************************************************************************************/
void fimgrhd_t_init(fimgrhd_t *initp)
{
    initp->fmd_stus = 0;
    initp->fmd_type = FMD_NUL_TYPE;
    initp->fmd_flg = 0;
    initp->fmd_sfblk = 0;
    initp->fmd_acss = 0;
    initp->fmd_newtime = 0;
    initp->fmd_acstime = 0;
    initp->fmd_fileallbk = 0;
    initp->fmd_filesz = 0;
    initp->fmd_fileifstbkoff = 0x200;
    initp->fmd_fileiendbkoff = 0;
    initp->fmd_curfwritebk = 0;
    initp->fmd_curfinwbkoff = 0;
    for (uint_t i = 0; i < FBLKS_MAX; i++)
    {
        filblks_t_init(&initp->fmd_fleblk[i]);
    }
    initp->fmd_linkpblk = 0;
    initp->fmd_linknblk = 0;
    return;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:分配空间模拟存储空间
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t new_rfsdevext_mmblk(device_t *devp, size_t blksz)
{
    adr_t blkp = krlnew(blksz);//分配模拟储存介质的内存空间，大小为4MB
    rfsdevext_t *rfsexp = (rfsdevext_t *)krlnew(sizeof(rfsdevext_t));//分配rfsdevext_t结构实例的内存空间
    if (blkp == NULL || rfsexp == NULL)
    {
        return DFCERRSTUS;
    }
    rfsdevext_t_init(rfsexp);//初始化rfsdevext_t结构
    rfsexp->rde_mstart = (void *)blkp;
    rfsexp->rde_msize = blksz;
    //把rfsdevext_t结构的地址放入device_t 结构的dev_extdata字段中，这里dev_extdata字段就起作用
    devp->dev_extdata = (void *)rfsexp;
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description: 获取存储设备中保存 虚拟设备信息
*     
*            
*  返回 null
**************************************************************************************************/
rfsdevext_t *ret_rfsdevext(device_t *devp)
{
    return (rfsdevext_t *)devp->dev_extdata;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:根据块号返回储存设备的块地址
*     
*            
*  返回 null
**************************************************************************************************/
void *ret_rfsdevblk(device_t *devp, uint_t blknr)
{
    rfsdevext_t *rfsexp = ret_rfsdevext(devp);
    void *blkp = rfsexp->rde_mstart + (blknr * FSYS_ALCBLKSZ);
    if (blkp >= (void *)((size_t)rfsexp->rde_mstart + rfsexp->rde_msize))
    {
        return NULL;
    }
    return blkp;
}

/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description: 获取存储设备的超级块中的数量
*     
*            
*  返回 null
**************************************************************************************************/
uint_t ret_rfsdevmaxblknr(device_t *devp)
{
    rfsdevext_t *rfsexp = ret_rfsdevext(devp);
    return (uint_t)(((size_t)rfsexp->rde_msize) / FSYS_ALCBLKSZ);
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:把逻辑储存块中的数据，读取到4KB大小的缓冲区中
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t read_rfsdevblk(device_t *devp, void *rdadr, uint_t blknr)
{
    void *p = ret_rfsdevblk(devp, blknr);//块号返回地址
    if (p == NULL)
    {
        return DFCERRSTUS;
    }
    hal_memcpy(p, rdadr, FSYS_ALCBLKSZ);
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:把缓冲区中超级块的数据写入到储存设备的第0个逻辑储存块中
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t write_rfsdevblk(device_t *devp, void *weadr, uint_t blknr)
{
    void *p = ret_rfsdevblk(devp, blknr);//返回储存设备中第blknr块的逻辑存储块的地址
    if (p == NULL)
    {
        return DFCERRSTUS;
    }
    hal_memcpy(weadr, p, FSYS_ALCBLKSZ);
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:分配缓冲区
*     
*            
*  返回 null
**************************************************************************************************/
void *new_buf(size_t bufsz)
{
    return (void *)krlnew(bufsz);
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:释放缓冲区
*     
*            
*  返回 null
**************************************************************************************************/
void del_buf(void *buf, size_t bufsz)
{
    if (krldelete((adr_t)buf, bufsz) == FALSE)
    {
        hal_sysdie("del buf err");
    }
    return;
}

/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:设置驱动 常规函数
*     
*            
*  返回 null
**************************************************************************************************/
void rfs_set_driver(driver_t *drvp)
{
    drvp->drv_dipfun[IOIF_CODE_OPEN] = rfs_open;
    drvp->drv_dipfun[IOIF_CODE_CLOSE] = rfs_close;
    drvp->drv_dipfun[IOIF_CODE_READ] = rfs_read;
    drvp->drv_dipfun[IOIF_CODE_WRITE] = rfs_write;
    drvp->drv_dipfun[IOIF_CODE_LSEEK] = rfs_lseek;
    drvp->drv_dipfun[IOIF_CODE_IOCTRL] = rfs_ioctrl;
    drvp->drv_dipfun[IOIF_CODE_DEV_START] = rfs_dev_start;
    drvp->drv_dipfun[IOIF_CODE_DEV_STOP] = rfs_dev_stop;
    drvp->drv_dipfun[IOIF_CODE_SET_POWERSTUS] = rfs_set_powerstus;
    drvp->drv_dipfun[IOIF_CODE_ENUM_DEV] = rfs_enum_dev;
    drvp->drv_dipfun[IOIF_CODE_FLUSH] = rfs_flush;
    drvp->drv_dipfun[IOIF_CODE_SHUTDOWN] = rfs_shutdown;
    drvp->drv_name = "rfsdrv";
    return;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:设置驱动 设备信息
*     
*            
*  返回 null
**************************************************************************************************/
void rfs_set_device(device_t *devp, driver_t *drvp)
{

    devp->dev_flgs = DEVFLG_SHARE;
    devp->dev_stus = DEVSTS_NORML;
    devp->dev_id.dev_mtype = FILESYS_DEVICE;
    devp->dev_id.dev_stype = 0;
    devp->dev_id.dev_nr = 0;

    devp->dev_name = "rfs";
    return;
}
/***********************************************************************************************************************/
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:驱动 入口函数
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_entry(driver_t *drvp, uint_t val, void *p)
{
    if (drvp == NULL)
    {
        return DFCERRSTUS;
    }
    //分配device_t结构并对其进行初级初始化
    device_t *devp = new_device_dsc();
    if (devp == NULL)
    {
        return DFCERRSTUS;
    }
    rfs_set_driver(drvp);
    rfs_set_device(devp, drvp);
    //分配模拟储存设备的内存空间
    if (new_rfsdevext_mmblk(devp, FSMM_BLK) == DFCERRSTUS)
    {
        if (del_device_dsc(devp) == DFCERRSTUS) //注意释放资源。
        {
            return DFCERRSTUS;
        }
        return DFCERRSTUS;
    }
    //把设备加入到驱动程序之中
    if (krldev_add_driver(devp, drvp) == DFCERRSTUS)
    {
        if (del_device_dsc(devp) == DFCERRSTUS) //注意释放资源。
        {
            return DFCERRSTUS;
        }
        return DFCERRSTUS;
    }
    //向内核注册设备
    if (krlnew_device(devp) == DFCERRSTUS)
    {
        if (del_device_dsc(devp) == DFCERRSTUS) //注意释放资源
        {
            return DFCERRSTUS;
        }
        return DFCERRSTUS;
    }
    init_rfs(devp);
    //kprint("\n");
    // kprint("rfs\n");
   
    test_rfs_superblk(devp);
    test_rfs_bitmap(devp);
    test_rfs_rootdir(devp);
    test_fsys(devp);
    return DFCOKSTUS;
}
drvstus_t rfs_exit(driver_t *drvp, uint_t val, void *p)
{
    return DFCERRSTUS;
}

/***********************************************************************************************************************/
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description: open file
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_open(device_t *devp, void *iopack)
{
    objnode_t *obp = (objnode_t *)iopack;

    if (obp->on_acsflgs == FSDEV_OPENFLG_OPEFILE)
    {
        return rfs_open_file(devp, iopack);
    }
    if (obp->on_acsflgs == FSDEV_OPENFLG_NEWFILE)
    {
        return rfs_new_file(devp, obp->on_fname, 0);
    }

    return DFCERRSTUS;
}

drvstus_t rfs_close(device_t *devp, void *iopack)
{
    return rfs_close_file(devp, iopack);
}

drvstus_t rfs_read(device_t *devp, void *iopack)
{
    return rfs_read_file(devp, iopack);
}

drvstus_t rfs_write(device_t *devp, void *iopack)
{
    return rfs_write_file(devp, iopack);
}

drvstus_t rfs_lseek(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t rfs_ioctrl(device_t *devp, void *iopack)
{
    objnode_t *obp = (objnode_t *)iopack;
    if (obp->on_ioctrd == FSDEV_IOCTRCD_DELFILE)
    {

        return rfs_del_file(devp, obp->on_fname, 0);
    }
    return DFCERRSTUS;
}

drvstus_t rfs_dev_start(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t rfs_dev_stop(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t rfs_set_powerstus(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t rfs_enum_dev(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t rfs_flush(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t rfs_shutdown(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}
/******************************************************************************************/




/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:新建文件接口函数
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_new_file(device_t *devp, char_t *fname, uint_t flg)
{
    char_t fne[DR_NM_MAX];
    hal_memset((void *)fne, 0, DR_NM_MAX);
    if (rfs_ret_fname(fne, fname) != 0)//提取文件名
    {
        return DFCERRSTUS;
    }
    if (rfs_chkfileisindev(devp, fne) != 0)//是否存在该文件
    {
        return DFCERRSTUS;
    }

    return rfs_new_dirfileblk(devp, fne, RDR_FIL_TYPE, 0);
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:删除文件接口函数
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_del_file(device_t *devp, char_t *fname, uint_t flg)
{
    if (flg != 0)
    {
        return DFCERRSTUS;
    }
    drvstus_t rets = rfs_del_dirfileblk(devp, fname, RDR_FIL_TYPE, 0);
    return rets;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:读取文件接口函数
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_read_file(device_t *devp, void *iopack)
{
    objnode_t *obp = (objnode_t *)iopack;
    if (obp->on_finode == NULL || obp->on_buf == NULL ||
        obp->on_bufsz != FSYS_ALCBLKSZ)
    {
        return DFCERRSTUS;
    }
    return rfs_readfileblk(devp, (fimgrhd_t *)obp->on_finode, obp->on_buf, obp->on_len);
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:写文件接口函数
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_write_file(device_t *devp, void *iopack)
{
    objnode_t *obp = (objnode_t *)iopack;
    if (obp->on_finode == NULL || obp->on_buf == NULL ||
        obp->on_bufsz != FSYS_ALCBLKSZ)
    {
        return DFCERRSTUS;
    }
    return rfs_writefileblk(devp, (fimgrhd_t *)obp->on_finode, obp->on_buf, obp->on_len);
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description: 打开文件接口函数
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_open_file(device_t *devp, void *iopack)
{
    objnode_t *obp = (objnode_t *)iopack;
    //检查objnode_t中的文件路径名
    if (obp->on_fname == NULL)
    {
        return DFCERRSTUS;
    }
    //调用打开文件的核心函数
    void *fmdp = rfs_openfileblk(devp, (char_t *)obp->on_fname);
    if (fmdp == NULL)
    {

        return DFCERRSTUS;
    }
    //把返回的fimgrhd_t结构的地址保存到objnode_t中的on_finode字段中
    obp->on_finode = fmdp;
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description: 关闭文件
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_close_file(device_t *devp, void *iopack)
{
    objnode_t *obp = (objnode_t *)iopack;
    if (obp->on_finode == NULL)
    {
        return DFCERRSTUS;
    }

    return rfs_closefileblk(devp, obp->on_finode);
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description: 比较是否相等
*     
*            
*  返回 null
**************************************************************************************************/
sint_t rfs_strcmp(char_t *str_s, char_t *str_d)
{
    for (;;)
    {
        if (*str_s != *str_d)
        {
            return 0;
        }
        if (*str_s == 0)
        {
            break;
        }
        str_s++;
        str_d++;
    }
    return 1;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description: 获取名字长度
*     
*            
*  返回 null
**************************************************************************************************/
sint_t rfs_strlen(char *str_s)
{
    sint_t chaidx = 0;
    while (*str_s != 0)
    {
        str_s++;
        chaidx++;
    }
    return chaidx;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description: 复制名字
*     
*            
*  返回 null
**************************************************************************************************/
sint_t rfs_strcpy(char_t *str_s, char_t *str_d)
{
    sint_t chaidx = 0;
    while (*str_s != 0)
    {
        *str_d = *str_s;
        str_s++;
        str_d++;
        chaidx++;
    }
    *str_d = *str_s;
    return chaidx;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description: 根目录 初始化
*     
*            
*  返回 null
**************************************************************************************************/
void init_rfs(device_t *devp)
{
    rfs_fmat(devp);
    return;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:根目录 格式化
*     
*            
*  返回 null
**************************************************************************************************/
void rfs_fmat(device_t *devp)
{
    if (create_superblk(devp) == FALSE)
    {
        hal_sysdie("create superblk err");
    }
    if (create_bitmap(devp) == FALSE)
    {
        hal_sysdie("create bitmap err");
    }
    if (create_rootdir(devp) == FALSE)
    {
        hal_sysdie("create rootdir err");
    }
    return;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:写文件函数
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_writefileblk(device_t *devp, fimgrhd_t *fmp, void *buf, uint_t len)
{
    if (fmp->fmd_sfblk != fmp->fmd_curfwritebk ||
        fmp->fmd_curfwritebk != fmp->fmd_fleblk[0].fb_blkstart)
    {
        return DFCERRSTUS;
    }
    if ((fmp->fmd_curfinwbkoff + len) >= FSYS_ALCBLKSZ)
    {
        return DFCERRSTUS;
    }
    //指向将要写入数据的内存空间 追加写
    void *wrp = (void *)((uint_t)fmp + fmp->fmd_curfinwbkoff);
    hal_memcpy(buf, wrp, len);
    fmp->fmd_filesz += len;
    fmp->fmd_curfinwbkoff += len;
    //把文件数据写入到相应的逻辑储存块中，完成数据同步
    write_rfsdevblk(devp, (void *)fmp, fmp->fmd_curfwritebk);
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:删除文件函数
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_readfileblk(device_t *devp, fimgrhd_t *fmp, void *buf, uint_t len)
{
    if (fmp->fmd_sfblk != fmp->fmd_curfwritebk ||
        fmp->fmd_curfwritebk != fmp->fmd_fleblk[0].fb_blkstart)
    {
        return DFCERRSTUS;
    }
    if (len > (FSYS_ALCBLKSZ - fmp->fmd_fileifstbkoff))
    {
        return DFCERRSTUS;
    }
    void *wrp = (void *)((uint_t)fmp + fmp->fmd_fileifstbkoff);
    hal_memcpy(wrp, buf, len);
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description: 关闭文件
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_closefileblk(device_t *devp, void *fblkp)
{
    fimgrhd_t *fmp = (fimgrhd_t *)fblkp;
    write_rfsdevblk(devp, fblkp, fmp->fmd_sfblk);//保证
    del_buf(fblkp, FSYS_ALCBLKSZ);
    return DFCOKSTUS;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description: 打开文件核心函数
*     
*            
*  返回 null
**************************************************************************************************/
void *rfs_openfileblk(device_t *devp, char_t *fname)
{
    char_t fne[DR_NM_MAX];
    void *rets = NULL, *buf = NULL;
    hal_memset((void *)fne, 0, DR_NM_MAX);
    if (rfs_ret_fname(fne, fname) != 0)//从文件路径名中提取纯文件名
    {
        return NULL;
    }

    void *rblkp = get_rootdirfile_blk(devp);//获取根目录文件
    if (rblkp == NULL)
    {

        return NULL;
    }
    fimgrhd_t *fmp = (fimgrhd_t *)rblkp;

    if (fmp->fmd_type != FMD_DIR_TYPE)//判断根目录文件的类型是否合理
    {
        rets = NULL;
        goto err;
    }
    //判断根目录文件里有没有数据
    if (fmp->fmd_curfwritebk == fmp->fmd_fleblk[0].fb_blkstart &&
        fmp->fmd_curfinwbkoff == fmp->fmd_fileifstbkoff)
    {
        rets = NULL;
        goto err;
    }
    rfsdir_t *dirp = (rfsdir_t *)((uint_t)(fmp) + fmp->fmd_fileifstbkoff);
    void *maxchkp = (void *)((uint_t)rblkp + FSYS_ALCBLKSZ - 1);
    for (; (void *)dirp < maxchkp;)//寻找相同名字的文件
    {
        if (dirp->rdr_type == RDR_FIL_TYPE)
        {

            if (rfs_strcmp(dirp->rdr_name, fne) == 1)
            {
                goto opfblk;
            }
        }
        dirp++;
    }

    rets = NULL;
    goto err;
opfblk:
    buf = new_buf(FSYS_ALCBLKSZ);
    if (buf == NULL)
    {
        rets = NULL;
        goto err;
    }
    //读取该文件占用的逻辑储存块
    if (read_rfsdevblk(devp, buf, dirp->rdr_blknr) == DFCERRSTUS)
    {
        rets = NULL;
        goto err1;
    }
    fimgrhd_t *ffmp = (fimgrhd_t *)buf;
    if (ffmp->fmd_type == FMD_NUL_TYPE || ffmp->fmd_fileifstbkoff != 0x200)
    {
        rets = NULL;
        goto err1;
    }
    rets = buf;//设置缓冲区首地址为返回值
    goto err;

err1:
    del_buf(buf, FSYS_ALCBLKSZ);
err:
    del_rootdirfile_blk(devp, rblkp);
    return rets;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:新建文件
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_new_dirfileblk(device_t *devp, char_t *fname, uint_t flgtype, uint_t val)
{
    drvstus_t rets = DFCERRSTUS;
    if (flgtype != RDR_FIL_TYPE)
    {
        return DFCERRSTUS;
    }
    void *buf = new_buf(FSYS_ALCBLKSZ);//分配一个4KB大小的缓冲区
    if (buf == NULL)
    {
        return DFCERRSTUS;
    }
    hal_memset(buf, 0, FSYS_ALCBLKSZ);
    uint_t fblk = rfs_new_blk(devp);//分配一个新的空闲逻辑储存块
    if (fblk == 0)
    {
        rets = DFCERRSTUS;
        goto err1;
    }
    void *rdirblk = get_rootdirfile_blk(devp);//获取根目录文件
    if (rdirblk == NULL)
    {
        rets = DFCERRSTUS;
        goto err1;
    }
    fimgrhd_t *fmp = (fimgrhd_t *)rdirblk;
    //指向文件当前的写入地址，因为根目录文件已经被读取到内存中了
    rfsdir_t *wrdirp = (rfsdir_t *)((uint_t)rdirblk + fmp->fmd_curfinwbkoff);
    if (((uint_t)wrdirp) >= ((uint_t)rdirblk + FSYS_ALCBLKSZ))
    {
        rets = DFCERRSTUS;
        goto err;
    }
    wrdirp->rdr_stus = 0;
    wrdirp->rdr_type = flgtype;
    wrdirp->rdr_blknr = fblk;
    rfs_strcpy(fname, wrdirp->rdr_name);
    fmp->fmd_filesz += (uint_t)(sizeof(rfsdir_t));
    fmp->fmd_curfinwbkoff += (uint_t)(sizeof(rfsdir_t));
    fimgrhd_t *ffmp = (fimgrhd_t *)buf;//指向新分配的缓冲区
    fimgrhd_t_init(ffmp);
    ffmp->fmd_type = FMD_FIL_TYPE;
    ffmp->fmd_sfblk = fblk;
    ffmp->fmd_curfwritebk = fblk;
    ffmp->fmd_curfinwbkoff = 0x200;
    //把文件储存块数组的第1个元素的开始块，设为刚刚分配的空闲逻辑储存块
    ffmp->fmd_fleblk[0].fb_blkstart = fblk;
    //因为只分配了一个逻辑储存块，所以设为1
    ffmp->fmd_fleblk[0].fb_blknr = 1;
    if (write_rfsdevblk(devp, buf, fblk) == DFCERRSTUS)
    {
        rets = DFCERRSTUS;
        goto err;
    }
    rets = DFCOKSTUS;
err:
    del_rootdirfile_blk(devp, rdirblk);
err1:
    del_buf(buf, FSYS_ALCBLKSZ);
    return rets;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:删除文件
*     
*            
*  返回 null
**************************************************************************************************/
drvstus_t rfs_del_dirfileblk(device_t *devp, char_t *fname, uint_t flgtype, uint_t val)
{
    if (flgtype != RDR_FIL_TYPE || val != 0)
    {
        return DFCERRSTUS;
    }
    char_t fne[DR_NM_MAX];
    hal_memset((void *)fne, 0, DR_NM_MAX);
    if (rfs_ret_fname(fne, fname) != 0)//文件名 去 /
    {
        return DFCERRSTUS;
    }
    if (del_dirfileblk_core(devp, fne) != 0)
    {
        return DFCERRSTUS;
    }
    return DFCOKSTUS;
}

/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:删除文件核心函数
*     
*            
*  返回 null
**************************************************************************************************/
sint_t del_dirfileblk_core(device_t *devp, char_t *fname)
{
    sint_t rets = 6;
    void *rblkp = get_rootdirfile_blk(devp);
    if (rblkp == NULL)
    {

        return 5;
    }
    fimgrhd_t *fmp = (fimgrhd_t *)rblkp;

    if (fmp->fmd_type != FMD_DIR_TYPE)
    {
        rets = 4;
        goto err;
    }
    if (fmp->fmd_curfwritebk == fmp->fmd_fleblk[0].fb_blkstart &&
        fmp->fmd_curfinwbkoff == fmp->fmd_fileifstbkoff)
    {
        rets = 3;
        goto err;
    }
    rfsdir_t *dirp = (rfsdir_t *)((uint_t)(fmp) + fmp->fmd_fileifstbkoff);
    void *maxchkp = (void *)((uint_t)rblkp + FSYS_ALCBLKSZ - 1);
    for (; (void *)dirp < maxchkp;)
    {
        if (dirp->rdr_type == RDR_FIL_TYPE)
        {

            if (rfs_strcmp(dirp->rdr_name, fname) == 1)
            {
                rfs_del_blk(devp, dirp->rdr_blknr);
                rfsdir_t_init(dirp);
                dirp->rdr_type = RDR_DEL_TYPE;
                rets = 0;
                goto err;
            }
        }
        dirp++;
    }
    rets = 1;
err:
    del_rootdirfile_blk(devp, rblkp);
    return rets;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:获取根目录文件
*     
*            
*  返回 null
**************************************************************************************************/
void *get_rootdirfile_blk(device_t *devp)
{
    void *retptr = NULL;
    //获取根目录文件的rfsdir_t结构
    rfsdir_t *rtdir = get_rootdir(devp);
    if (rtdir == NULL)
    {
        return NULL;
    }
    void *buf = new_buf(FSYS_ALCBLKSZ);
    if (buf == NULL)
    {
        retptr = NULL;
        goto errl1;
    }
    hal_memset(buf, 0, FSYS_ALCBLKSZ);
    //读取根目录文件的逻辑储存块到缓冲区中
    if (read_rfsdevblk(devp, buf, rtdir->rdr_blknr) == DFCERRSTUS)
    {
        retptr = NULL;
        goto errl;
    }
    retptr = buf;
    goto errl1;
errl:
    del_buf(buf, FSYS_ALCBLKSZ);
errl1:
    del_rootdir(devp, rtdir);

    return retptr;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:释放根目录文件
*     
*            
*  返回 null
**************************************************************************************************/
void del_rootdirfile_blk(device_t *devp, void *blkp)
{
    fimgrhd_t *fmp = (fimgrhd_t *)blkp;
    if (write_rfsdevblk(devp, blkp, fmp->fmd_sfblk) == DFCERRSTUS)
    {
        hal_sysdie("del_rootfile_blk err");
    }
    del_buf(blkp, FSYS_ALCBLKSZ);
    return;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:获取根目录文件的rfsdir_t结构
*     
*            
*  返回 null
**************************************************************************************************/
rfsdir_t *get_rootdir(device_t *devp)
{
    rfsdir_t *retptr = NULL;
    rfssublk_t *sbp = get_superblk(devp);
    if (sbp == NULL)
    {
        return NULL;
    }
    void *buf = new_buf(sizeof(rfsdir_t));
    if (buf == NULL)
    {
        retptr = NULL;
        goto errl;
    }
    hal_memcpy((void *)(&sbp->rsb_rootdir), buf, sizeof(rfsdir_t));
    retptr = (rfsdir_t *)buf;
errl:
    del_superblk(devp, sbp);
    return retptr;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:释放根目录文件的rfsdir_t结构
*     
*            
*  返回 null
**************************************************************************************************/
void del_rootdir(device_t *devp, rfsdir_t *rdir)
{
    del_buf((void *)rdir, sizeof(rfsdir_t));
    return;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:获取 超级块
*     
*            
*  返回 null
**************************************************************************************************/
rfssublk_t *get_superblk(device_t *devp)
{
    void *buf = new_buf(FSYS_ALCBLKSZ);//分配4KB大小的缓冲区
    if (buf == NULL)
    {
        return NULL;
    }
    hal_memset(buf, 0, FSYS_ALCBLKSZ);//清零缓冲区
    //读取第0个逻辑储存块中的数据到缓冲区中，如果读取失败则释放缓冲区
    if (read_rfsdevblk(devp, buf, 0) == DFCERRSTUS)
    {
        del_buf(buf, FSYS_ALCBLKSZ);
        return NULL;
    }
    return (rfssublk_t *)buf;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:获取 超级块
*     
*            
*  返回 null
**************************************************************************************************/
void del_superblk(device_t *devp, rfssublk_t *sbp)
{//回写超级块，因为超级块中的数据可能已经发生了改变，如果出错则死机
    if (write_rfsdevblk(devp, (void *)sbp, 0) == DFCERRSTUS)
    {
        hal_sysdie("del superblk err");
    }
    del_buf((void *)sbp, FSYS_ALCBLKSZ);
    return;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:获取位图块
*     
*            
*  返回 null
**************************************************************************************************/
u8_t *get_bitmapblk(device_t *devp)
{
    rfssublk_t *sbp = get_superblk(devp);//获取超级块
    if (sbp == NULL)
    {
        return NULL;
    }
    void *buf = new_buf(FSYS_ALCBLKSZ);//分配4KB大小的缓冲区
    if (buf == NULL)
    {
        return NULL;
    }
    hal_memset(buf, 0, FSYS_ALCBLKSZ);
    if (read_rfsdevblk(devp, buf, sbp->rsb_bmpbks) == DFCERRSTUS)//读取sbp->rsb_bmpbks块（位图块），到缓冲区中
    {
        del_buf(buf, FSYS_ALCBLKSZ);
        del_superblk(devp, sbp);
        return NULL;
    }
    del_superblk(devp, sbp);
    return (u8_t *)buf;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:释放位图块
*     
*            
*  返回 null
**************************************************************************************************/
void del_bitmapblk(device_t *devp, u8_t *bitmap)
{
    rfssublk_t *sbp = get_superblk(devp);
    if (sbp == NULL)
    {
        hal_sysdie("del superblk err");
        return;
    }
    //回写位图块，因为位图块中的数据可能已经发生改变
    if (write_rfsdevblk(devp, (void *)bitmap, sbp->rsb_bmpbks) == DFCERRSTUS)
    {
        del_superblk(devp, sbp);
        hal_sysdie("del superblk err1");
    }
    del_superblk(devp, sbp);
    del_buf((void *)bitmap, FSYS_ALCBLKSZ);
    return;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:分配新的空闲逻辑储存块
*     
*            code so easy !
*  返回 null
**************************************************************************************************/
uint_t rfs_new_blk(device_t *devp)
{
    uint_t retblk = 0;
    u8_t *bitmap = get_bitmapblk(devp);
    if (bitmap == NULL)
    {
        return 0;
    }
    for (uint_t blknr = 2; blknr < FSYS_ALCBLKSZ; blknr++)
    {
        if (bitmap[blknr] == 0)
        {
            bitmap[blknr] = 1;
            retblk = blknr;
            goto retl;
        }
    }
    retblk = 0;
retl:
    del_bitmapblk(devp, bitmap);
    return retblk;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:释放逻辑储存块
*     
*            code so easy !
*  返回 null
**************************************************************************************************/
void rfs_del_blk(device_t *devp, uint_t blknr)
{
    if (blknr > FSYS_ALCBLKSZ)
    {
        hal_sysdie("rfs del blk err");
        return;
    }

    u8_t *bitmap = get_bitmapblk(devp);
    if (bitmap == NULL)
    {
        hal_sysdie("rfs del blk err1");
        return;
    }
    bitmap[blknr] = 0;
    del_bitmapblk(devp, bitmap);
    return;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:提取纯文件名
*     
*            
*  返回 null
**************************************************************************************************/
sint_t rfs_chkfilepath(char_t *fname)
{
    char_t *chp = fname;
    if (chp[0] != '/')
    {
        return 2;
    }
    for (uint_t i = 1;; i++)
    {
        if (chp[i] == '/')
        {
            return 3;
        }
        if (i >= DR_NM_MAX)
        {
            return 4;
        }
        if (chp[i] == 0 && i > 1)
        {
            break;
        }
    }
    return 0;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:提取纯文件名
*     
*            
*  返回 null
**************************************************************************************************/
sint_t rfs_ret_fname(char_t *buf, char_t *fpath)
{
    if (buf == NULL || fpath == NULL)
    {
        return 6;
    }
    //检查文件路径名是不是“/xxxx”的形式
    sint_t stus = rfs_chkfilepath(fpath);
    if (stus != 0)
    {
        return stus;
    }
    //从路径名字符串的第2个字符开始复制字符到buf中
    rfs_strcpy(&fpath[1], buf);
    return 0;
}
/************************************************************************************************
* @2022/05/12
* Aen
* https://aeneag.xyz
* description:判断文件是否存在
*     
*            
*  返回 null
**************************************************************************************************/
sint_t rfs_chkfileisindev(device_t *devp, char_t *fname)
{
    sint_t rets = 6;
    //获取文件名的长度，注意不是文件路径名
    sint_t ch = rfs_strlen(fname);
    if (ch < 1 || ch >= (sint_t)DR_NM_MAX)
    {
        return 4;
    }
    //get root dir file
    void *rdblkp = get_rootdirfile_blk(devp);
    if (rdblkp == NULL)
    {
        return 2;
    }
    // 转为文件管理头
    fimgrhd_t *fmp = (fimgrhd_t *)rdblkp;
    //检查该fimgrhd_t结构的类型是不是FMD_DIR_TYPE，即这个文件是不是目录文件
    if (fmp->fmd_type != FMD_DIR_TYPE)
    {
        rets = 3;
        goto err;
    }
    //检查根目录文件是不是为空，即没有写入任何数据，所以返回0，表示根目录下没有对应的文件
    if (fmp->fmd_curfwritebk == fmp->fmd_fleblk[0].fb_blkstart &&
        fmp->fmd_curfinwbkoff == fmp->fmd_fileifstbkoff)
    {
        rets = 0;
        goto err;
    }
    //指向根目录文件的第一个字节
    rfsdir_t *dirp = (rfsdir_t *)((uint_t)(fmp) + fmp->fmd_fileifstbkoff);
    //指向根目录文件的结束地址
    void *maxchkp = (void *)((uint_t)rdblkp + FSYS_ALCBLKSZ - 1);
    //当前的rfsdir_t结构的指针比根目录文件的结束地址小，就继续循环
    for (; (void *)dirp < maxchkp;)
    {
        if (dirp->rdr_type == RDR_FIL_TYPE)
        {

            if (rfs_strcmp(dirp->rdr_name, fname) == 1)
            {
                rets = 1;
                goto err;
            }
        }
        dirp++;
    }
    rets = 0;
err:
    del_rootdirfile_blk(devp, rdblkp);
    return rets;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:建立超级块
*     
*            
*  返回 null
**************************************************************************************************/
bool_t create_superblk(device_t *devp)
{
    void *buf = new_buf(FSYS_ALCBLKSZ);//分配4KB大小的缓冲区，清零
    if (buf == NULL)
    {
        return FALSE;
    }
    hal_memset(buf, 0, FSYS_ALCBLKSZ);
    //使rfssublk_t结构的指针指向缓冲区并进行初始化
    rfssublk_t *sbp = (rfssublk_t *)buf;
    rfssublk_t_init(sbp);
    //获取储存设备的逻辑储存块数并保存到超级块中
    sbp->rsb_fsysallblk = ret_rfsdevmaxblknr(devp);
    //把缓冲区中超级块的数据写入到储存设备的第0个逻辑储存块中
    if (write_rfsdevblk(devp, buf, 0) == DFCERRSTUS)
    {
        return FALSE;
    }
    del_buf(buf, FSYS_ALCBLKSZ);//释放缓冲区
    return TRUE;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:建立位图
*     
*            
*  返回 null
**************************************************************************************************/
bool_t create_bitmap(device_t *devp)
{
    bool_t rets = FALSE;
    //获取超级块，失败则返回FALSE
    rfssublk_t *sbp = get_superblk(devp);
    if (sbp == NULL)
    {
        return FALSE;
    }
    //分配4KB大小的缓冲区
    void *buf = new_buf(FSYS_ALCBLKSZ);
    if (buf == NULL)
    {
        return FALSE;
    }
    //获取超级块中位图块的开始块号
    uint_t bitmapblk = sbp->rsb_bmpbks;
    //获取超级块中储存介质的逻辑储存块总数
    uint_t devmaxblk = sbp->rsb_fsysallblk;
    //如果逻辑储存块总数大于4096，就认为出错了
    if (devmaxblk > FSYS_ALCBLKSZ)
    {
        rets = FALSE;
        goto errlable;
    }
    //把缓冲区中每个字节都置成1
    hal_memset(buf, 1, FSYS_ALCBLKSZ);
    u8_t *bitmap = (u8_t *)buf;
    //把缓冲区中的第3个字节到第devmaxblk个字节都置成0
    for (uint_t bi = 2; bi < devmaxblk; bi++)
    {
        bitmap[bi] = 0;
    }
    //把缓冲区中的数据写入到储存介质中的第bitmapblk个逻辑储存块中，即位图块中
    if (write_rfsdevblk(devp, buf, bitmapblk) == DFCERRSTUS)
    {
        rets = FALSE;
        goto errlable;
    }
    //设置返回状态
    rets = TRUE;
errlable:
//释放超级块
    del_superblk(devp, sbp);
//释放缓冲区
    del_buf(buf, FSYS_ALCBLKSZ);
    return rets;
}
/************************************************************************************************
* @2022/05/11
* Aen
* https://aeneag.xyz
* description:建立根目录
*     
*            
*  返回 null
**************************************************************************************************/
bool_t create_rootdir(device_t *devp)
{
    bool_t rets = FALSE;
    rfssublk_t *sbp = get_superblk(devp);//获取超级块
    if (sbp == NULL)
    {
        return FALSE;
    }
    void *buf = new_buf(FSYS_ALCBLKSZ);//分配4KB大小的缓冲区
    if (buf == NULL)
    {
        rets = FALSE;
        goto errlable1;
    }
    hal_memset(buf, 0, FSYS_ALCBLKSZ);
    uint_t blk = rfs_new_blk(devp);//分配一个空闲的逻辑储存块
    if (blk == 0)
    {
        rets = FALSE;
        goto errlable;
    }
    sbp->rsb_rootdir.rdr_name[0] = '/';             //设置超级块中的rfsdir_t结构中的名称为“/”
    sbp->rsb_rootdir.rdr_type = RDR_DIR_TYPE;       //设置超级块中的rfsdir_t结构中的类型为目录类型
    sbp->rsb_rootdir.rdr_blknr = blk;               //设置超级块中的rfsdir_t结构中的块号为新分配的空闲逻辑储存块的块号
    fimgrhd_t *fmp = (fimgrhd_t *)buf;
    fimgrhd_t_init(fmp);                            //初始化fimgrhd_t结构
    fmp->fmd_type = FMD_DIR_TYPE;                   //因为这是目录文件所以fimgrhd_t结构的类型设置为目录类型
    fmp->fmd_sfblk = blk;                           //fimgrhd_t结构自身所在的块设置为新分配的空闲逻辑储存块
    fmp->fmd_curfwritebk = blk;                     //fimgrhd_t结构中正在写入的块设置为新分配的空闲逻辑储存块
    fmp->fmd_curfinwbkoff = 0x200;                  //fimgrhd_t结构中正在写入的块的偏移设置为512字节
    fmp->fmd_fleblk[0].fb_blkstart = blk;           //设置文件数据占有块数组的第0个元素
    fmp->fmd_fleblk[0].fb_blknr = 1;
    //把缓冲区中的数据写入到新分配的空闲逻辑储存块中，其中包含已经设置好的 fimgrhd_t结构
    if (write_rfsdevblk(devp, buf, blk) == DFCERRSTUS)
    {
        rets = FALSE;
        goto errlable;
    }
    rets = TRUE;
errlable:
    del_buf(buf, FSYS_ALCBLKSZ);
errlable1:
    del_superblk(devp, sbp);

    return rets;
}
/**************************************************************************************************************/
//测试程序
void test_rfs_bitmap(device_t *devp)
{
    kprint("开始文件系统位图测试\n");
    void *buf = new_buf(FSYS_ALCBLKSZ);
    if (buf == NULL)
    {
        hal_sysdie("chkbitmap err");
    }
    hal_memset(buf, 0, FSYS_ALCBLKSZ);
    if (read_rfsdevblk(devp, buf, 1) == DFCERRSTUS)
    {
        hal_sysdie("chkbitmap err1");
    }
    u8_t *bmp = (u8_t *)buf;
    uint_t b = 0;
    for (uint_t i = 0; i < FSYS_ALCBLKSZ; i++)
    {
        if (bmp[i] == 0)
        {
            b++;
        }
    }
    kprint("文件系统空闲块数:%d\n", b);
    del_buf(buf, FSYS_ALCBLKSZ);
    kprint("结束文件系统位图测试\n\n");
    die(0x400);
    return;
}

void test_allocblk(device_t *devp)
{
    uint_t ai[64];
    uint_t i = 0, j = 0;
    for (;;)
    {
        for (uint_t k = 0; k < 64; k++)
        {
            i = rfs_new_blk(devp);
            if (i == 0)
                hal_sysdie("alloc blkk err");

            kprint("alloc blk:%x\n", i);
            ai[k] = i;
            j++;
        }
        for (uint_t m = 0; m < 64; m++)
        {
            rfs_del_blk(devp, ai[m]);
            kprint("free blk:%x\n", ai[m]);
        }
    }
    kprint("alloc indx:%x\n", j);
    return;
}
void test_rfs_rootdir(device_t *devp)
{
    kprint("开始文件系统根目录测试\n");
    rfsdir_t *dr = get_rootdir(devp);
    void *buf = new_buf(FSYS_ALCBLKSZ);
    if (buf == NULL)
    {
        hal_sysdie("testdir err");
    }
    hal_memset(buf, 0, FSYS_ALCBLKSZ);
    if (read_rfsdevblk(devp, buf, dr->rdr_blknr) == DFCERRSTUS)
    {
        hal_sysdie("testdir1 err1");
    }
    fimgrhd_t *fmp = (fimgrhd_t *)buf;
    kprint("文件管理头类型:%d 文件数据大小:%d 文件在开始块中偏移:%d 文件在结束块中的偏移:%d\n",
            fmp->fmd_type, fmp->fmd_filesz, fmp->fmd_fileifstbkoff, fmp->fmd_fileiendbkoff);
    kprint("文件第一组开始块号:%d 块数:%d\n", fmp->fmd_fleblk[0].fb_blkstart, fmp->fmd_fleblk[0].fb_blknr);
    del_buf(buf, FSYS_ALCBLKSZ);
    del_rootdir(devp, dr);
    kprint("结束文件系统根目录测试\n\n");
    die(0x400);
    return;
}

void test_rfs_superblk(device_t *devp)
{
    kprint("开始文件系统超级块测试\n");
    rfssublk_t *sbp = get_superblk(devp);
    kprint("文件系统标识:%d,版本:%d\n", sbp->rsb_mgic, sbp->rsb_vec);
    kprint("文件系统超级块块数:%d,逻辑储存块大小:%d\n", sbp->rsb_sblksz, sbp->rsb_dblksz);
    kprint("文件系统位图块号:%d,文件系统整个逻辑储存块数:%d\n", sbp->rsb_bmpbks, sbp->rsb_fsysallblk);
    kprint("文件系统根目录块号:%d 类型:%d\n", sbp->rsb_rootdir.rdr_blknr, sbp->rsb_rootdir.rdr_type);
    kprint("文件系统根目录名称:%s\n", sbp->rsb_rootdir.rdr_name);
    del_superblk(devp, sbp);
    kprint("结束文件系统超级块测试\n\n");
    die(0x400);
    return;
}


void test_fsys(device_t *devp)
{ 
    kprint("开始文件操作测试\n");
    void *rwbuf = new_buf(FSYS_ALCBLKSZ);
    if (rwbuf == NULL)
    {
        hal_sysdie("rwbuf is NULL");
    }
    hal_memset(rwbuf, 0x0f, FSYS_ALCBLKSZ);
    objnode_t *ondp = krlnew_objnode();
    if (ondp == NULL)
    {
        hal_sysdie("ondp is NULL");
    }

    ondp->on_acsflgs = FSDEV_OPENFLG_NEWFILE;
    ondp->on_fname = "/testfile";
    ondp->on_buf = rwbuf;
    ondp->on_bufsz = FSYS_ALCBLKSZ;
    ondp->on_len = 512;
    ondp->on_ioctrd = FSDEV_IOCTRCD_DELFILE;
    if (rfs_open(devp, ondp) == DFCERRSTUS)
    {
        hal_sysdie("新建文件错误");
    }
    kprint("新建文件成功\n");
    ondp->on_acsflgs = FSDEV_OPENFLG_OPEFILE;

    if (rfs_open(devp, ondp) == DFCERRSTUS)
    {
        hal_sysdie("打开文件错误");
    }
    kprint("打开文件成功\n");
    if (rfs_write(devp, ondp) == DFCERRSTUS)
    {
        hal_sysdie("写入文件错误");
    }

    hal_memset(rwbuf, 0, FSYS_ALCBLKSZ);
    kprint("写入文件成功\n");
    if (rfs_read(devp, ondp) == DFCERRSTUS)
    {
        hal_sysdie("读取文件错误");
    }
    kprint("读取文件成功\n");
    if (rfs_close(devp, ondp) == DFCERRSTUS)
    {
        hal_sysdie("关闭文件错误");
    }
    kprint("关闭文件成功\n");
    u8_t *cb = (u8_t *)rwbuf;
    for (uint_t i = 0; i < 512; i++)
    {
        if (cb[i] != 0x0f)
        {
            hal_sysdie("检查文件内容错误");
        }
       // kprint("testfile文件第[%x]个字节数据:%x\n", i, (uint_t)cb[i]);
    }
    kprint("检查文件成功\n");
    if (rfs_ioctrl(devp, ondp) == DFCERRSTUS)
    {
        hal_sysdie("删除文件错误");
    }
    kprint("删除文件成功\n");
    ondp->on_acsflgs = FSDEV_OPENFLG_OPEFILE;

    if (rfs_open(devp, ondp) == DFCERRSTUS)
    {
        kprint("再次打开文件失败\n");
    }
    kprint("结束文件操作测试\n");
    die(0x400);
    return;
}
// void test_file(device_t *devp)
// {
//     //test_rfs(devp);
//     //chk_rfsbitmap(devp);
//     //test_dir(devp);
//     test_fsys(devp);
//     hal_sysdie("test file run");
//     return;
// }

// void test_fsys(device_t *devp)
// {
//     die(0x400);
//     void *rwbuf = new_buf(FSYS_ALCBLKSZ);
//     if (rwbuf == NULL)
//     {
//         hal_sysdie("rwbuf is NULL");
//     }
//     hal_memset(rwbuf, 0xff, FSYS_ALCBLKSZ);
//     objnode_t *ondp = krlnew_objnode();
//     if (ondp == NULL)
//     {
//         hal_sysdie("ondp is NULL");
//     }

//     ondp->on_acsflgs = FSDEV_OPENFLG_NEWFILE;
//     ondp->on_fname = "/testfile";
//     ondp->on_buf = rwbuf;
//     ondp->on_bufsz = FSYS_ALCBLKSZ;
//     ondp->on_len = 512;
//     ondp->on_ioctrd = FSDEV_IOCTRCD_DELFILE;
//     if (rfs_open(devp, ondp) == DFCERRSTUS)
//     {
//         hal_sysdie("new file is err");
//     }

//     ondp->on_acsflgs = FSDEV_OPENFLG_OPEFILE;

//     if (rfs_open(devp, ondp) == DFCERRSTUS)
//     {
//         hal_sysdie("open file is err");
//     }

//     if (rfs_write(devp, ondp) == DFCERRSTUS)
//     {
//         hal_sysdie("write file is err");
//     }
//     hal_memset(rwbuf, 0, FSYS_ALCBLKSZ);

//     if (rfs_read(devp, ondp) == DFCERRSTUS)
//     {
//         hal_sysdie("read file is err");
//     }

//     if (rfs_close(devp, ondp) == DFCERRSTUS)
//     {
//         hal_sysdie("close file is err");
//     }
//     u8_t *cb = (u8_t *)rwbuf;
//     for (uint_t i = 0; i < 512; i++)
//     {
//         if (cb[i] != 0xff)
//         {
//             hal_sysdie("chek rwbuf err");
//         }
//         kprint("rwblk[%x]:%x\n", i, (uint_t)cb[i]);
//     }

//     if (rfs_ioctrl(devp, ondp) == DFCERRSTUS)
//     {
//         hal_sysdie("del file is err");
//     }
//     ondp->on_acsflgs = FSDEV_OPENFLG_OPEFILE;

//     if (rfs_open(devp, ondp) == DFCERRSTUS)
//     {
//         hal_sysdie("2open2 file is err");
//     }
//     hal_sysdie("test_fsys ok");
//     return;
// }
