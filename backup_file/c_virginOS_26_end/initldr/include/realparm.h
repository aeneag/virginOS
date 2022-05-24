//Aen @2022.03.31
//公众号：技术乱舞
//https://aeneag.xyz
//filename:realparm.h
//description: 

#ifndef REALPARM_H
#define REALPARM_H


#define ETYBAK_ADR 0x2000
#define PM32_EIP_OFF (ETYBAK_ADR)
#define PM32_ESP_OFF (ETYBAK_ADR+4)
#define RM16_EIP_OFF (ETYBAK_ADR+8)
#define RM16_ESP_OFF (ETYBAK_ADR+12)

#define RWHDPACK_ADR (ETYBAK_ADR+32)
#define E80MAP_NR (ETYBAK_ADR+64)
#define E80MAP_ADRADR (ETYBAK_ADR+68)
#define E80MAP_ADR (0x5000)
#define VBEINFO_ADR (0x6000)
#define VBEMINFO_ADR (0x6400)

#define READHD_BUFADR 0x3000



#endif // !REALPARM_H
