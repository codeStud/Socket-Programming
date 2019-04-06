#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>


//A separate user defined method for displaying errors
void error(char *msg)
{
perror(msg);
exit(1);
}

int main(int argc, char *argv[])
{
int sockfd, newsockfd, portno, clilen;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
int n;

if (argc < 2){
	fprintf(stderr,"ERROR, no port provided\n");
	exit(1);
	}

sockfd = socket(AF_INET, SOCK_STREAM, 0);

if (sockfd < 0){
	error("ERROR opening socket");
	}

// Before using serv_addr, clear this complete structure
// Values like IP address, port no etc are stored in serv_addr
bzero((char *) &serv_addr, sizeof(serv_addr));

// Storing port number passed through command line in a variable
// It is stored in char array. atoi is used to convert character to int
portno = atoi(argv[1]);

// Telling serv_addr that all the addresses are in IP address format
serv_addr.sin_family = AF_INET;
// Telling serv_addr to fetch IP addr of this PC automatically
serv_addr.sin_addr.s_addr = INADDR_ANY;
// htons coverts integer portno to a network understandable/compatible format
serv_addr.sin_port = htons(portno);


// Binding socket to serv_addr which is having all details like address and port no.
if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
	error("ERROR on binding");
	}
	
while(1) {
	// Step into listening mode using socket variable and no. of clients that can be handled simultaneously
	listen(sockfd,5);

	clilen = sizeof(cli_addr);

	// accepting client req by creating a new socket( TCP creates child process for each client to serve 'em simultaneously ) for that 		   client
	// and storing it in a separate variable
	// cli_addr structure will have all details of the client which will be stored in newsockfd variable
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);

	if (newsockfd < 0){
		error("ERROR on accept");
		}
	// clearing buffer before getting values that client has sent
	bzero(buffer,256);

	// read from newsockfd into buffer of len 255
	n = read(newsockfd,buffer,255);

	if (n < 0){
		error("ERROR reading from socket");
		}
	// printing msg which has been read into buffer
	printf("Here is the message: %s\n",buffer);

	// writing back to client whose info is in newsockfd, msg, msg-length
	n = write(newsockfd,"I got your message",18);

	if (n < 0){
		error("ERROR writing to socket");
		}
	}
	
return 0;
}
