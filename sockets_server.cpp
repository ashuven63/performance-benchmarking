#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h> 
#include <string.h> 
#include <sys/stat.h>  
#include <time.h>  
#include <cassert>
#include <stdlib.h>

#define SERV_TCP_PORT 8010 /* server's port number */
#define MAX_SIZE 80

using namespace std;

ssize_t write_all (int fd, const char* buffer, size_t count) 
{
  size_t left_to_write = count; 
  while (left_to_write > 0) {
    size_t written = write (fd, buffer, count); 
    if (written == -1) 
      /* An error occurred; bail.  */ 
      return -1; 
    else 
      /* Keep count of how much more we need to write.  */ 
      left_to_write -= written; 
  } 
  /* We should have written no more than COUNT bytes!   */ 
  assert (left_to_write == 0); 
  /* The number of bytes written is exactly COUNT.  */ 
  return count; 
} 

ssize_t read_all(int fd, char* buffer, size_t count){
    size_t left_to_read = count; 
    while (left_to_read > 0) {
        size_t readBytes = read(fd, buffer, count); 
        if (readBytes == -1) {
            /* An error occurred; bail.  */ 
            return -1;     
        } else {
            /* Keep count of how much more we need to write.  */ 
            left_to_read -= readBytes; 
            buffer += readBytes;
        }
    } 
    /* We should have written no more than COUNT bytes!   */ 
    assert (left_to_read == 0); 
    /* The number of bytes written is exactly COUNT.  */ 
    return count;  
}

int main(int argc, char *argv[])
{
  if(argc != 2) {
        cout << "Error in number of arguments\n";
        return 0;
    }

  int messageSize = atoi(argv[1]);
  int sockfd, newsockfd;
  socklen_t clilen;
  struct sockaddr_in cli_addr, serv_addr;
  int port;
  char* readMessage = new char[messageSize];
  int len;
  
  /* command line: server [port_number] */
  port = SERV_TCP_PORT;
  
  /* open a TCP socket (an Internet stream socket) */
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
     perror("can't open stream socket");
     return 1;
  }
  	
  /* bind the local address, so that the client can send to server */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);
  
  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
     perror("can't bind local address");
     return 1;
  }
	
  // cout << "Listening on " << sockfd;
  /* listen to the socket */
  listen(sockfd, 5);
   /* wait for a connection from a client*/
   clilen = sizeof(cli_addr);
   newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	
   if(newsockfd < 0) {
      perror("can't bind local address");
   }
 
   /* read a message from the client */
   len = read_all(newsockfd, readMessage, messageSize); 
   write_all(newsockfd, readMessage, messageSize);
   /* make sure it's a proper string */
//   printf("%s\n", readMessage);
   close(newsockfd);
   close(sockfd);
}

