//#define LOGGER_LIBRARY
#define ELPP_NO_DEFAULT_LOG_FILE
#include "logger.h"

INITIALIZE_EASYLOGGINGPP
namespace DataKinesis {
int Logger::runOnceInd = runOnce();
int Logger::runOnce() {
	LOG(INFO) << "Registered logger";
	el::Loggers::getLogger("Logger");
	return 0;
}
Logger::Logger() {
	LOG(INFO) << "Logger ctor";
}
Logger::Logger(int argc, char** argv) {
	START_EASYLOGGINGPP(argc,argv);
	LOG(INFO) << "Logger ctor(argc,argv)";
}
Logger::~Logger() {
	LOG(INFO) << "Logger dtor";
}
void Logger::event(int a) {
	VLOG(1) << "Logger::event start";
	LOG(INFO) << "Process event["<<a<<"]";
	VLOG(1) << "Logger::event end";
}
el::base::type::StoragePointer Logger::getStorage() {
	return el::Helpers::storage();
}
}

