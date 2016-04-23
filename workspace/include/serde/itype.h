#pragma once
#include "iserde.h"
#include <string>

namespace DataKinesis {
	class Dialect;
	class IType {
	public:
		virtual ~IType() {}
		virtual void set(ISerde &out, const  void *data, const Field &field) const = 0;
		virtual void get(ISerde &in, void *data, const Field &field) const = 0;
		virtual std::string getName(const Dialect &dialect,const Field &field) const=0;
		virtual void fromString(const std::string &str,void * data) const=0;
		virtual std::string toString(const void *data) const=0;
	};
}
