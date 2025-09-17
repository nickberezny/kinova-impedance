#include <iostream>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>

#include "../include/admittance.h"
#include "../include/structs.h"
#include "../include/impedance.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;


int main() {

	return 0;

}


Eigen::VectorXd computedTorque(struct dynamicModel model, struct states s, Eigen::VectorXd xstar)
{
	//computed-torque (cartesian space)
	//returns command torque 

	return model.M*xstar + model.C*s.dx + model.G - s.F; 

}


Eigen::VectorXd impedance(Eigen::MatrixXd M, struct desiredImpedance desImp, struct states s, struct equilibriumTrajectory s0)
{
	//returns xstar

	//inverse too slow??
	return M.inverse()*(s0.ddx0 - desImp.Bd*(s.dx-s0.dx0) - desImp.Kd*(s.x-s0.x0) + s.F);
}



//sliding mode control