#include <iostream>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>
#include "../include/structs.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

struct momentumStates {
	Eigen::VectorXd p;
	Eigen::VectorXd dp;
	Eigen::MatrixXd L;
	Eigen::VectorXd Fest;
};

void generalMomentum(struct dynamicModel model, struct states s, struct momentumStates * sp, Eigen::MatrixXd torque)
{
	sp->dp = (model.C).transpose()*s.dx - model.G + torque + sp->L*(sp->p-model.M*s.dx);
	sp->p = sp->p + sp->dp*0.001;
	sp->Fest = -sp->L*(sp->p-model.M*s.dx);
}

