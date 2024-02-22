//
// Created by joey on 2/22/24.
//
#include <netinet/in.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#define PORT 9000
#define BUFFSIZE 1024

int socketfd;
int recieve(int acceptedfd){
    char buff[BUFFSIZE];
    FILE *file  = fopen("/var/tmp/aesdsocketdata", "a+");
    int end = 0;
    while (recv(acceptedfd, buff, BUFFSIZE,0 )>0){
        for (int i=0; i<BUFFSIZE; i++){
            if (buff[i]==0){
                break;
            }
            else if (buff[i]=='\n' || buff[i]=='\r'){
                fputc(buff[i], file);
                end=1;
                break;
            }else{
                fputc(buff[i], file);
            }
        }
        strcpy(buff, "");
        if (end){
            break;
        }
//        if (buff[strlen(buff)-1] == '\n'){
//            break;
//        }
//
    }
    if (end==0){
        fputs("\n", file);
    }
    fclose(file);
    return 0;
}

int sendback(int acceptedfd){
    char buff[BUFFSIZE];
    FILE *file  = fopen("/var/tmp/aesdsocketdata", "r");
    int x = fread(buff, 1, BUFFSIZE, file);
    while (x > 0){
        //printf("%s", buff);
        send(acceptedfd, buff, x, 0);
        strcpy(buff, "");
        x = fread(buff, 1, BUFFSIZE, file);
    }

    if (strlen(buff)){
        //printf("\t%s", buff);
        send(acceptedfd, buff, strlen(buff), 0);
    }
//while (recv())
    fclose(file);
    return 0;
}

void handleSig(int signo){
    if (signo == SIGTERM || signo == SIGINT){
        syslog(LOG_ERR, "Caught signal, exiting\n");
        if (socketfd){
            close(socketfd);
        }
        remove("/var/tmp/aesdsocketdata");
        exit(0);
    }
}

int main(int argc, char **argv){
    openlog(NULL, 0, LOG_USER);
    signal(SIGTERM, handleSig);
    signal(SIGINT, handleSig);
    char buff[BUFFSIZE];
    struct sockaddr_in address;
    address.sin_family = PF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    socklen_t addlen = sizeof(address);

    socketfd = socket(PF_INET, SOCK_STREAM, 0);
    int one=1;
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR,&(int){1}, sizeof (int))==-1){
        return -1;
    }

    int bsuc = bind(socketfd, (struct sockaddr*) &address, sizeof(address));
    if (bsuc != 0){
        perror("Unable to Bind");
        exit(-1);
    }
    if (argc >1 && strcmp(argv[0], "-d")){
        pid_t pid;
        pid = fork();
        if (pid <0){
            exit(-1);
        }
        if (pid > 0){
            return 0;
        }
    }
    listen(socketfd, 1);
    while (1){
        int acceptedfd = accept(socketfd, (struct sockaddr*) &address, &addlen);
        char addr[INET_ADDRSTRLEN];
        inet_ntop(PF_INET,&address.sin_addr, addr, INET_ADDRSTRLEN);
        syslog(LOG_INFO, "Accepted a connection from %s", addr);
        recieve(acceptedfd);
        sendback(acceptedfd);
        syslog(LOG_INFO, "Closed a connection from %s", addr);
        close(acceptedfd);

    }
}
