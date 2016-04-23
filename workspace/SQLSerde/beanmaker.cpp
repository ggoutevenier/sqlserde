#include "spring/bean.h"
#include "sqlserde.h"

#if defined (_WIN32) || defined (_WIN62)
#define DllExport __declspec(dllexport)
#else
#define DllExport
#endif

namespace DataKinesis {
namespace BeanSqlSerde {
class Serde : public Bean<SqlSerde> {
	void construct() {
		if (!isConstructed()) {
			add(new SqlSerde(getRef<IConnection>("connection"),getRef<SqlDialect>("dialect")));
		}
	}
public:
	Serde(const std::string &name, const IBean::Parameters &values,
			const IBean::Parameters &refs) :
			Bean(name, values, refs) {
	}
};
}
}
extern "C" {

DllExport
DataKinesis::IBean *makeSqlSerde(const std::string &name,
		const DataKinesis::IBean::Parameters &values,
		const DataKinesis::IBean::Parameters &refs) {
	return new DataKinesis::BeanSqlSerde::Serde(name, values, refs);
}
}
