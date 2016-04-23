#ifndef RESULTSET_H
#define RESULTSET_H
//#include "log4cxx/logger.h"
#include <boost/cstdint.hpp>
#include <vector>
#include "sqlhandle.h"
#include "sqlexception.h"
#include "iresultset.h"
#include "istatement.h"
namespace DataKinesis
{

class ResultSet : public IResultSet
{
	StatementHandle stmtHandle;
	PreparedStatementHandle pstmtHandle;
protected:
    IStatement *stmt;
	virtual IDriver *getDriver();
	virtual IResultSetMetaData *getMetaData_()=0;
public:
	virtual IStatement *getStatement() {return stmt;}
	ResultSet(IStatement *stmt) : stmt(stmt) {}
	virtual ~ResultSet() {}

    virtual void get(bool &data, unsigned int column);
	virtual void get(std::string &data, unsigned int column)=0;
	virtual void get(char &data, unsigned int column);
	virtual void get(boost::int16_t &data, unsigned int column);
	virtual void get(boost::int32_t &data, unsigned int column);
	virtual void get(boost::int64_t &data, unsigned int column);
	virtual void get(boost::uint16_t &data, unsigned int column);
	virtual void get(boost::uint32_t &data, unsigned int column);
	virtual void get(boost::uint64_t &data, unsigned int column);
	virtual void get(std::vector<boost::uint8_t> &data, unsigned int column)=0;

	virtual void get(float &data, unsigned int column);
	virtual void get(double &data, unsigned int column)=0;
	virtual void ownHandle(StatementHandle &statementHandle);
	virtual void ownHandle(PreparedStatementHandle &pstatementHandle);
	virtual ResultSetMetaDataHandle getMetaData();
};
inline IDriver *ResultSet::getDriver() {
	return stmt->getDriver();
}
inline void ResultSet::get(bool &data, unsigned int column) { // should have valid true/false type
    char v;

    get(v,column);
    switch(v) {
        case '1':
        case 'T':
        case 'Y':
        case 't':
        case 'y':
            data=true;
            break;
        default:
            data=false;
            break;
    }
}

inline void ResultSet::get(char &data, unsigned int column) {
	std::string v;
	get(v, column);
	data = v.at(0);
}

inline void ResultSet::get(boost::int16_t &data, unsigned int column) {
	double v;
	get(v, column);
	data = (boost::int16_t)(v + .1);
	if (v - data>1e-9)
		throw SQLException("Rounding issues");
}
inline void ResultSet::get(boost::int32_t &data, unsigned int column) {
    double v;
    get(v,column);
    data=(boost::int32_t)(v+.1);    
    if(v-data>1e-9)
        throw SQLException("Rounding issues");
}
inline void ResultSet::get(boost::int64_t &data, unsigned int column) {
    double v;
    get(v,column);
    data=(boost::int64_t)(v+.1);    
    if(v-data>1e-9)
        throw SQLException("Rounding issues");
}

inline void ResultSet::get(boost::uint16_t &data, unsigned int column) {
	double v;
	get(v, column);
	data = (boost::uint16_t)(v + .1);
	if (v - data>1e-9)
		throw SQLException("Rounding issues");
}
inline void ResultSet::get(boost::uint32_t &data, unsigned int column) {
    double v;
    get(v,column);
    data=(boost::uint32_t)(v+.1);    
    if(v-data>1e-9)
        throw SQLException("Rounding issues");
}
inline void ResultSet::get(boost::uint64_t &data, unsigned int column) {
    double v;
    get(v,column);
    data=(boost::uint64_t)(v+.1);    
    if(v-data>1e-9)
        throw SQLException("Rounding issues");
}

inline void ResultSet::get(float &data, unsigned int column) {
    double v;
    get(v,column);
    data=(float)v;
    if(v-data>1e-9)
        throw SQLException("Rounding issues");    
}
inline void ResultSet::ownHandle(StatementHandle &stmtHandle) {
	this->stmtHandle = stmtHandle;
}
inline void ResultSet::ownHandle(PreparedStatementHandle &pstmtHandle) {
	this->pstmtHandle = pstmtHandle;
}
inline ResultSetMetaDataHandle ResultSet::getMetaData() {
	return ResultSetMetaDataHandle(getMetaData_(),getFreeResultSetMetaData());
}
}

#endif // RESULTSET_H
