#ifndef CONNECTION_H
#define CONNECTION_H
#include <memory>
#include <string>
#include <map>
#include "istatement.h"
#include "iconnection.h"
#include "idriver.h"
#include "serde/sqldialect.h"
namespace DataKinesis
{
	class Connection : public IConnection
	{
	protected:
		IDriver *driver;
		std::map<std::string, std::string> properties;
		std::string url;
		bool autoCommit;
		virtual IStatement *createStatement_() = 0;
		virtual IPreparedStatement *createPreparedStatement_(const std::string &SQL) = 0;
		virtual IDatabaseMetaData *getDatabaseMetaData_() = 0;
		IDriver *getDriver() { return driver; }
		const std::map<std::string, std::string> &getProperties() const { return properties; }
		const std::string &getURL() const { return url; }
	public:
		Connection(IDriver *driver,const std::string &URL,const std::map<std::string,std::string> &properties):
		  driver(driver),properties(properties),url(URL),autoCommit(false) {}
		  virtual ~Connection() {}

		  virtual void commit();
		  virtual void rollback();
		  virtual ResultSetHandle executeQuery(const std::string &SQL); 
		  virtual void beginTransaction();
		  virtual void endTransaction();
		  virtual bool getAutoCommit() const;
		  virtual void setAutoCommit(bool autoCommit);
		  virtual void execute(const std::string &sql);
		  virtual bool isReadOnly() const { return false; }
		  virtual void setReadOnly(bool) {}
		  StatementHandle createStatement() {
			  return StatementHandle(createStatement_(), getFreeStatement());
		  }
		  PreparedStatementHandle createPreparedStatement(const std::string &SQL) {
			  return PreparedStatementHandle(createPreparedStatement_(SQL), getFreePreparedStatement());
		  }
		  DatabaseMetaDataHandle getDatabaseMetaData() {
			  return DatabaseMetaDataHandle(getDatabaseMetaData_(), getFreeDatabaseMetaData());
		  }
	};
	inline void Connection::execute(const std::string &SQL) {
		StatementHandle stmt = createStatement();
		stmt->execute(SQL);
	}
	inline ResultSetHandle Connection::executeQuery(const std::string &SQL) {
		StatementHandle stmt = createStatement();
		ResultSetHandle rset = stmt->executeQuery(SQL);
		rset->ownHandle(stmt);
		return rset;
	}

	inline void Connection::commit() {
		if(!getAutoCommit()) {
			execute("commit");
			beginTransaction();
		}
	}
	inline void Connection::rollback() {
		if (getAutoCommit()) {
			throw SQLException("Cannot rollback autocommit connection");
		}
		else {
			execute("rollback");
			beginTransaction();
		}
	}

	inline void Connection::beginTransaction() { 
		execute("begin transaction");
	}

	inline void Connection::endTransaction() {
		execute("end transaction");
	}

	inline bool Connection::getAutoCommit() const {
		return autoCommit;
	}
	inline void Connection::setAutoCommit(bool autoCommit) {
		this->autoCommit = autoCommit;
		if(autoCommit)
			commit();
		else
			beginTransaction();
	}
}
#endif
