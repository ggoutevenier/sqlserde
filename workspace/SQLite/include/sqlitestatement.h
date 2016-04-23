#ifndef SQLITESTATEMENT_H
#define SQLITESTATEMENT_H

#include <cstdint>
#include "sql/resultset.h"
#include "sql/statement.h" // Base class: DataKinesis::Statement
#include "sqlitedriver.h"
#include <array>
#include <ctime>

extern "C" {
typedef struct sqlite3_stmt sqlite3_stmt;
typedef struct sqlite3 sqlite3;
}

namespace DataKinesis {
class SQLiteResultSet;
class SQLiteConnection;
class SQLiteResultSetMetaData;
class SQLiteStatement : public Statement
{
    friend class SQLiteResultSet;
    friend class SQLiteResultSetMetaData;
    std::array<char,1024*64> buff;
    void *getBuff(unsigned int bytes);
    size_t buffOffset;
    void resetBuff();
	void checkStatement();
protected:
    sqlite3_stmt *stmt;
    SQLiteConnection *getSQLiteConnection();
	virtual IResultSet *executeQuery_();
	static void _free(IResultSet *);
	virtual freeResultSet getFreeResultSet() const;
public:
    SQLiteStatement(IConnection *conn);
    virtual ~SQLiteStatement();
//	ResultSetHandle executeQuery();
//	ResultSetHandle executeQuery(const std::string &SQL);
    unsigned int getUpdateCount();
//	unsigned int executeUpdate();
    virtual void close();
 
    virtual void setSQL(const std::string &SQL);
	virtual void set(const char &data, unsigned int column);   
	virtual void set(const std::string &data, unsigned int column,unsigned int size);   
	virtual void set(const std::int16_t &data, unsigned int column);
	virtual void set(const std::int32_t &data, unsigned int column);
	virtual void set(const std::int64_t &data, unsigned int column);
//	virtual void set(const boost::uint16_t &data, unsigned int column);
//	virtual void set(const boost::uint32_t &data, unsigned int column);
//	virtual void set(const boost::uint64_t &data, unsigned int column);
	virtual void set(const float &data, unsigned int column);
	virtual void set(const double &data, unsigned int column);
	virtual void set(const std::vector<boost::uint8_t> &data, unsigned int column);
	virtual void set(const tm &data, unsigned int column);
    virtual void execute();
    virtual void reset();
};
}
#endif // SQLITESTATEMENT_H
