#include <sqliteconnection.h>
#include <sqliteresultdetmetadata.h>
#include <sqliteresultset.h>
#include <sqlitestatement.h>
#include "sqlite3.h"
#include <cstring>
#include <cstdio>

namespace DataKinesis {
SQLiteResultSet::SQLiteResultSet(SQLiteResultSet &resultSet)
    :ResultSet(resultSet),lastStatus(0)
{
}
SQLiteResultSet::SQLiteResultSet(SQLiteStatement &statement)
    :ResultSet((Statement*)&statement),lastStatus(0)
{
}
SQLiteResultSet::~SQLiteResultSet()
{
}
bool SQLiteResultSet::completed() {
    return lastStatus==SQLITE_OK || lastStatus==SQLITE_DONE || SQLITE_ROW;
}
bool SQLiteResultSet::next() {
    lastStatus=sqlite3_step(getSQLiteStatement()->stmt);
	return lastStatus==SQLITE_ROW;
}

void SQLiteResultSet::get(char &data, unsigned int column) {
    if(getColumnSize(column)>1)
        throw SQLException("Multiple bytes cannot be placed in char");
        
 	data=*(char*)sqlite3_column_text(getSQLiteStatement()->stmt,column-1);
}
void SQLiteResultSet::get(std::string &data, unsigned int column) {
 	const char *cptr=(const char*)sqlite3_column_text(getSQLiteStatement()->stmt,column-1);
	if(!*cptr)
		data.clear();
	else {
		data = cptr;
    }
}

void SQLiteResultSet::get(boost::int16_t &data, unsigned int column) {
	data=(boost::int16_t)sqlite3_column_int(getSQLiteStatement()->stmt,column-1);
}
void SQLiteResultSet::get(boost::int32_t &data, unsigned int column) {
	data=(boost::int32_t)sqlite3_column_int(getSQLiteStatement()->stmt,column-1);
}
void SQLiteResultSet::get(boost::int64_t &data, unsigned int column) {
	data=(boost::int64_t)sqlite3_column_int64(getSQLiteStatement()->stmt,column-1);
}
void SQLiteResultSet::get(boost::uint16_t &data, unsigned int column) {
	data=(boost::uint16_t)sqlite3_column_int(getSQLiteStatement()->stmt,column-1);
}
void SQLiteResultSet::get(boost::uint32_t &data, unsigned int column) {
	data=(boost::uint32_t)sqlite3_column_int(getSQLiteStatement()->stmt,column-1);
}
void SQLiteResultSet::get(boost::uint64_t &data, unsigned int column) {
	data=(boost::uint64_t)sqlite3_column_int64(getSQLiteStatement()->stmt,column-1);
}

void SQLiteResultSet::get(float &data, unsigned int column) {
	data=(float)sqlite3_column_double(getSQLiteStatement()->stmt,column-1);
}
size_t SQLiteResultSet::getColumnSize(unsigned int column) {
    return sqlite3_column_bytes(getSQLiteStatement()->stmt,column-1);
}
void SQLiteResultSet::get(double &data, unsigned int column)  {
	data=(double)sqlite3_column_double(getSQLiteStatement()->stmt,column-1);
}
void SQLiteResultSet::get(tm &data, unsigned int column) {
	const char *cptr = (const char*)sqlite3_column_text(getSQLiteStatement()->stmt, column - 1);
	TypeOps<tm>::fromString(cptr, data);
}
void SQLiteResultSet::get(std::vector<boost::uint8_t> &data, unsigned int column)  {
	size_t bytes=getColumnSize(column);    
    void *ptr=(void*)sqlite3_column_blob(getSQLiteStatement()->stmt,column-1);
	data.resize(bytes);
	memcpy(data.data(),ptr,bytes);
}

/*
 * Upcast statement
 * */

SQLiteStatement *SQLiteResultSet::getSQLiteStatement(){
	return (SQLiteStatement *)getStatement();
}

void SQLiteResultSet::close() {
    getStatement()->close();
}

IResultSetMetaData *SQLiteResultSet::getMetaData_() {
	return new SQLiteResultSetMetaData(this);
}
IResultSet::freeResultSetMetaData SQLiteResultSet::getFreeResultSetMetaData() const {
	return _free;
}
void SQLiteResultSet::_free(IResultSetMetaData *mdata) {
	if (mdata)
		delete mdata;
}
}
