#ifndef SQLITERESULTSETMETADATA_H
#define SQLITERESULTSETMETADATA_H

#include "sql/resultsetmetadata.h"
namespace DataKinesis
{
class SQLiteStatement;
class SQLiteResultSet;
class SQLiteResultSetMetaData : public DataKinesis::ResultSetMetaData
{
protected:
    SQLiteStatement *getSQLiteStatement();
public:
    SQLiteResultSetMetaData(IResultSet *rset);
    virtual ~SQLiteResultSetMetaData();
    unsigned int getColumnCount();
    std::string getColumnName(unsigned int column);
//    int getColumnType()=0;
//   virtual  std::string getColumnTypeName()=0;
//  virtual std::string getSchemaName()=0;
    std::string getTableName(unsigned int column);
//  virtual bool isNullable()=0;

};

}

#endif // SQLITERESULTSETMETADATA_H
