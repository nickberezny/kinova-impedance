CC=g++
INCLUDES=-I/mnt/c/Users/Nick/Documents/Github/kinova_impedance/eigen-3.4.0/

eigen:
	$(CC) $(INCLUDES) eigen_test.cpp -o eigen_test

admittance:
	$(CC) $(INCLUDES) src/admittance.cpp -o admittance

run:
	./admittance




