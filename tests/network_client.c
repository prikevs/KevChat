//gcc -o client network_client.c ../libcrypt/hash.c ../libcrypt/symmetric.c ../libcrypt/libcrypt.c -I../include/ -ltomcrypt -Wall

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include "libcrypt.h"
#include "datauct.h"
#include <time.h>

#define BUFSIZE 4096

void generateData(DataFromClient *data, unsigned char *key)
{
    unsigned char iv[16];
    unsigned long len;
    int i;
    int err;
    MsgFromClient msg;
    RealMsgFromClient realmsg;
    unsigned char res[100];

    generateRandom(data->session, 16);
    generateRandom(iv, 16);

    memset(&msg, 0, sizeof(msg));
    memset(&realmsg, 0, sizeof(realmsg));

    printf("message: ");
    scanf("%s", realmsg.message);

    printf("session: ");
    for(i = 0; i < 16; i++) {
        printf("%02x ", data->session[i]);    
    }
    printf("\n");
    printf("iv: ");
    for(i = 0; i < 16; i++) {
        printf("%02x ", iv[i]);    
    }
    printf("\n");
    memcpy(realmsg.userid, "\x00\x00\x00\x01", 4);
    printf("userid: ");
    for(i = 0; i < 4; i++) {
        printf("%02x ", realmsg.userid[i]);    
    }
    printf("\n");

    memcpy(msg.realmsg, &realmsg, sizeof(msg.realmsg));

    len = sizeof(msg.hmac);
    hashHMAC(key, 16, msg.realmsg, sizeof(msg.realmsg), msg.hmac, &len);
    printf("len=%ld\n", len);
    printf("hmac: ");
    for(i = 0; i < 20; i++)
        printf("%02x ", msg.hmac[i]);
    printf("\n");
    memcpy(data->msg, &msg, sizeof(data->msg));
    printf("before: ");
    for(i = 0; i < 40; i++)
        printf("%02x ", data->msg[i]);
    printf("\n");
    err = symmetricEncrypt(key, 16, data->msg, sizeof(data->msg), iv, 16);
    printf("%s\n", crypt_error_to_string(err));
    printf("after : ");
    for(i = 0; i < 40; i++)
        printf("%02x ", data->msg[i]);
    printf("\n");
    printf("%s\n", (*(RealMsgFromClient *)&(*(MsgFromClient *)data->msg)).message);
    memcpy(data->iv, iv, 16);
}

int main()
{
    int sockfd, res;
    struct sockaddr_in servaddr;
    unsigned char key[16], buff[BUFSIZE];
    DataFromClient data;
    printf("key: ");
    memset(key, 0, sizeof(key));
    scanf("%s", key);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
//    symmetricEncrypt(key, 16, buff, sizeof(buff), key, 16);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(9999);
    memset(&data, 0, sizeof(data));
    generateData(&data, key);
    res = sendto(sockfd, &data, sizeof(data), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("%d bytes send.\n", res);
    return 0;
}
