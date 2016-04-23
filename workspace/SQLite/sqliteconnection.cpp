#include <sql/connection.h>
#include <sqliteconnection.h>
#include <sqlitedatabasemetadata.h>
#include <sqlitestatement.h>
#include "sqlite3.h"
#include <sstream>
#include "logger.h"

namespace DataKinesis {
SQLiteConnection::SQLiteConnection(IDriver *driver, const std::string &URL,
		const std::map<std::string, std::string> &properties) :
		Connection(driver, URL, properties) {
	LOG(INFO) << "SQLiteConnection ctor: " << this;
	open(getFlags());
}

SQLiteConnection::~SQLiteConnection() {
	close();
	LOG(INFO) << "SQLiteConnection dtor: " << this;
}

void SQLiteConnection::open(int flags) {
	std::string filename = getURL(); // might need to parse filename out
	int rtn;
	if (flags)
		rtn = sqlite3_open_v2(filename.c_str(), &conn, flags, 0);
	else
		rtn = sqlite3_open(filename.c_str(), &conn);
	if (rtn != SQLITE_OK) {
		std::stringstream ss;
		ss << "SQLiteConnection::_open unable to open " << filename;
		LOG(ERROR) << ss;
		throw SQLException(ss.str());
	}
	setAutoCommit(false);
}
void SQLiteConnection::close() {
	if (!conn) {
		commit();
		sqlite3_close(conn);
		conn = 0;
	}
}
IStatement *SQLiteConnection::createStatement_() {
	SQLiteStatement *stmt = new SQLiteStatement(this);
 	return stmt;
}
IPreparedStatement *SQLiteConnection::createPreparedStatement_(
		const std::string &SQL) {
	SQLiteStatement *pstmt = new SQLiteStatement(this);
	pstmt->setSQL(SQL);
	return pstmt;
}

IDatabaseMetaData *SQLiteConnection::getDatabaseMetaData_() {
	SQLiteDatabaseMetaData *mdata = new SQLiteDatabaseMetaData(this);
 	return mdata;
}

ResultSetHandle SQLiteConnection::getOptions() {
	return executeQuery("pragma compile_options");
}

bool SQLiteConnection::isReadOnly() const {
	return isReadOnly("main");
}

bool SQLiteConnection::isReadOnly(const std::string &databaseName) const {
	return (sqlite3_db_readonly(conn, databaseName.c_str()) == 1);
}

void SQLiteConnection::setReadOnly(bool readOnly) {
	setReadOnly(readOnly, "main");
}

void SQLiteConnection::setReadOnly(bool readOnly,
		const std::string &databaseName) {
	if (readOnly && !isReadOnly()) {
		commit();
		close();
		open(SQLITE_OPEN_READONLY);
	} else if (!readOnly && isReadOnly()) {
		commit();
		close();
		open(SQLITE_OPEN_READWRITE);
	}
}

int SQLiteConnection::getFlags() {
	int rtn = 0;
	if (properties.find("SQLITE_OPEN_READONLY") != properties.end())
		rtn |= 0x00000001;
	if (properties.find("SQLITE_OPEN_READWRITE") != properties.end())
		rtn |= 0x00000002;
	if (properties.find("SQLITE_OPEN_CREATE") != properties.end())
		rtn |= 0x00000004;
	if (properties.find("SQLITE_OPEN_URI") != properties.end())
		rtn |= 0x00000040;
	if (properties.find("SQLITE_OPEN_MEMORY") != properties.end())
		rtn |= 0x00000080;
	if (properties.find("SQLITE_OPEN_NOMUTEX") != properties.end())
		rtn |= 0x00008000;
	if (properties.find("SQLITE_OPEN_FULLMUTEX") != properties.end())
		rtn |= 0x00010000;
	if (properties.find("SQLITE_OPEN_SHAREDCACHE") != properties.end())
		rtn |= 0x00020000;
	if (properties.find("SQLITE_OPEN_PRIVATECACHE") != properties.end())
		rtn |= 0x00040000;
	return rtn;
}

void SQLiteConnection::beginTransaction() {
	if (sqlite3_get_autocommit(conn))
		Connection::beginTransaction();
}

void SQLiteConnection::_free(IStatement *stmt) {
	if (stmt)
		delete stmt;
}

void SQLiteConnection::_free(IPreparedStatement *pstmt) {
	if (pstmt)
		delete pstmt;
}
void SQLiteConnection::_free(IDatabaseMetaData *mdata) {
	if (mdata)
		delete mdata;
}

IConnection::freeStatement SQLiteConnection::getFreeStatement() const {
	return _free;
}
IConnection::freePreparedStatement SQLiteConnection::getFreePreparedStatement() const {
	return _free;
}
IConnection::freeDatabaseMetaData SQLiteConnection::getFreeDatabaseMetaData() const {
	return _free;
}
}
