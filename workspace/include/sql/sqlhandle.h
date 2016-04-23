#ifndef SQLHANDLE_H
#define SQLHANDLE_H
#include <memory>

namespace DataKinesis
{
	class IConnection;
	class IDriver;
	class IStatement;
	class IPreparedStatement;
	class IDatabaseMetaData;
	class IResultSetMetaData;
	class IResultSet;
	class SqlDialect;

	typedef std::shared_ptr<IConnection> ConnectionHandle;
	typedef std::shared_ptr<IStatement> StatementHandle;
	typedef std::shared_ptr<IPreparedStatement> PreparedStatementHandle;
	typedef std::shared_ptr<IDatabaseMetaData> DatabaseMetaDataHandle;
	typedef std::shared_ptr<IResultSetMetaData> ResultSetMetaDataHandle;
	typedef std::shared_ptr<IResultSet> ResultSetHandle;
	typedef std::shared_ptr<IDriver> DriverHandle;
}
#endif
