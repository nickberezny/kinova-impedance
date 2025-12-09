KDL::Chain loadKDLChain(std::string urdf_path);
void torqueToForce(double *tau[2], double *q[3], double *l[3], double *Fz);
void simplifiedInverseKinematics(double * q[3], double * X[2], double * phi, double * l[3]);
void simplifiedForwardKinematics(double * q[3], double * X[2], double * phi, double * l[3]);