#include "gpio_config.h"
#include "memmap.h"
#include "stdio.h"

//[3:0]: 0:SHUB_GPIO,1:SHUB_PWM,2:SHUB_SPI2

//GPIO_DIR[7:0]:0:input,1:output
int init_gpio()
{
	//1. init led: pin function choice : gpio 
	if (0 > memmap(GPIO_CTL_REG_3_(0), 3*4)) {
		return -1;
	}

	uint32_t* p_origin = global_map_info.map_addr;
	for (int i=0;i<3;i++) {
		//gpio iocfg_reg
		uint32_t new_data = ((*p_origin) | 0x5f0) & 0xfffffff0;
		*p_origin = new_data;
		p_origin += 1;
	}
	memunmap();

	//2. gpio direct config : output 
	if (0 > memmap(GPIO_REG_HUB(3)+GPIO_REG_DIR_OFFSET,1*4)) {
		return -1;
	}
	p_origin = global_map_info.map_addr;
	uint32_t new_data = (*p_origin) | 0x00000007;
	*p_origin = new_data;
	memunmap();

	//3. init key: pin function choice: gpio
	if (0 > memmap(GPIO_CTL_REG_KEY, 1*4)) {
		return -1;
	}
	p_origin = global_map_info.map_addr;
	//gpio iocfg_reg
	new_data = ((*p_origin) | 0x5f0) & 0xfffffff0;
	*p_origin = new_data;
	p_origin += 1;
	memunmap();

	//4. gpio direct config: input
	if (0 > memmap(GPIO_REG(16) + GPIO_REG_DIR_OFFSET,1*4)) {
		return -1;
	}
	p_origin = global_map_info.map_addr;
	new_data = (*p_origin) & 0xfffffff8;
	*p_origin = new_data;
	memunmap();
	return 0;
}

