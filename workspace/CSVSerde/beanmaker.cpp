#include "spring/bean.h"
#include <fstream>
#include <iostream>
#include "csvserde.h"

namespace DataKinesis {
	namespace BeanCsvSerde {
		class Serializer : public Bean<CsvSerde> {
		public:
			Serializer(const std::string &name, const IBean::Parameters &values, const IBean::Parameters &refs) :
				Bean<CsvSerde>(name, values, refs) {}
			virtual ~Serializer() {}
			void construct() {
				add(new CsvSerde(getRef<std::ostream>("ofile")));
			}
		};
		class Deserializer : public Bean<CsvSerde> {
		public:
			Deserializer(const std::string &name, const IBean::Parameters &values, const IBean::Parameters &refs) :
				Bean<CsvSerde>(name, values, refs) {}
			virtual ~Deserializer() {}
			void construct() {
				add(new CsvSerde(getRef<std::istream>("ifile")));
			}
		};
	}
	// the following istream/ostream should be defined in applicationcontext
	namespace BeanCommon {
		class ifstream : public Bean<std::ifstream> {
		public:
			ifstream(const std::string &name, const IBean::Parameters &values, const IBean::Parameters &refs) :
				Bean<std::ifstream>(name, values, refs) {}
			virtual ~ifstream() {}
			void construct() {
				add(new std::ifstream(getValue<std::string>("filename")));
			}
		};

		class ofstream : public Bean<std::ofstream> {
		public:
			ofstream(const std::string &name, const IBean::Parameters &values, const IBean::Parameters &refs) :
				Bean<std::ofstream>(name, values, refs) {}
			virtual ~ofstream() {}
			void construct() {
				add(new std::ofstream(getValue<std::string>("filename")));
			}
		};
		class cout : public Bean<std::ostream> {
		public:
			cout(const std::string &name, const IBean::Parameters &values, const IBean::Parameters &refs) :
				Bean<std::ostream>(name, values, refs) {}
			virtual ~cout() {}
			void construct() {
				add(std::cout);
			}
		};
	}
}
extern "C" {
	DllExport
	DataKinesis::IBean *makeCsvSerializer(const std::string &name, const DataKinesis::IBean::Parameters &values, const DataKinesis::IBean::Parameters &refs) {
		return new DataKinesis::BeanCsvSerde::Serializer(name, values, refs);
	}
	DllExport
	DataKinesis::IBean *makeCsvDeserializer(const std::string &name, const DataKinesis::IBean::Parameters &values, const DataKinesis::IBean::Parameters &refs) {
		return new DataKinesis::BeanCsvSerde::Deserializer(name, values, refs);
	}
	DllExport
	DataKinesis::IBean *makeCout(const std::string &name, const DataKinesis::IBean::Parameters &values, const DataKinesis::IBean::Parameters &refs) {
		return new DataKinesis::BeanCommon::cout(name, values, refs);
	}
	DllExport
	DataKinesis::IBean *makeIfstream(const std::string &name, const DataKinesis::IBean::Parameters &values, const DataKinesis::IBean::Parameters &refs) {
		return new DataKinesis::BeanCommon::ifstream(name, values, refs);
	}
}
