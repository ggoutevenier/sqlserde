#include "sqliteconnection.h"
#include "sqliteresultset.h"
#include "sqlitestatement.h"
#include "sqlite3.h"
#include <cstring>
#include <cstdio>
#include "logger.h"

class conn;
namespace DataKinesis {
SQLiteStatement::~SQLiteStatement()
{        
	close();
	LOG(INFO) << "SQLiteStatement dtor: " << this;
}


SQLiteStatement::SQLiteStatement(IConnection *conn) 
    :Statement(conn),stmt(0) {
	LOG(INFO) << "SQLiteStatement ctor: " << this;
    resetBuff();        

}

IResultSet* SQLiteStatement::executeQuery_() {
	return new SQLiteResultSet(*this);
}

SQLiteConnection *SQLiteStatement::getSQLiteConnection() {
    return (SQLiteConnection*)getConnection();    
}

void SQLiteStatement::setSQL(const std::string &SQL) {
    if(!SQL.empty()) {    
        if(stmt)
            close();
        Statement::setSQL(SQL);
        const char *cptr=this->SQL.c_str();
        sqlite3 *conn = (sqlite3*)getSQLiteConnection()->conn;
        if(sqlite3_prepare_v2(conn,cptr,strlen(cptr),&stmt,0)!=SQLITE_OK){
            throw SQLException(std::string("SQLiteStatement::setSQL error ")+sqlite3_errmsg(getSQLiteConnection()->conn));
        }
        resetBuff();
    }    
}
void SQLiteStatement::reset() {
	sqlite3_clear_bindings(stmt);
	resetBuff();
}
void SQLiteStatement::close() {
	if(stmt) {

		sqlite3_finalize(stmt);
		stmt=0;   
    }
}
void SQLiteStatement::checkStatement() {
	if (!stmt) {
		throw SQLException( "SQLiteStatement stmt is null");
	}
}

void SQLiteStatement::set(const char &data, unsigned int column) {
	checkStatement();
	sqlite3_bind_text(stmt,column,&data,1,0);
}
void SQLiteStatement::set(const std::string &data, unsigned int column,unsigned int ) {
	checkStatement();
	char *cptr = (char*)getBuff(data.length());
	memcpy(cptr, data.data(), data.length());
	sqlite3_bind_text(stmt,column,cptr,data.length(),0);
}
void SQLiteStatement::set(const boost::int16_t &data, unsigned int column) {
	checkStatement();
	sqlite3_bind_int(stmt,column,(int)data);
}
void SQLiteStatement::set(const boost::int32_t &data, unsigned int column) {
	checkStatement();
	sqlite3_bind_int(stmt,column,(int)data);
}
void SQLiteStatement::set(const boost::int64_t &data, unsigned int column) {
	checkStatement();
	sqlite3_bind_int64(stmt,column,(boost::int64_t)data);
}
/*void SQLiteStatement::set(const boost::uint16_t &data, unsigned int column) {
	checkStatement();
	sqlite3_bind_int(stmt,column,(unsigned int)data);
}
void SQLiteStatement::set(const boost::uint32_t &data, unsigned int column) {
	checkStatement();
	sqlite3_bind_int64(stmt,column,(unsigned int)data);
}
void SQLiteStatement::set(const boost::uint64_t &data, unsigned int column) {
	checkStatement();
	sqlite3_bind_int64(stmt,column,(boost::int64_t)data);
}*/
void SQLiteStatement::set(const float &data, unsigned int column) {
	checkStatement();
	sqlite3_bind_double(stmt, column, (double)data);
}
void SQLiteStatement::set(const double &data, unsigned int column) {
	checkStatement();
	sqlite3_bind_double(stmt,column,(double)data);
}

void SQLiteStatement::set(const std::vector<boost::uint8_t> &data, unsigned int column) {
	checkStatement();
	sqlite3_bind_blob(stmt,column,data.data(),data.size(),0);
}

void SQLiteStatement::set(const tm &data, unsigned int column) {
	char *dt = (char*)getBuff(20);
	strncpy(dt,TypeOps<tm>::toString(data).c_str(),20);
	sqlite3_bind_text(stmt, column, dt, 20, 0);
}

void *SQLiteStatement::getBuff(unsigned int bytes) {
    unsigned int offset=buffOffset;
    if(offset+bytes>buff.size())
        throw SQLException("SQLiteStatement::getBuff Buffer too small");
    buffOffset+=bytes;
    void *ptr=&buff.at(offset);
    memset(ptr,0,bytes);
    return ptr;
}

void SQLiteStatement::resetBuff() {
    buffOffset=0;
}



unsigned int SQLiteStatement::getUpdateCount() {
	return  (unsigned int) sqlite3_changes((sqlite3*)getSQLiteConnection()->conn);
}

void SQLiteStatement::execute() {
	int rtn=sqlite3_step(stmt);
	if (!(rtn==SQLITE_OK || rtn==SQLITE_DONE) ) {
		throw SQLException(sqlite3_errmsg(getSQLiteConnection()->conn));
	}
	sqlite3_reset(stmt);
    resetBuff();
}
void SQLiteStatement::_free(IResultSet *rset) {
	if (rset)
		delete rset;
}
IStatement::freeResultSet SQLiteStatement::getFreeResultSet() const {
	return _free;
}
}
