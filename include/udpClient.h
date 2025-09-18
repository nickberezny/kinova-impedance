struct UDPSocket {
    int sockfd; 
    struct sockaddr_in servaddr;
    unsigned int server_struct_length;
    int32_t msg[9];
    int32_t temp[4];
    uint64_t sendData;
    double F[3];
    double T[3];
};



void openUDP(struct UDPSocket * socket);
void tareForceSensor(struct UDPSocket * udp_socket);
void startForceSensorStream(struct UDPSocket * udp_socket);
void stopForceSensorStream(struct UDPSocket * udp_socket);
void reverseBits(struct UDPSocket * udp_socket, int32_t * num);
void readForceSensor(struct UDPSocket * udp_socket);