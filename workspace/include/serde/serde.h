#pragma once
#include "serde/iserde.h"

namespace DataKinesis {
	class Record;
	class Field;
	class Serde : public ISerde {
	protected:
		const Record *record;
	public:
		Serde() : record(0) {}
		~Serde() {}
		virtual void open_(const Record &record) {this->record=&record;}
		virtual void close() {this->record=0;}
		virtual bool isOpen() const {return record!=0;}
	};

}
