bool checkCommandAngle(Eigen::VectorXd qcmd, Eigen::VectorXd qprev, int numJnts, double thresh_low, double thresh_high);
bool checkVelocities(Eigen::VectorXd dq, int numJnts, double thresh);
bool checkCartPos(double x, double y, double z);
bool checkCommandAngleSimplified(double * qcmd, double * qprev, double thresh);