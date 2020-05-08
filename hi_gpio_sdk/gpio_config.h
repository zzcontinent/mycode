#ifndef _GPIO_CONFIG_H
#define _GPIO_CONFIG_H

#include <stdint.h>
//pin control reg
#define GPIO_CTL_REG_3_(n) (0x18050060 + (n<<2)) //led0-3
#define GPIO_CTL_REG_KEY    0x1F001040 		 //reset - gpio-16.7


//pin reg
#define GPIO_REG(n) (0x12140000 + (n<<12)) //gpio [0-16]
#define GPIO_REG_HUB(n) (0x180D0000 + (n<<12)) //gpio shub [0-4]

//GPIO_DATA
#define GPIO_REG_DATA_OFFSET 0x3FC
//GPIO_DIR
#define GPIO_REG_DIR_OFFSET  0x400


extern int init_gpio();


#endif
