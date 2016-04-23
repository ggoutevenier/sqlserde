#ifndef SQLITEDATABASEMETADATA_H
#define SQLITEDATABASEMETADATA_H

#include "sql/databasemetadata.h"
#include "sql/statement.h"

namespace DataKinesis
{
	class SQLiteConnection;
class SQLiteDatabaseMetaData : public DatabaseMetaData
{
public:
    SQLiteDatabaseMetaData(Connection *conn);    
    virtual ~SQLiteDatabaseMetaData();
    ResultSetHandle getTables(const std::string &catalogName,const std::string &schemaName, const std::string &tableNamePattern);
	IConnection *getConnection();
};

}

#endif // SQLITEDATABASEMETADATA_H
