#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t locked;

void *faktorial(void *input){
    pthread_mutex_lock(&locked);
    int *temp; 
    temp = (int *)input;//void to Int
    long long int hasil=1;
    int i;
    for(i=*temp;i>1;i--)hasil*=i;//Di rumus faktorial
    printf(" %d ! = %lld\n",*temp,hasil);
    pthread_mutex_unlock(&locked);
}

int main(int argc,char *argv[]){
    pthread_mutex_init(&locked,NULL);
    pthread_t threadid[argc];
    int i,j,tempo;
    int temp[1000];
    for(i=1;i<argc;i++){
        temp[i]=atoi(argv[i]);//str to intL
        //printf("%d\n",tempa[i]);
    }
    for(i=1;i<argc;i++){//selection sort
        for(j=i;j<argc;j++){
            if(temp[i]>temp[j]){
                tempo=temp[j];
                temp[j]=temp[i];
                temp[i]=tempo;
            }
            //printf("%d %d %d\n",temp[i],temp[j],vtempo);
        }
    }
    for(i=1;i<argc;i++){
        //printf("%d\on",temp[i]);
        pthread_create(&(threadid[i]),NULL,&faktorial,(void *)&temp[i]);
        sleep(1);
    }
    for(i=1;i<argc;i++){
        pthread_join(threadid[i],NULL);
    }
    pthread_mutex_destroy(&locked);
    exit(0);
    return 0;
}
