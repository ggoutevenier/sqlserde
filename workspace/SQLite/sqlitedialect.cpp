#include "sqlitedialect.h"
#include "serde/record.h"
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
}
