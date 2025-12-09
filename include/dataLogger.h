void openLogFile(std::ofstream * outputFile);
void writeDataToLog(std::ofstream * outputFile, const Kinova::Api::BaseCyclic::Feedback data, struct ForceSensorData * forceSensorData, double x, double y, double z, int64_t now);
void writeAllData(std::ofstream * outputFile, double ** q, double ** dq, double *z, double * Fsense, double * Fest, double * iact, int len);
