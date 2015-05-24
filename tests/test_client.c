//gcc -o client test_client.c ../libcrypt/hash.c ../libcrypt/symmetric.c ../libcrypt/libcrypt.c ../libcrypt/dh.c -I../include/ -ltomcrypt -lgmp

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include "libcrypt.h"
//#include "datauct.h"
#include <time.h>

#define BUFSIZE 4096


void decrypt(unsigned char *buf, int len, unsigned char *key, int keylen, unsigned char *salt)
{
    symmetricDecrypt(key, keylen, buf+16, len-16, buf, 16);
    memcpy(salt, buf+16, 16);
}

void dump(unsigned char *buff, int len)
{
    int i;
    for(i = 0; i < len; i++)
        printf("%02x ", buff[i]);
    printf("\n");
}

int main()
{
    int sockfd, res;
    struct sockaddr_in servaddr;
    unsigned char key[16], buff[BUFSIZE];
    unsigned char pri[16], salt[16];
    unsigned long keylen;
    unsigned long publen;

    memset(key, 0, sizeof(key));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
//    symmetricEncrypt(key, 16, buff, sizeof(buff), key, 16);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(9999);
    res = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr));
    if (res != 0) {
        printf("connect failed.\n");    
        return 0;
    }

    generateRandom(pri, 16);
//    printf("sockfd=%d\n", sockfd);
    res = recv(sockfd, buff, BUFSIZE, 0);
    printf("%d bytes recv.\n", res);
    keylen = sizeof(key);
    getKey(key, &keylen, buff, 16, pri, 16);

    publen = sizeof(buff);
//    printf("sockfd=%d\n", sockfd);
    getPublic(buff, &publen, pri, 16);
//    printf("sockfd=%d\n", sockfd);
    res = send(sockfd, buff, publen, 0);
    perror("send  ");
    printf("%d bytes sent.\n", res);
    dump(key, res);

    res = recv(sockfd, buff, BUFSIZE, 0);
    printf("%d bytes recv.\n", res);

    decrypt(buff, res, key, 16, salt);
    printf("salt: ");
    dump(salt, 16);
    //res = sendto(sockfd, &data, sizeof(data), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
//    sleep(10);
    return 0;
}
