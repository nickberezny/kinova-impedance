CC=g++
LOCAL=/home/nick/Documents/Github/kinova-impedance/
INCLUDES=-I/usr/include/eigen3 -I/usr/include/kdl -I/usr/include/ -I$(LOCAL)cxxopts/ -I$(LOCAL)include/ 
K_INCLUDES=-I$(LOCAL)/include/matlab/ -I$(LOCAL)kortex_api/include/ -I$(LOCAL)kortex_api/include/messages -I$(LOCAL)kortex_api/include/client -I$(LOCAL)kortex_api/include/client_stubs -I$(LOCAL)kortex_api/include/common
LIBS=-L./lib/  -L/usr/lib/ -L/home/nick/Documents/Github/kinova-impedance/kortex_api/lib/release/ -lkinovaDynamics -ltinyxml -lurdf -lkdl_parser -lorocos-kdl -lKortexApiCpp -lpthread

test_forcesensor:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/forceSensor.c  src/dataLogger.cpp forceSensorTest.cpp $(LIBS) -o build/testForceSensor

test_matlab:
	sudo g++ src/archive/main.cpp -L./lib/ -lkinovaDynamics -I./include/matlab/ -o main 

torControl:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/01-torque_control_cyclic.cpp $(LIBS) -o build/torControl

ZAdmCartControl:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/inverse_dynamics.cpp src/forceSensor.c src/admittance.cpp src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/admittance_1axis.cpp $(LIBS) -o build/ZAdmCartControl


AdmCartControl:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/filters.cpp src/forceSensor.c src/admittance.cpp src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/admittance_multiaxis.cpp $(LIBS) -o build/AdmCartControl

followTrajectory:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/readCsv.cpp src/forceSensor.c src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/followTrajectory.cpp $(LIBS) -o build/followTrajectory


cartControl:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/forceSensor.c src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/cartControl.cpp $(LIBS) -o build/CartControl

jointJogger:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/forceSensor.c src/safety.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/jointJogger.cpp $(LIBS) -o build/jointJogger

admittance:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/admittance.cpp src/utilities.cpp src/dataLogger.cpp src/inverse_kinematics.cpp src/admittance_main.cpp $(LIBS) -o build/admittance


kortex: 
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES) src/dataLogger.cpp src/utilities.cpp src/01-actuator_low_level_velocity_control.cpp $(LIBS) -o build/kortex

eigen:
	$(CC) $(INCLUDES) eigen_test.cpp -o build/eigen_test

kdl:
	$(CC) $(INCLUDES) src/inverse_kinematics.cpp src/safety.cpp kdl_test.cpp $(LIBS)  -o build/kdl_test

adm_test: 
	$(CC) $(INCLUDES) src/admittance.cpp $(LIBS) -o build/adm_test

forceSensor:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES)  src/forceSensor.c $(LIBS) -o build/force 

kinova:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES)  src/utilities.cpp src/main.cpp $(LIBS) -o build/kinova 

udp:
	$(CC) src/todo/udpClient.cpp -o build/udp

logger:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES)  src/dataLogger.cpp test_log.cpp $(LIBS) -o build/logger






