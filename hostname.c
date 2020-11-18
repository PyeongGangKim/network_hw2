#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
    struct hostent *host;
    
    if(argc!=2){
        printf("Usage: %s <URL>\n",argv[0]);
        exit(1);
    }
    host = gethostbyname(argv[1]);
    
    printf("Official name: %s\n",host->h_name);
    
    int i = 0;
    while(host->h_aliases[i] != NULL){
        printf("Aliases %d: %s\n",i+1,host->h_aliases[i]);
        i++;
    }
    char *addrtype;
    if(host->h_addrtype == 2) addrtype = "AF_INET";
    else addrtype = "AF_INET6";
    printf("Address type: %s\n",addrtype);
    
    i = 0;
    
    while(host->h_addr_list[i] != NULL){
        printf("IP addr %d: %s\n",i+1,inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
        i++;
    }
    

}