#ifndef IDRIVER_H
#define IDRIVER_H

#include <string>
#include <memory>
//#include <boost/log/sources/severity_logger.hpp>
#include <map>
#include "sql/sqlhandle.h"

namespace DataKinesis {    
class IDriver
{
protected:
	typedef void(*freeConnection)(IConnection *);
	virtual freeConnection getFreeConnection() const = 0;
public:
	IDriver() {}
	virtual ~IDriver() {}
    virtual ConnectionHandle createConnection(const std::string url) = 0;
    virtual ConnectionHandle createConnection(const std::string url,const std::map<std::string,std::string> &properties) = 0;
};

}
#endif
