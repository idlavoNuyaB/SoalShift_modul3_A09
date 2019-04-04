#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>

int *value;
static struct termios lama;//untuk menyimpan original terminal attribut

int kbhit(){
    struct timeval timeout;
    fd_set apa;
    timeout.tv_usec=0;
    timeout.tv_sec=0;
    FD_ZERO(&apa);//inisialisasi
    FD_SET(STDIN_FILENO,&apa);//set bit
    select(STDIN_FILENO+1,&apa,NULL,NULL,&timeout);
    return FD_ISSET(STDIN_FILENO,&apa);
}

void resetinput(){
    tcsetattr(STDIN_FILENO,TCSANOW,&lama);
}

void setinputmode(){
    static struct termios baru;
    tcgetattr(STDIN_FILENO,&lama);
    resetinput();
    tcgetattr(STDIN_FILENO,&baru);
    baru.c_lflag &= ~(ICANON|ECHO);//clear icanon dan echo
    tcsetattr(STDIN_FILENO,TCSANOW,&baru);
}

void menu(){
    system("clear");
    printf("Shop\n");
    printf("Food stock : %d\n",*value);
    printf("Choices\n");
    printf("1. Restock\n");
    printf("2. Exit\n");
}

void *pilihanku(void *pilihana){
    char pilihan;
    while(!kbhit());
    pilihan=getchar();
    if(pilihan=='1'){
        *value+=1;
    }
    else if(pilihan=='2'){
        resetinput();
        exit(0);
        return 0;
    }
}

int main(){
    int pilihan;
    pthread_t threadid;
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    setinputmode();
    while(1){
        menu();
        pthread_create(&(threadid),NULL,pilihanku,NULL);
    }
}