Eigen::VectorXd computedTorque(struct dynamicModel model, struct states s, Eigen::VectorXd xstar);
Eigen::VectorXd impedance(Eigen::MatrixXd M, struct desiredImpedance desImp, struct states s, struct equilibriumTrajectory s0);
