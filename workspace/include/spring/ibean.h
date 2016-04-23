#pragma once
#include <string>
#include <map>
#include <stdexcept>

namespace DataKinesis {
	class ApplicationContext;
	class IBean {
		friend class ApplicationContext;
	public:
		virtual std::string getName() const = 0;
		virtual ~IBean() {}
		typedef std::map<std::string, std::string> Parameters;
		struct Ref {
			std::string name;
			std::shared_ptr<IBean> bean;
			Ref() :name(""), bean(0) {}
			Ref(const std::string &name) :name(name), bean(0) {}
			Ref(const Ref &ref) :name(ref.name), bean(ref.bean) {}
		};
		virtual void *get() = 0;
		typedef std::map<std::string, Ref> Refs;
		virtual bool isConstructed() = 0;
		virtual Parameters &getParameters() = 0;
	protected:
		virtual Refs &getRefs() = 0;
		virtual void construct() = 0;
	};
}
