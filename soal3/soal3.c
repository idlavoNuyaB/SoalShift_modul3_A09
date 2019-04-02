#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int statusAgmal=0;
int statusIraj=100;
int Agmalblmbangun=1;
int Irajblmtidur=1;
int counterbangun=0;
int countertidur=0;
int cek=0;

void *bangunAgmal(void *woibangun){
    while(Agmalblmbangun==1 && Irajblmtidur==1){
        system("clear");
        if(cek==1){
            printf("Fitur Agmal Ayo Bangun disabled 10s\n");
            printf("1. All Status\n");
            printf("2. Agmal Ayo Bangun\n");
            printf("3. Iraj Ayo Tidur\n");
            sleep(10);
            countertidur=0;
            counterbangun=0;
            cek=0;
            pthread_exit(NULL);
        }
        statusAgmal+=15;
        printf("Wake Up Status Agmal bertambah menjadi %d\n",statusAgmal);
        if(statusAgmal>=100 || Irajblmtidur==0){
            Agmalblmbangun=0;
            printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
            exit(0);
        }
        //printf("%d %d\n",countertidur,counterbangun);
        printf("1. All Status\n");
        printf("2. Agmal Ayo Bangun\n");
        printf("3. Iraj Ayo Tidur\n");
        pthread_exit(NULL);
    }
}

void *tidurIraj(void *woitidur){
    while(Agmalblmbangun==1 && Irajblmtidur==1){
        system("clear");
        if(cek==2){
            printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
            printf("1. All Status\n");
            printf("2. Agmal Ayo Bangun\n");
            printf("3. Iraj Ayo Tidur\n");
            sleep(10);
            counterbangun=0;
            countertidur=0;
            cek=0;
            pthread_exit(NULL);
        }
        statusIraj-=20;
        printf("Spirit Status Iraj berkurang menjadi %d\n",statusIraj);
        if(statusIraj<=0|| Agmalblmbangun==0){
            Irajblmtidur==0;
            printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
            exit(0);
        }
        //printf("%d %d\n",countertidur,counterbangun);
        printf("1. All Status\n");
        printf("2. Agmal Ayo Bangun\n");
        printf("3. Iraj Ayo Tidur\n");
        pthread_exit(NULL);
    }
}

void *stats(void *mereka){
    system("clear");
    counterbangun=0;
    countertidur=0;
    printf("Agmal WakeUp_Status = %d\n",statusAgmal);
    printf("Iraj Spirit_Status = %d\n",statusIraj);
    printf("1. All Status\n");
    printf("2. Agmal Ayo Bangun\n");
    printf("3. Iraj Ayo Tidur\n");
}

int main(){
    pthread_t threadid[3];
    int input;
    printf("1. All Status\n");
    printf("2. Agmal Ayo Bangun\n");
    printf("3. Iraj Ayo Tidur\n"); 
    while(Agmalblmbangun==1&&Irajblmtidur==1){
        scanf("%d",&input);
        if(input==1){
            pthread_create(&(threadid[0]),NULL,stats,NULL);
        }
        else if(input==2){
            if(countertidur!=0 && countertidur%3==0){
                system("clear");
                cek=1;
            }
            pthread_create(&(threadid[1]),NULL,bangunAgmal,NULL);
            counterbangun+=1;
            countertidur=0;
        }
        else if(input==3){
            if(counterbangun!=0 && counterbangun%3==0){
                system("clear");
                cek=2;
            }
            pthread_create(&(threadid[2]),NULL,tidurIraj,NULL);
            countertidur+=1;
            counterbangun=0;
        }
    }
    pthread_join(threadid[0],NULL);
    pthread_join(threadid[1],NULL);
    pthread_join(threadid[2],NULL);
}

