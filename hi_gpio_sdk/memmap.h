#ifndef _MEMMAP_H_
#define _MEMMAP_H_
#include <stdint.h>
#define REG_LEN 10
#define PAGE_SIZE 0x1000
#define PAGE_SIZE_MASK 0xfffffffffffff000

struct map_info {
	int fd;
	volatile uint32_t* phy_addr; 
	int phy_len; 
	
	volatile uint32_t* phy_addr_to_page_addr; 
	int phy_addr_to_page_addr_offset; 
	int phy_len_in_page; 

	volatile uint32_t* map_addr; 
	int map_len; 
};

extern struct map_info global_map_info[10];

extern int memmap(uint32_t phy_addr, int size, struct map_info *p_map_info);

extern int memunmap(struct map_info *p_map_info);

#endif
