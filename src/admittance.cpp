#include <iostream>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>
#include "../include/admittance.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

/*
int main() {

	VectorXd K(2);
	K << 2.0, 3.5;
	VectorXd D(2);
	D << 1.0, 4.5;
	VectorXd M(2);
	M << 0.5, 0.75;

	MatrixXd A(4, 4);
	MatrixXd Ad(4, 4);
	VectorXd Bd(4);

	A = constructA(K,D,M,2);
	Ad = discretizeA(A,2);
	std::cout << Ad << std::endl;

	Bd = discretizeB(M,Ad,A,2);
	std::cout << Bd << std::endl;

}
*/

Eigen::VectorXd virtualTrajectory(Eigen::MatrixXd Ad, Eigen::MatrixXd Bd, Eigen::VectorXd F, Eigen::VectorXd X,Eigen::VectorXd X0)
{
	return X0+Ad*(X-X0) + Bd*F;
}

Eigen::MatrixXd discretizeB(Eigen::VectorXd M, Eigen::MatrixXd Ad, Eigen::MatrixXd A, const int dim)
{
	MatrixXd B(2*dim,dim);
	Eigen::MatrixXd I = Eigen::MatrixXd::Identity(2*dim, 2*dim);

	for(int i = 0; i < dim; i++)
	{
		for(int j = 0; j < 2*dim; j++)
		{
			if(j==2*i+1) B(j,i) = 1/M(i);
			else B(j,i) = 0.0;
		}

	}

	return (A.inverse())*(Ad - I)*B;
}

Eigen::MatrixXd discretizeA(Eigen::MatrixXd A, const int dim)
{
	//discretize admittance system with parameters K, B, M
	//dim = number of cartesian dimensions in defintion of addmitance (number of states = 2*dim)

	return (0.001*A).exp(); 
}

Eigen::MatrixXd constructA(Eigen::VectorXd K, Eigen::VectorXd D, Eigen::VectorXd M, const int dim)
{
	MatrixXd A(2*dim,2*dim);

	for(int i = 0; i < dim; i++)
	{
		for(int j = 0; j < 2*dim; j++)
		{
			if(j == 2*i+1) A(2*i,j) = 1.0;
			else A(2*i,j) = 0.0;
		}

		for(int j = 0; j < 2*dim; j++)
		{
			if(j == 2*i+1) A(2*i+1,j) = -D(i)/M(i);
			else if(j == 2*i) A(2*i+1,j) = -K(i)/M(i);
			else A(2*i+1,j) = 0.0;
		}
	}

	return A;
}