#ifndef ICONNECTION_H
#define ICONNECTION_H
#include <memory>
#include <string>
#include <map>
//#include "log4cxx/logger.h"
#include "istatement.h"
#include "resultset.h"
#include "sqlhandle.h"
//#include "logger.h"

namespace DataKinesis
{
	class Statement;
	class DatabaseMetaData;
	class IConnection
	{
		friend class Statement;
		friend class DatabaseMetaData;
	protected:
		virtual IDriver *getDriver() = 0;
		typedef void(*freeStatement)(IStatement *);
		typedef void(*freePreparedStatement)(IPreparedStatement *);
		typedef void(*freeDatabaseMetaData)(IDatabaseMetaData *);
		virtual freeStatement getFreeStatement() const = 0;
		virtual freePreparedStatement getFreePreparedStatement() const = 0;
		virtual freeDatabaseMetaData getFreeDatabaseMetaData() const = 0;

		virtual const std::map<std::string, std::string> &getProperties() const = 0;
		virtual const std::string &getURL() const = 0;
	public:    


		virtual void execute(const std::string &SQL)=0;
		virtual ResultSetHandle executeQuery(const std::string &SQL)=0;    
		IConnection() {};
		virtual ~IConnection() {};

		virtual void close()=0;
		virtual bool getAutoCommit() const=0;
		virtual void setAutoCommit(bool autoCommit)=0;
		virtual void commit()=0;
		virtual void rollback()=0;
		virtual DatabaseMetaDataHandle getDatabaseMetaData()=0;
		virtual bool isReadOnly() const=0;
		virtual void setReadOnly(bool readOnly)=0;
		virtual StatementHandle createStatement()=0;
		virtual PreparedStatementHandle createPreparedStatement(const std::string &sql) = 0;
		virtual void beginTransaction() = 0;
		virtual void endTransaction() = 0;
	};
}
#endif
