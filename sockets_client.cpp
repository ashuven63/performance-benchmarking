#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <iostream>
#include <fcntl.h> 
#include <string.h> 
#include <sys/stat.h>  
#include <unistd.h>
#include <sys/stat.h>  
#include <time.h>  
#include <cassert>
#include <stdlib.h>
#include "timers.h"
#define SERV_TCP_PORT 8010 /* server's port */
using namespace std;

char* getString(int sLength){
    char* str = new char[sLength];
    for(int i = 0; i < sLength; i++ ){
        str[i] = 'a';
    }
    return str;
}

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
  int sockfd;
  struct sockaddr_in serv_addr;
  char *serv_host = "localhost";
  struct hostent *host_ptr;
  int port = SERV_TCP_PORT;
  int buff_size = 0;
  int messageSize;
  timespec startTime, endTime;
  InitRdtsc();
  /* command line: client messageSize*/
  if(argc >= 2){
    messageSize = atoi(argv[1]);
  }

  /* get the address of the host */
  if((host_ptr = gethostbyname(serv_host)) == NULL) {
     perror("gethostbyname error");
     return 1;
  }
  
  if(host_ptr->h_addrtype !=  AF_INET) {
     perror("unknown address type");
     return 1;
  }
  
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = 
     ((struct in_addr *)host_ptr->h_addr_list[0])->s_addr;
  serv_addr.sin_port = htons(port);
  

  /* open a TCP socket */
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
     perror("can't open stream socket");
     return 1;
  }

  /* connect to the server */    
  if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
     perror("can't connect to server");
     return 1;
  }

  char* message = getString(messageSize);
  char* readMessage = new char[messageSize];
  /* write a message to the server */
  //Start timer 
  GetRdtscTime(&startTime);
  write_all(sockfd, message, messageSize);
  read_all(sockfd, readMessage, messageSize);
  //End timer 
  GetRdtscTime(&endTime);
  cout << diff(startTime, endTime).tv_nsec / 2 << endl;readMessage;

  delete[] readMessage;
  delete[] message;
  close(sockfd);
}

