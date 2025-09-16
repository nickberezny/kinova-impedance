CC=g++
LOCAL=/home/nick/Documents/kinova_impedance/
INCLUDES=-I/usr/include/eigen3 -I/usr/local/include/kdl -I/usr/local/include/ -I$(LOCAL)cxxopts/ -I$(LOCAL)include/ 
K_INCLUDES=-I$(LOCAL)kortex_api/include/ -I$(LOCAL)kortex_api/include/messages -I$(LOCAL)kortex_api/include/client -I$(LOCAL)kortex_api/include/client_stubs -I$(LOCAL)kortex_api/include/common
LIBS=-L/usr/local/lib/ -L/home/nick/Documents/kinova_impedance/kortex_api/lib/release/ -ltinyxml -lurdf -lkdl_parser -lorocos-kdl -lKortexApiCpp -lpthread


eigen:
	$(CC) $(INCLUDES) eigen_test.cpp -o build/eigen_test

kdl:
	$(CC) $(INCLUDES) kdl_test.cpp $(LIBS)  -o build/kdl_test

admittance:
	$(CC) $(INCLUDES) src/admittance.cpp -o build/admittance

kinova:
	$(CC) -D_OS_UNIX $(INCLUDES) $(K_INCLUDES)  src/utilities.cpp src/main.cpp $(LIBS) -o build/kinova 
run:
	./build/admittance




