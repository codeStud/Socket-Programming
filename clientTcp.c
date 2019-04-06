#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(char *msg)
{
perror(msg);
exit(0);
}

int main(int argc, char *argv[])
{
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;

char buffer[256];

// <3 bcz argv[0] = ./a.out   argv[1] = localhost   argv[2] = 8000
if (argc < 3){
	fprintf(stderr,"use %s hostname port no.\n", argv[0]);
	exit(0);
	}

portno = atoi(argv[2]);

sockfd = socket(AF_INET, SOCK_STREAM, 0);

if(sockfd < 0){
	error("ERROR opening socket");
	}

// getting host's IP address in the variable server
server = gethostbyname(argv[1]);
if (server == NULL){
	fprintf(stderr, "ERROR, no such host\n");
	exit(0);
	}

// clearing the cashe value in the serv_addr structure and telling serv_addr that address format is IP 
bzero((char*) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;

// copying values of server address to right side argument and specifying the length of data to be copied
bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

// converting portno to network format and storing in serv_addr structure
serv_addr.sin_port = htons(portno);

// connection by specifying socket, serv_addr and sizeof(serv_addr)
if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0){
	error("ERROR connecting");
	}
	
printf("Please enter the message: ");
bzero(buffer,256);
fgets(buffer,255,stdin);
n = write(sockfd,buffer,strlen(buffer));

if (n < 0){
	error("ERROR writing to socket");
	}

// clearing buffer before reading reply server has sent into the buffer
bzero(buffer,256);
// read response from server into the buffer
n = read(sockfd,buffer,255);

if (n < 0){
	error("ERROR reading from socket");
	}
	
printf("%s\n",buffer);

return 0;
}
