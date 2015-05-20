#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE 4096

int main()
{
    int sockfd, res;
    struct sockaddr_in servaddr;
    unsigned char buff[BUFSIZE];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(9999);
    while(1) {
        scanf("%s", buff);
        //buff[100] = 97;
        res = sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
        printf("%d bytes send.\n", res);
    }
}
