#include "memmap.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "stdio.h"
#include <errno.h>

static int fd = -1;
static const char dev[]="/dev/mem";

struct map_info global_map_info[10];

/* no need considering page_size of 4K */
int memmap(uint32_t phy_addr, int size, struct map_info *p_map_info)
{
	if (!p_map_info)
		return -4;
	p_map_info->phy_addr = (uint32_t*)phy_addr;
	p_map_info->phy_len = size;

	uint32_t phy_addr_in_page;
	uint32_t phy_addr_in_page_off;

	uint32_t size_in_page;

	if(size <= 0)
	{
		return -3;
	}

	/* dev not opened yet, so open it */
	fd = open (dev, O_RDWR | O_SYNC);
	extern int errno;
	if (fd < 0)
		goto openfail;

	/* addr align in page_size(4K) */
	phy_addr_in_page = phy_addr & PAGE_SIZE_MASK;
	phy_addr_in_page_off = phy_addr - phy_addr_in_page;

	/* size in page_size */
	void *addr=0;
	size_in_page = (1 + (size + phy_addr_in_page_off)/PAGE_SIZE) * PAGE_SIZE;
	uint32_t s = (size + phy_addr_in_page_off)/PAGE_SIZE;
	uint32_t l = (size + phy_addr_in_page_off)%PAGE_SIZE;
	size_in_page = l > 0 ? (s+1)*PAGE_SIZE : s*PAGE_SIZE;

	addr = mmap((void *)0, size_in_page, PROT_READ|PROT_WRITE, MAP_SHARED, fd, phy_addr_in_page);
	if (addr == MAP_FAILED)
		goto mmapfail;

	p_map_info->fd = fd;
	p_map_info->phy_addr_to_page_addr = (uint32_t*)phy_addr_in_page;
	p_map_info->phy_addr_to_page_addr_offset = phy_addr_in_page_off;
	p_map_info->phy_len_in_page = size_in_page;

	p_map_info->map_addr = (uint32_t*)((char*)addr + phy_addr_in_page_off);
	p_map_info->map_len = size_in_page - phy_addr_in_page_off;
	return 0;

openfail:
	printf("dev open error");
	printf("errno=%d\n",errno);
	return -1;

mmapfail:
	close(fd);
	if (p_map_info->fd > 0){
		p_map_info->fd = -1;
	}
	printf("errno=%d\n",errno);
	return -2;
}


int memunmap(struct map_info *p_map_info)
{
	if (!p_map_info)
		return -2;
	if (p_map_info->fd > 0){
		close(p_map_info->fd);
		p_map_info->fd = -1;
	}
	if (0 != munmap((void*)p_map_info->phy_addr_to_page_addr,p_map_info->phy_len_in_page)) {
		return -1;
	}
	return 0;
}

