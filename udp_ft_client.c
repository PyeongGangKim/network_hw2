#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 256
void error_handling(char *message);
int main(int argc, char*argv[]){
    int server_sock;
    char buf[BUF_SIZE];
    int str_len;
    char* filename;
    FILE* fp;
    socklen_t client_adr_sz;

    struct sockaddr_in server_adr, from_adr;
    
    if(argc!=4){
        printf("Usage: %s <IP> <port> <filename>\n",argv[0]);
        exit(1);
    }
    server_sock = socket(PF_INET,SOCK_DGRAM,0);
    if(server_sock == -1){
        error_handling("UDP socket creation error");
    }

    memset(&server_adr,0,sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    server_adr.sin_addr.s_addr = inet_addr(argv[1]);
    server_adr.sin_port = htons(atoi(argv[2]));
    filename = argv[3];
    fp = fopen(filename,"rb");
    printf("%d\n",sendto(server_sock,filename,strlen(filename)+1,0,(struct sockaddr*)&server_adr,sizeof(server_adr)));
    int read_cnt = 0;
    while(1){
        read_cnt = fread((void*)buf,1,BUF_SIZE-1,fp);
        printf("read_cnt : %d\n",read_cnt);
        if(read_cnt == 0) break;
        if(read_cnt <BUF_SIZE-1){
            printf("%s\n",buf);
            printf("%d\n",sendto(server_sock,buf,read_cnt+1,0,(struct sockaddr*)&server_adr,sizeof(server_adr)));    
        }
        printf("%s\n",buf);
        printf("%d\n",sendto(server_sock,buf,read_cnt+1,0,(struct sockaddr*)&server_adr,sizeof(server_adr)));
        memset(buf,'\0',BUF_SIZE);
    }
    fclose(fp);
}
void error_handling(char* message){
   fputs(message,stderr);
   fputc('\n',stderr);
   exit(1);
}