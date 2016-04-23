#pragma once
#include <cstdint>
#include <string>
namespace DataKinesis {
	class Record;
	class Field;

	class ISerde {
	public:
		virtual void set(const bool &data, const Field &field) = 0;
		virtual void set(const std::string &data, const Field &field) = 0;
		virtual void set(const char &data, const Field &field) = 0;
		virtual void set(const std::int16_t &data, const Field &field) = 0;
		virtual void set(const std::int32_t &data, const Field &field) = 0;
		virtual void set(const std::int64_t &data, const Field &field) = 0;
//		virtual void set(const std::uint8_t &data, const Field &field) = 0;
		virtual void set(const std::uint16_t &data, const Field &field) = 0;
		virtual void set(const std::uint32_t &data, const Field &field) = 0;
		virtual void set(const std::uint64_t &data, const Field &field) = 0;
		virtual void set(const float &data, const Field &field) = 0;
		virtual void set(const double &data, const Field &field) = 0;
		virtual void setRecord() = 0;
		virtual void get(bool &data, const Field &field) = 0;
		virtual void get(std::string &data, const Field &field) = 0;
		virtual void get(char &data, const Field &field) = 0;
		virtual void get(std::int16_t &data, const Field &field) = 0;
		virtual void get(std::int32_t &data, const Field &field) = 0;
		virtual void get(std::int64_t &data, const Field &field) = 0;
		virtual void get(std::uint16_t &data, const Field &field) = 0;
		virtual void get(std::uint32_t &data, const Field &field) = 0;
		virtual void get(std::uint64_t &data, const Field &field) = 0;
		virtual void get(float &data, const Field &field) = 0;
		virtual void get(double &data, const Field &field) = 0;
		virtual void get(void *data) = 0;
		virtual ~ISerde() {}
		virtual void open_(const Record &record) = 0;
		template<class T> void open(); // function defined in record.h
		virtual void close() = 0;
		virtual bool next() = 0;
		virtual void set(const void *data) = 0;
		virtual bool isOpen() const=0;
	};
}
