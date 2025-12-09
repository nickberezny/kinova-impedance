#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <fcntl.h>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>

#include <sys/time.h>
#include "../include/forceSensor.h"

void initForceSensorUDP(struct ForceSensorData * forceSensorData)
{

    //forceSensorData->server_addr;
    forceSensorData->server_struct_length = sizeof(forceSensorData->server_addr);
    forceSensorData->socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    int flags = fcntl(forceSensorData->socket_desc, F_GETFL, 0);
    //if (flags == -1) return false;
    flags = (flags | O_NONBLOCK);
    fcntl(forceSensorData->socket_desc, F_SETFL, flags) == 0;
    
    if(forceSensorData->socket_desc < 0){
        printf("Error while creating socket\n");
        return;
    }
    printf("Socket created successfully\n");
    
    forceSensorData->server_addr.sin_family = AF_INET;
    forceSensorData->server_addr.sin_port = htons(10547);
    forceSensorData->server_addr.sin_addr.s_addr = inet_addr("192.168.1.11");
    
    forceSensorData->server_struct_length = sizeof(forceSensorData->server_addr);

    forceSensorData->F[0]=0.0;
    forceSensorData->F[1]=0.0;
    forceSensorData->F[2]=0.0;
    forceSensorData->T[0]=0.0;
    forceSensorData->T[1]=0.0;
    forceSensorData->T[2]=0.0;

}

void startForceSensorStream(struct ForceSensorData * forceSensorData)
{
    forceSensorData->sendData = 256;//33567762; // start stream (0x0002) 

    if(sendto(forceSensorData->socket_desc, &forceSensorData->sendData, 8, 0,
     (struct sockaddr*)&(forceSensorData->server_addr), forceSensorData->server_struct_length) < 0){
    printf("Unable to send message\n");
    }

    sleep(0.1);
    readFroceSensor(forceSensorData);
    sleep(0.1);

    //tareForceSensor(forceSensorData);
    sleep(0.1);

    return;
}

void tareForceSensor(struct ForceSensorData * forceSensorData)
{
    forceSensorData->sendData = 12288;//72057595162014738; //bias set (0x0043) //

    if(sendto(forceSensorData->socket_desc, &forceSensorData->sendData, 8, 0,
     (struct sockaddr*)&(forceSensorData->server_addr), forceSensorData->server_struct_length) < 0){
    printf("Unable to send message\n");
    }

    return;
}


void readForceSensor(struct ForceSensorData * forceSensorData)
{

    int i = 0;

    while(recvfrom(forceSensorData->socket_desc, forceSensorData->msg, 52, 0,
     (struct sockaddr*)&(forceSensorData->server_addr), &(forceSensorData->server_struct_length)) != -1)
    {
        i = i + 1;
        //printf("%d\n",i);
    }

    //printf("%d\n",forceSensorData->msg[1]);

    //uint64_t time_test = recv_buffer[2] + (recv_buffer[3] << 16) + (recv_buffer[4] << 32) + (recv_buffer[5] << 48);
    //int32_t force_test = recv_buffer[15] + (recv_buffer[16] << 16);

    //printf("%lu\n", time_test);
    //printf("%f\n", (double)((int32_t)__builtin_bswap32((uint32_t)force_test))/1000000.0);

    if(i>0)
    {
        forceSensorData->temp = forceSensorData->msg[11] + (forceSensorData->msg[12] << 16);
        forceSensorData->F[0] = (double)((int32_t)__builtin_bswap32((uint32_t)forceSensorData->temp))/1000000.0;

        forceSensorData->temp = forceSensorData->msg[13] + (forceSensorData->msg[14] << 16);
        forceSensorData->F[1] = (double)((int32_t)__builtin_bswap32((uint32_t)forceSensorData->temp))/1000000.0;

        forceSensorData->temp = forceSensorData->msg[15] + (forceSensorData->msg[16] << 16);
        forceSensorData->F[2] = (double)((int32_t)__builtin_bswap32((uint32_t)forceSensorData->temp))/1000000.0;

        forceSensorData->temp = forceSensorData->msg[17] + (forceSensorData->msg[18] << 16);
        forceSensorData->T[0] = (double)((int32_t)__builtin_bswap32((uint32_t)forceSensorData->temp))/1000000.0;

        forceSensorData->temp = forceSensorData->msg[19] + (forceSensorData->msg[20] << 16);
        forceSensorData->T[1] = (double)((int32_t)__builtin_bswap32((uint32_t)forceSensorData->temp))/1000000.0;

        forceSensorData->temp = forceSensorData->msg[21] + (forceSensorData->msg[22] << 16);
        forceSensorData->T[2] = (double)((int32_t)__builtin_bswap32((uint32_t)forceSensorData->temp))/1000000.0;
        /*
        reverseBits(forceSensorData,&(forceSensorData->msg[6]));
        forceSensorData->T[0] = (double)forceSensorData->msg[6]/1000000.0;
        reverseBits(forceSensorData,&(forceSensorData->msg[7]));
        forceSensorData->T[1] = (double)forceSensorData->msg[7]/1000000.0;
        reverseBits(forceSensorData,&(forceSensorData->msg[8]));
        forceSensorData->T[2] = (double)forceSensorData->msg[8]/1000000.0;
        */
    }

    if(forceSensorData->F[2]>100 || forceSensorData->F[2]<-100)
    {
        printf("%d, %f",i,forceSensorData->F[2]);
    }

}