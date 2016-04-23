#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <memory>
#include <map>
#include "idriver.h"


namespace DataKinesis {

class Driver : public IDriver
{
private:
protected:
	virtual IConnection *createConnection_(const std::string url, const std::map<std::string, std::string> &properties) = 0;
public:
	Driver() {}
	virtual ~Driver() {}
	
	virtual ConnectionHandle createConnection(const std::string url) {
		return createConnection(url, std::map<std::string, std::string>());
	}
	virtual ConnectionHandle createConnection(const std::string url, const std::map<std::string, std::string> &properties) {
		return ConnectionHandle(createConnection_(url,properties), getFreeConnection());
	};
};

}

#endif // DRIVER_H
