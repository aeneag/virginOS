//Aen @2022.04.08
//公众号：技术乱舞
//https://aeneag.xyz
//filename:krlobjnode.h
//description: 内核对象节点头文件
#ifndef _KRLOBJNODE_H
#define _KRLOBJNODE_H
void objnode_t_init(objnode_t* initp);
objnode_t* krlnew_objnode();
bool_t krldel_objnode(objnode_t* onodep);
#endif // KRLOBJNODE_H
