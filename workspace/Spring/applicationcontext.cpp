#include "spring/applicationcontext.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <memory>
#include <iostream>

namespace DataKinesis {
	typedef IBean *(*BeanMakerFunc)(const std::string &, const IBean::Parameters &, const IBean::Parameters &);
#if defined (_WIN32) || defined (_WIN62)
	#include "windows.h"
	class BeanMaker {
	protected:
		typedef HINSTANCE Handle;
		typedef std::map<std::string, Handle> Dll;
		Dll dll;
		 Handle load(const std::string &fileName) {
			Handle handle = 0;
			std::wstring wstr(fileName.length(), L' ');
			std::copy(fileName.begin(), fileName.end(), wstr.begin());
			handle = LoadLibrary(wstr.c_str());
			dll[fileName] = handle;
			return handle;
		}
		 Handle getDll(const std::string &fileName) {
			Handle handle = 0;
			Dll::iterator it = dll.find(fileName);
			if (it != dll.end())
				handle = it->second;
			else {
				handle = load(fileName);
			}
			return handle;
		}
	public:
		BeanMakerFunc getBeanMaker(const std::string &fileName, const std::string &funcName) {
			Handle handle = getDll(fileName);
			return (BeanMakerFunc)GetProcAddress(handle, funcName.c_str());
		}
		~BeanMaker() {
			for (Dll::const_iterator it = dll.begin(); it != dll.end(); ++it)
				FreeLibrary(it->second);
		}
	};
#else
#include <dlfcn.h>
	class BeanMaker {
	protected:
		typedef void * Handle;
		typedef std::map<std::string, Handle> Dll;
		Dll dll;
		Handle load(const std::string &fileName) {
			Handle handle = dlopen(fileName.c_str(), RTLD_LAZY);
			if(!handle) {
				std::cout <<dlerror() <<std::endl;
				throw std::runtime_error("Unable to load library "+fileName);
			}
			dll[fileName] = handle;
			return handle;
		}
		 Handle getDll(const std::string &fileName) {
			Handle handle = 0;
			Dll::iterator it = dll.find(fileName);
			if (it != dll.end())
				handle = it->second;
			else
				handle = load(fileName);

			return handle;
		}
	public:
		BeanMakerFunc getBeanMaker(const std::string &fileName, const std::string &funcName) {
			Handle handle = getDll(fileName);
			BeanMakerFunc func =(BeanMakerFunc)dlsym(handle, funcName.c_str());
			if(!func){
				throw std::runtime_error("Unable to find function "+funcName+
						" in library "+fileName);
			}
			return func;
		}
		~BeanMaker() {
			for (Dll::const_iterator it = dll.begin(); it != dll.end(); ++it)
				dlclose(it->second);
		}
	};
#endif


	ApplicationContext::ApplicationContext(const std::string &fileName) {
		libs = std::unique_ptr<BeanMaker>(new BeanMaker);
		readXML(fileName);
	}
	ApplicationContext::~ApplicationContext() {
		// make sure that beans are all destroyed before libs are freed
		beans.clear();
		libs.reset();
	}
	std::shared_ptr<IBean> ApplicationContext::getBean(std::string name) {
		Beans::const_iterator it = beans.find(name);
		if (it == beans.end())
			throw std::runtime_error("bean not found " + name);
		std::shared_ptr<IBean> bean = it->second;
		loadDep(bean);
		if(!bean->isConstructed())
			bean->construct();
		return bean;
	}
	void ApplicationContext::loadDep(std::shared_ptr<IBean> bean) {
		typedef IBean::Refs R;
		R &r = bean->getRefs();
		for (R::iterator it = r.begin(); it != r.end(); ++it)
			it->second.bean = getBean(it->second.name);
	}

	void ApplicationContext::addBean(IBean *bean) {
		if (!bean)
			throw std::runtime_error("invalid bean");
		if (!beans.insert(std::make_pair(bean->getName(),
				std::unique_ptr<IBean>(bean)) ).second)
			throw std::runtime_error("Unable to add bean " + bean->getName());
	}
	void ApplicationContext::loadBean(
		const std::string &fileName,
		const std::string &funcName,
		const std::string &beanName,
		const IBean::Parameters &parameters,
		const IBean::Parameters &refs) {

		BeanMakerFunc beanMaker = libs->getBeanMaker(fileName, funcName);
		addBean(beanMaker(beanName, parameters, refs));
	}
	void ApplicationContext::readXML(const std::string &fileName) {
		struct Vals {
			std::string id;
			std::string library;
			std::string maker;
			std::map<std::string, std::string> parameters, refs;
		};
		boost::property_tree::ptree pt;
		boost::property_tree::read_xml(fileName, pt);

		BOOST_FOREACH(boost::property_tree::ptree::value_type &bean, pt.get_child("beans")) {
//		for(boost::property_tree::ptree::value_type bean : pt.get_child("bean")) {
			Vals vals;
			vals.id = bean.second.get<std::string>("<xmlattr>.id");
			vals.library = bean.second.get<std::string>("<xmlattr>.library");
			vals.maker = bean.second.get<std::string>("<xmlattr>.maker");
			boost::optional<boost::property_tree::ptree &> properties=bean.second.get_child_optional("properties");
			if(properties) {
				BOOST_FOREACH(boost::property_tree::ptree::value_type &prop, properties.get()) {
					std::string propName = prop.second.get<std::string>("<xmlattr>.name");
					boost::optional<std::string> propValue = prop.second.get_optional<std::string>("<xmlattr>.value");
					boost::optional<std::string> propRef = prop.second.get_optional<std::string>("<xmlattr>.ref");
					if (propValue)
						vals.parameters[propName] = propValue.get();
					else if (propRef)
						vals.refs[propName] = propRef.get();
				}
			}
			loadBean(vals.library, vals.maker, vals.id, vals.parameters, vals.refs);
		}
	}
	void ApplicationContext::updateParameter(
			const std::string &beanName,
			const std::string &parameterName,
			const std::string &parameterValue) {
		Beans::const_iterator it = beans.find(beanName);
		if (it == beans.end())
					throw std::runtime_error("bean not found " + beanName);
		IBean::Parameters &p=it->second->getParameters();
		p[parameterName]=parameterValue;
	}
}
