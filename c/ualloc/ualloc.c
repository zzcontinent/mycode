#ifndef __UALLOC_C__
#define __UALLOC_C__

#define u8 unsigned char
#define U_PAGE 32
#define U_PAGE_CNT 256
#define U_BUF_LEN U_PAGE_CNT*U_PAGE

static u8 ubuf[U_BUF_LEN] = {0};
static int ubuf_free_list[U_PAGE_CNT] = {0};

#define FREE_FLAG 0
#define PAD_FLAG -1

u8* ualloc(int len)
{
	int page_cnt = len%U_PAGE == 0 ? len/U_PAGE : len/U_PAGE+1;
	int i=0;
	for (i=0; i<U_PAGE_CNT; i+=2)
	{
		int j = 0;
		int found_cnt = 0;
		for (j=0; j<page_cnt;j++)
		{
			if(ubuf_free_list[i+j] == FREE_FLAG)
			{
				if (++found_cnt == page_cnt)
					break;
			}
		}
		if(found_cnt == page_cnt)
		{
			int k=0;
			for(k=0; k<page_cnt; k++)
			{
				ubuf_free_list[i+k] = PAD_FLAG;
			}
			ubuf_free_list[i] = len;
			return ubuf + i*U_PAGE;
		}
	}
	return 0;
}

void ufree(u8* addr)
{
	int pos = (addr - ubuf)/U_PAGE;
	int page_cnt = ubuf_free_list[pos];
	int j=0;
	for(j=0; j<page_cnt;j++)
	{
		ubuf_free_list[pos+j] = FREE_FLAG;
	}
}

void print_msg()
{
	int i = 0;
	for (i=0; i< U_PAGE_CNT; i++)
	{
		printf("%d:%d  ", i, ubuf_free_list[i]);
	}
	printf("\n");
}


#endif /*__UALLOC_C__*/
