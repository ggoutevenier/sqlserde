#ifndef SQLITERESULTSET_H
#define SQLITERESULTSET_H

#include "sql/resultset.h"
#include "sqlitedriver.h"


extern "C" {
typedef struct sqlite3_stmt sqlite3_stmt;
typedef struct sqlite3 sqlite3;
}

namespace DataKinesis {
class SQLiteStatement;
class SQLiteResultSetMetaData;
class SQLiteResultSet : public ResultSet
{
    friend class SQLiteResultSetMetaData;

//    sqlite3 * _getDB();
	virtual IResultSetMetaData *getMetaData_();
    int lastStatus;
//    sqlite3_stmt* _stmt();
	SQLiteStatement *getSQLiteStatement();
protected:
	static void _free(IResultSetMetaData *);
	virtual freeResultSetMetaData getFreeResultSetMetaData() const;
public:
    SQLiteResultSet(SQLiteResultSet &resultSet);
    SQLiteResultSet(SQLiteStatement &statement);
    virtual ~SQLiteResultSet();
    bool next();
    bool completed();
	void get(char &data, unsigned int column);
	void get(std::string &data, unsigned int column);
	void get(boost::int16_t &data, unsigned int column);
	void get(boost::int32_t &data, unsigned int column);
	void get(boost::int64_t &data, unsigned int column);
	void get(boost::uint16_t &data, unsigned int column);
	void get(boost::uint32_t &data, unsigned int column);
	void get(boost::uint64_t &data, unsigned int column);
	void get(tm &data, unsigned int colulmn);
	void get(float &data, unsigned int column);
    size_t getColumnSize(unsigned int column);
	void get(double &data, unsigned int column);
	void get(std::vector<boost::uint8_t> &data, unsigned int column);

//    unsigned int getUpdateCount();
	void close();
//	ResultSetMetaDataHandle getMetaData();

};
}
#endif // SQLITERESULTSET_H
