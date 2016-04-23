#ifndef SQLSERDE_H
#define SQLSERDE_H
#include "serde/serde.h"
#include "serde/sqldialect.h"
#include "sql/driver.h"
#include "sql/sqlhandle.h"

namespace DataKinesis {
	class  SqlSerde : public Serde {
		IConnection *conn;
		ResultSetHandle rset;
		PreparedStatementHandle pstmt;
		SqlDialect *dialect;
	protected:
		std::string makeSelect();
		std::string makeInsert();
		std::string makeCreateTable();
		std::string makeCreateIndexes();
		std::string getTableName();
		bool tableExists();
	public:
//		SqlSerde(ConnectionHandle conn);
		SqlSerde(IConnection &conn, SqlDialect &dialect);
		virtual ~SqlSerde();
		virtual void open_(const Record &record);
		virtual void close();
		virtual void get(bool &data, const Field &field);
		virtual void get(char &data, const Field &field);
		virtual void get(std::string &data, const Field &field);
		virtual void get(std::int16_t &data, const Field &field);
		virtual void get(std::int32_t &data, const Field &field);
		virtual void get(std::int64_t &data, const Field &field);
		virtual void get(std::uint16_t &data, const Field &field);
		virtual void get(std::uint32_t &data, const Field &field);
		virtual void get(std::uint64_t &data, const Field &field);
		virtual void get(std::vector<std::uint8_t> &data, const Field &field);
		virtual void get_(void *data, const Field &field);
		virtual void get(float &data, const Field &field);
		virtual void get(double &data, const Field &field);
		virtual void get(tm &data, const Field &field);
		virtual void get(void *data);
		virtual bool next();

		virtual void setRecord();
		virtual void set(const bool &data, const Field &field);
		virtual void set(const char &data, const Field &field);
		virtual void set(const std::string &data, const Field &field);
		virtual void set(const std::int16_t &data, const Field &field);
		virtual void set(const std::int32_t &data, const Field &field);
		virtual void set(const std::int64_t &data, const Field &field);
		virtual void set(const std::uint16_t &data, const Field &field);
		virtual void set(const std::uint32_t &data, const Field &field);
		virtual void set(const std::uint64_t &data, const Field &field);
		virtual void set(const std::vector<std::uint8_t> &data, const Field &field);
		virtual void set_(const void *data, const Field &field);
		virtual void set(const float &data, const Field &field);
		virtual void set(const double &data, const Field &field);
		virtual void set(const tm &data, const Field &field);
		virtual void set(const void *data);
	};
}

#endif
