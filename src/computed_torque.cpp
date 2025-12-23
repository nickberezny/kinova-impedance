#include "kdl/jacobian.hpp"
#include "kdl/tree.hpp"
#include "kdl/chain.hpp"
#include "kdl/frames.hpp"
#include "kdl/jntarray.hpp"
#include "kdl/chainiksolverpos_lma.hpp"
#include <kdl_parser/kdl_parser.hpp>
#include "kdl/chainjnttojacsolver.hpp"
#include "kdl/chaindynparam.hpp"
#include <chainfksolvervel_recursive.hpp>
#include <chainfksolverpos_recursive.hpp>

#include "kdl/chainiksolvervel_pinv.hpp"
#include <stdio.h>
#include <iostream>
#include <cmath> 

#include "../include/structs.h"
#include "../include/inverse_kinematics.h"
 
using Eigen::MatrixXd;
using Eigen::VectorXd;

void SMC(double *dz, double *zs, double Ks, double *Fa)
{
	//calc sigma

	*Fa = - Ks*((*dz-*zs > 0) - (*dz-*zs < 0)); //K*sgn(sigma) 
	*Fa = -Ks*(tanh((*dz-*zs)/50.0)); // K*tanh(sigma/phi)
	//force to torque
	//add tau_est

}

void slidingSurface(double *z, double *z0, double *zs, double *dz, double *Fz, double *Kd, double *Bd, double *Md, double *z_int, double F_int)
{
	*z_int = *z_int + 0.001*(*z-*z0);
	*F_int = *F_int + 0.001*(*Fz);

	*zs = *z0 - (*Bd/ *Md)*(*z-*z0) - (*Kd/ *Md)*(*z_int) + (1.0/ *Md)*(*F_int); 
	return;
}