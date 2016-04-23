#ifndef STATEMENT_H
#define STATEMENT_H
#include <memory>
#include <cstdint>
#include <string>
#include <vector>

#include "typeops.h"
#include "iconnection.h"
#include "resultset.h"

namespace DataKinesis
{
	class Statement : public IPreparedStatement
	{
		IConnection *conn;
	protected:
		virtual IConnection *getConnection() { return conn; }
		virtual IDriver *getDriver();
		virtual IResultSet *executeQuery_() = 0;

		std::string SQL;
		std::string getSQL() const { return SQL; }
		virtual void setSQL(const std::string &SQL) {this->SQL = SQL;}
	public:
		Statement(IConnection *conn) :conn(conn) {}
		virtual ~Statement() {}

		virtual ResultSetHandle executeQuery();
		virtual void execute() = 0;
		virtual ResultSetHandle executeQuery(const std::string &SQL);
		virtual unsigned int executeUpdate(const std::string &SQL);
		virtual unsigned int executeUpdate();
		virtual void execute(const std::string &SQL);
		virtual void close() = 0;
		virtual unsigned int getUpdateCount() = 0;
		virtual void set(const bool &data, unsigned int column);
		virtual void set(const std::string &data, unsigned int column,unsigned int size) = 0;
		virtual void set(const char &data, unsigned int column) = 0;
		virtual void set(const std::int16_t &data, unsigned int column);
		virtual void set(const std::int32_t &data, unsigned int column);
		virtual void set(const std::int64_t &data, unsigned int column);
		virtual void set(const boost::uint16_t &data, unsigned int column);
		virtual void set(const boost::uint32_t &data, unsigned int column);
		virtual void set(const boost::uint64_t &data, unsigned int column);
		virtual void set(const float &data, unsigned int column);
		virtual void set(const double &data, unsigned int column) = 0;
		virtual void set(const std::vector<boost::uint8_t> &data, unsigned int column) = 0;
		virtual void reset() {}
	};

	inline unsigned int Statement::executeUpdate() {
		execute();
		return getUpdateCount();
	}

	inline unsigned int Statement::executeUpdate(const std::string &SQL) {
		setSQL(SQL);
		return executeUpdate();
	}

	inline IDriver *Statement::getDriver() {
		return getConnection()->getDriver();
	}

	inline void Statement::execute(const std::string &SQL) {
		setSQL(SQL);
		 execute();
	}
	inline ResultSetHandle Statement::executeQuery() {
		return ResultSetHandle(executeQuery_(),getFreeResultSet());
	}
	inline ResultSetHandle Statement::executeQuery(const std::string &SQL) {
		setSQL(SQL);
		return executeQuery();
	}


	inline void Statement::set(const bool &data, unsigned int column) {
		char c = '0';

		if (data)
			c = '1';
		set(c, column);
	}

	inline void Statement::set(const std::int16_t &data, unsigned int column) {
		double v = (double)data;
		set(v, column);
	}
	inline void Statement::set(const std::int32_t &data, unsigned int column) {
		double v = (double)data;
		set(v, column);
	}
	inline void Statement::set(const std::int64_t &data, unsigned int column) {
		double v = (double)data;
		set(v, column);
	}
	inline void Statement::set(const std::uint16_t &data, unsigned int column) {
		boost::int32_t v = (boost::int32_t)data;
		set(v, column);
	}
	inline void Statement::set(const std::uint32_t &data, unsigned int column) {
		boost::int64_t v = (boost::int64_t)data;
		set(v, column);
	}
	inline void Statement::set(const std::uint64_t &data, unsigned int column) {
		static boost::uint64_t M = std::numeric_limits<boost::uint64_t>::max() >> 1;
		boost::int64_t v = ((boost::int64_t)data & M);
		set(v, column);
	}
	inline void Statement::set(const float &data, unsigned int column) {
		double v = (double)data;
		set(v, column);
	}
}

#endif // STATEMENT_H
