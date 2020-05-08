#include "api.h"
#include <unistd.h>
#include "stdio.h"
#include "memmap.h"
void main(int argc, char **argv)
{
	printf("main\n");
	if (0 > init_gpio()){
		printf("init_gpio error\n");
	}
	int led_id=0;
	int on_off=0;
	if (argc != 3) { 
		printf("\n \
			run : ./test_input led_id on_off \n \ 
			demo: ./test_input 1 1\n");
		return;
	}

	sscanf(argv[1],"%d",&led_id);
	sscanf(argv[2],"%d",&on_off);
	printf("led_id=%d,on_off=%d\n",led_id,on_off);
	set_led_by_id(led_id,on_off);
	gpio_destruct();
}
