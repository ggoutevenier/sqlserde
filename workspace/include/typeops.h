#ifndef TYPEOPS_H
#define TYPEOPS_H
#include <ctime>
#include <string>
#include <cstdio>
#include <boost/lexical_cast.hpp>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace DataKinesis {
	template<class T>
	class TypeOps {
		TypeOps() {}
	public:
		static bool less(const  T &a, const  T &b) { return a < b; }
		static bool equal(const  T &a, const  T &b) {return !(TypeOps<T>::less(a,b)  || TypeOps<T>::less(b,a));}
		static std::string toString(const T &data) { return boost::lexical_cast<std::string>(data); }
		static void fromString(const std::string str, T &data) { data = boost::lexical_cast<T>(str); }
		static void maxValue(T &data) {data = std::numeric_limits<T>::max();}
		static void minValue(T &data) { data = std::numeric_limits<T>::min(); }
		static void randValue(T &data) { 
			data = (T)(std::rand());
		}
	};
	template<>
	class TypeOps<std::string> {
		typedef std::string T;
		TypeOps() {}
	public:
		static bool less(const  T &a, const  T &b) { return a < b; }
		static bool equal(const  T &a, const  T &b) {return !(TypeOps<T>::less(a,b)  || TypeOps<T>::less(b,a));}
		static std::string toString(const T &data) { return data; }
		static void fromString(const std::string str, T &data) { data = str; }
		static void maxValue(T &data) { data = "max"; }
		static void minValue(T &data) { data = "min"; }
		static void randValue(T &data) { data = boost::lexical_cast<T>(std::rand()); }
	};

	template<>
	class TypeOps<std::vector<boost::uint8_t> > {
		typedef std::vector<boost::uint8_t> T;
		TypeOps() {}
	public:
		static bool less(const  T &a, const  T &b) { return memcmp(a.data(), b.data(), a.size())<0; }
		static bool equal(const  T &a, const  T &b) {return !(TypeOps<T>::less(a,b)  || TypeOps<T>::less(b,a));}
		static std::string toString(const T &data) { return (const char*)data.data(); }
		static void fromString(const std::string str, T &data) { 
			data.resize(str.length());
			memcpy(data.data(),str.data(),data.size()); 
		}
		static void maxValue(T &data) { data.resize(3); memcpy(data.data(),"max",3); }
		static void minValue(T &data) { data.resize(3); memcpy(data.data(), "max", 3); }
		static void randValue(T &data) { 
			std::string str = boost::lexical_cast<std::string>(std::rand());
			data.resize(str.length()+1);
//			strncpy_s((char*)data.data(),data.size(),str.c_str(),str.length());
			strncpy((char*)data.data(),str.c_str(),str.length());
		}
	};
	template<>
	class TypeOps<tm> {
		TypeOps() {}
	public:
		static void maxValue(tm &data) {
//			time_t t;
//			TypeOps<time_t>::maxValue(t);
//			t &= 0x6FFFFFFFF; //localtime overflows mask data
//			t = (time_t)(t*1.09);
			data.tm_year = 3000-1900;
			data.tm_mon = 12-1;
			data.tm_mday = 31;
			data.tm_hour = 23;
			data.tm_min = 59;
			data.tm_sec = 59;
//			t=mktime(&data);
//			localtime_s(&data, &t);
		}
		static void minValue(tm &data) {
			data.tm_year = 1900 - 1900;
			data.tm_mon = 1 - 1;
			data.tm_mday = 1;
			data.tm_hour = 0;
			data.tm_min = 0;
			data.tm_sec = 0;
		}
		static void randValue(tm &data) {
			time_t t;
			TypeOps<time_t>::randValue(t);
			t &= 0x6FFFFFFFF; //localtime overflows mask data
//			localtime_s(&data, &t);
			memcpy(&data,localtime(&t),sizeof(tm));
		}
		static bool less(const  tm &a, const  tm &b) {
			if (a.tm_year < b.tm_year) 
				return true;
			else if (a.tm_mon < b.tm_mon) 
				return true;
			else if (a.tm_mday < b.tm_mday) 
				return true;
			else if (a.tm_hour < b.tm_hour) 
				return true;
			else if (a.tm_min < b.tm_min) 
				return true;
			else if (a.tm_sec < b.tm_sec) 
				return true;
			return false;
		}
		static bool equal(const  tm &a, const  tm &b) {return !(TypeOps<tm>::less(a,b)  || TypeOps<tm>::less(b,a));}
		static std::string toString(const tm &data) {
			char dt[60];
			sprintf(dt, "%4d-%02d-%02d %02d:%02d:%02d", data.tm_year + 1900, data.tm_mon + 1, data.tm_mday, data.tm_hour, data.tm_min, data.tm_sec);
			return dt;
		}
		static void fromString(const std::string str, tm &data) {
			sscanf(str.c_str(), "%4d-%02d-%02d %02d:%02d:%02d", &data.tm_year, &data.tm_mon, &data.tm_mday, &data.tm_hour, &data.tm_min, &data.tm_sec);
			data.tm_year -= 1900;
			data.tm_mon -= 1;
		}
	};
}
#endif
