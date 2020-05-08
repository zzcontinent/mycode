#include "utils/ring_buf.h"
#include <signal.h>


extern int create_buf(int n);

extern void release_buf();

extern int read_block(BUF_TYP *rbuf, int n);

extern int write_block(BUF_TYP *wbuf, int n);

static SDL_threadID pid_producer;
static SDL_threadID pid_consumer;

int SDLCALL producer(void *data)
{
	pid_producer = SDL_ThreadID();
	BUF_TYP w_buf[] = {1};
	while(1) {
		if(write_block(w_buf, 1) > 0) {
    			SDL_Log("produce ---------- %d\n", w_buf[0]);
			w_buf[0]++;
		}
		else
    			SDL_Log("ring full -----\n");

		SDL_Delay(500);	
	}
}


int SDLCALL consumer(void *data)
{
	pid_consumer = SDL_ThreadID();
	BUF_TYP r_buf[1]; 
	while(1) {
		if(read_block(r_buf, 1) > 0)
    			SDL_Log("consume ***** %d\n", r_buf[0]);
		else
    			SDL_Log("ring empty ***** \n");

		SDL_Delay(1000);	
	}
}

void term_handler()
{
	printf("SIGTERM SIGINT CAPTURE\n");
	release_buf();
	printf("ring buffer releasing\n");
}


int main()
{
	create_buf(5);	
	sigset( SIGTERM, term_handler );
	sigset( SIGINT, term_handler );
	if(NULL == SDL_CreateThread(producer,"producer",NULL))
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create thread!\n");

	if(NULL == SDL_CreateThread(consumer,"consumer",NULL)) 
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create thread!\n");

	while(1)
		SDL_Delay(1000*60);	
}
