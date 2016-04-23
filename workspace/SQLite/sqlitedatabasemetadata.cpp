#include <sqliteconnection.h>
#include <sqlitedatabasemetadata.h>

namespace DataKinesis
{


SQLiteDatabaseMetaData::SQLiteDatabaseMetaData(Connection *conn):DatabaseMetaData(conn) {
}

SQLiteDatabaseMetaData::~SQLiteDatabaseMetaData()
{
}

IConnection *SQLiteDatabaseMetaData::getConnection () {
    return conn;
}
ResultSetHandle SQLiteDatabaseMetaData::getTables(const std::string &catalogName,const std::string &schemaName, const std::string &tableNamePattern) {
    std::string sql ="select type,name,tbl_name,rootpage,sql from sqlite_master";
    if(!tableNamePattern.empty())
        sql=sql+" where tbl_name='"+tableNamePattern+"'";
    return getConnection()->executeQuery(sql);
}

}
