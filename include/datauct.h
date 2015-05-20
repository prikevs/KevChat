#ifndef DATAUCT_H
#define DATAUCT_H

#define USERLEN 16
#define IDLEN 4

#define MSGALLLEN 100
#define SESSIONLEN 16
#define IVLEN 16
#define HMACLEN 20
#define CRYPTLEN (MSGALLLEN - SESSIONLEN - IVLEN)
#define TOCRYPTLEN (CRYPTLEN - HMACLEN)
#define TIMELEN 8

#define AESKEYLEN 16

typedef struct _DataFromClient {
    unsigned char session[SESSIONLEN];
    unsigned char iv[IVLEN];
    unsigned char msg[CRYPTLEN];
}DataFromClient;

typedef struct _MsgFromClient {
    unsigned char hmac[HMACLEN];
    unsigned char realmsg[TOCRYPTLEN];
}MsgFromClient;

typedef struct _RealMsgFromClient {
    unsigned char userid[USERLEN];
    unsigned char time[TIMELEN];
}RealMsgFromClient;

#endif
