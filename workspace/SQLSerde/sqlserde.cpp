#include "serde/record.h"
#include "sqlserde.h"
#include "serde/sqldialect.h"
#include "sql/connection.h"
#include "sql/resultset.h"
#include "sql/databasemetadata.h"
#include <iostream>
#include "boost/algorithm/string.hpp"

namespace DataKinesis {
	SqlSerde::SqlSerde(IConnection &conn, SqlDialect &dialect) : conn(&conn), dialect(&dialect) {}
	SqlSerde::~SqlSerde() { close(); }

	void SqlSerde::open_(const Record &record) {
		if (!isOpen()) {
			Serde::open_(record);
			if (!tableExists()) {
				std::string sql=makeCreateTable();

				conn->execute(sql);
				if(!tableExists()) {
					throw std::runtime_error("Unable to create table " + getTableName());
				}

				// create indexes
				typedef std::vector<std::string> Sqls;
				Sqls sqls;
				std::string tmp=makeCreateIndexes();
				boost::split(sqls, tmp, boost::is_any_of(";"));
				for (Sqls::const_iterator it = sqls.begin(); it != sqls.end(); ++it) {
					std::string sql = *it;
					boost::trim(sql);
					if (!sql.empty())
						conn->execute(sql);
				}
			}
			rset = conn->executeQuery(makeSelect());
			pstmt = conn->createPreparedStatement(makeInsert());
			conn->beginTransaction();
		}
	}
//	void SqlSerde::close() {Serde::close();	}
	std::string SqlSerde::makeSelect() {
		return dialect->get(SqlDialect::SELECT, record);
	}

	void SqlSerde::get(bool &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get(std::string &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get(char &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get(std::int16_t &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get(std::int32_t &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get(std::int64_t &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get(std::uint16_t &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get(std::uint32_t &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get(std::uint64_t &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get(std::vector<std::uint8_t> &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get(float &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get(double &data, const Field &field) {
		rset->get(data, field.getPosition()+1);
	}
	void SqlSerde::get_(void *data, const Field &field) {
//!		std::string str;
//!		SqlSerde::get(str, field);
//!		field.getType().fromString(str, data);
	}
	void SqlSerde::get(tm &data, const Field &field) {
		rset->get(data, field.getPosition() + 1);
	}
	void SqlSerde::get(void *data) {
	//		for (Record::field_iterator it = record.beginField(); it != record.endField(); ++it) {
	//			const Field &field = *it->second;
	//			field.get(*this, data);
	//		}
		record->get(*this,data);
	}
	bool SqlSerde::next() {
		return rset->next();
	}

	void SqlSerde::close() {
		pstmt->close();
		conn->commit();
		Serde::close();
	}

	std::string SqlSerde::getTableName() {
		return record->getName();
	}
	bool SqlSerde::tableExists() {
		DatabaseMetaDataHandle meta = conn->getDatabaseMetaData();
		std::string tableName = getTableName();
		if (tableName.empty())
			throw std::runtime_error("Table name not specified for structure ");
		if (meta->getTables("", "", tableName)->next())
			return true;
		else
			return false;
	}

	void SqlSerde::setRecord() { pstmt->execute(); }
	void SqlSerde::set(const bool &data, const Field &field) {	pstmt->set(data, field.getPosition()+1);}
	void SqlSerde::set(const char &data, const Field &field) { pstmt->set(data, field.getPosition()+1); }
	void SqlSerde::set(const std::string &data, const Field &field) { pstmt->set(data, field.getPosition()+1, field.getSize()); }
	void SqlSerde::set(const std::int16_t &data, const Field &field) { pstmt->set(data, field.getPosition()+1); }
	void SqlSerde::set(const std::int32_t &data, const Field &field) { pstmt->set(data, field.getPosition()+1); }
	void SqlSerde::set(const std::int64_t &data, const Field &field) { pstmt->set(data, field.getPosition()+1); }
	void SqlSerde::set(const std::uint16_t &data, const Field &field) { pstmt->set(data, field.getPosition()+1); }
	void SqlSerde::set(const std::uint32_t &data, const Field &field) { pstmt->set(data, field.getPosition()+1); }
	void SqlSerde::set(const std::uint64_t &data, const Field &field) { pstmt->set(data, field.getPosition()+1); }
	void SqlSerde::set(const std::vector<std::uint8_t> &data, const Field &field) { pstmt->set(data, field.getPosition()+1); }

	void SqlSerde::set(const float &data, const Field &field) { pstmt->set(data, field.getPosition()+1); }
	void SqlSerde::set(const double &data, const Field &field) { pstmt->set(data, field.getPosition()+1); }
	void SqlSerde::set(const tm &data, const Field &field) { pstmt->set(data, field.getPosition() + 1); }
	void SqlSerde::set_(const void *data, const Field &field) { SqlSerde::set(field.getType().toString(data), field);	}
	void SqlSerde::set(const void *data) {record->set(*this, data);}

	std::string SqlSerde::makeInsert() {
		return dialect->get(SqlDialect::INSERT, record);
	}
	std::string SqlSerde::makeCreateTable() {
		return dialect->get(SqlDialect::CREATE_TABLE, record);
	}
	std::string SqlSerde::makeCreateIndexes() {
		return dialect->get(SqlDialect::CREATE_INDEX, record);
	}
}
