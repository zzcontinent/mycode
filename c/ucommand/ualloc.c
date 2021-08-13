#ifndef __UALLOC_C__
#define __UALLOC_C__

#define u8 unsigned char
#define U_PAGE_SIZE 128
#define U_PAGE_CNT 512
#define U_BUF_LEN U_PAGE_CNT*U_PAGE_SIZE

static u8 ubuf[U_BUF_LEN] = {0};
static int ubuf_free_list[U_PAGE_CNT] = {0};

#define FREE_FLAG 0
#define PAD_FLAG -1

void* ualloc(int len)
{
	int page_cnt = len%U_PAGE_SIZE == 0 ? len/U_PAGE_SIZE : len/U_PAGE_SIZE+1;
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
			return ubuf + i*U_PAGE_SIZE;
		}
	}
	return 0;
}

void ufree(void* addr)
{
	int pos = ((u8*)addr - ubuf)/U_PAGE_SIZE;
	int page_cnt = ubuf_free_list[pos]/U_PAGE_SIZE;
	int j=0;
	for(j=0; j<page_cnt;j++)
	{
		ubuf_free_list[pos+j] = FREE_FLAG;
	}
}

void uprint_msg()
{
	int i = 0;
	for (i=0; i< U_PAGE_CNT; i++)
	{
		printf("%d:%d  ", i, ubuf_free_list[i]);
	}
	printf("\n");
}


#endif /*__UALLOC_C__*/
