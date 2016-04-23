#include "spring/bean.h"
#include "sqliteconnection.h"
#include "sqlitedriver.h"
#include "sqlitedialect.h"

#if defined (_WIN32) || defined (_WIN62)
#define DllExport __declspec(dllexport)
#else
#define DllExport
#endif

namespace DataKinesis {
namespace BeanSQLite {
class Connection : public Bean<IConnection> {
	void construct() {
		IDriver &driver=getRef<IDriver>("driver");
		if (!isConstructed()) {
			add(driver.createConnection(
					getValue<std::string>("url")
//					std::map<std::string, std::string>()
					));
		}
	}
public:
	Connection(const std::string &name, const IBean::Parameters &values,
			const IBean::Parameters &refs) :
			Bean(name, values, refs) {
	}
};

class Driver : public Bean<SQLiteDriver> {
	void construct() {
		if (!isConstructed()) {
			add(new SQLiteDriver());
		}
	}
public:
	Driver(const std::string &name, const IBean::Parameters &values,
			const IBean::Parameters &refs) :
			Bean(name, values, refs) {
	}
};

class Dialect : public Bean<SQLiteDialect> {
	void construct() {
		if (!isConstructed()) {
			add(new SQLiteDialect());
		}
	}
public:
	Dialect(const std::string &name, const IBean::Parameters &values,
			const IBean::Parameters &refs) :
			Bean(name, values, refs) {
	}
};
}
}
extern "C" {
DllExport
DataKinesis::IBean *makeDriver(const std::string &name,
		const DataKinesis::IBean::Parameters &values,
		const DataKinesis::IBean::Parameters &refs) {
	return new DataKinesis::BeanSQLite::Driver(name, values, refs);
}

DllExport
DataKinesis::IBean *makeConnection(const std::string &name,
		const DataKinesis::IBean::Parameters &values,
		const DataKinesis::IBean::Parameters &refs) {
	return new DataKinesis::BeanSQLite::Connection(name, values, refs);
}

DllExport
DataKinesis::IBean *makeDialect(const std::string &name,
		const DataKinesis::IBean::Parameters &values,
		const DataKinesis::IBean::Parameters &refs) {
	return new DataKinesis::BeanSQLite::Dialect(name, values, refs);
}
}
