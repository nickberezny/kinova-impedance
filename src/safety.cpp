#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath> 

#include <Eigen/Dense>

using Eigen::VectorXd;

bool checkCartPos(double x, double y, double z)
{
	//double xb[2] = {0.37,0.65};
	//double yb[2] = {-0.23,0.23};
	//double zb[2] = {0.31,0.63};
	/*
	if(x < 0.5 || x > 0.6) return false;
	if(y < -0.1 || y > 0.1) return false;
	if(z < 0.038 || z > 0.4) return false;
	*/
	if(x < -0.6 || x > -0.2) return false;
	if(y < -0.23 || y > 0.23) return false;
	if(z < 0.2 || z > 0.6) return false;
	

	return true;
}

bool checkCommandAngle(VectorXd qcmd, VectorXd qprev, int numJnts, double thresh_low, double thresh_high)
{
	for(int i = 0; i < 4; i++)
	{
		if(std::abs(qcmd(i)-qprev(i)) > thresh_low && std::abs(qcmd(i)+6.2831853-qprev(i)) > thresh_low && std::abs(qcmd(i)-6.2831853-qprev(i)) > thresh_low)
		{
			return false;
		}
	}

	for(int i = 4; i < numJnts; i++)
	{
		if(std::abs(qcmd(i)-qprev(i)) > thresh_high && std::abs(qcmd(i)+6.2831853-qprev(i)) > thresh_high && std::abs(qcmd(i)-6.2831853-qprev(i)) > thresh_high)
		{
			return false;
		}
	}

	return true;

}

bool checkVelocities(VectorXd dq, int numJnts, double thresh)
{
	for(int i = 0; i< numJnts; i++)
	{
		if(std::abs(dq(i)) > thresh)
		{
			return false;
		}
	}

	return true;
}