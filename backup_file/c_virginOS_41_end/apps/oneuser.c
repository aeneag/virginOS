/************************************************************************************************
* Aen @2022.05.24
* 公众号：技术乱舞
* https://aeneag.xyz
* filename: 
* description:
************************************************************************************************/
#include "libtypes.h"
#include "libdev.h"
#include "stdio.h"

int Add(int a, int b);
int Sub(int a, int b);
int Mul(int a, int b);
int Div(int a, int b);
void menu();
void calc(int (*pfun) (int, int),hand_t *hand);
int cal_main(hand_t *hand);



typedef struct shellkbbuff
{
	unsigned long skb_tail;
	unsigned long skb_start;
	unsigned long skb_curr;
	char skb_buff[512];
}shellkbbuff_t;

static shellkbbuff_t shellkbuff;
static shellkbbuff_t calbuff;
static int gl_fl = 0;
/************************************************************************************************
* @2022/05/24
* Aen
* https://aeneag.xyz
* description: 打开驱动设备，即键盘
*     
*            
*  返回 null
**************************************************************************************************/
hand_t open_keyboard()
{
	devid_t dev;
	dev.dev_mtype = UART_DEVICE;
	dev.dev_stype = 0;
	dev.dev_nr = 0;

	hand_t fd = open(&dev, RW_FLG | FILE_TY_DEV, 0);
	if (fd == -1)
	{
		return -1;
	}
	return fd;
}

void close_keyboard(hand_t hand)
{
	close(hand);
	return;
}
/************************************************************************************************
* @2022/05/24
* Aen
* https://aeneag.xyz
* description: shell主函数
*     
*            
*  返回 null
**************************************************************************************************/
u16_t read_keyboard_code(sint_t hand)
{
	u16_t code_buf[8];
	sysstus_t rets = read(hand, (char_t*)code_buf, 8, 0);
	if(0 > rets)
	{
		printf(" read err:%x\n", rets);
	}
	if(0 == code_buf[0])
	{
		return code_buf[2];
	}
	return 0xffff;	
}

void skb_buff_write(shellkbbuff_t* skb, char_t code)
{
	if(skb->skb_tail >= 511)
	{
		skb->skb_tail = 0;
	}

	skb->skb_buff[skb->skb_tail] = code;
	skb->skb_tail++;
	skb->skb_buff[skb->skb_tail] = 0;
	return;
}

void skb_buff_disp(shellkbbuff_t* skb)
{
	if(skb->skb_curr >= 511)
	{
		skb->skb_curr = 0;
	}
	printf("%s", &skb->skb_buff[skb->skb_curr]);
	skb->skb_curr++;
	return;
}

void skb_buff_clear(shellkbbuff_t* skb)
{
	memset((void*)skb, 0, sizeof(shellkbbuff_t));
	return;
}
/************************************************************************************************
* @2022/05/24
* Aen
* https://aeneag.xyz
* description: shc_cmd_run 执行进程，在shell中
*     
*            
*  返回 null
**************************************************************************************************/
sint_t shc_cmd_run(char_t* cmdstr)
{
	if(NULL == cmdstr)
	{
		return -1;
	}
	if(exel(cmdstr, 0) != SYSSTUSERR)
	{
		return 0;
	}
	return -2;
}

sint_t shc_cmd_clear(char_t* cmdstr){
	if(cmdstr[0]== 'c' &&cmdstr[1]== 'l' &&cmdstr[2]== 'e' &&cmdstr[3]== 'a' &&cmdstr[4]== 'r'){
		printf("     clear code,we can’t get screen x,y");
		return 0;
	}
	return 1;
}
sint_t shc_cmd_cal(char_t* cmdstr,hand_t *hand){
	if(cmdstr[0]== 'c' &&cmdstr[1]== 'a' &&cmdstr[2]== 'l' ){
		cal_main(hand);
		return 0;
	}
	return 1;
}
void skb_buff_enter(shellkbbuff_t* skb,hand_t *hand)
{
	printf("\n");
	if(0 == shc_cmd_run(&skb->skb_buff[skb->skb_start]))
	{
		return;
	}
	if(0 == shc_cmd_clear(&skb->skb_buff[skb->skb_start]))
	{
		return;
	}
	if(0 == shc_cmd_cal(&skb->skb_buff[skb->skb_start],hand))
	{
		return;
	}
	printf("     https://aeneag.xyz@Aen:>");
	return;
}

void skb_buff_backspace(shellkbbuff_t* skb)
{
	if(1 > skb->skb_tail)
	{
		return;
	}
	if(0 < skb->skb_curr)
	{
		skb->skb_curr--;
	}
	skb->skb_tail--;
	skb->skb_buff[skb->skb_tail] = 0;
	printf("\n     https://aeneag.xyz@Aen:>");
	printf("%s", &skb->skb_buff[skb->skb_start]);
	return;	
}

/************************************************************************************************
* @2022/05/24
* Aen
* https://aeneag.xyz
* description: shell主函数
*     
*            
*  返回 null
**************************************************************************************************/
void shell(hand_t *hand)
{
	
	char_t charbuff[2] = {0,0};
	u16_t kbcode; 
	*hand = open_keyboard();
	memset((void*)&shellkbuff, 0, sizeof(shellkbbuff_t));
	printf("     virginOS shell is successfully initialized !\n");
	printf("     https://aeneag.xyz@Aen:>");
	for(;;)
	{
		kbcode = read_keyboard_code(*hand);
		if(32 <= kbcode && 127 >= kbcode)
		{
			charbuff[0] = (char_t)kbcode;
			skb_buff_write(&shellkbuff, charbuff[0]);
			skb_buff_disp(&shellkbuff);
			continue;
		}
		if(0x103 == kbcode)//enter
		{
			skb_buff_enter(&shellkbuff,hand);
			
			skb_buff_clear(&shellkbuff);
			continue;
		}
		if(0x104 == kbcode)//backspace
		{
			skb_buff_backspace(&shellkbuff);		
			continue;
		}
	}
	close_keyboard(*hand);
	return;
}

int main(void)
{
	hand_t hand = -1;
    shell(&hand);
    return 0;
}



int Add(int a, int b)
{
	return a + b;
}
int Sub(int a, int b)
{
	return a - b;
}
int Mul(int a, int b)
{
	return a * b;
}
int Div(int a, int b)
{
	return a / b;
}
 
void menu()
{
	printf("     ****************************\n");
	printf("     ***** 1.add      2.sub *****\n");
	printf("     ***** 3.mul      4.div *****\n");
	printf("     ******     0.exit      *****\n");
	printf("     ****************************\n");
}
int char_to_int(char* buf,uint_t len){
	int res = 0;
	int i = 1 ;
	int mm = 1;
	while(i <= (int)len){
		res += mm*((int)buf[len-i] - 48);
		mm *= 10;
		++i;
	}
	return res;
}
int is_ok_str(char* buf){
	while(*buf != 0 && *buf == ' ')++buf;//前置 空格
	if(*buf == 0)return -1;
	while(*buf != 0 && *buf != ' ')++buf;//第一个 数字
	if(*buf == 0)return -1;
	while(*buf != 0 && *buf == ' ')++buf;//中间 空格
	if(*buf == 0)return -1;
	if(*buf != 0 && *buf != ' ')return 1;//1 代表 条件符合
	return -1;

}
void out_res(int x,int y,int res){
	if(gl_fl == 1)
		printf("\n     %d + %d = %d\n", x,y,res);
	if(gl_fl == 2)
		printf("\n     %d - %d = %d\n", x,y,res);
	if(gl_fl == 3)
		printf("\n     %d * %d = %d\n", x,y,res);
	if(gl_fl == 4)
		printf("\n     %d / %d = %d\n", x,y,res);
}
void calc(int (*pfun) (int, int),hand_t *hand)
{
	int x = 0;
	int y = 0;
	
	u16_t kbcode; 
	char *calbuf;
	char cx[10];
	char cy[10];
	int i = 0;
reinput:
	printf("     输入两个数(空格隔开):");
	for(;;){
		kbcode = read_keyboard_code(*hand);
		if(32 <= kbcode && 127 >= kbcode)
		{
			skb_buff_write(&calbuff, (char_t)kbcode);
			skb_buff_disp(&calbuff);
			continue;
		}
		if(0x103 == kbcode)//enter
		{	
			calbuf = calbuff.skb_buff;
			int k = is_ok_str(calbuf);
			// printf("kkkk = %d",k);
			if(*calbuf == 0 || k != 1 ){
				skb_buff_clear(&calbuff);
				printf("\n");
				goto reinput;
			}
			while(*calbuf == ' ')++calbuf;
			while(*calbuf != ' '){
				cx[i++] = *calbuf;
				++calbuf;
			}
			cx[i] = 0;
			while(*calbuf == ' ')++calbuf;
			i = 0;
			while(*calbuf != 0){
				if(*calbuf == ' ')break;
				cy[i++] = *(calbuf++);
			}
			cy[i] = 0;
			x = char_to_int(cx,strlen(cx));
			y = char_to_int(cy,strlen(cy));
			out_res(x,y,pfun(x, y));
			skb_buff_clear(&calbuff);
			break;
		}
	}
}
int cal_main(hand_t *hand)
{
	int input = 1;
	memset((void*)&calbuff, 0, sizeof(shellkbbuff_t));
	int(*p[5])(int, int) = { 0, Add, Sub, Mul, Div };
	int kbcode; 
	while (1)
	{
cal_st:
		menu();
		printf("     请选择：");
		for(;;){
			kbcode = read_keyboard_code(*hand);
			if(32 <= kbcode && 127 >= kbcode){
				input = kbcode - 48;
				gl_fl =  input;
				printf("%d\n", input);
				if(!input)break;
				if(input < 0 || input > 4){
cal_err:					
					printf("\n     input err,please restart input !\n");
					goto cal_st;
				}	
				
				if (input >= 1 && input <= 4)
				{
					calc(p[input],hand);          
					goto cal_st;
				}
			}
			if(0x103 == kbcode){
				goto cal_err;
			}
		}
		break;

	}
	printf("     exit!");
	return 0;

}
