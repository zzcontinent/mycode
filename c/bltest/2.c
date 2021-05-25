#include <stdio.h>
 
static union { char c[4]; unsigned long l; } endian_test = { { 'l', '?', '?', 'b' } };
#define ENDIANNESS ((char)endian_test.l)
 
void f1(void)
{
    printf("ENDIAN: %c\n" , ENDIANNESS);
}
 
 
int main()
{
    f1();
}
