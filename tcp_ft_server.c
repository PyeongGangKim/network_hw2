#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 20

void error_handling(char* message);

int main(int argc, char *argv[]){
    int server_sd, client_sd;
    FILE * fp;
    char buf[BUF_SIZE+1];
    int read_cnt;

    struct sockaddr_in server_adr, client_adr;
    socklen_t client_adr_sz;

    if(argc!=2){
        printf("Usage: %s <port>\n",argv[0]);
        exit(1);
    }
    server_sd = socket(PF_INET, SOCK_STREAM, 0);
    if(server_sd == -1){
        error_handling("socket() error");
    }
    memset(&server_adr, 0 , sizeof(server_adr));
    server_adr.sin_family=AF_INET;
    server_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_adr.sin_port=htons(atoi(argv[1]));

    if(bind(server_sd, (struct sockaddr*)&server_adr, sizeof(server_adr))== -1){
        error_handling("bind() error");
    }
    
    if(listen(server_sd,5) == -1){
        error_handling("listen() error");
    }
    char* filename= (char*)malloc(256*sizeof(char));
    memset(buf,'\0',BUF_SIZE);
    for(;;){
        int count = 0;

        client_adr_sz = sizeof(client_adr);
        client_sd = accept(server_sd,(struct sockaddr*)&client_adr,&client_adr_sz);
        if(client_sd == -1){
            error_handling("accept() error");
        }

        while((read_cnt = recv(client_sd,buf,BUF_SIZE,0))!=0){
            if(count==0){
                strcpy(filename,strtok(buf,"\n"));
                printf("%s\n",filename);
                fp = fopen(filename,"wb");
            }
            else{
                if(read_cnt == BUF_SIZE){
                    buf[BUF_SIZE]='\0';
                    printf("%s\n",buf);
                    fwrite((void*)buf,1,read_cnt,fp);
                }
                else{
                    printf("%s\n",buf);
                    fwrite((void*)buf,1,read_cnt,fp);
                }
            }
            count++;
            memset(buf,'\0',BUF_SIZE);
        }
        printf("you get the %s file\n",filename);
        fclose(fp);
    }
    
}
void error_handling(char* message){
   fputs(message,stderr);
   fputc('\n',stderr);
   exit(1);
}