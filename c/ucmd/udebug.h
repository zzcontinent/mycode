/* ********************************************
 * FILE NAME  : udebug.h
 * PROGRAMMER : zhaozz
 * START DATE : 2021-09-01 17:39:59
 * DESCIPTION : standalone udebug
 * *******************************************/

#ifndef  __UDEBUG_H__
#define  __UDEBUG_H__


typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long u64;

// ============================================================wait if
#define wait_if(expr, cnt, desc) ({\
		udebug("wait max=%d, desc=%s", cnt, desc); \
		int _i = 0; \
		for (;_i < cnt; _i++) { if (!(expr)) break; if (0 == _i%100) uclean(".");} \
		uclean("==> ret=%d\n", _i); \
		_i;\
		})
// ============================================================wait if

// ============================================================log
#define LEVEL_DEBUG   0
#define LEVEL_INFO    1
#define LEVEL_ERROR   2
#define LEVEL_SIMPLE  3
#define ULOG_LEVEL    LEVEL_DEBUG

#if ULOG_LEVEL ==  LEVEL_DEBUG
#define udebug(fmt, args...) uart_printf("[DEBUG][%s:%d][%s] "fmt, __FILE__,__LINE__,__FUNCTION__,##args)
#define uinfo(fmt, args...) uart_printf("[INFO][%s:%d][%s] "fmt, __FILE__,__LINE__,__FUNCTION__,##args)
#define uerror(fmt, args...) uart_printf("[ERROR][%s:%d][%s] "fmt, __FILE__,__LINE__,__FUNCTION__,##args)
#define uclean uart_printf
#define ulog uinfo
#elif ULOG_LEVEL == LEVEL_INFO
#define udebug(fmt, args...) do {;} while(0)
#define uinfo(fmt, args...) uart_printf("[INFO][%s:%d][%s] "fmt, __FILE__,__LINE__,__FUNCTION__,##args)
#define uerror(fmt, args...) uart_printf("[ERROR][%s:%d][%s] "fmt, __FILE__,__LINE__,__FUNCTION__,##args)
#define uclean uart_printf
#define ulog uinfo
#elif ULOG_LEVEL == LEVEL_ERROR
#define udebug(fmt, args...) do {;} while(0)
#define uinfo(fmt, args...) do {;} while(0)
#define uerror(fmt, args...) uart_printf("[ERROR][%s:%d][%s] "fmt, __FILE__,__LINE__,__FUNCTION__,##args)
#define uclean uart_printf
#define ulog uinfo
#elif ULOG_LEVEL == LEVEL_SIMPLE
#define udebug(fmt, args...) uart_printf("[DEBUG][%d] "fmt, __LINE__, ##args)
#define uinfo(fmt, args...)  uart_printf("[INFO][%d] "fmt, __LINE__, ##args)
#define uerror(fmt, args...) uart_printf("[ERROR][%d] "fmt, __LINE__, ##args)
#define uclean uart_printf
#define ulog uinfo
#else
#define udebug uart_printf
#define uinfo uart_printf
#define uerror uart_printf
#define uclean uart_printf
#define ulog uart_printf
#endif
// ============================================================log


// ============================================================read write Transfer
#define _P2V(addr)	(0x9000000000000000 | ((u64)(addr)))	/* physical address to virtual address */

#define READ8_REGISTER(x, y) do {			\
	y= *((volatile uchar *)(_P2V(x))) ;	\
} while(0)
#define READ_REGISTER(x, y) do {			\
	y = *((volatile u32*)(_P2V(x)));	\
} while(0)
#define READ64_REGISTER(x, y) do {			\
	y = *((volatile u64 *)(_P2V(x)));	\
} while(0)
#define WRITE8_REGISTER(x, y) do {			\
	*((volatile u8 *)(_P2V(x)))= (u8)y ;	\
} while(0)
#define WRITE_REGISTER(x, y) do {			\
	*((volatile u32 *)(_P2V(x))) = y;	\
} while(0)
#define WRITE64_REGISTER(x, y) do {			\
	*((volatile u64 *)(_P2V(x))) = y;	\
} while(0)

#define phx_read_u8(where)						\
	({                                                              \
	 u8 val;                                        \
	 READ8_REGISTER(where, val);				\
	 val;                                                    \
	 })
#define phx_read_u32(where)						\
	({                                                              \
	 volatile u32 val;                                       \
	 READ_REGISTER(where, val);				\
	 val;                                                    \
	 })
#define phx_read_u64(where)						\
	({                                                              \
	 volatile u64 val;                                       \
	 READ64_REGISTER(where, val);				\
	 val;                                                    \
	 })

#define phx_write_u8(where, val)   WRITE8_REGISTER(where, val)
#define phx_write_u32(where, val)  WRITE_REGISTER(where, val)
#define phx_write_u64(where, val)  WRITE64_REGISTER(where, val)
// ============================================================read write Transfer


// ============================================================printf uart
#define uart_printf_wait uart_printf
typedef __builtin_va_list __gnuc_va_list;
#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
typedef __gnuc_va_list va_list;

static const char cmap[16] = "0123456789abcdef";

#define UART_BASE_ADDR		0x1f080000 //0x1f0c8000
static void inline uart_putc_wait(u32 data)
{
	while ((phx_read_u32(UART_BASE_ADDR + 0x2C) & 0x10)); //until NOT FULL
	phx_write_u8(UART_BASE_ADDR + 0x30, data);
}

static void inline printn(unsigned n, unsigned bas)
{
	unsigned r;
	if ((r = n / bas) != 0)
		printn(r, bas);
	uart_putc_wait(cmap[n % bas]);
}

void inline uart_printf(const char *fmt, ...)
{
	char c, *s;
	unsigned long n;

	va_list args;
	unsigned long t;

	va_start(args, fmt);

	while ((c = *fmt++) != '\0') {
		if (c != '%') {
			uart_putc_wait(c);
			continue;
		}

		switch (c = *fmt++) {
			case 'o':
				t = va_arg(args, int);
				printn(t, 8);
				break;

			case 'd':
			case 'i':
			case 'l':
				t = va_arg(args, int);
				printn(t, 10);
				break;
				//        case 'h':
			case 'x':
				n = va_arg(args, unsigned long);
				// 64-bit
				if(n>>60)
					uart_putc_wait(cmap[(n >> 60) & 0x0f]);
				if(n>>56)
					uart_putc_wait(cmap[(n >> 56) & 0x0f]);
				if(n>>52)
					uart_putc_wait(cmap[(n >> 52) & 0x0f]);
				if(n>>48)
					uart_putc_wait(cmap[(n >> 48) & 0x0f]);
				if(n>>44)
					uart_putc_wait(cmap[(n >> 44) & 0x0f]);
				if(n>>40)
					uart_putc_wait(cmap[(n >> 40) & 0x0f]);
				if(n>>36)
					uart_putc_wait(cmap[(n >> 36) & 0x0f]);
				if(n>>32)
					uart_putc_wait(cmap[(n >> 32) & 0x0f]);

				//32-bit

				if(n>>28)
					uart_putc_wait(cmap[(n >> 28) & 0x0f]);
				if(n>>24)
					uart_putc_wait(cmap[(n >> 24) & 0x0f]);
				if(n>>20)
					uart_putc_wait(cmap[(n >> 20) & 0x0f]);
				if(n>>16)
					uart_putc_wait(cmap[(n >> 16) & 0x0f]);
				if(n>>12)
					uart_putc_wait(cmap[(n >> 12) & 0x0f]);
				if(n>>8)
					uart_putc_wait(cmap[(n >> 8) & 0x0f]);
				if(n>>4)
					uart_putc_wait(cmap[(n >> 4) & 0x0f]);
				uart_putc_wait(cmap[n & 0x0f]);
				break;

			case 's':
				s = va_arg(args, char *);
				for (; *s != '\0'; ++s)
				{
					uart_putc_wait(*s);
				}
				break;

			default:
				c = va_arg(args, int);
				uart_putc_wait(c);
				continue;
		}
	}
	va_end(args);
}
// ============================================================printf uart

#endif  /* __UDEBUG_H__ */

