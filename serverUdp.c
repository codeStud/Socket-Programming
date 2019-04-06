#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/types.h>
#include<stdio.h>

void error(char *msg)
{
perror(msg);
exit(0);
}

int main(int argc, char *argv[])
{
int sock, length, fromlen, n;
// Here 2 separate variables for server and client is created but its not neccessary.. All the info is stored in a single sockaddr_in structure itself
struct sockaddr_in server;
struct sockaddr_in from;
char buf[1024];

// 2 commandline arguments i.e ./a.out and 8000(port no)
if (argc < 2){
	fprintf(stderr, "ERROR, no port provided\n");
	exit(0);
	}

// creating socket by specifying AF_INET => IP format is used, SOCK_DGRAM => Socket has to be created for UDP, 0 => Accordingly create the socket
sock=socket(AF_INET, SOCK_DGRAM, 0);

if (sock < 0){
	error("ERROR opening socket");
}

length = sizeof(server);
// Clearing the server structure variable
bzero(&server,length);

// Family used is IP address format
server.sin_family=AF_INET;
// Get this PC's IP address automatically
server.sin_addr.s_addr=INADDR_ANY;
// Getting port number, converting it to integer and then to network understandable format
server.sin_port=htons(atoi(argv[1]));

// Binding socket to an address.. 'sock' has socket info and structure 'server' has all the info like IP addr and Port no
if (bind(sock,(struct sockaddr *)&server,length)<0){
	error("binding");
	}

fromlen = sizeof(struct sockaddr_in);

while (1){
	// Receive the msg from client in the buffer with size 1024. '0' is default argument, structure 'from' address is specified
	n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
	if (n < 0){
		error("recvfrom");
		}
	
	// 1 => Writing back to ONE client since UDP
	write(1,"Received a datagram: ",21);
	write(1,buf,n);
	
	// Sending data to client
	n = sendto(sock,"Got your message\n",17,0,(struct sockaddr *)&from,fromlen);
	if (n < 0){
		error("sendto");
		}
	}
}
