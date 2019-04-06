#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/types.h>
#include<stdio.h>
#include<arpa/inet.h>

void error(char *);

void error(char *msg)
{
perror(msg);
exit(0);
}

int main(int argc, char *argv[])
{
int sock, length, n;
struct sockaddr_in server, from;
struct hostent *hp;
char buffer[256];

if (argc != 3){
	printf("Use server port no\n");
	exit(1);
	}

sock= socket(AF_INET, SOCK_DGRAM, 0);

if(sock < 0){
	error("ERROR opening socket");
	}

server.sin_family = AF_INET;

//Getting Host's IP address
hp = gethostbyname(argv[1]);

if(hp == 0){
	error("Unknown Host");
	}

// Copying the value of LHS argument to RHS argument & specifying length of it. 
// This is required bcz structure server has to have all details including IP address of the server
bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);

// Taking port no and converting it to int and then Network Understandable format
server.sin_port = htons(atoi(argv[2]));

length=sizeof(struct sockaddr_in);

printf("Please enter the message: ");
// Clearing buffer before reading msg into it
bzero(buffer,256);
fgets(buffer,255,stdin);

// Send the value in the buffer to server by passing server address (&server) and it's length
n = sendto(sock,buffer,strlen(buffer),0,&server,length);

if (n < 0){
	error("ERROR sending data");
	}
	
bzero(buffer,256);
// Receiving reply back from server and reading it into the buffer
n = recvfrom(sock, buffer, 256, 0, &from, &length);

if (n < 0){
	error("ERROR receiving data");
	}
	
write(1,"Got an ack: ",12);
// Print the msg read into the buffer
write(1,buffer,n);

return 0;
}
