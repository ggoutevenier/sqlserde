#include "sqliteresultdetmetadata.h"
#include "sqliteresultset.h"
#include "sqlitestatement.h"

#include "sql/resultset.h"
#include "sqlite3.h"
namespace DataKinesis
{

	SQLiteResultSetMetaData::SQLiteResultSetMetaData(IResultSet *rset):ResultSetMetaData(rset) {

	}

SQLiteResultSetMetaData::~SQLiteResultSetMetaData()
{
}

SQLiteStatement *SQLiteResultSetMetaData::getSQLiteStatement() {
    return (SQLiteStatement*)rset->getStatement();
}
unsigned int SQLiteResultSetMetaData::getColumnCount() {
    return sqlite3_column_count(getSQLiteStatement()->stmt);
}

std::string SQLiteResultSetMetaData::getColumnName(unsigned int column) {
    const char *cptr="";
    if(sqlite3_compileoption_used("SQLITE_ENABLE_COLUMN_METADATA"))
        cptr=sqlite3_column_name(getSQLiteStatement()->stmt,column);

    return cptr;
}

std::string SQLiteResultSetMetaData::getTableName(unsigned int column) {
    const char *cptr="";
    if(sqlite3_compileoption_used("SQLITE_ENABLE_COLUMN_METADATA"))
        cptr=sqlite3_column_origin_name(getSQLiteStatement()->stmt,column);

    return cptr;
}


}

