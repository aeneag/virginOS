;Aen @2022.03.31
;公众号：技术乱舞
;https://aeneag.xyz
;filename:imginithead.asm
;description: init grub head assembly code

MBT_HDR_FLAGS	EQU 0x00010003
MBT_HDR_MAGIC	EQU 0x1BADB002	;多引导协议头魔数
MBT2_MAGIC	EQU 0xe85250d6		;第二版多引导协议头魔数
global _start					;导出_start符号
extern inithead_entry			;导入外部的inithead_entry函数符号
[section .text]					;定义.text代码节
[bits 32]						;汇编成32位代码

_start:
	jmp _entry

ALIGN 4
mbt_hdr:						;GRUB所需要的头
	dd MBT_HDR_MAGIC
	dd MBT_HDR_FLAGS
	dd -(MBT_HDR_MAGIC+MBT_HDR_FLAGS)
	dd mbt_hdr
	dd _start
	dd 0
	dd 0
	dd _entry
	;
	; multiboot header
	;
ALIGN 8
mbhdr:
								;GRUB2所需要的头
	DD	0xE85250D6
	DD	0
	DD	mhdrend - mbhdr
	DD	-(0xE85250D6 + 0 + (mhdrend - mbhdr))
	DW	2, 0
	DD	24
	DD	mbhdr
	DD	_start
	DD	0
	DD	0
	DW	3, 0
	DD	12
	DD	_entry 
	DD      0  
	DW	0, 0
	DD	8
mhdrend:

;上面两个包含两个头是为了同时兼容GRUB、GRUB2

_entry:
	cli							;关中断

	in al, 0x70     			;关闭不可屏蔽中断
	or al, 0x80	
	out 0x70,al

	lgdt [GDT_PTR]				;重新加载GDT
	jmp dword 0x8 :_32bits_mode

_32bits_mode:					;初始化C语言可能会用到的寄存器
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	xor eax,eax
	xor ebx,ebx
	xor ecx,ecx
	xor edx,edx
	xor edi,edi
	xor esi,esi
	xor ebp,ebp
	xor esp,esp
	mov esp,0x7c00				;为什么要设置成0x7c00？
	call inithead_entry			;跳到这个函数把两个bin文件复制到指定内存中
	jmp 0x200000				;这个地址是initldrkrl.bin文件的地址



GDT_START:						;CPU 工作模式设置
knull_dsc: dq 0
kcode_dsc: dq 0x00cf9e000000ffff
kdata_dsc: dq 0x00cf92000000ffff
k16cd_dsc: dq 0x00009e000000ffff
k16da_dsc: dq 0x000092000000ffff
GDT_END:
GDT_PTR:
GDTLEN	dw GDT_END-GDT_START-1	;GDT界限
GDTBASE	dd GDT_START
