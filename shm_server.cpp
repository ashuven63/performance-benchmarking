#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <iostream>
using namespace std;

#define SHMOBJ_PATH         "/shmperf"

sem_t* write_sem_id;
sem_t* read_sem_id;

void cleanup_semaphores(){
    /**
     * Semaphore Close: Close a named semaphore
     */
    if ( sem_close(write_sem_id) < 0 )
    {
        perror("sem_close");
    }
    if ( sem_close(read_sem_id) < 0 )
    {
        perror("sem_close");
    }

    /**
     * Semaphore unlink: Remove a named semaphore  from the system.
     */
    if ( sem_unlink("/writesem") < 0 )
    {
        perror("sem_unlink");
    }
    if ( sem_unlink("/readsem") < 0 )
    {
        perror("sem_unlink");
    }
}

void signal_callback_handler(int signum)
{
    
    cleanup_semaphores();

   // Terminate program
   exit(signum);
}

int main(int argc, char *argv[]) {
    
    if(argc !=2) {
        printf("Error in number of arguents");
        return -1;
    }
    int messageSize = atoi(argv[1]);
    int shmfd;
    int vol, cur;
    int shared_seg_size = (messageSize * sizeof(char));   
    char* shared_msg;      

    signal(SIGINT, signal_callback_handler);

    /* creating the shared memory object    --  shm_open()  */
    shmfd = shm_open(SHMOBJ_PATH, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    if (shmfd < 0)
    {
        perror("In shm_open()");
        exit(1);
    }

    fprintf(stderr, "Created shared memory object %s\n", SHMOBJ_PATH);

    /* adjusting mapped file size (make room for the whole segment to map)      --  ftruncate() */
    ftruncate(shmfd, shared_seg_size);

	/**
	 * Semaphore open
	 */
	write_sem_id = sem_open("/writesem", O_CREAT, S_IRUSR | S_IWUSR, 1);
    read_sem_id = sem_open("/readsem", O_CREAT, S_IRUSR | S_IWUSR, 1);

    /* requesting the shared segment    --  mmap() */
    shared_msg = (char*)mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (shared_msg == NULL)
    {
        perror("In mmap()");
        exit(1);
    }

    fprintf(stderr, "Shared memory segment allocated correctly (%d bytes).\n", shared_seg_size);
    printf("Waiting for semaphore\n");

    sem_wait(write_sem_id);
    sleep(5);
    // TODO: Start Timer
    for(int i = 0; i < messageSize; i++){
        shared_msg[i] = 'a';
        cout << i+" ";
    }
    sem_post(write_sem_id);

    sem_wait(read_sem_id);
    char* readMessage = new char[messageSize];
    for(int i = 0; i < messageSize; i++){
        readMessage[i] = shared_msg[i];
    }
    sem_post(read_sem_id);

    // TODO: End timer

    cout << "Received " <<readMessage << endl;
    if (shm_unlink(SHMOBJ_PATH) != 0) {
        perror("In shm_unlink()");
        exit(1);
    }
    cleanup_semaphores();
    delete[] readMessage;
    return 0;
}