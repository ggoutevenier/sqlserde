#pragma once
#include "serde/sqldialect.h"
namespace DataKinesis {
class SQLiteDialect : public SqlDialect {
public:
	~SQLiteDialect() {}
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
};
}
