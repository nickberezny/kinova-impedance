#include <iostream>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>

Eigen::MatrixXd constructA(Eigen::VectorXd K, Eigen::VectorXd D, Eigen::VectorXd M, const int dim);
Eigen::MatrixXd discretizeA(Eigen::MatrixXd A, const int dim);
Eigen::MatrixXd discretizeB(Eigen::VectorXd M, Eigen::MatrixXd Ad, Eigen::MatrixXd A, const int dim);

using Eigen::MatrixXd;
using Eigen::VectorXd;

int main() {

	VectorXd K(1);
	K << 0.01;
	VectorXd D(1);
	D << 100.0;
	VectorXd M(1);
	M << 1.0;

	MatrixXd A(2, 2);
	MatrixXd Ad(2, 2);
	VectorXd Bd(2);

	VectorXd test(4);
	test << 1.0,2.0,3.0,4.0;
	std::cout << test << std::endl;

	for(int i = 3; i > 0; i--)
	{
		test(i) = test(i-1);
	}
	
	std::cout << test << std::endl;
	/*

	A = constructA(K,D,M,1);
	std::cout << A << std::endl;
	Ad = discretizeA(A,1);
	std::cout << Ad << std::endl;

	Bd = discretizeB(M,Ad,A,1);
	std::cout << Bd << std::endl;

	VectorXd Xv(2);
	Xv << 1.0,0.0;

	VectorXd F(1);
	F << 0.0;

	std::cout << Xv << std::endl;
	Xv = Ad*Xv;
	std::cout << Xv << std::endl;
	*/

}

Eigen::VectorXd virtualTrajectory(Eigen::MatrixXd Ad, Eigen::VectorXd Bd, Eigen::VectorXd F, Eigen::VectorXd X,Eigen::VectorXd X0)
{
	return X0 + Ad*(X-X0) + Bd*F;
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
			if(j == 2*i+1) A(2*i+1,j) = -D(i)/M(i);
			else if(j == 2*i) A(2*i+1,j) = -K(i)/M(i);
			else A(2*i+1,j) = 0.0;
		}
	}

	return A;
}