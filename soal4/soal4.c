#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h> 
 
pthread_t tid1, tid2, tid3, tid4, tid5, tid6;
int status=0;
int nomor;
 
void* fungsi1(void *arg)
{
    
    system("ps -aux | tail -10 > SimpanProses1.txt");
    system("mkdir Documents/FolderProses1");
    system("mv SimpanProses1.txt Documents/FolderProses1/");
    status++;

}

void* fungsi2(void *arg)
{  
   system("ps -aux | tail -10 > SimpanProses2.txt");
   system("mkdir Documents/FolderProses2");
   system("mv SimpanProses2.txt Documents/FolderProses2/");
   status++;

}

void* fungsi3(void *arg)
{  while(status<2){};
   system("zip -m Documents/FolderProses1/KompresProses1.zip Documents/FolderProses1/SimpanProses1.txt");
   status++;

}

void* fungsi4(void *arg)
{  while(status<2){};
   system("zip -m Documents/FolderProses2/KompresProses2.zip Documents/FolderProses2/SimpanProses2.txt");
   status++;

}
void* fungsi5(void *arg)
{  while(status<4){};
   printf("Menunggu 15 detik untuk mengekstrak kembali\n");
   fflush(stdout);   
   sleep(15);
   system("unzip Documents/FolderProses1/KompresProses1.zip");
   system("rm Documents/FolderProses1/KompresProses1.zip");
}

void* fungsi6(void *arg)
{  while(status<4){};
   sleep(15);  
   system("unzip Documents/FolderProses2/KompresProses2.zip");
   system("rm Documents/FolderProses2/KompresProses2.zip");
}

int main(void)
{

    pthread_create(&(tid1), NULL, fungsi1, NULL);
    pthread_create(&(tid2), NULL, fungsi2, NULL);
    pthread_create(&(tid3), NULL, fungsi3, NULL);
    pthread_create(&(tid4), NULL, fungsi4, NULL);
    pthread_create(&(tid5), NULL, fungsi5, NULL);
    pthread_create(&(tid6), NULL, fungsi6, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);
    pthread_join(tid6, NULL);
	
    return 0;
}
