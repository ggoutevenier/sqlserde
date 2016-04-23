#ifndef SQLEXCEPTION_H
#define SQLEXCEPTION_H

#include <stdexcept>
#include <string>

namespace DataKinesis
{

class SQLException : public std::runtime_error
{
public:
	SQLException(const std::string &msg):runtime_error(msg) {}
//  virtual ~SQLException();
};

}

#endif // SQLEXCEPTION_H
