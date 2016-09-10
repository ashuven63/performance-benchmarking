#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h> 
#include <string.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <time.h>  
#include <cassert>
#include "timers.h"
#include <stdlib.h>

#define SERV_TCP_PORT 8000 /* server's port */

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

int main(int argc, char *argv[]){

    if(argc != 2) {
        cout << "Error in number of arguments\n";
        return 0;
    }

    int messageSize = atoi(argv[1]);
    int pid, server[2], client[2];

    pipe(server);
    pipe(client);

    pid = fork();
    InitRdtsc();
    

    // The parent which will behave as the client 
    if(pid == 0) {
        close(client[0]); // Write using client[1]
        close(server[1]); // Read using server[0]
        char* message = getString(messageSize);
        char* readMessage = new char[messageSize];
        timespec startTime, endTime;
        // cout << message <<endl;
        // Start Timer 
        GetRdtscTime(&startTime);
        int writtenBytes = write_all(client[1], message, messageSize);
        if(writtenBytes != messageSize) {
            cout <<  "Error in writing: Wrote " << writtenBytes << endl;
            return 0;
        }
        int readBytes = read_all(server[0], readMessage, messageSize);
        if(readBytes != messageSize) {
            cout <<  "Error in reading: Read " << readBytes << endl;
            return 0;
        }
        //End timer
        GetRdtscTime(&endTime);
        cout << diff(startTime, endTime).tv_nsec / 2 << endl;
        //cout << "Reading the string in client: " << strlen(readMessage) << endl; 
        // Free the space allocated on the heap
        delete[] message;
        delete[] readMessage;
         

    } else { // Child process which will behave as the server
        close(client[1]); // Read using client[0]
        close(server[0]); // Write using server[1]
        char* readMessage = new char[messageSize];
        int readBytes = read_all(client[0], readMessage, messageSize);
        if(readBytes != messageSize) {
            cout <<  "Error in reading: Read " << readBytes << endl;
            return 0;
        }
        // cout << "Read the string in server : " << strlen(readMessage) << endl;
        int writtenBytes = write_all(server[1], readMessage, messageSize);
        if(writtenBytes != messageSize) {
            cout <<  "Error in writing: Wrote " << writtenBytes << endl;
            return 0;
        }
        delete[] readMessage;
    }
}
