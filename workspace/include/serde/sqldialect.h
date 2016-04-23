#ifndef SQLDIALECT_H
#define SQLDIALECT_H
#include "serde/dialect.h"

namespace DataKinesis {
	class Index;
	class SqlDialect : public Dialect {
	protected:
		std::string makeSelect(const Record &record) const;
		std::string makeCreateTable(const Record &record) const;
		std::string makeInsert(const Record &record) const;
		std::string makeCreateIndexes(const Record &record) const;
		std::string makeCreateIndex(const Index &index) const;
	public:
		enum Cmd{INSERT,SELECT,CREATE_TABLE,CREATE_INDEX};
		virtual std::string getTypeName(const bool *, const Field &field) const;
		virtual std::string getTypeName(const std::string *, const Field &field) const;
		virtual std::string getTypeName(const char *, const Field &field) const;
		virtual std::string getTypeName(const std::int16_t *, const Field &field) const;
		virtual std::string getTypeName(const std::uint16_t *, const Field &field) const;
		virtual std::string getTypeName(const std::int32_t *, const Field &field) const;
		virtual std::string getTypeName(const std::uint32_t *, const Field &field) const;
		virtual std::string getTypeName(const std::int64_t *, const Field &field) const;
		virtual std::string getTypeName(const std::uint64_t *, const Field &field) const;
		virtual std::string getTypeName(const std::vector<std::uint8_t> *, const Field &field) const;
		virtual std::string getTypeName(const float *, const Field &field) const;
		virtual std::string getTypeName(const double *, const Field &field) const;
		virtual std::string getTypeName(const tm *, const Field &field) const;
		virtual std::string get(SqlDialect::Cmd cmd, const Record *record) const;
		virtual ~SqlDialect() {}
	};
}


#endif
