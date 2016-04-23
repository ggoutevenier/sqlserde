#include "serde/record.h"
#include <iostream>
#include "structure.h"
#include "spring/applicationcontext.h"

#include "logger.h"
//SHARE_EASYLOGGINGPP(::Logger::getStorage())

using namespace DataKinesis;
// create mapping of metadata
void makeRecord(const Test &data, Record &record) {
	record.setName("Test");
	record.addField(data, data.married, "married");
	record.addField(data, data.bignumber, "bignum").PK(1);
	record.addField(data, data.zip, "postal_code").IE("IE1",1);
	record.addField(data, data.firstName, "first_name").setSize(30).AK("AK1",1);
	record.addField(data, data.lastName, "last_name").setSize(30).AK("AK1",2);
	record.addField(data, data.age, "age").setNull(false);
	record.addField(data, data.income, "income");
	record.addField(data, data.networth, "networth");
}
void removeIfExists(const std::string &filename) {
	FILE *fp=fopen(filename.c_str(),"rb");
	if(fp!=0) {
		fclose(fp);
		if(remove(filename.c_str()))
			throw std::runtime_error("unable to remove file "+ filename);
	}
}
int main() {
	try {
		el::Configurations conf("logger.cfg");
		el::Loggers::reconfigureAllLoggers(conf);

		LOG(INFO) << "start";
		typedef std::list<Test> DataList;
		std::string sqliteFile="/tmp/sqlite.db";
		removeIfExists(sqliteFile);
		ApplicationContext context("Spring.xml");

		context.updateParameter("infile","filename","/tmp/test.fil");	// override input filename
		context.updateParameter("sqliteConnection","url",sqliteFile);

		ISerde &reader=context.get<ISerde>("in");
		ISerde &display=context.get<ISerde>("display");

		ISerde &sql=context.get<ISerde>("sql");

		DataList dataList;
		reader.open<Test>();
		// read test data from stream into list using metadata record
		while (reader.next()) {
			Test test;
			reader.get(&test);
			dataList.push_back(test);
		}
		//write data
		sql.open<Test>();
		for(DataList::const_iterator it=dataList.begin();it!=dataList.end();++it)
			sql.set(&*it);
		sql.close();

		// re-read test data
		DataList dataList2;
		sql.open<Test>();
		while (reader.next()) {
			Test test;
			reader.get(&test);
			dataList2.push_back(test);
		}
		sql.close();

		//write data
		display.open<Test>();
		for(DataList::const_iterator it=dataList.begin();it!=dataList.end();++it)
			display.set(&*it);
		display.close();
	}
	catch (std::exception &e) {
		std::cout << e.what()<< std::endl;
		return 1;
	}
	return 0;
}
