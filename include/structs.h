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
