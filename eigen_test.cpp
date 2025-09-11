#include <iostream>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>

Eigen::MatrixXd constructA(Eigen::VectorXd K, Eigen::VectorXd D, Eigen::VectorXd M, const int dim);
Eigen::MatrixXd discretizeA(Eigen::MatrixXd A, const int dim);
Eigen::MatrixXd discretizeB(Eigen::VectorXd M, Eigen::MatrixXd Ad, Eigen::MatrixXd A, const int dim);

using Eigen::MatrixXd;
using Eigen::VectorXd;

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

Eigen::VectorXd virtualTrajectory(Eigen::MatrixXd Ad, Eigen::VectorXd Bd, Eigen::VectorXd F, Eigen::VectorXd X)
{
	return Ad*X + Bd*F;
}

Eigen::MatrixXd discretizeB(Eigen::VectorXd M, Eigen::MatrixXd Ad, Eigen::MatrixXd A, const int dim)
{
	VectorXd B(2*dim);
	Eigen::MatrixXd I = Eigen::MatrixXd::Identity(2*dim, 2*dim);

	for(int i = 0; i < dim; i++)
	{
		B(2*i) = 0.0;
		B(2*i+1) = 1/M(i);
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
			if(j == 2*i) A(2*i+1,j) = -D(i)/M(i);
			else if(j == 2*i+1) A(2*i+1,j) = -K(i)/M(i);
			else A(2*i+1,j) = 0.0;
		}
	}

	return A;
}