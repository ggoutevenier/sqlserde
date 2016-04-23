#ifndef IRESULTSET_H
#define IRESULTSET_H
//#include "log4cxx/logger.h"
#include <cstdint>
#include <vector>
#include "sqlhandle.h"
#include "sqlexception.h"

namespace DataKinesis
{
class IResultSet
{
protected:
	virtual IDriver *getDriver() = 0;
	typedef void(*freeResultSetMetaData)(IResultSetMetaData *);
	virtual freeResultSetMetaData getFreeResultSetMetaData() const = 0;
public:
	IResultSet() {}
	virtual ~IResultSet() {}
    virtual IStatement *getStatement()=0;
    virtual bool next()=0;
    virtual bool completed()=0;
    virtual void close() =0;
    virtual size_t getColumnSize(unsigned int column)=0;
    virtual void get(bool &data, unsigned int column)=0;
	virtual void get(std::string &data, unsigned int column)=0;
	virtual void get(char &data, unsigned int column) = 0;
	virtual void get(boost::int16_t &data, unsigned int column)=0;
	virtual void get(boost::int32_t &data, unsigned int column)=0;
	virtual void get(boost::int64_t &data, unsigned int column)=0;

	virtual void get(boost::uint16_t &data, unsigned int column) = 0;
	virtual void get(boost::uint32_t &data, unsigned int column)=0;
	virtual void get(boost::uint64_t &data, unsigned int column)=0;
	virtual void get(std::vector<boost::uint8_t> &data, unsigned int column) = 0;

	virtual void get(float &data, unsigned int column)=0;
	virtual void get(double &data, unsigned int column)=0;
	virtual void get(tm &data, unsigned int column)=0;
    virtual ResultSetMetaDataHandle getMetaData()=0;
	virtual void ownHandle(StatementHandle &stmtHandle) = 0;
	virtual void ownHandle(PreparedStatementHandle &pstmtHandle) = 0;
};
}
#endif
