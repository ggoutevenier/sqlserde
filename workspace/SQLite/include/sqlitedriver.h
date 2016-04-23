#ifndef SQLITEDRIVER_H
#define SQLITEDRIVER_H
#include "sql/driver.h"
namespace DataKinesis
{
//	class Statement;
//	class PreparedStatement;
//	class Connection;
//	class ResultSet;
//	class DatabaseMetaData;
//	class ResultSetMetaData;
	class SQLiteDriver : public Driver
	{
	protected:
		virtual IConnection *createConnection_(const std::string url, const std::map<std::string, std::string> &properties);
		static void freeConnection_(IConnection *);
	public:
		SQLiteDriver();
		~SQLiteDriver();
		virtual freeConnection getFreeConnection() const;
	};

}

#endif // SQLITEDRIVER_H
