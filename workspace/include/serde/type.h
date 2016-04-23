#pragma once
#include "itype.h"
#include "dialect.h"
#include "typeops.h"
namespace DataKinesis {
	/*! recasts void pointer to correct data type and calls serde
	*/
	template<class T>
	class Type : public IType {
		T &typed_(void *v) const { return *(T *)v; }
		const T &typed_(const void *v) const { return *(const T *)v; }
	public:
		virtual ~Type() {}
		void set(ISerde &writer, const  void *data, const Field &field) const { writer.set(typed_(data), field); }
		void get(ISerde &reader, void *data, const Field &field) const { reader.get(typed_(data), field); }
		std::string getName(const Dialect &dialect,const Field &field) const {return dialect.getTypeName((T*)0,field);}
		virtual void fromString(const std::string &str,void * data) const {TypeOps<T>::fromString(str,*(T*)data);}
		virtual std::string toString(const void * data) const {return TypeOps<T>::toString(*(T*)data);}
	};
}
