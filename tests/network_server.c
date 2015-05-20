#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE 4096

int main()
{
    int sockfd, res;
    socklen_t addrlen;
    struct sockaddr_in servaddr, clientaddr;
    unsigned char buff[4096];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);    
    if (sockfd == -1) {
        printf("create socket error:");
        return -1;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(9999);
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        printf("bind socket error:"); 
        return -1;
    }
    while(1) {
        memset(&clientaddr, 0, sizeof(clientaddr));
        memset(buff, 0, sizeof(buff));
        addrlen = sizeof(clientaddr);
        res = recvfrom(sockfd, buff, BUFSIZE, 0, (struct sockaddr *)&clientaddr, &addrlen);
        printf("%d bytes recieved: %s\n", res, buff);
        fflush(stdout);
    }
    return 0; 
}
