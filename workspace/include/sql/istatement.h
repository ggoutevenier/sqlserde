#ifndef ISTATEMENT_H
#define ISTATEMENT_H
#include <memory>
#include "sqlhandle.h"
#include <cstdint>
#include <string>
#include <vector>

#include "typeops.h"
#include "idriver.h"

namespace DataKinesis
{
	class ResultSet;
	class IStatement
	{
		friend ResultSet;
	protected:
		virtual IConnection *getConnection()=0;
		virtual IDriver *getDriver() = 0;
		typedef void(*freeResultSet)(IResultSet *);
		virtual freeResultSet getFreeResultSet() const = 0;
	public:
		IStatement() {}
		virtual ~IStatement() {}
		virtual std::string getSQL() const=0;
		virtual void setSQL(const std::string &SQL)=0;
		virtual ResultSetHandle executeQuery()=0;
		virtual unsigned int executeUpdate()=0;
		virtual void execute()=0;
		virtual ResultSetHandle executeQuery(const std::string &SQL)=0;
		virtual unsigned int executeUpdate(const std::string &SQL)=0;
		virtual void execute(const std::string &SQL)=0;
		virtual void close()=0;
		virtual unsigned int getUpdateCount()=0;
	};
	class IPreparedStatement : public IStatement
	{
	public:
		IPreparedStatement() {}
		virtual ~IPreparedStatement() {}
		virtual void set(const bool &data, unsigned int column)=0;
		virtual void set(const std::string &data, unsigned int column,unsigned int size)=0;
		virtual void set(const char &data, unsigned int column) = 0;
		virtual void set(const std::int16_t &data, unsigned int column)=0;
		virtual void set(const std::int32_t &data, unsigned int column)=0;
		virtual void set(const std::int64_t &data, unsigned int column)=0;
		virtual void set(const boost::uint16_t &data, unsigned int column)=0;
		virtual void set(const boost::uint32_t &data, unsigned int column)=0;
		virtual void set(const boost::uint64_t &data, unsigned int column)=0;
		virtual void set(const float &data, unsigned int column)=0;
		virtual void set(const double &data, unsigned int column)=0;
		virtual void set(const std::vector<boost::uint8_t> &dat, unsigned int column)=0;
		virtual void set(const tm &data, unsigned int column)=0;
		virtual void reset()=0;
	};
}
#endif
