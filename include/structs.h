

struct dynamicModel {
	Eigen::MatrixXd M;
	Eigen::MatrixXd C;
	Eigen::VectorXd G;
};

struct states {
	Eigen::VectorXd x;
	Eigen::VectorXd dx;
	Eigen::VectorXd F;
};

struct equilibriumTrajectory {
	Eigen::VectorXd x0;
	Eigen::VectorXd dx0;
	Eigen::VectorXd ddx0;
};

struct desiredImpedance {
	Eigen::MatrixXd Md;
	Eigen::MatrixXd Bd;
	Eigen::MatrixXd Kd;
};

struct kdl_solvers {
	std::unique_ptr<KDL::ChainIkSolverPos_LMA> IK_solver;
	std::unique_ptr<KDL::ChainFkSolverPos_recursive> FK_solver_pos;
	std::unique_ptr<KDL::ChainFkSolverVel_recursive> FK_solver_vel;
	std::unique_ptr<KDL::ChainJntToJacSolver> Jac_solver;

    kdl_solvers(KDL::Chain chain_) 
    {
    	IK_solver = std::make_unique<KDL::ChainIkSolverPos_LMA>(chain_);
	    FK_solver_pos = std::make_unique<KDL::ChainFkSolverPos_recursive>(chain_);
	    FK_solver_vel = std::make_unique<KDL::ChainFkSolverVel_recursive>(chain_);
	    Jac_solver = std::make_unique<KDL::ChainJntToJacSolver>(chain_);
    }

};

