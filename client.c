#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define RCVBUFSIZE 8

int main(int argc, char *argv[])
{
    int sock;
    int datasock;
    int connection;
    struct sockaddr_in servaddr;
    struct sockaddr_in dataaddr;
    short servport;
    short dataport;
    char *serverip;

    serverip = "127.0.0.1";
    servport = 21;
    printf(serverip);
    printf(" Membuat Socket\n");
    if((sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        perror(strerror(errno));
        exit(-1);
    }
    else printf("Berhasil Membuat Socket\n");

    if((datasock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror(strerror(errno));
        exit(-1);
    }
    else printf("Berhasil Membuat Data Socket\n");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(serverip);
    servaddr.sin_port = htons(servport);

    if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror(strerror(errno));
        exit(-1);
    }
    else printf("Koneksi Berhasil\n");

    dataport = 20;
    bzero(&dataaddr, sizeof(dataaddr));
    dataaddr.sin_family = AF_INET;
    dataaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    dataaddr.sin_port = htons(dataport);

    printf("%d  %s", argc, argv[0]);
}
