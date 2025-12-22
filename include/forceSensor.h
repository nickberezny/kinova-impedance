struct ForceSensorData {
	int socket_desc;
	struct sockaddr_in server_addr;
	socklen_t server_struct_length;
	int16_t msg[44];
	int32_t temp;
 	uint64_t sendData;
	double F[3];
 	double T[3];
};

void estimateForceFromTorque(double tau, double offset,  double * q, double pi, double * Fest);
void initForceSensorUDP(struct ForceSensorData * forceSensorData);
void startForceSensorStream(struct ForceSensorData * forceSensorData);
void readForceSensor(struct ForceSensorData * forceSensorData);
void tareForceSensor(struct ForceSensorData * forceSensorData);