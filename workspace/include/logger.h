#pragma once
#include "easylogging++.h"
namespace DataKinesis {
	class Logger {
	public:
		Logger();
		Logger(int, char**);
		~Logger();
		void event(int a);
		static el::base::type::StoragePointer getStorage();
	private:
		static int runOnceInd;
		static int runOnce();
	};
}

