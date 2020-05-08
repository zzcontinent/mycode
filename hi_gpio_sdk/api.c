#include "api.h"
#include "memmap.h"
#include "gpio_config.h"
#include "stdio.h"

//usage:
//number : bits[2:0]->[led2-led0]
void set_led_by_bits(unsigned int bit_num, int on_off)
{
	volatile uint32_t *p_origin = global_map_info[0].map_addr;

	if (on_off == 1) {
		uint32_t new_data = (*p_origin) | (bit_num & 0x00000007);
		*p_origin = new_data;
	} else if (on_off == 0) {
		uint32_t new_data = (*p_origin) & (~(bit_num & 0x00000007));
		*p_origin = new_data;
	}
}

//led_id:1,2,3; on_off: on:1,off:0
void set_led_by_id(unsigned int led_id, int on_off)
{
	unsigned int bit_num = 0;

	bit_num = 1<<(led_id-1);
	set_led_by_bits(bit_num, on_off);
}


//usage:
//ret: 0:not pressed, 1:pressed, -1:error
int get_recovery_btn(void)
{
	volatile uint32_t *p_origin = global_map_info[1].map_addr;
	int out = *p_origin;

	return !(out&0x80);
}

