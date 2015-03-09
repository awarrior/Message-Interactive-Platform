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
    short port=6000;//端口号   
       
    wVersionRequested = MAKEWORD( 1, 1 );   
    err = WSAStartup( wVersionRequested, &wsaData );//初始化套接字   
    if ( err != 0 )   
    {   
        return;   
    }   
       
    if ( LOBYTE( wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion ) != 1 )   
    {   
        WSACleanup( );   
        return;   
    }   
       
    SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);//创建套接字   
    SOCKET sockConn;//用来和客户端通信的套接字   
    SOCKADDR_IN addrSrv;//用来和客户端通信的套接字地址   
    addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);   
    addrSrv.sin_family=AF_INET;   
    addrSrv.sin_port=htons(port);   
       
    bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//绑定端口   
    listen(sockSrv,5);//侦听   
       
    SOCKADDR_IN addrClient;   
       
    int len=sizeof(SOCKADDR);   
//    char buf[4096];//接收的数据   
    char rbuf[100]=   
        "<cross-domain-policy> "     
        "<allow-access-from domain=\"*\" to-ports=\"6000\"/>"     
        "</cross-domain-policy> ";//套接字策略文件   
       
    while(1)   
    {
		printf("Server %d is listening......\n",port);   

        //接受连接   
        sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);   
        printf("Accept connection from %s\n",inet_ntoa(addrClient.sin_addr));   
           
recv:   
        //接收数据   
/*        int bytes;   
        if((bytes=recv(sockConn,buf,sizeof(buf),0))==SOCKET_ERROR)   
        {   
            printf("接收数据失败!\n");   
            exit(-1);   
        }   
  
        buf[bytes]='\0';   
        printf("Message from %s: %s\n",inet_ntoa(addrClient.sin_addr),buf);        
 */ 
 /*       if (0 == strcmp(buf,"<policy-file-request/>"))   
        {   
            //发送数据   
            if(send(sockConn,rbuf,strlen(rbuf)+1,0)==SOCKET_ERROR)   
            {   
                printf("发送数据失败！");   
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
                printf("发送数据失败！");   
                exit(-1);   
            }   
            printf("Message to %s: %s\n",inet_ntoa(addrClient.sin_addr),a/*buf*/);   
            goto recv;   
        //}   
        //清理套接字占用的资源   
        closesocket(sockConn);   
    }   
}  
