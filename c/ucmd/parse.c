#include <stdio.h>
#include <unistd.h>
#include "ualloc.c"

int usplit(char *src, char **out)
{
	char *pbuf = ualloc(512);
	char *phead = src;
	char *ptail = src;
	int split_cnt = 0;
	char prechar = *src;
	char curchar = *src;
	while(*src)
	{
		curchar = *src;
		if (prechar == ' ' && curchar != ' ')
		{
			//find head
			phead = src;
			ptail = src;
			prechar = curchar;
			if (*(++src) == 0)
			{
				ptail = src;
				goto process_split_one;
			}
			continue;
		} else if (prechar != ' ' && curchar == ' ') {
			// find tail
			ptail = src;
			prechar = curchar;
			src++;
			goto process_split_one;
		} else if (prechar != ' ' && curchar != ' ') {
			// no head or tail found
			ptail = src;
			prechar = curchar;
			if (*(++src) == 0)
			{
				ptail = src;
				goto process_split_one;
			}
			continue;
		} else {
			src ++;
			continue;
		}

process_split_one:
		*out++ = pbuf;
		while(phead < ptail)
		{
			*(pbuf++) = *(phead++);
		}
		*(pbuf++) = 0;
		split_cnt++;
	}
	return split_cnt;
}

int ustrcmp(char* p1, char* p2)
{
	while(*p1 && *p2 && *p1 == *p2)
	{
		p1++;
		p2++;
	}
	return (*p1==0 && *p2==0);
}

// 0:10 1:16 -1:not null
unsigned long str2n(char *strn)
{
	int len = 0;
	unsigned long retn = 0;
	char *strn_bak = strn;
	while(*strn_bak)
	{
		if (!((*strn_bak >='0' && *strn_bak <='9') || (*strn_bak == 'x') || (*strn_bak >= 'a' && *strn_bak <='f') ||
					(*strn_bak >='A' && *strn_bak <= 'F')))
			return -1;
		len++;
		strn_bak++;
	}
	// base 16
	if (len>2 && strn[0]=='0' && strn[1]=='x')
	{
		int i = 0;
		for (i=2; i<len; i++)
		{
			if(strn[i] >= '0' && strn[i] <= '9')
			{
				retn *= 16;
				retn += (strn[i] - '0');
			} else if(strn[i] >= 'a' && strn[i] <= 'f') {
				retn *= 16;
				retn += (strn[i] - 'a' + 10);
			} else if(strn[i] >= 'A' && strn[i] <= 'F') {
				retn *= 16;
				retn += (strn[i] - 'A' + 10);
			}
		}
		return retn;
	} else {
		// base 10
		int i = 0;
		for (i=0; i< len; i++)
		{
			if (!(strn[i] >= '0' && strn[i] <= '9'))
				return retn;

			retn *=10;
			retn += (strn[i] - '0');
		}
		return retn;
	}
	return retn;
}

int exec_command(char *cmd)
{
	char *split_cmds[20];
	int ret = usplit(cmd, split_cmds);
	if (ret <= 0)
		return -1;
	if (ustrcmp(split_cmds[0], "mm4") || ustrcmp(split_cmds[0], "mm"))
	{
		if (ret == 2)
		{
			unsigned long waddr = str2n(split_cmds[1]);
			if (waddr == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			printf("read: 0x%lx ==> ", waddr);
			printf("0x%x\n", *(volatile unsigned int *)(waddr));
			ufree(split_cmds[0]);
			return 0;
		} else if (ret == 3) {
			unsigned long waddr = str2n(split_cmds[1]);
			if (waddr == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			unsigned long wdata = str2n(split_cmds[2]);
			if (wdata == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			printf("write: 0x%lx <== 0x%lx\n", waddr, wdata);
			*(volatile unsigned int*)(waddr) = wdata;
			ufree(split_cmds[0]);
			return 0;
		}
	} else if (ustrcmp(split_cmds[0], "mm1")){
		if (ret == 2)
		{
			unsigned long waddr = str2n(split_cmds[1]);
			if (waddr == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			printf("read: 0x%lx ==> ", waddr);
			printf("0x%x\n", *(volatile unsigned char *)(waddr));
			ufree(split_cmds[0]);
			return 0;
		} else if (ret == 3) {
			unsigned long waddr = str2n(split_cmds[1]);
			if (waddr == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			unsigned long wdata = str2n(split_cmds[2]);
			if (wdata == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			printf("write: 0x%lx <== 0x%lx\n", waddr, wdata);
			*(volatile unsigned char*)(waddr) = wdata;
			ufree(split_cmds[0]);
			return 0;
		}
	} else if (ustrcmp(split_cmds[0], "mm2")){
		if (ret == 2)
		{
			unsigned long waddr = str2n(split_cmds[1]);
			if (waddr == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			printf("read: 0x%lx ==> ", waddr);
			printf("0x%x\n", *(volatile unsigned short *)(waddr));
			ufree(split_cmds[0]);
			return 0;
		} else if (ret == 3) {
			unsigned long waddr = str2n(split_cmds[1]);
			if (waddr == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			unsigned long wdata = str2n(split_cmds[2]);
			if (wdata == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			printf("write: 0x%lx <== 0x%lx\n", waddr, wdata);
			*(volatile unsigned short*)(waddr) = wdata;
			ufree(split_cmds[0]);
			return 0;
		}
	} else if (ustrcmp(split_cmds[0], "mm8")){
		if (ret == 2)
		{
			unsigned long waddr = str2n(split_cmds[1]);
			if (waddr == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			printf("read: 0x%lx ==> ", waddr);
			printf("0x%lx\n", *(volatile unsigned long *)(waddr));
			ufree(split_cmds[0]);
			return 0;
		} else if (ret == 3) {
			unsigned long waddr = str2n(split_cmds[1]);
			if (waddr == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			unsigned long wdata = str2n(split_cmds[2]);
			if (wdata == -1)
			{
				printf("param error:[%s]\n", split_cmds[1]);
				return -1;
			}
			printf("write: 0x%lx <== 0x%lx\n", waddr, wdata);
			*(volatile unsigned long*)(waddr) = wdata;
			ufree(split_cmds[0]);
			return 0;
		}
	} else if (ustrcmp(split_cmds[0], "exit")){
		printf("exit command!\n");
		return -2;
	} else {
		printf("command illegal, format: mm[1/2/4/8] xxx [yyy]\n");
	}
	ufree(split_cmds[0]);
	return -1;
}

int wait_uart_exec_command()
{
	char cmd_buf[200] = {0};
	u8 cur_ch = 0;
	int cnt = 0;
	while(1)
	{
		int tmp_read_cnt = read(0, cmd_buf+cnt, 1);
		cnt += tmp_read_cnt;
		if (tmp_read_cnt == 0 && cmd_buf[cnt-1] == '\n')
		{
			cmd_buf[cnt-1] = 0;
			cnt = 0;
			printf("[%s]\n", cmd_buf);
			if (-2 == exec_command(cmd_buf))
			{
				break;
			}
		}
	}
	return 0;
}

int main(int argc, char**argv)
{
	printf("%d\n", exec_command("mm 0"));
	return 1;
	char test_buf[100] = "abcdefghiklmn";
	printf("0x%lx, %s\n", (unsigned long)test_buf, test_buf);
	wait_uart_exec_command();
	return 1;
}


//int main(void)
//{
//	char cmd_buf[100] = {0};
//	char testbuf[100] = "abcdefghiklmn";
//	char *rcmd = "mm 0x%lx";
//	sprintf(cmd_buf, rcmd, testbuf);
//	printf("%s\n", cmd_buf);
//	exec_command(cmd_buf);
//
//	char *wcmd = "mm 0x%lx %d";
//	sprintf(cmd_buf, wcmd, testbuf, 12345);
//	printf("%s\n", cmd_buf);
//	exec_command(cmd_buf);
//	printf("%s\n", testbuf);
//
//
//	wcmd = "mm1 0x%lx %d";
//	sprintf(cmd_buf, wcmd, testbuf, 'E');
//	printf("%s\n", cmd_buf);
//	exec_command(cmd_buf);
//	printf("%s\n", testbuf);
//
//
//	wcmd = "mm2 0x%lx 0x%lx";
//	sprintf(cmd_buf, wcmd, testbuf, 0x4647);
//	printf("%s\n", cmd_buf);
//	exec_command(cmd_buf);
//	printf("%s\n", testbuf);
//
//	wcmd = "mm8 0x%lx %ld";
//	sprintf(cmd_buf, wcmd, testbuf, 0x48494A4B4C4D4E4F);
//	printf("%s\n", cmd_buf);
//	exec_command(cmd_buf);
//	printf("%s\n", testbuf);
//
//	return 0;
//}

