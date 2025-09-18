// Client side implementation of UDP client-server model 
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include "../../include/udpClient.h"
  
#define PORT     8080 
#define MAXLINE 1024 

void openUDP(struct UDPSocket * udp_socket)
{
    if ( (udp_socket->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 

    memset(&(udp_socket->servaddr), 0, sizeof(udp_socket->servaddr)); 

    udp_socket->servaddr.sin_family = AF_INET; 
    udp_socket->servaddr.sin_port = htons(PORT); 
    udp_socket->servaddr.sin_addr.s_addr = INADDR_ANY; 

    udp_socket->server_struct_length = sizeof(udp_socket->servaddr);
}
  

void tareForceSensor(struct UDPSocket * udp_socket)
{
    udp_socket->sendData = 1124086802;// TODO set to tare command

    if(sendto(udp_socket->sockfd, &udp_socket->sendData, 8, 0,
     (struct sockaddr*)&(udp_socket->servaddr), udp_socket->server_struct_length) < 0){
    printf("Unable to send message\n");
    }

    return;
}


void startForceSensorStream(struct UDPSocket * udp_socket)
{
    udp_socket->sendData = 33567762; // start stream (0x0002) 

    if(sendto(udp_socket->sockfd, &udp_socket->sendData, 8, 0,
     (struct sockaddr*)&(udp_socket->servaddr), udp_socket->server_struct_length) < 0){
    printf("Unable to send message\n");
    }

    sleep(0.1);
    readForceSensor(udp_socket);
    sleep(0.1);
    sleep(0.1);

    return;
}


void stopForceSensorStream(struct UDPSocket * udp_socket)
{
    udp_socket->sendData = 0; //stop(0x000)

    if(sendto(udp_socket->sockfd, &udp_socket->sendData, 8, 0,
     (struct sockaddr*)&(udp_socket->servaddr), (udp_socket->server_struct_length)) < 0){
    printf("Unable to send message\n");
    }

    return;
}

void readForceSensor(struct UDPSocket * udp_socket)
{

    int i = 0;

    while(recvfrom(udp_socket->sockfd, udp_socket->msg, 4*9, 0,
     (struct sockaddr*)&(udp_socket->servaddr), &udp_socket->server_struct_length) != -1)
    {
        i = i + 1;
    }

    if(i>0)
    {
        reverseBits(udp_socket,&(udp_socket->msg[3]));
        udp_socket->F[0] = (double)udp_socket->msg[3]/1000000.0;
        reverseBits(udp_socket,&(udp_socket->msg[4]));
        udp_socket->F[1] = (double)udp_socket->msg[4]/1000000.0;
        reverseBits(udp_socket,&(udp_socket->msg[5]));
        udp_socket->F[2] = (double)udp_socket->msg[5]/1000000.0;

        reverseBits(udp_socket,&(udp_socket->msg[6]));
        udp_socket->T[0] = (double)udp_socket->msg[6]/1000000.0;
        reverseBits(udp_socket,&(udp_socket->msg[7]));
        udp_socket->T[1] = (double)udp_socket->msg[7]/1000000.0;
        reverseBits(udp_socket,&(udp_socket->msg[8]));
        udp_socket->T[2] = (double)udp_socket->msg[8]/1000000.0;
    }

    if(udp_socket->F[2]>100 || udp_socket->F[2]<-100)
    {
        printf("%d, %f",i,udp_socket->F[2]);
    }

}

void reverseBits(struct UDPSocket * udp_socket, int32_t * num)
{
    
    udp_socket->temp[0] = (*num &  0xff000000) >> 24; 
    udp_socket->temp[1] = (*num &  0x00ff0000) >> 8; 
    udp_socket->temp[2] = (*num &  0x0000ff00) << 8; 
    udp_socket->temp[3] = (*num &  0x000000ff) << 24; 
    *num = udp_socket->temp[0] | udp_socket->temp[1] | udp_socket->temp[2] | udp_socket->temp[3];

}

int main() { 

    struct UDPSocket udp_socket;

    openUDP(&udp_socket);
    
    char buffer[MAXLINE]; 
    const char *hello = "Hello from client"; 
  
    int n;
    socklen_t len; 
      
    sendto(udp_socket.sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &udp_socket.servaddr,  
            sizeof(udp_socket.servaddr)); 
    std::cout<<"Hello message sent."<<std::endl; 
          
    n = recvfrom(udp_socket.sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &udp_socket.servaddr, 
                &len); 
    buffer[n] = '\0'; 
    std::cout<<"Server :"<<buffer<<std::endl; 
  
    close(udp_socket.sockfd); 
    return 0; 
}
