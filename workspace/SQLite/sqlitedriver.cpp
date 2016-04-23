#include "sqliteconnection.h"
#include "sqlitedriver.h"
#include "sqlitedialect.h"
#include "sql/connection.h"
#include "sql/statement.h"
#include "sql/resultset.h"
#include "sql/resultsetmetadata.h"
#include "sql/databasemetadata.h"

namespace DataKinesis {

SQLiteDriver::SQLiteDriver() {
}

SQLiteDriver::~SQLiteDriver() {
}

IConnection *SQLiteDriver::createConnection_(const std::string url,
		const std::map<std::string, std::string> &properties) {
	return new SQLiteConnection(this, url, properties);
}

void SQLiteDriver::freeConnection_(IConnection *conn) {
	if (conn)
		delete conn;
}

IDriver::freeConnection SQLiteDriver::getFreeConnection() const {
	return freeConnection_;
}
}



