#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include<sys/sendfile.h>
#include<sys/stat.h>

int main(int argc, char *argv[])
{
    int sock;
    int datasock;
    struct sockaddr_in servaddr;
    struct stat obj;
    struct sockaddr_in dataaddr;
    short servport;
    short dataport;
    char *serverip;
    char *command[100];
    char *p, *f;
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

    if ((datasock = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP)) < 0)
    {
    	perror(strerror(errno));
    	exit(-1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(serverip);
    servaddr.sin_port = htons(servport);

    if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror(strerror(errno));
        exit(-1);
    }

    dataport = 20;
    bzero(&dataaddr, sizeof(dataaddr));
    dataaddr.sin_family = AF_INET;
    dataaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    dataaddr.sin_port = htons(servport);
    printf("Koneksi Berhasil\n");

    bind(datasock,(struct sockaddr *) &dataaddr, sizeof(dataaddr));
    listen(datasock, 3);

    p = strcpy(command, "USER msacred67\r\n");
    send(sock, command, strlen(command), 0);
    p = strcpy(command, "PASS dilarangmasuk\r\n");
    send(sock, command, strlen(command), 0);

    int filehandle, size, status,abc;
    char filename[20];
    while(1)
    {
    printf("1- download\n2- upload\n3- quit\n");
      scanf("%d", &abc);
      switch(abc)
	{
	case 1:
	  printf("Masukkan nama file: ");
	  scanf("%s", filename);
	  strcpy(buf, "get ");
	  strcat(buf, filename);
	  send(sock, buf, 100, 0);
	  recv(sock, &size, sizeof(int), 0);
	  if(!size)
	    {
	      printf("File tidak ditemukan\n\n");
	    break;
	    }
	  f = malloc(size);
	  recv(sock, f, size, 0);
	  while(1)
	    {
	      filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY);
	      if(filehandle == -1)
		{
		  sprintf(filename + strlen(filename), "%d", 1);
		}
	      else break;
	    }
	  write(filehandle, f, size);
	  close(filehandle);
	  break;
	case 2:
	  printf("Masukkan nama file: ");
          scanf("%s", filename);
	  filehandle = open(filename, O_RDONLY);
          if(filehandle == -1)
            {
              printf("File tidak ditemukan\n\n");
              break;
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
	    printf("File berhasil disimpan\n");
	  else
	    printf("gagal\n");
	  break;
    case 3:
	  strcpy(buf, "quit");
          send(sock, buf, 100, 0);
          recv(sock, &status, 100, 0);
	  if(status)
	    {
	      printf("Koneksi ditutup\n");
	      close(sock);
	      close(datasock);
	      exit(0);
	    }
	    printf("gagal menutup koneksi\n");
	  }
    }
}
