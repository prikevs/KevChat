//gcc -o server network_server.c ../libcrypt/hash.c ../libcrypt/symmetric.c -I../include/ -ltomcrypt

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "libcrypt.h"
#include "datauct.h"


#define BUFSIZE 4096

void handle(unsigned char *buff, unsigned char *key)
{
    int i;
    unsigned long len;
    DataFromClient data;
    MsgFromClient msg;
    RealMsgFromClient realmsg;
    unsigned char hmac[20];

    memset(&data, 0, sizeof(data));
    memset(&msg, 0, sizeof(msg));
    memset(&realmsg, 0, sizeof(realmsg));

    memcpy(&data, buff, sizeof(data));   
    printf("session: ");
    for(i = 0; i < 16; i++) {
        printf("%02x ", data.session[i]);     
    }
    printf("\n");
    printf("iv: ");
    for(i = 0; i < 16; i++) {
        printf("%02x ", data.iv[i]);     
    }
    printf("\n");
    printf("encrypted msg: ");
    for(i = 0; i < 40; i++)
        printf("%02x ", data.msg[i]);
    printf("\n");
    symmetricDecrypt(key, 16, data.msg, sizeof(data.msg), data.iv, 16);
    printf("encrypted msg: ");
    for(i = 0; i < 40; i++)
        printf("%02x ", data.msg[i]);
    printf("\n");
    memcpy(&msg, data.msg, sizeof(data.msg));

    len = sizeof(hmac);
    hashHMAC(key, 16, msg.realmsg, sizeof(msg.realmsg), hmac, &len);
    printf("msg hmac: ");
    for(i = 0; i < 20; i++) {
        printf("%02x ", msg.hmac[i]);     
    }
    printf("\n");
    
/*    if (memcmp(hmac, msg.hmac, len) != 0) {
        printf("wrong key!\n"); 
        return;
    }
    */
    memset(&realmsg, 0, sizeof(realmsg));
    memcpy(&realmsg, msg.realmsg, sizeof(msg.realmsg));
    printf("userid: ");
    for(i = 0; i < IDLEN; i++)
       printf("%02x ", realmsg.userid[i]); 
    printf("\n");

    printf("message: %s\n", realmsg.message); 

}

int main()
{
    int sockfd, res;
    socklen_t addrlen;
    struct sockaddr_in servaddr, clientaddr;
    unsigned char buff[4096], key[16];
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
    symmetricEncrypt(key, 16, buff, sizeof(buff), key, 16);
    while(1) {
        printf("key: ");
        scanf("%s", key);
        memset(&clientaddr, 0, sizeof(clientaddr));
        memset(buff, 0, sizeof(buff));
        addrlen = sizeof(clientaddr);
        res = recvfrom(sockfd, buff, BUFSIZE, 0, (struct sockaddr *)&clientaddr, &addrlen);
        printf("%d bytes recieved: %s\n", res, buff);
        fflush(stdout);
        handle(buff, key);
    }
    return 0; 
}
