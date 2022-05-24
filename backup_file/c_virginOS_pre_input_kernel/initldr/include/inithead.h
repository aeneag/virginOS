//Aen @2022.03.31
//公众号：技术乱舞
//https://aeneag.xyz
//filename:inithead.h
//description: copy bin file to memory

#ifndef INITHEAD_H
#define INITHEAD_H

//入口函数，复制两个bin文件到内存中去
void inithead_entry();

void write_realintsvefile();
fhdsc_t* find_file(char_t* fname);
void write_ldrkrlfile();
void error(char_t* estr);
int strcmpl(const char *a,const char *b);

void write_shellfile();
void write_kernelfile();



#endif // !INITHEAD_H