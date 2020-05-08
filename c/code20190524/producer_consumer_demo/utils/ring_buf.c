#include "ring_buf.h"

static struct ring_buf *mem = NULL;

int create_buf(int n)
{
	//malloc struct ring_buf 
	mem = (struct ring_buf *)SDL_malloc(sizeof(struct ring_buf));
	if (!mem) {
	        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ring_buf alloc out of memory");
       		return 0;
    	}
	
	//malloc BUF_TYP
	mem->buf = (BUF_TYP*)SDL_malloc(n * sizeof(BUF_TYP));
	if (!mem->buf) {
	        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "BUF_TYP alloc out of memory");
       		return 0;
    	}

	//init rw_lock
    	if ((mem->rw_lock = SDL_CreateMutex()) == NULL) {
       		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create mutex: %s\n", SDL_GetError());
		return 0;
    	}
	mem->r_cond = SDL_CreateCond();
	mem->w_cond = SDL_CreateCond();
	mem->size = n; 
	mem->free_cnt = 10;
	mem->use_cnt = 0;
	mem->r_pos = 0;
	mem->w_pos = 0;
	return 1;
}

void release_buf()
{
	if(NULL == mem)
		return;
	SDL_free(mem->buf);
	SDL_DestroyMutex(mem->rw_lock);
	SDL_DestroyCond(mem->r_cond);
	SDL_DestroyCond(mem->w_cond);
	SDL_free(mem);
}

int _read_one(BUF_TYP *ret)
{
	if(mem->use_cnt > 0){
		*ret = mem->buf[mem->r_pos];
		mem->r_pos++;
		mem->r_pos %= mem->size;	
		mem->use_cnt--;
		mem->free_cnt++;
		return 1;
	}
	return 0; 
}

int read_non_block(BUF_TYP *rbuf, int n)
{
	if(SDL_LockMutex(mem->rw_lock) < 0) {
            	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock mutex: %s", SDL_GetError());
		exit(1);
	}

	int r_cnt = 0;
	while(n-- > 0) {
		if (1 == _read_one(&rbuf[r_cnt]))
			r_cnt ++;
		else
			break;
	}

        if (SDL_UnlockMutex(mem->rw_lock) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't unlock mutex: %s", SDL_GetError());
            exit(1);
        }

	return r_cnt;
}

int read_block(BUF_TYP *rbuf, int n)
{
	if(SDL_LockMutex(mem->rw_lock) < 0) {
            	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock mutex: %s", SDL_GetError());
		exit(1);
	}

	int r_cnt = 0;
	while(n > 0) {
		if (1 == _read_one(&rbuf[r_cnt])) {
			r_cnt++;
			n--;
			SDL_CondSignal(mem->w_cond);
		} else
			SDL_CondWait(mem->r_cond, mem->rw_lock);	
	}

        if (SDL_UnlockMutex(mem->rw_lock) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't unlock mutex: %s", SDL_GetError());
            exit(1);
        }

	return r_cnt;
}


int _write_one(BUF_TYP in)
{
	if(mem->free_cnt > 0){
		mem->buf[mem->w_pos] = in;
		mem->w_pos++;
		mem->w_pos %= mem->size;	
		mem->free_cnt--;
		mem->use_cnt++;
		return 1;
	}
	return 0; 
}


int write_non_block(BUF_TYP *wbuf, int n)
{
	if(SDL_LockMutex(mem->rw_lock) < 0) {
            	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock mutex: %s", SDL_GetError());
		exit(1);
	}

	int w_cnt = 0;
	while(n-- > 0) {
		if (1 == _write_one(wbuf[w_cnt]))
			w_cnt ++;
		else
			break;
	}

        if (SDL_UnlockMutex(mem->rw_lock) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't unlock mutex: %s", SDL_GetError());
            exit(1);
        }

	return w_cnt;
}

int write_block(BUF_TYP *wbuf, int n)
{
	if(SDL_LockMutex(mem->rw_lock) < 0) {
            	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock mutex: %s", SDL_GetError());
		exit(1);
	}

	int w_cnt = 0;
	while(n > 0) {
		if (1 == _write_one(wbuf[w_cnt])) {
			w_cnt ++;
			n --;
			SDL_CondSignal(mem->r_cond);
		} else 
			SDL_CondWait(mem->w_cond, mem->rw_lock);	
	}

        if (SDL_UnlockMutex(mem->rw_lock) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't unlock mutex: %s", SDL_GetError());
            exit(1);
        }

	return w_cnt;
}
