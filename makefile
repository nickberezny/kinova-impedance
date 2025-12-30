CC=g++
LOCAL=/home/nick/Documents/Github/kinova-impedance/
INCLUDES=-I/usr/include/eigen3 -I/usr/include/kdl -I/usr/include/ -I$(LOCAL)cxxopts/ -I$(LOCAL)include/ 
K_INCLUDES=-I$(LOCAL)/include/matlab/ -I$(LOCAL)kortex_api/include/ -I$(LOCAL)kortex_api/include/messages -I$(LOCAL)kortex_api/include/client -I$(LOCAL)kortex_api/include/client_stubs -I$(LOCAL)kortex_api/include/common
LIBS=-L./lib/  -L/usr/lib/ -L/home/nick/Documents/Github/kinova-impedance/kortex_api/lib/release/ -lkinovaDynamics -ltinyxml -lurdf -lkdl_parser -lorocos-kdl -lKortexApiCpp -lpthread

impedanceTorque:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/filters.cpp src/inverse_dynamics.cpp src/forceSensor.c src/admittance.cpp src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/controllers/impedance_torque.cpp $(LIBS) -o build/impedanceTorque

impedanceSMC:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/computed_torque.cpp src/filters.cpp src/inverse_dynamics.cpp src/forceSensor.c src/admittance.cpp src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/controllers/impedance_sliding.cpp $(LIBS) -o build/impedanceSMC


ZAdmCartControl:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/filters.cpp src/inverse_dynamics.cpp src/forceSensor.c src/admittance.cpp src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/controllers/admittance_1axis.cpp $(LIBS) -o build/ZAdmCartControl

collectData:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/inverse_dynamics.cpp src/forceSensor.c src/admittance.cpp src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/controllers/collectData.cpp $(LIBS) -o build/collectData


AdmCartControl:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/filters.cpp src/forceSensor.c src/admittance.cpp src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/controllers/admittance_multiaxis.cpp $(LIBS) -o build/AdmCartControl

followTrajectory:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/readCsv.cpp src/forceSensor.c src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/controllers/followTrajectory.cpp $(LIBS) -o build/followTrajectory

testKin: 
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/inverse_kinematics.cpp simple_kin_test.cpp $(LIBS) -o build/kinTest



