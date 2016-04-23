#ifndef RESULTSETMETATDATA_H
#define RESULTSETMETATDATA_H
#include <string>

#include "resultset.h"
namespace DataKinesis
{

class IResultSetMetaData
{
protected:
    virtual IStatement *getStatement()=0;
public:
	IResultSetMetaData() {}
	virtual ~IResultSetMetaData() {}
    virtual unsigned int getColumnCount()=0;
    virtual std::string getColumnName(unsigned int column)=0;
    virtual std::string getTableName(unsigned int column)=0;
};
class ResultSetMetaData : public IResultSetMetaData
{
protected:
    IResultSet *rset;
	IStatement *getStatement() {return rset->getStatement();}
public:
	ResultSetMetaData(IResultSet *rset):rset(rset) {}
	virtual ~ResultSetMetaData() {}
};
}

#endif // RESULTSETMETATDATA_H
