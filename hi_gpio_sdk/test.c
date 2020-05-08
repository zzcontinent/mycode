#include "api.h"
#include <unistd.h>
#include "stdio.h"
#include "memmap.h"
#if 0
void main()
{
	printf("main\n");
	if (0 > init_gpio()){
		printf("init_gpio error\n");
	}
	//led api test
	/*
	printf("set_led_by_bits(1,1)\n");
	set_led_by_bits(1,1);
	printf("set_led_by_bits(2,1)\n");
	set_led_by_bits(2,1);
	printf("set_led_by_bits(4,1)\n");
	set_led_by_bits(4,1);
	sleep(1);

	printf("set_led_by_bits(1,0)\n");
	set_led_by_bits(1,0);
	sleep(1);
	printf("set_led_by_bits(2,0)\n");
	set_led_by_bits(2,0);
	sleep(1);
	printf("set_led_by_bits(4,0)\n");
	set_led_by_bits(4,0);
	sleep(1);
	printf("set_led_by_bits(7,1)\n");
	set_led_by_bits(7,1);
	sleep(1);
	printf("set_led_by_bits(7,0)\n");
	set_led_by_bits(7,0);
	sleep(1);
	*/
	while (1){
		for(int i=0;i<3;i++){
			printf("set_led_by_id(%d%3+1,1)\n",i);
			set_led_by_id(i%3+1,1);
			sleep(1);
		}
		for(int i=0;i<3;i++){
			printf("set_led_by_id(%d%3+1,0)\n",i);
			set_led_by_id(i%3+1,0);
			sleep(1);
		}

		// button read test
		/*
		int i = 0;
		for(;;){
			if (get_recovery_btn()==1){
				i++;
				printf("btn_pressed:%d\n",i);
			}else
				i=0;
			usleep(10);
		}
		*/
	}
}
#endif
