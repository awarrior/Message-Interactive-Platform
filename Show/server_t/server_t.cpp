//#include "stdafx.h"


#include <winsock2.h>


#include <windows.h>

#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib,"ws2_32.lib")



  
void main()   
{    
    WORD wVersionRequested;   
    WSADATA wsaData;   
    int err;   
    short port=6000;//�˿ں�   
       
    wVersionRequested = MAKEWORD( 1, 1 );   
    err = WSAStartup( wVersionRequested, &wsaData );//��ʼ���׽���   
    if ( err != 0 )   
    {   
        return;   
    }   
       
    if ( LOBYTE( wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion ) != 1 )   
    {   
        WSACleanup( );   
        return;   
    }   
       
    SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);//�����׽���   
    SOCKET sockConn;//�����Ϳͻ���ͨ�ŵ��׽���   
    SOCKADDR_IN addrSrv;//�����Ϳͻ���ͨ�ŵ��׽��ֵ�ַ   
    addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);   
    addrSrv.sin_family=AF_INET;   
    addrSrv.sin_port=htons(port);   
       
    bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//�󶨶˿�   
    listen(sockSrv,5);//����   
       
    SOCKADDR_IN addrClient;   
       
    int len=sizeof(SOCKADDR);   
//    char buf[4096];//���յ�����   
    char rbuf[100]=   
        "<cross-domain-policy> "     
        "<allow-access-from domain=\"*\" to-ports=\"6000\"/>"     
        "</cross-domain-policy> ";//�׽��ֲ����ļ�   
       
    while(1)   
    {
		printf("Server %d is listening......\n",port);   

        //��������   
        sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);   
        printf("Accept connection from %s\n",inet_ntoa(addrClient.sin_addr));   
           
recv:   
        //��������   
/*        int bytes;   
        if((bytes=recv(sockConn,buf,sizeof(buf),0))==SOCKET_ERROR)   
        {   
            printf("��������ʧ��!\n");   
            exit(-1);   
        }   
  
        buf[bytes]='\0';   
        printf("Message from %s: %s\n",inet_ntoa(addrClient.sin_addr),buf);        
 */ 
 /*       if (0 == strcmp(buf,"<policy-file-request/>"))   
        {   
            //��������   
            if(send(sockConn,rbuf,strlen(rbuf)+1,0)==SOCKET_ERROR)   
            {   
                printf("��������ʧ�ܣ�");   
                exit(-1);   
            }   
            printf("Message to %s: %s\n",inet_ntoa(addrClient.sin_addr),rbuf);   
        }   
        else  
        {   
*/            //Echo
			char a[100];
			scanf("%s",a);
			if (a[0] == 'r')
			{
				continue;
			}
            if(send(sockConn,/*buf,strlen(buf)+1*/a,101,0)==SOCKET_ERROR)   
            {   
                printf("��������ʧ�ܣ�");   
                exit(-1);   
            }   
            printf("Message to %s: %s\n",inet_ntoa(addrClient.sin_addr),a/*buf*/);   
            goto recv;   
        //}   
        //�����׽���ռ�õ���Դ   
        closesocket(sockConn);   
    }   
}  
