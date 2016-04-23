#pragma once
#include <map>
#include <memory>
#include "ibean.h"
namespace DataKinesis {
	class BeanMaker;
	class ApplicationContext {
		std::unique_ptr<BeanMaker> libs;
		typedef std::map<std::string, std::shared_ptr<IBean> > Beans;
		Beans beans;
		std::shared_ptr<IBean>getBean(std::string name);
		void loadDep(std::shared_ptr<IBean> bean);
		void readXML(const std::string &fileName);
		void addBean(IBean *bean);
		void loadBean(
			const std::string &fileName,
			const std::string &funcName,
			const std::string &beanName,
			const IBean::Parameters &parameters,
			const IBean::Parameters &refs);
	public:
		ApplicationContext(const std::string &fileName);
		~ApplicationContext();
		void updateParameter(const std::string &beanName,const std::string &parameterName,const std::string &parameterValue);
		template<class T>
		T &get(const std::string &name) { return *(T*)getBean(name)->get(); }
	};
}
