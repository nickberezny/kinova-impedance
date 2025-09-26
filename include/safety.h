bool checkCommandAngle(Eigen::VectorXd qcmd, Eigen::VectorXd qprev, int numJnts, double thresh_low, double thresh_high);
bool checkVelocities(Eigen::VectorXd dq, int numJnts, double thresh);