#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include<pthread.h>
#define PORT 8080

pthread_t tid1	;
int status=0;
void* cetak(void *arg){
    key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
   while(status==0){
      printf("Stock saat ini: %d\n", *value);
      fflush(stdout);
      sleep(5);
           }
   shmdt(value);
   shmctl(shmid, IPC_RMID, NULL);
   }

int main(int argc, char const *argv[]) {
    key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    *value = 10;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char test[]="tambah";
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
  pthread_create(&(tid1), NULL, cetak, NULL);

while(1){
    valread = read( new_socket , buffer, 1024);
    //printf("Stock saat ini %d\n",*value);
    if(strcmp(buffer,test)==0){
    *value=*value+1;       
    }
    else if(strcmp(buffer,"stop")==0){
    status=1;
    break;       
    }
    //printf("Stock stelah ditambah %d\n",*value);
    memset(buffer, 0, sizeof buffer);
    }
pthread_join(tid1, NULL);
shmdt(value);
shmctl(shmid, IPC_RMID, NULL);
return 0;
}
