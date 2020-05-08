#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h> 
#include <signal.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
void term_handler();

static long fsize = 0;
static long sent_size= 0;
unsigned long get_file_size(const char *path);
static int BUF_SIZE = 1034;
static char CANCEL_CMD[10] = "CANL#__#";
static char DATA_CMD[10] = "DATA#__#";
static int CMD_LEN = 10;
static int sockfd = -1, portno, n;

static int is_cancel = 0;
static int main_pid = 9999;

void add_header_CANCEL(char *buf)
{
	for (int i=0;i<10;i++)
		buf[i] = CANCEL_CMD[i];
}

void add_header_DATA(char *buf)
{
	for (int i=0;i<10;i++)
		buf[i] = DATA_CMD[i];
}

int main(int argc, char *argv[])
{
	main_pid = getpid();
	setvbuf(stdout,NULL,_IONBF,0);

	sigset( SIGTERM, term_handler );
	sigset( SIGINT, term_handler );

	struct sockaddr_in serv_addr;
	struct hostent *server;
    	char buffer[BUF_SIZE];
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");
	printf("Please enter file path:");
	bzero(buffer,BUF_SIZE);
	fgets(buffer,BUF_SIZE,stdin);
	for(int i=0; i<strlen(buffer); i++){
		if(buffer[i] == '\n'){
			buffer[i] = 0;
			break;
		}
	}
	int fd = open(buffer, O_RDONLY);	
	fsize = get_file_size((char*)buffer);
	if(fd == -1 || fsize == -1) {
         	error("ERROR file open or file stat wrong\n");
		return -1;
	}

    	bzero(buffer,BUF_SIZE);
	int read_n = read(fd,buffer+CMD_LEN,BUF_SIZE-CMD_LEN);	
	if (-1 == read_n){
         	error("ERROR file read\n");
		return -1;
	}
	add_header_DATA(buffer);
	while(read_n){
		// write buffer to socket
    		n = write(sockfd,buffer,read_n+CMD_LEN);
		if (n < 0) 
			error("ERROR writing to socket");
		// calc sending process
		sent_size += n;
		sent_size -= CMD_LEN;
		printf("\rSend :%dKB/%dKB-%2d%%",sent_size/1024,fsize/1024,sent_size*100/(fsize > 0 ? fsize:1));

		// continue read file to send
    		bzero(buffer,BUF_SIZE);
		read_n = read(fd,buffer+CMD_LEN,BUF_SIZE-CMD_LEN);	
		if (-1 == read_n){
       		  	error("ERROR file read\n");
			return -1;
		}
		// add header to control trans process
		add_header_DATA(buffer);
	}
	close(sockfd);
	return 0;
}

unsigned long get_file_size(const char *path)
{
	unsigned long filesize = -1;	
	struct stat statbuff;
	if(stat(path, &statbuff) < 0){
		return filesize;
	}else{
		filesize = statbuff.st_size;
	}
	return filesize;
}


void term_handler()
{
	printf("Are you sure to cancel ? [y/n]");
	char buffer[10];
	bzero(buffer,10);
	fgets(buffer,10,stdin);
	if('y'== buffer[0]){
		if(sockfd < 0 )
			return;
    		write(sockfd,CANCEL_CMD,sizeof(CANCEL_CMD));
	}
	kill(main_pid,SIGKILL);
}
