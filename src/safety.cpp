#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath> 

#include <Eigen/Dense>

using Eigen::VectorXd;

bool checkCommandAngle(VectorXd qcmd, VectorXd qprev, int numJnts, double thresh)
{
	for(int i = 0; i < numJnts; i++)
	{
		if(std::abs(qcmd(i)-qprev(i)) > thresh)
		{
			return false;
		}
	}

	return true;

}