#ifndef DATABASEMETADATA_H
#define DATABASEMETADATA_H
#include "driver.h"
#include "connection.h"
#include "statement.h"
namespace DataKinesis
{
	class IDatabaseMetaData
	{
	protected:
	public:
		IDatabaseMetaData();
		virtual ~IDatabaseMetaData();
		virtual IDriver *getDriver() = 0;
		virtual IConnection *getConnection()=0;
		virtual ResultSetHandle getTables(
			const std::string &catalogName,
			const std::string &schegmaName, 
			const std::string &tableNamePattern
			)=0;
	};

	class DatabaseMetaData : public IDatabaseMetaData
	{
		//	ConnectionHandle connHandle;
	protected:
//StatementHandle stmt;
		IConnection *conn;
	public:
		virtual IDriver *getDriver() { return getConnection()->getDriver(); }
		DatabaseMetaData(IConnection *conn);
		virtual ~DatabaseMetaData();
		virtual IConnection *getConnection();
	};
	inline IDatabaseMetaData::IDatabaseMetaData()
	{
	}

	inline IDatabaseMetaData::~IDatabaseMetaData()
	{
	}

	inline DatabaseMetaData::DatabaseMetaData(IConnection*conn)
		: conn(conn) {
//	stmt = this->conn->createStatement();
	}

	inline DatabaseMetaData::~DatabaseMetaData()
	{
	}

	inline IConnection *DatabaseMetaData::getConnection() {
		return conn;
	}

}


#endif // DATABASEMETADATA_H
