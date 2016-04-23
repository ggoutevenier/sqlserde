#ifndef RECORD_H_
#define RECORD_H_
#include <string>
#include <list>
#include <map>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <stdexcept>
#include "field.h"
namespace DataKinesis {
	/*! record class
	holds list of fields 
	*/
	class Index : public std::map<unsigned int,const Field *> {
		std::string name;
		bool unique;
		const Record &record;
	public:
		Index(const Record &record,const std::string &name,bool unique=false):name(name),unique(unique),record(record) {}
		std::string getName() const {return name;}
		bool isUnique() const {return unique;}
		const Record &getRecord() const {return record;}
		void addField(unsigned int order,const Field &field) {
			if(!insert(std::make_pair(order,&field)).second)
				throw std::runtime_error("Unable to add field "+field.getName()+ " to index "+getName());
		}
	};
	class Record : public std::list<Field > {
		std::string name;
		Index primaryKey;

	public:
		typedef std::map<std::string,Index> Indexes;
		void setName(std::string name) { this->name = name; }
		std::string getName() const {return this->name;}
		Record():name(""),primaryKey(*this,"PK",true) {}
		~Record() {	}
		void set(ISerde &writer, const void *data) const { /*!< serialization member function*/
			for (const_iterator it = begin(); it != end(); ++it) 
				it->set(writer, data);
			writer.setRecord();
		}
		
		void get(ISerde &reader, void *data) const { /*!< deserialization function */
			for (const_iterator it = begin(); it != end(); ++it)
				it->get(reader, data);
		}
		/*! add new field to record*/
		template<class S,class T>
		Field &addField(const S &base, const T &offset, const std::string name) {
			static Type<T> type;
			size_t i = (size_t)&offset - (size_t)&base;
			if (i >= sizeof(S)) // bound check
				throw std::runtime_error("Bounds error " + name);
			Field field(*this,name, i, type,size());
			if (std::find_if(begin(), end(), field) != end()) // make sure field is only added once
				throw std::runtime_error("Field already added " + name);
			push_back(field);
			return back();
		}
		void PK(unsigned int order,const Field &field) {
			if(!primaryKey.insert(std::make_pair(order,&field)).second)
				throw std::runtime_error("Unable to add field to primaryKey "+name+" "+field.getName());
		}
		void Idx(const std::string &name,bool unique,unsigned int order,Field &field) {
			Indexes::iterator it = indexes.find(name);
			Index *ptr=0;
			if(it!=indexes.end()) {
				ptr=&it->second;
				if(ptr->isUnique()!=unique)
					throw std::runtime_error("Uninque/NonUnique ? "+ptr->getName());
			}
			else {
				std::pair<Indexes::iterator,bool> rtn=indexes.insert(std::make_pair(name,Index(*this,name,unique)));

				if(!rtn.second)
					throw std::runtime_error("Unable to add index "+name);
				ptr=&rtn.first->second;
			}
			ptr->addField(order,field);
		}
		bool hasPK() const {return !primaryKey.empty();}
		const Index &getPK() const {return primaryKey;}
		const Indexes &getIndexes() const {return indexes;}
	private:
		Indexes indexes;

	};
	Field &Field::PK(unsigned int order) {
		record.PK(order,*this);
		return *this;
	}
	Field &Field::AK(const std::string &name,unsigned int order) {
		record.Idx(name,true,order,*this);
		return *this;
	}
	Field &Field::IE(const std::string &name,unsigned int order) {
		record.Idx(name,false,order,*this);
		return *this;
	}
	template<class T> void ISerde::open() {
		static Record r;
		if (r.size() == 0) {
			makeRecord(T(), r);
		}
		return open_(r);
	}

}
#endif 
