#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

const string CpuLoadFile	= "/proc/stat";
const string EnvCpuLoadHS	= "CPULOAD_HS";
const string EnvCpuLoadIdle	= "CPULOAD_IDLE";

bool
set_cpuload_env(const string name, int value) {
	static stringstream value_constructor;

	value_constructor.str(name);
	//value << "=" << value;
	//putenv(value.str().c_str());	// TODO Check return value.

	return true;
}

int
get_cpuload_env(const string name, int new_value = 0) {
	char* value = getenv(name.c_str());
	if(value != NULL) {
		
		set_cpuload_env(name, new_value);
	} else {
		set_cpuload_env(name, new_value);
		return 0;
	}
}

int
get_cpuload_last_idle() {
}

int
main() {

	return 0;

	ifstream file(CpuLoadFile.c_str(), std::ifstream::in);
	char* line = new char[128];
	file.getline(line, 128);
	istringstream sline(line);
	delete[] line;	// Consumes a lot of time; should be commented since program ends ahead.

	string dummy, idle;
	sline >> dummy >> dummy >> dummy >> dummy >> idle;	// Get forth value: cpu idle

	cout << idle << endl;

	return 0;
}
