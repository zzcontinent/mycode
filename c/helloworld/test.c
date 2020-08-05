#include <stdio.h>
#include <stdlib.h>

int uzp1(u_int16_t* src, int size, u_int8_t *dst1, u_int8_t *dst2){
    if(size < 0 || !src || !dst1 || !dst2){
        return -1;
    }
    u_int8_t res1;
    u_int8_t res2;
    u_int16_t ruler;

    for (int i = 0; i < size; ++i) {
        res1 = 0;
        res2 = 0;
        ruler = 0x8000;
        for (int j = 8; j > 0 ; --j) {
            res1 = res1 | ((src[i] & ruler)>>j);
            ruler = ruler >> 1;
            res2 = res2 | ((src[i] & ruler)>>(j-1));
            ruler = ruler >> 1;
        }
        dst1[i] = res1;
        dst2[i] = res2;
    }
    return 0;
}

void main()
{
	u_int16_t src[1] = {0xFFFF};
	u_int8_t dst1 = 0;
	u_int8_t dst2 = 0;
	uzp1(src,1, &dst1, &dst2);
}
