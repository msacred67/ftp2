#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int sock;
    int datasock;
    int connection;
    struct sockaddr_in servaddr;
    struct stat obj;
    struct sockaddr_in dataaddr;
    short servport;
    short dataport;
    char *serverip;
    char *command[100];
    char *p;
    char buf[100];

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


    int filehandle, size, status;
    char filename[20];
    printf("Enter filename to put to server: ");
          scanf("%s", filename);
	  filehandle = open(filename, O_RDONLY);
          if(filehandle == -1)
            {
              printf("No such file on the local directory\n\n");
              return 0;
            }
          strcpy(buf, "put ");
	  strcat(buf, filename);
	  send(sock, buf, 100, 0);
	  stat(filename, &obj);
	  size = obj.st_size;
	  send(sock, &size, sizeof(int), 0);
	  sendfile(sock, filehandle, NULL, size);
	  recv(sock, &status, sizeof(int), 0);
	  if(status)
	    printf("File stored successfully\n");
	  else
	    printf("File failed to be stored to remote machine\n");
}
