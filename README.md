# SoalShift_modul3_A09

1. Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan

    Contoh:
 
    ./faktorial 5 3 4

    3! = 6

    4! = 24

    5! = 120

    ### Jawaban :

    Membuat sebuah program yang menggunakan thread untuk menghitung nilai faktorial dari setiap argumen yang ada, lalu diprint secara urut
    
    [**Source Code**](https://github.com/idlavoNuyaB/SoalShift_modul3_A09/blob/master/soal1/soal1.c)
    
    ### Penjelasan :
    
    * Mengurutkan nilai dari input argument dengan selection sort
    * Buat sebuah thread untuk menghitung faktorial yang diurutkan dan mencetak hasilnya serta tiap thread yang ada harus menunggu thread sebelumnya selesai menggunakan mutual exclusion
    
2. Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:

    a. Terdapat 2 server: server penjual dan server pembeli

    b. 1 server hanya bisa terkoneksi dengan 1 client

    c. Server penjual dan server pembeli memiliki stok barang yang selalu sama

    d. Client yang terkoneksi ke server penjual hanya bisa menambah stok
       
      - Cara menambah stok: client yang terkoneksi ke server penjual mengirim string “tambah” ke server lalu stok bertambah 1

    e. Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok
       
      - Cara mengurangi stok: client yang terkoneksi ke server pembeli mengirim string “beli” ke server lalu stok berkurang 1

    f. Server pembeli akan mengirimkan info ke client yang terhubung dengannya apakah transaksi berhasil atau tidak berdasarkan ketersediaan stok
       
      - Jika stok habis maka client yang terkoneksi ke server pembeli akan mencetak “transaksi gagal”
      - Jika stok masih ada maka client yang terkoneksi ke server pembeli akan mencetak “transaksi berhasil”

    g. Server penjual akan mencetak stok saat ini setiap 5 detik sekali

    h. Menggunakan thread, socket, shared memory

    ### Jawaban :
    
    Ada 2 server dan 2 client. 1 server penjual , 1 server pembeli, 1 client penjual dan 1 client pembeli

    Server penjual :
    * Menggunakan port 8080 dengan mendefine port 8080
      
          #define PORT 8080
    
    * Bagian utama dari server penjual
      
      Thread yang berulang-ulang mengprint nilai stock
      
          void* cetak(void *arg){
            key_t key = 1234; //inisialisasi shared memory stock
            int *value;  //inisialisasi shared memory stock
            int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666); // inisialisasi shared memory
            value = shmat(shmid, NULL, 0); //inisialisasi shared memory
            while(status==0){    //infinity loop mengprint nilai stock 
              printf("Stock saat ini: %d\n", *value); // mengprint value yang merupakan stock 
              //shared memory
              fflush(stdout);  //mengflush agar printf tidak buffer
              sleep(5);  // sleep setiap 5 detik
            }
            shmdt(value);
            shmctl(shmid, IPC_RMID, NULL);
          }

    * Inisialisasi server

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
     * Loop pengelola hasil kiriman dari client

            while(1){
              valread = read( new_socket , buffer, 1024);
              //printf("Stock saat ini %d\n",*value);
              if(strcmp(buffer,test)==0){  // jika client mengirim “tambah” value shared memory ++
                *value=*value+1;       
             }
              else if(strcmp(buffer,"stop")==0){ // menstop program ketika client mengirim “stop”
                status=1; // menghentikan loop pada thread
                break;       
              }
              //printf("Stock stelah ditambah %d\n",*value);
              memset(buffer, 0, sizeof buffer); // menset ulang nilai buffer untuk digunakan kembali
            }

    Client Penjual:

    * Menggunakan port 8080 ( sama seperti server penjual)
    
    * Bagian utama client:

    * Inisialisasi Client agar konek ke server port 8080
    
          struct sockaddr_in address;
          int sock = 0, valread;
          struct sockaddr_in serv_addr;
          char kirimkan[100]; // array of char untuk send()
          char buffer[1024] = {0}; 
          if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
          }
  
          memset(&serv_addr, '0', sizeof(serv_addr));
  
          serv_addr.sin_family = AF_INET;
          serv_addr.sin_port = htons(PORT);
      
          if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
          }
  
          if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            return -1;
          }

     * Loop untuk mengirimkan command “tambah” ke server penjual
    
            while(strcmp(kirimkan,"stop")!=0){
              scanf("%s",kirimkan);
              if(strcmp(kirimkan,"tambah")==0){
                send(sock , kirimkan , strlen(kirimkan) , 0 ); // mengirim string “tambah” ke server
              }
              else if(strcmp(kirimkan,"stop")==0){
                send(sock , kirimkan , strlen(kirimkan) , 0 );
                break;
              }
              else{
                printf("input salah\n");
              }
              memset(buffer, 0, sizeof buffer); // menset ulang buffer
            }

    Server pembeli :

    * Menggunakan port 8070
       
          #define PORT 8070

    * Bagian utama server pembeli :

    * Inisialisasi shared memory dari stock

          key_t key = 1234;
          int *value;
          int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
          value = shmat(shmid, NULL, 0);
    * Inisialisasi server

          int server_fd, new_socket, valread;
          struct sockaddr_in address;
          int opt = 1;
          int addrlen = sizeof(address);
          char buffer[1024] = {0};
          int stock=5;
          char test[]="beli"; //char untuk mengelola input client
          char gagal[]="Transaksi gagal";//char untuk mengelola input client
          char berhasil[]="Transaksi berhasil"; //char untuk mengelola input client
          char salah[]="input yang anda masukkan salah";//char untuk mengelola input client
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

    * Loop untuk mengelola input client

          while(strcmp(buffer,"stop")!=0){
            valread = read( new_socket , buffer, 1024);
            //printf("Stock saat ini %d\n",*value);
            if(strcmp(buffer,test)==0){
              if(*value==0){ // ketika value stock = 0 server mengirim char gagal
                send(new_socket , gagal , strlen(gagal) , 0 ); 
               }
              else{
                *value=*value-1; // server mengurangi stock -1 dan mengirim char berhasil
                send(new_socket , berhasil , strlen(berhasil) , 0 );
               }
            }
            else{
              send(new_socket , salah , strlen(salah) , 0 );
            };
            //printf("Stock stelah dibeli %d\n",*value);
            memset(buffer, 0, sizeof buffer);// menset ulang buffer
          }

    Client pembeli :

    * Menggunakan port 8070

    * Bagian utama:

    * Inisialisasi server

    * Loop untuk mengirim perintah ke server pembeli

          while(strcmp(kirimkan,"stop")!=0){
            scanf("%s",kirimkan);
            if(strcmp(kirimkan,"beli")==0){
              send(sock , kirimkan , strlen(kirimkan) , 0 ); // mengirim command beli
              valread = read( sock , buffer, 1024);
              printf("%s\n",buffer );
            }
            else if(strcmp(kirimkan,"stop")==0){break;}
            else {
              printf("input salah\n");
            }
            memset(buffer, 0, sizeof buffer); // mereset buffer
          }

    Kesimpulan : server penjual dan pembeli berbagi memory stock. Client pembeli dengan command “beli” dapat melakukan transaksi pembelian dan mengurangi stock. Client penjual dengan command “jual” dapat menambah stock. 
    
   [**Source Code Klien Pembeli**](https://github.com/idlavoNuyaB/SoalShift_modul3_A09/blob/master/soal2/soal2_clientpembeli.c)
   
   [**Source Code Klien Penjual**](https://github.com/idlavoNuyaB/SoalShift_modul3_A09/blob/master/soal2/soal2_clientpenjual.c)
   
   [**Source Code Server Pembeli**](https://github.com/idlavoNuyaB/SoalShift_modul3_A09/blob/master/soal2/soal2_serverpembeli.c)
   
   [**Source Code Server Penjual**](https://github.com/idlavoNuyaB/SoalShift_modul3_A09/blob/master/soal2/soal2_serverpenjual.c)

3. Agmal dan Iraj merupakan 2 sahabat yang sedang kuliah dan hidup satu kostan, sayangnya mereka mempunyai gaya hidup yang berkebalikan, dimana Iraj merupakan laki-laki yang sangat sehat,rajin berolahraga dan bangun tidak pernah kesiangan sedangkan Agmal hampir menghabiskan setengah umur hidupnya hanya untuk tidur dan ‘ngoding’. Dikarenakan mereka sahabat yang baik, Agmal dan iraj sama-sama ingin membuat satu sama lain mengikuti gaya hidup mereka dengan cara membuat Iraj sering tidur seperti Agmal, atau membuat Agmal selalu bangun pagi seperti Iraj. Buatlah suatu program C untuk menggambarkan kehidupan mereka dengan spesifikasi sebagai berikut:

    a. Terdapat 2 karakter Agmal dan Iraj

    b. Kedua karakter memiliki status yang unik
   
      - Agmal mempunyai WakeUp_Status, di awal program memiliki status 0
      - Iraj memiliki Spirit_Status, di awal program memiliki status 100
      - Terdapat 3 Fitur utama
        - All Status, yaitu menampilkan status kedua sahabat.
          Ex: 
            - Agmal WakeUp_Status = 75  
            - Iraj Spirit_Status = 30
        - “Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point
        - “Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point
      - Terdapat Kasus yang unik dimana:
        - Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled 10 s”)
        - Jika Fitur  “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled 10 s”)
      - Program akan berhenti jika Salah Satu :
        - WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal Terbangun,mereka bangun pagi dan berolahraga”)
        - Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan bangun kesiangan bersama Agmal”)
      - Syarat Menggunakan Lebih dari 1 Thread

    ### Jawaban :
    
    Membuat sebuah program yang mensimulasikan kelakuan si Agmal dan Iraj
    
    [**Source Code**](https://github.com/idlavoNuyaB/SoalShift_modul3_A09/blob/master/soal3/soal3.c)
    
    ### Penjelasan :
    
    * Membuat thread untuk mengatur kelakuan Agmal beserta timer disable fitur Agmal
    * Membuat thread untuk mengatur kelakuan Iraj beserta timer disable fitur Iraj
    * Buat loop untuk menangani input user 
    
4. Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 

    Dengan Syarat : 

    - Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
    - Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
    - Ketika Mengekstrak file .zip juga harus secara bersama-sama
    - Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”
    - Wajib Menggunakan Multithreading
    - Boleh menggunakan system

    ### Jawaban :
    
    * Menggunakan thread berbeda beda tetapi berjalan bersamaan

    * Menyimpan hasil ps -aux kedalam bentuk .txt secara bersamaan 

          void* fungsi1(void *arg){
            system("ps -aux | tail -10 > SimpanProses1.txt"); // menyimpan hasil ps -aux ke .txt
            system("mkdir Documents/FolderProses1"); // membuat folder baru
            system("mv SimpanProses1.txt Documents/FolderProses1/"); // hasil dikirim ke folder 
            status++;
          }

          void* fungsi2(void *arg){  
            system("ps -aux | tail -10 > SimpanProses2.txt"); // menyimpan hasil ps -aux
            system("mkdir Documents/FolderProses2"); // membuat folder baru
            system("mv SimpanProses2.txt Documents/FolderProses2/"); // memindahkan .txt ke folder
            status++;
          }

     * Menzip SimpanProses1.txt dan SimpanProses2.txt menjadi KompresProses1.zip dan KompresProses2.zip

            void* fungsi3(void *arg){  
              while(status<2){};  // menunggu fungsi 1 dan 2 selesai,
              system("zip -m Documents/FolderProses1/KompresProses1.zip Documents/FolderProses1/SimpanProses1.txt"); // menzip SimpanProses1.txt
              status++;
            }

            void* fungsi4(void *arg){  
              while(status<2){}; // menunggu fungsi 1 dan 2 selesai
              system("zip -m Documents/FolderProses2/KompresProses2.zip Documents/FolderProses2/SimpanProses2.txt");// menzip SimpanProses2.txt
              status++;
             }

      * Menunggu 15 detik lalu mengekstrak kembali

            void* fungsi5(void *arg){  
              while(status<4){}; // menunggu fungsi 3 dan 4 selesai
              printf("Menunggu 15 detik untuk mengekstrak kembali\n");
              fflush(stdout);   
              sleep(15);
              system("unzip Documents/FolderProses1/KompresProses1.zip"); 
              system("rm Documents/FolderProses1/KompresProses1.zip");
             }

            void* fungsi6(void *arg){
              while(status<4){}; // menunggu fungsi 3 dan 4 selessai
              sleep(15);  
              system("unzip Documents/FolderProses2/KompresProses2.zip");
              system("rm Documents/FolderProses2/KompresProses2.zip");
             }

      [**Source Code**](https://github.com/idlavoNuyaB/SoalShift_modul3_A09/blob/master/soal4/soal4.c)

5. Angga, adik Jiwang akan berulang tahun yang ke sembilan pada tanggal 6 April besok. Karena lupa menabung, Jiwang tidak mempunyai uang sepeserpun untuk membelikan Angga kado. Kamu sebagai sahabat Jiwang ingin membantu Jiwang membahagiakan adiknya sehingga kamu menawarkan bantuan membuatkan permainan komputer sederhana menggunakan program C. Jiwang sangat menyukai idemu tersebut. Berikut permainan yang Jiwang minta. 

    a. Pemain memelihara seekor monster lucu dalam permainan. Pemain dapat  memberi nama pada monsternya.

    b. Monster pemain memiliki hunger status yang berawal dengan nilai 200 (maksimalnya) dan nanti akan berkurang 5 tiap 10 detik.Ketika hunger status mencapai angka nol, pemain akan kalah. Hunger status dapat bertambah 15 apabila pemain memberi makan kepada monster, tetapi banyak makanan terbatas dan harus beli di Market.

    c. Monster pemain memiliki hygiene status yang berawal dari 100 dan nanti berkurang 10 tiap 30 detik. Ketika hygiene status mencapai angka nol, pemain akan kalah. Hygiene status' dapat bertambah 30 hanya dengan memandikan monster. Pemain dapat memandikannya setiap 20 detik(cooldownnya 20 detik).

    d. Monster pemain memiliki health status yang berawal dengan nilai 300. Variabel ini bertambah (regenerasi)daa 5 setiap 10 detik ketika monster dalam keadaan standby.

    e. Monster pemain dapat memasuki keadaan battle. Dalam keadaan ini, food status(fitur b), hygiene status'(fitur c), dan ‘regenerasi’(fitur d) tidak akan berjalan. Health status dari monster dimulai dari darah saat monster pemain memasuki battle. Monster pemain akan bertarung dengan monster NPC yang memiliki darah 100. Baik monster pemain maupun NPC memiliki serangan sebesar 20. Monster pemain dengan monster musuh akan menyerang secara bergantian. 

    f. Fitur shop, pemain dapat membeli makanan sepuas-puasnya selama stok di toko masih tersedia.
      - Pembeli (terintegrasi dengan game)
        - Dapat mengecek stok makanan yang ada di toko.
        - Jika stok ada, pembeli dapat membeli makanan.
      - Penjual (terpisah)
        - Bisa mengecek stok makanan yang ada di toko
        - Penjual dapat menambah stok makanan.

    Spesifikasi program:

    A. Program mampu mendeteksi input berupa key press. (Program bisa berjalan tanpa perlu menekan tombol enter)

    B. Program terdiri dari 3 scene yaitu standby, battle, dan shop.

    C. Pada saat berada di standby scene, program selalu menampilkan health status, hunger status, hygiene status, stok makanan tersisa, dan juga status kamar mandi (“Bath is ready” jika bisa digunakan, “Bath will be ready in [bath cooldown]s” jika sedang cooldown). Selain itu program selalu menampilkan 5 menu, yaitu memberi makan, mandi, battle, shop, dan exit. 

    Contoh :

    Standby Mode

    Health : [health status]

    Hunger : [hunger status]

    Hygiene : [hygiene status]

    Food left : [your food stock]

    Bath will be ready in [cooldown]s

    Choices

    1.	Eat
    2.	Bath
    3.	Battle
    4.	Shop
    5.	Exit

    D. Pada saat berada di battle scene, program selalu menampilkan health status milik pemain dan monster NPC. Selain itu, program selalu menampilkan 2 menu yaitu serang atau lari. 

    Contoh :

    Battle Mode

    Monster’s Health : [health status]

    Enemy’s Health : [enemy health status]

    Choices

    1.	Attack
    2.	Run

    E. Pada saat berada di shop scene versi pembeli, program selalu menampilkan food stock toko dan milik pemain. Selain itu, program selalu menampilkan 2 menu yaitu beli dan kembali ke standby scene. 

    Contoh :

    Shop Mode

    Shop food stock : [shop food stock]

    Your food stock : [your food stock]

    Choices

    1.	Buy
    2.	Back

    F. Pada program penjual, program selalu menampilkan food stock toko. Selain itu, program juga menampilkan 2 menu yaitu restock dan exit. 

    Contoh :

    Shop

    Food stock : [shop food stock]

    Choices

    1.	Restock
    2.	Exit

    G. Pastikan terminal hanya mendisplay status detik ini sesuai scene terkait (hint: menggunakan system(“clear”))

    ### Jawaban :
    
    Membuat program game untuk menjalankan gamenya beserta program shop khusus penjual
    
    [**Source Code Game**](https://github.com/idlavoNuyaB/SoalShift_modul3_A09/blob/master/soal5/soal5_game.c)
    
    [**Source Code Penjual**](https://github.com/idlavoNuyaB/SoalShift_modul3_A09/blob/master/soal5/soal5_penjual.c)

    ### Penjelasan :
    
    * Membuat shared memory di game dan penjualnya
    
    * Game
        
        * Meminta untuk memasukkan nama monster
        * Buat thread untuk regenerasi health, hunger, hygiene dan bath cooldown
        * Ubah display input menjadi raw mode (Input menjadi tidak kelihatan) 
        * Buat loop untuk menangani :
        
          * Menangkap input user
          * Fungsi yang menampilkan display dan menangani input user
        
        * Mengembalikan input menjadi terminal mode jika selesai
      
     * Penjual
     
        * Ubah display input menjadi raw mode (Input menjadi tidak kelihatan)
        * Buat loop untuk menangani :
          
          * Menangkap input user
          * Fungsi yang menampilkan display
          * Thread yang menangani input user
          
        * Mengubah kembali input menjadi terminal mode jika selesai
