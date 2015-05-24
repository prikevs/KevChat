/************************************************** 
    >FileName:      msgdispose.h
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
        * functions attached to analyzing or construct
          messages
***************************************************/

#ifndef MSGDISPOSE_H
#define MSGDISPOSE_H

void Auth_MakeDHMsg(unsigned char *, unsigned long *, unsigned char *, unsigned long);

void Auth_ParseDHMsg(unsigned char *, unsigned long , unsigned char *, unsigned long , unsigned char *, unsigned long *);

void Auth_MakeSaltMsg(unsigned char *, unsigned long *, unsigned char *, unsigned long *, unsigned char *, unsigned long);

int Auth_Verify(unsigned char *, unsigned long , unsigned char *, unsigned long, unsigned char *, unsigned long);

void Auth_MakeSuccessMsg(unsigned char *, unsigned long *, unsigned char *, unsigned long, unsigned char *, unsigned long);

#endif
