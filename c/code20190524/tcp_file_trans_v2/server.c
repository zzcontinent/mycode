/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

static int BUF_SIZE = 1034;
static char CANCEL_CMD[10] = "CANL#__#";
static char DATA_CMD[10] = "DATA#__#";
static int CMD_LEN = 10;

void dostuff(int); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
	// gen file name and create or open file
	char filename[256];
	time_t intps;
	struct tm* tmi;
	bzero(filename,256);
	intps = time(NULL);
	tmi = localtime(&intps);
	sprintf(filename,"%04d_%02d_%02d_%02d_%02d_%02d",1900+tmi->tm_year,tmi->tm_mon+1,tmi->tm_mday,tmi->tm_hour,tmi->tm_min,tmi->tm_sec);

	int fd;
	if ((fd=open(filename,O_CREAT|O_WRONLY,0600))==-1)
	{
		error("open fail");
	}
	// ready to wirte file 
	int n = 1;
	char buffer[BUF_SIZE];
   	while (n > 0){
		bzero(buffer,BUF_SIZE);
		n = read(sock,buffer,BUF_SIZE);
		if (n < 0) error("ERROR reading from socket");
		// 0: data, 1:cancel, -1:other type
		int cmd_typ = parse_cmd_typ(buffer);
		if (1 == cmd_typ){
			remove(filename);
		}else if (0 == cmd_typ){
			if(write(fd,buffer+CMD_LEN,n-CMD_LEN)==-1)
				error("write fail");
		}else if (2 == cmd_typ){
			
		}	
	}
}

// 0: data, 1:cancel, -1:other type
int parse_cmd_typ(char *buf)
{
	int is_cancel = 1;
	// check cancel cmd
	for (int i=0;i<CMD_LEN;i++)
	{
		if (buf[i] != CANCEL_CMD[i]){
			is_cancel = 0;
			break;
		}
	}

	// check data cmd
	int is_data = 1;
	for (int i=0;i<CMD_LEN;i++)
	{
		if (buf[i] != DATA_CMD[i]){
			is_data = 0;
			break;
		}
	}
	if (is_cancel)
		return 1;
	if (is_data)
		return 0;
	return -1;
}
