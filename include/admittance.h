Eigen::MatrixXd constructA(Eigen::VectorXd K, Eigen::VectorXd D, Eigen::VectorXd M, const int dim);
Eigen::MatrixXd discretizeA(Eigen::MatrixXd A, const int dim);
Eigen::MatrixXd discretizeB(Eigen::VectorXd M, Eigen::MatrixXd Ad, Eigen::MatrixXd A, const int dim);