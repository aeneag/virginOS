//Aen @2022.04.08
//公众号：技术乱舞
//https://aeneag.xyz
//filename: krltime.h
//description: 时间头文件
#ifndef _KRLTIME_H
#define _KRLTIME_H
void ktime_t_init(ktime_t* initp);
void init_ktime();
void krlupdate_times(uint_t year,uint_t mon,uint_t day,uint_t date,uint_t hour,uint_t min,uint_t sec);
sysstus_t krlsvetabl_time(uint_t swinr,stkparame_t* stkparv);
sysstus_t krlsve_time(time_t* time);
#endif // KRLTIME_H
