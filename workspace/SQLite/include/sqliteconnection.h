#ifndef SQLITECONNECTION_H
#define SQLITECONNECTION_H

#include "sql/connection.h"
#include "sql/statement.h"
#include "sqlitedriver.h"
#include "sql/sqlhandle.h"

extern "C" {
typedef struct sqlite3 sqlite3;
}

namespace DataKinesis
{
class SQLiteStatement;
class SQLiteResultSet;
class SQLiteConnection : public Connection
{
    friend class SQLiteStatement;
    friend class SQLiteResultSet;
    void open(int flags);
    void close();
	sqlite3  *conn;
    int getFlags();

	static void _free(IStatement *);
	static void _free(IPreparedStatement *);
	static void _free(IDatabaseMetaData *);

	virtual freeStatement getFreeStatement() const;
	virtual freePreparedStatement getFreePreparedStatement() const;
	virtual freeDatabaseMetaData getFreeDatabaseMetaData() const;

	virtual IStatement *createStatement_();
	virtual IPreparedStatement *createPreparedStatement_(const std::string &SQL);
	virtual IDatabaseMetaData *getDatabaseMetaData_();
public:
    SQLiteConnection(IDriver *driver,const std::string &URL,const std::map<std::string,std::string> &properties);
    virtual ~SQLiteConnection();
	ResultSetHandle getOptions();
    bool isReadOnly(const std::string &databaseName) const;
    bool isReadOnly() const;
    void setReadOnly(bool readOnly);
    void setReadOnly(bool readOnly,const std::string &databaseName);
	virtual void beginTransaction();
};

}

#endif // SQLITECONNECTION_H
