#pragma once
#include <string>
#include <memory>
#include <map>
#include <exception>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <exception>
#include <boost/lexical_cast.hpp>
#include "ibean.h"
#include <iostream>
#include "defs.h"

namespace DataKinesis {
	template<class T>
	class Bean : public IBean {
	protected:
		std::string name;
		Parameters values;
		Refs refs;
		virtual Refs &getRefs() { return refs; }
		std::shared_ptr<T> smartT;
		T *t;
		// if pointer is passed in assume ownership needs to be taken
		void add(T *t) {
			smartT = std::shared_ptr<T>(t);
			add(*t);
		}
		void add(T &t) {
			this->t = &t;
		}
		void add(std::shared_ptr<T> t) {
			smartT=t;
			add(*t);
		}
		template<class U>
		U &getRef(std::string name) {
			Refs::iterator it = getRefs().find(name);
			if (it == getRefs().end())
				throw std::runtime_error("unable to find ref " + name);
			return *(U*)it->second.bean->get();
		}

	public:
		bool isConstructed() { return t != 0; }
		Bean(std::string name,const Parameters &values,const Parameters &refs) :name(name),values(values),t(0) {
			for (Parameters::value_type val : refs) 
				this->refs[val.first] = Ref(val.second);
		}
		~Bean() {
		}
		std::string getName() const { return name; }
		template<class U>
		U getValue(const std::string  &name) const {
			Parameters::const_iterator it = values.find(name);
			if (it == values.end())
				throw std::runtime_error("Value not found for " + name);
			return boost::lexical_cast<U>(it->second);
		}
		void *get() {
			if (!t)
				throw std::runtime_error("Bean not constructed");
			return t;
		}
		virtual void construct()=0;
		Parameters &getParameters() {return values;}
	};
}
