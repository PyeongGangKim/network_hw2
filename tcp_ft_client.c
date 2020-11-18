#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 256

void error_handling(char* message);
int main(int argc, char *argv[])
{
    int server_sd;
    FILE * fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in server_adr;
    

    if(argc!=4){
        printf("Usage: %s <port> <port> <filname>\n",argv[0]);
        exit(1);
    }

    char * filename = argv[3];
    char * linechange = "\n";
    fp = fopen(filename,"rb");
    server_sd = socket(PF_INET, SOCK_STREAM, 0);
    if(server_sd == -1){
        error_handling("socket() error");
    }
    
    memset(&server_adr, 0 , sizeof(server_adr));
    server_adr.sin_family=AF_INET;
    server_adr.sin_addr.s_addr=inet_addr(argv[1]);
    server_adr.sin_port=htons(atoi(argv[2]));
    
    if(connect(server_sd, (struct sockaddr*)&server_adr, sizeof(server_adr)) == -1){
        error_handling("bind() error");
    }
    
    strcat(filename,linechange);
    int w_count = 0;
    int count = 0;
    memset(buf,'\0',BUF_SIZE);
    while(1){
        if(count==0){
            send(server_sd,filename,strlen(filename),0);
        }
        read_cnt = fread((void*)buf,1,BUF_SIZE,fp);        
        if(read_cnt == 0) break;
        if(read_cnt <BUF_SIZE){
            printf("%s\n",buf);
            send(server_sd,buf,read_cnt,0);
            break;
        }
        count++;
        printf("%s\n",buf);
        w_count=send(server_sd,buf,read_cnt,0);
        memset(buf,'\0',BUF_SIZE);
    }
    fclose(fp);
    
}
void error_handling(char* message){
   fputs(message,stderr);
   fputc('\n',stderr);
   exit(1);
}