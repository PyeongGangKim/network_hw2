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

    struct sockaddr_in server_adr, client_adr;
    
    if(argc!=2){
        printf("Usage: %s <port>\n",argv[0]);
        exit(1);
    }
    server_sock = socket(PF_INET,SOCK_DGRAM,0);
    if(server_sock == -1){
        error_handling("UDP socket creation error");
    }

    memset(&server_adr,0,sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    server_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_adr.sin_port = htons(atoi(argv[1]));

    if(bind(server_sock, (struct sockaddr*)&server_adr, sizeof(server_adr))== -1){
        error_handling("bind() error");
    }

    recvfrom(server_sock,buf,BUF_SIZE,0,(struct sockaddr*)&client_adr,&client_adr_sz);
    printf("%s\n",buf);
    filename = buf;
    fp = fopen(filename,"wb");
    int read_cnt = 0;
    while((read_cnt = recvfrom(server_sock,buf,BUF_SIZE,0,(struct sockaddr*)&client_adr,&client_adr_sz)) != 0){
        printf("%d\n",fwrite((void*)buf,1,strlen(buf),fp));
        printf("%s\n",buf);
        if(read_cnt < BUF_SIZE) break;
    }
    fclose(fp);
}
void error_handling(char* message){
   fputs(message,stderr);
   fputc('\n',stderr);
   exit(1);
}