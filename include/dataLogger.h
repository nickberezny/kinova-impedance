void openLogFile(std::ofstream * outputFile);
void writeDataToLog(std::ofstream * outputFile, const Kinova::Api::BaseCyclic::Feedback data, struct ForceSensorData * forceSensorData, int64_t now);