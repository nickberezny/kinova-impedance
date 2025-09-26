#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath> 

#include <Eigen/Dense>

using Eigen::VectorXd;

bool checkCommandAngle(VectorXd qcmd, VectorXd qprev, int numJnts, double thresh_low, double thresh_high)
{
	for(int i = 0; i < 4; i++)
	{
		if(std::abs(qcmd(i)-qprev(i)) > thresh_low)
		{
			return false;
		}
	}

	for(int i = 4; i < numJnts; i++)
	{
		if(std::abs(qcmd(i)-qprev(i)) > thresh_high)
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