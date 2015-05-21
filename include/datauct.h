/************************************************** 
    >FileName:      datauct.h
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
      * UCT structure for messages
***************************************************/

#ifndef DATAUCT_H
#define DATAUCT_H

#define USERLEN 20
#define HASHLEN 20
#define IDLEN 4

#define MSGALLLEN 1068
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
    unsigned char userid[IDLEN];
    unsigned char time[TIMELEN];
    unsigned char message[10];
}RealMsgFromClient;

#endif
