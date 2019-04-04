#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>

int makanan=10;
int hunger=200;
int hygiene=100;
int regenerasi=300;
int ehealth=100;
int bathcd=0;
int pilihanku=1;//
char nama[1000];
int *value;
int counter=0;
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

void *hungere(void *kelaparan){
    while(1){
        if(pilihanku!=2){
            if(hunger<=0){
                hunger=0;
                //pthread_exit(NULL);
            }
            sleep(10);
            hunger-=5;
            //pthread_exit(NULL);
        }
    }
}

void *hygienes(void *baubadan){
    while(1){
        if(pilihanku!=2){
            if(hygiene<=0){
                hygiene=0;
                //pthread_exit(NULL);
            }
            sleep(30);
            hygiene-=10;
            //pthread_exit(NULL);        
        }
    }
}

void *regeneration(void *kesembuhan){
    while(1){
        if(pilihanku==1){
            if(regenerasi>=300){
                regenerasi=300;
                //pthread_exit(NULL);
            }
            sleep(10);
            regenerasi+=5;
            //pthread_exit(NULL);  
        }
    }
}

void *bath(void *cooldown){
    while(1){
        if(bathcd<=0){
            bathcd=0;
            //pthread_exit(NULL);
        }
        sleep(1);
        bathcd--;
        //pthread_exit(NULL);  
    }
}

void standbymenu(){
    printf("Standby Mode\n");
    printf("Health : %d\n",regenerasi);
    printf("Hunger : %d\n",hunger);
    printf("Hygiene : %d\n",hygiene);
    printf("Food Left : %d\n",makanan);
    if(bathcd==0){
        printf("Bath is ready\n");
    }
    else{
        printf("Bath will be ready in %ds\n",bathcd);
    }
    printf("Choices\n");
    printf("1. Eat\n");
    printf("2. Bath\n");
    printf("3. Battle\n");
    printf("4. Shop\n");
    printf("5. Exit\n");
}

void battlemenu(){
    system("clear");
    printf("Battle Mode\n");
    printf("%s's Health : %d\n",nama,regenerasi);
    printf("Enemy's Health : %d\n",ehealth);
    printf("Choices\n");
    printf("1. Attack\n");
    printf("2. Run\n");
}

void shopmenu(){
    system("clear");
    printf("Shop Mode\n");
    printf("Shop food stock : %d\n",*value);
    printf("Your food stock : %d\n",makanan);
    printf("Choices\n");
    printf("1. Buy\n");
    printf("2. Back\n");
}

void pilihanstandby(){
    char pilihan;
    while(!kbhit());
    pilihan=getchar();
    if(pilihan=='1'){
        if(makanan != 0){
            hunger+=15;
            makanan--;
            if(hunger>=200){
                hunger=200;
            }
            system("clear");
            pilihanku=1;
        }
        else{
            system("clear");
            printf("We need food\n");
            sleep(1);
            system("clear");
            pilihanku=1;
        }
    }
    else if(pilihan=='2'){
        if(bathcd==0){
            bathcd=20;
            hygiene+=30;
            if(hygiene>=100){
                hygiene=100;
            }
            system("clear");
            pilihanku=1;
        }
        else{
            system("clear");
            printf("Bath is cleaning\n");
            sleep(1);
            system("clear");
            pilihanku=1;
        }
    }
    else if(pilihan=='3'){
        system("clear");
        pilihanku=2;
    }
    else if(pilihan=='4'){
        system("clear");
        pilihanku=3;
    }
    else if(pilihan=='5'){
        resetinput();
        exit(0);
    }
    else{
        system("clear");
        pilihanku=1;
    }
}

void pilihangelut(){
    char pilihan;
    while(!kbhit());
    pilihan=getchar();
    if(pilihan=='1'){
        regenerasi-=20;
        ehealth-=20;
        if(regenerasi<=0){
            regenerasi=0;
            printf("%s died in battle\n",nama);
            resetinput();
            exit(0);
        }
        else if(ehealth<=0){
            printf("%s won the battle\n", nama);
            ehealth=100;
            sleep(1);
            system("clear");
            pilihanku=1;
        }
    }
    else if(pilihan=='2'){
        if(regenerasi<=0){
            regenerasi=0;
            printf("%s died in battle\n",nama);
            exit(0);
        }
        if(counter!=3){
            printf("Failed to run away\n");
            regenerasi-=20;
            counter+=1;
            //printf("%d",counter);
            sleep(1);
            system("clear");
        }
        else{
            counter=0;
            printf("%s success running from enemy\n", nama);
            sleep(1);
            hunger-=7;
            hygiene-=3;
            system("clear");
            ehealth=100;
            pilihanku=1;
        }
    }
}

void pilihanbeli(){
    char pilihan;
    while(!kbhit());
    pilihan=getchar();
    if(pilihan=='1'){
        if(*value>0){
            *value-=1;
            makanan++;
        }
        else{
            printf("Seller go outside to refill her shop\n");
            sleep(1);
            system("clear");
        }
    }
    else if(pilihan=='2'){
        system("clear");
        pilihanku=1;
    }
}

int main(){
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    pthread_t threadid[10];
    printf("Welcome to the Monstar\n");
    printf("Please insert the monster name : ");
    scanf("%s",nama);
    system("clear");
    pthread_create(&(threadid[0]),NULL,hungere,NULL);
    pthread_create(&(threadid[1]),NULL,hygienes,NULL);
    pthread_create(&(threadid[2]),NULL,regeneration,NULL);
    pthread_create(&(threadid[3]),NULL,bath,NULL);
    setinputmode();
    while(1){
        if(hunger<=0){
            hunger=0;
            printf("%s died in starvation",nama);
            resetinput();
            exit(0);
        }
        if(hygiene<=0){
            hygiene=0;
            printf("%s died because illness",nama);
            resetinput();
            exit(0);
        }
        if(pilihanku==1){
            standbymenu();
            pilihanstandby();
        }
        else if(pilihanku==2){
            battlemenu();
            pilihangelut();
        }
        else if(pilihanku==3){
            shopmenu();
            pilihanbeli();
        }
        else{
            standbymenu();
            pilihanstandby();
        }
    }
}