#ifndef DIALECT_H
#define DIALECT_H
#include <string>
#include <vector>
#include <cstdint>
#include <memory>
namespace DataKinesis {
	class Record;
	class Field;
class Dialect {
	public:
		virtual std::string getTypeName(const bool *, const Field &field) const = 0;
		virtual std::string getTypeName(const std::string *, const Field &field) const = 0;
		virtual std::string getTypeName(const std::int16_t *, const Field &field) const = 0;
		virtual std::string getTypeName(const std::uint16_t *, const Field &field) const = 0;
		virtual std::string getTypeName(const std::int32_t *, const Field &field) const = 0;
		virtual std::string getTypeName(const std::uint32_t *, const Field &field) const = 0;
		virtual std::string getTypeName(const std::int64_t *, const Field &field) const  = 0;
		virtual std::string getTypeName(const std::uint64_t *, const Field &field) const = 0;
		virtual std::string getTypeName(const std::vector<std::uint8_t> *, const Field &field) const = 0;
		virtual std::string getTypeName(const float *, const Field &field) const = 0;
		virtual std::string getTypeName(const double *, const Field &field) const = 0;
		virtual std::string getTypeName(const tm *, const Field &field) const = 0;
//		template<class T> std::string getTypeName(const T *, const Field &field) const {
//			return getTypeName(field, typeid(T()).hash_code());
//		}
//		virtual std::string getTypeName(const Field &field, size_t hash_code) const = 0;
//		virtual std::string get(std::string cmd, const Record *record) const = 0;
		virtual ~Dialect() {}
};
	typedef std::shared_ptr<Dialect> DialectHandle;
}
#endif
