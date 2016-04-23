#pragma once
#include "serde/serde.h"
#include <string>
#include <iostream>
#include <vector>
#include <cstdint>

namespace DataKinesis {
	class Record;
	class Field;
	class CsvSerde : public Serde {
	public:
		CsvSerde(std::ostream &out);
		CsvSerde(std::istream &in);
		~CsvSerde();
		virtual void open_(const Record &record);
		virtual void close();
		virtual bool next();
		virtual void setRecord();
		virtual void set(const void *data);
		virtual void get(void *data);
	private:
		template<class T>
		void set_(T data, const Field &field);
		virtual void set(const bool &data, const Field &field);
		virtual void set(const char &data, const Field &field);
		virtual void set(const std::string &data, const Field &field);
		virtual void set(const std::int16_t &data, const Field &field);
		virtual void set(const std::int32_t &data, const Field &field);
		virtual void set(const std::int64_t &data, const Field &field);
		virtual void set(const std::uint8_t &data, const Field &field);
		virtual void set(const std::uint16_t &data, const Field &field);
		virtual void set(const std::uint32_t &data, const Field &field);
		virtual void set(const std::uint64_t &data, const Field &field);
		virtual void set(const float &data, const Field &field);
		virtual void set(const double &data, const Field &field);
		virtual void get(bool &data, const Field &field);
		virtual void get(std::string &data, const Field &field);
		virtual void get(char &data, const Field &field);
		virtual void get(std::int16_t &data, const Field &field);
		virtual void get(std::int32_t &data, const Field &field);
		virtual void get(std::int64_t &data, const Field &field);
		virtual void get(std::uint16_t &data, const Field &field);
		virtual void get(std::uint32_t &data, const Field &field);
		virtual void get(std::uint64_t &data, const Field &field);
		virtual void get(float &data, const Field &field);
		virtual void get(double &data, const Field &field);

		template<class T>void get_(T &data, const Field &field);
		std::istream *in;
		std::ostream *out;
		typedef std::vector<std::string> values_t;
		values_t values;
		std::vector<unsigned int> transform;
		void writeHeader();
		void readHeader();
	};

}
