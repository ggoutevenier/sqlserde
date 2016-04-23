#pragma once
#include "type.h"
namespace DataKinesis {
	class Record;
	/*! offsets void pointer before calling types serde functions*/
	class Field {
		Record &record;
		std::string name; /*!< name of field*/
		size_t offset; /*!< memory address offset from base */
		IType &type; /*!< type class */
		unsigned int position; /*!< position within the record */
		unsigned int size;
		bool null;
		void *offset_(void *data) const { return (void*)(offset + (size_t)data); } /*!< member function to offset address from base*/
		const void *offset_(const void *data) const { return (const void*)(offset + (size_t)data); } /*!< member function to offset address from base*/
	public:
		Field(Record &record,std::string name, size_t offset, IType &type, size_t position) :
			record(record),
			name(name),
			offset(offset),
			type(type),
			position(position),
			size(255),
			null(true) {}
		Field(const Field &field) :
			record(field.record),
			name(field.name),
			offset(field.offset),
			type(field.type),
			position(field.position),
			size(field.size),
			null(field.null){}
		~Field() {};
		void set(ISerde &out, const  void *data) const { type.set(out, offset_(data), *this); } /*!< serialization member function*/
		void get(ISerde &in, void *data) const { type.get(in, offset_(data), *this); } /*!< deserialization member function */
		std::string getName() const { return name; }
		unsigned int getPosition() const { return position; }
		/*! used by std::find_if to see if field name is already in a stl container*/
		bool operator()(const Field &a) const { return getName().compare(a.getName()) == 0; }
		const IType &getType() const {return type;}
		Field &setSize(unsigned int size) {this->size=size;	return *this;}
		Field &setNull(bool null) {this->null=null;	return *this;}
		unsigned int getSize() const {return size;}
		bool isNull() const {return null;}
		Field &PK(unsigned int order); // defiend in record
		Field &AK(const std::string &name,unsigned int order);
		Field &IE(const std::string &name,unsigned int order);
	};
}
