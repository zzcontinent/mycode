#ifndef __RING_BUF_H__
#define __RING_BUF_H__

#include "stdio.h"
#include "SDL.h"

typedef int BUF_TYP;

struct ring_buf {
	int size;
	BUF_TYP *buf;
	int r_pos;
	int w_pos;
	int free_cnt;	
	SDL_mutex *rw_lock;		
	SDL_cond  *r_cond;
	SDL_cond  *w_cond;
	int use_cnt;
};

int create_buf(int n);

void release_buf();

int read_lock(BUF_TYP *rbuf, int n);

int write_lock(BUF_TYP *wbuf, int n);

#endif
