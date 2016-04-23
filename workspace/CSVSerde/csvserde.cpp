#include "csvserde.h"
#include "serde/record.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

namespace DataKinesis {
	void CsvSerde::writeHeader() {
		values.resize(record->size());
		if (record->size() == 0) {
			throw std::runtime_error("Record has no fields");
		}
		else {
			int i = 0;
			for (Record::const_iterator it = record->begin(); it != record->end(); ++it)
				values.at(i++) = it->getName();
			setRecord();
		}
	}

	CsvSerde::CsvSerde(std::ostream &out) : in(0), out(&out) {}
	CsvSerde::CsvSerde(std::istream &in) : in(&in), out(0) {}
	CsvSerde::~CsvSerde() { close(); }

	void CsvSerde::open_(const Record &record) {
		Serde::open_(record);
		if(out)
			writeHeader();
		else if(in)
			readHeader();
	}

	void CsvSerde::close() {}
	void CsvSerde::setRecord() {
		if (!values.empty())
			*out << boost::algorithm::join(values, ",") << std::endl;
	}

	void CsvSerde::set(const void *data) {
		if (record == 0)
			throw std::runtime_error("Need to open record");
		record->set(*this, data);
	}

	template<class T> void CsvSerde::set_(T data, const Field &field) {
		values.at(field.getPosition()) = boost::lexical_cast<std::string>(data);
	}
	void CsvSerde::set(const bool &data, const Field &field) { set_(data, field); }
	void CsvSerde::set(const char &data, const Field &field) { set_(data, field); }
	void CsvSerde::set(const std::string &data, const Field &field) { set_(data, field); }
	void CsvSerde::set(const boost::int16_t &data, const Field &field) { set_(data, field); }
	void CsvSerde::set(const boost::int32_t &data, const Field &field) { set_(data, field); }
	void CsvSerde::set(const boost::int64_t &data, const Field &field) { set_(data, field); }
	void CsvSerde::set(const boost::uint8_t &data, const Field &field) { set_(data, field); }
	void CsvSerde::set(const boost::uint16_t &data, const Field &field) { set_(data, field); }
	void CsvSerde::set(const boost::uint32_t &data, const Field &field) { set_(data, field); }
	void CsvSerde::set(const boost::uint64_t &data, const Field &field) { set_(data, field); }
	void CsvSerde::set(const float &data, const Field &field) { set_(data, field); }
	void CsvSerde::set(const double &data, const Field &field) { set_(data, field); }

	bool CsvSerde::next() {
		std::string line;
		bool rtn = false;
		values.clear();
		std::getline(*in, line);
		if (!line.empty()) {
			rtn = true;
			typedef boost::tokenizer<boost::escaped_list_separator<char>> Tok;
			for (std::string value : Tok(line))
				values.push_back(value);
		}
		return rtn;
	}
	void CsvSerde::get(bool &data, const Field &field) { get_(data, field); }
	void CsvSerde::get(std::string &data, const Field &field) { get_(data, field); }
	void CsvSerde::get(char &data, const Field &field) { get_(data, field); }
	void CsvSerde::get(boost::int16_t &data, const Field &field) { get_(data, field); }
	void CsvSerde::get(boost::int32_t &data, const Field &field) { get_(data, field); }
	void CsvSerde::get(boost::int64_t &data, const Field &field) { get_(data, field); }
	void CsvSerde::get(boost::uint16_t &data, const Field &field) { get_(data, field); }
	void CsvSerde::get(boost::uint32_t &data, const Field &field) { get_(data, field); }
	void CsvSerde::get(boost::uint64_t &data, const Field &field) { get_(data, field); }
	void CsvSerde::get(float &data, const Field &field) { get_(data, field); }
	void CsvSerde::get(double &data, const Field &field) { get_(data, field); }
	void CsvSerde::get(void *data) { record->get(*this, data); }

	void CsvSerde::readHeader() {
		if (!next())
			std::runtime_error("Error reading header");
		transform.resize(record->size());
		for (Record::const_iterator fit = record->begin(); fit != record->end(); ++fit) {
			values_t::const_iterator vit = std::find(values.begin(), values.end(), fit->getName());
			unsigned int i, j;
			i = fit->getPosition();
			if (vit == values.end())
				j = 0;
			else
				j = std::distance(values.cbegin(), vit) + 1;

			transform.at(i) = j;
		}
	}

	template<class T>
	void CsvSerde::get_(T &data, const Field &field) {
		unsigned int column = field.getPosition();
		if (values.size() <= column)
			throw std::runtime_error("No data");
		unsigned int i = transform.at(column);
		if (i != 0)
			data = boost::lexical_cast<T>(values.at(i - 1));
	}

}
