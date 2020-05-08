#ifndef _API_H_
#define _API_H_ 
#include <stdint.h>
#include "gpio_config.h"
#include "utils.h"


extern int init_gpio();
extern int gpio_destruct();
extern int execmd(char *cmd,char *result);

//usage: 
//number : bits[2:0]->[led2-led0]
extern void set_led_by_bits(unsigned int bit_num, int on_off);

//led_id:1,2,3; on_off: on:1,off:0
extern void set_led_by_id(unsigned int led_id, int on_off);

//usage:
//ret: 0:not pressed, 1:pressed
extern int get_recovery_btn();

#endif 
