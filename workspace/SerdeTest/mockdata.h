#include <sstream>
#include <fstream>
#include <unistd.h>

using namespace DataKinesis;

// create data
void mockData(std::ostream &out) {
	out << "age,first_name,last_name,married,postal_code,bignum,income,networth" << std::endl;
	out << "25,Joe,Smith,0,11050,123456789,49999.99,1e10" << std::endl;
	out << "30,Jane,Doe,1,11093,9876543210,99999,1e11" << std::endl;
}
void mockdata(const std::string &filename) {
	unlink(filename.c_str());

	std::stringstream ss;
	mockData(ss);
	std::ofstream out;
	out.open(filename);
	out << ss.str();
	out.close();
}
