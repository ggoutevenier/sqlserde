#include "serde/sqldialect.h"
#include "serde/record.h"
#include "serde/field.h"
#include "boost/algorithm/string.hpp"
#include <sstream>

namespace DataKinesis {
	std::string SqlDialect::getTypeName(const bool *, const Field &field) const { return "boolean"; }
	std::string SqlDialect::getTypeName(const char *, const Field &field) const { return "char"; }
	std::string SqlDialect::getTypeName(const std::int16_t *, const Field &field) const { return "integer"; }
	std::string SqlDialect::getTypeName(const std::uint16_t *, const Field &field) const { return "integer"; }
	std::string SqlDialect::getTypeName(const std::int32_t *, const Field &field) const { return "integer"; }
	std::string SqlDialect::getTypeName(const std::uint32_t *, const Field &field) const { return "integer"; }
	std::string SqlDialect::getTypeName(const std::int64_t *, const Field &field) const { return "bigint"; }
	std::string SqlDialect::getTypeName(const std::uint64_t *, const Field &field) const { return "bigint"; }
	std::string SqlDialect::getTypeName(const std::vector<std::uint8_t> *, const Field &field) const { return "blob"; };
	std::string SqlDialect::getTypeName(const float *, const Field &field) const { return "real"; }
	std::string SqlDialect::getTypeName(const double *, const Field &field) const { return "double"; }
	std::string SqlDialect::getTypeName(const tm *, const Field &field) const { return "timestamp"; }

	std::string SqlDialect::getTypeName(const std::string *, const Field &field) const {
		std::ostringstream ss;
		ss << "varchar(" << field.getSize() << ")";
		return ss.str();
	}
	std::string SqlDialect::makeCreateIndex(const Index &index) const {
		std::ostringstream ss;
		ss << "create" << (index.isUnique() ? " unique index " : " index ") << index.getName();
		ss << " on " << index.getRecord().getName() << "(";
		bool first=true;
		for (Index::const_iterator it = index.begin(); it != index.end(); ++it) {
			if (first)
				first=false;
			else
				ss << ",";
			ss << it->second->getName();
		}
		ss << ");\n";
		return ss.str();
	}
	std::string SqlDialect::makeCreateIndexes(const Record &record) const {
		std::ostringstream ss;
		for (Record::Indexes::const_iterator it = record.getIndexes().begin(); it != record.getIndexes().end(); ++it)
			ss << makeCreateIndex(it->second);
		
		return ss.str();
	}
	// todo checkfor resereved words
	std::string SqlDialect::makeSelect(const Record &record) const {
		std::stringstream ss;
		ss << "select ";
		bool first=true;
		for (Record::const_iterator it = record.begin(); it != record.end(); ++it) {
			const Field &field = *it;
			if (first)
				first=false;
			else
				ss << ",";
			ss << field.getName();
		}
		ss << " from " << record.getName();
		return ss.str();
	}
	std::string SqlDialect::makeCreateTable(const Record &record) const{
		std::ostringstream ss;
		if (record.empty())
			throw std::runtime_error("No fields to create table width");
		ss << "create table " << record.getName() << " (";
// add fields
		bool first=true;
		for (Record::const_iterator it = record.begin(); it != record.end(); ++it) {
			const Field &field = *it;
			if (first)
				first=false;
			else
				ss << ", ";
			ss << field.getName() << " " << field.getType().getName(*this, field);
			if (!field.isNull()) {
				ss << " not null ";
			}
		}
// add primary key
		if (record.hasPK()) {
			bool first=true;
			for (Index::const_iterator it = record.getPK().begin(); it != record.getPK().end(); ++it) {
				if (first) {
					ss << ",primary key(";
					first=false;
				}
				else {
					ss << ", ";
				}
				ss << it->second->getName();
			}
			ss << ")";
		}
		
		ss << ")";
		return ss.str();
	}
	std::string SqlDialect::makeInsert(const Record &record) const {
		std::ostringstream ss;
		ss << "insert into " << record.getName() << "(";
		bool first=true;
		for (Record::const_iterator it = record.begin(); it != record.end(); ++it) {
			if (first)
				first=false;
			else
				ss << ", ";
			ss << it->getName();
		}
		ss << ") values (";
		first=true;
		for (Record::const_iterator it = record.begin(); it != record.end(); ++it) {
			if (first)
				first=false;
			else
				ss << ", ";
			ss << "?";
		}
		ss << ")";
		return ss.str();
	}
	std::string SqlDialect::get(SqlDialect::Cmd cmd, const Record *record) const {
		std::string sql;
		if(record==0)
			throw std::runtime_error("Record not set");
		switch(cmd) {
		case SELECT:
			sql=makeSelect(*record);
			break;
		case INSERT:
			sql=makeInsert(*record);
			break;
		case CREATE_TABLE:
			sql=makeCreateTable(*record);
			break;
		case CREATE_INDEX:
			sql=makeCreateIndexes(*record);
			break;
		default:
			throw std::runtime_error("Unknown command in SqlDialect::get");
		}
		return sql;
	}
}
