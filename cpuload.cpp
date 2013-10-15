#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include <sys/time.h>
#include <unistd.h>

using namespace std;

const string CpuLoadFile	= "/proc/stat";
const string EnvCpuLoadHS	= "CPULOAD_HS";
const string EnvCpuLoadIdle	= "CPULOAD_IDLE";

static long
string_to_int(const string value) {
	static stringstream s;
	s.str(value);
	long result;
	s >> result;
	return result;
}

static long
time_hs() {
	timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec * 100 + static_cast<long>(t.tv_usec / 10000.0);
}

static bool
set_cpuload_env(const string name, int value) {
	static stringstream var_ctor;

	var_ctor.str(name);
	var_ctor << "=" << value;
	putenv(const_cast<char*>(var_ctor.str().c_str()));	// TODO Check return value.

	return true;
}

static int
get_cpuload_env(const string name, int new_value = 0) {
	char* value = getenv(name.c_str());
	set_cpuload_env(name, new_value);
	if(value != NULL) {	
		return string_to_int(string(value));
	} else {
		return 0;
	}
}

static float
calculate_cpu_load(int hs, int idle, int last_hs, int last_idle) {
	return 42;
}

static long
get_cpu_idle(int processor_index = -1) {
	string dummy, idle;

	if(processor_index > -1) {
		throw new runtime_error("CPU Load: Getting CPU load of any specific processor is unsupported.");
	} else {
		ifstream file(CpuLoadFile.c_str(), std::ifstream::in);
		char* line = new char[128];
		file.getline(line, 128);
		istringstream sline(line);
		delete[] line;	// Consumes a lot of time; should be commented since program ends ahead.

		sline >> dummy >> dummy >> dummy >> dummy >> idle;	// Get forth value: cpu idle
	}

	return string_to_int(idle);
}

int
main() {

	long idle = get_cpu_idle();	

	float cpu_load = calculate_cpu_load(time_hs(), idle, 0, 0);

	cout << setw(2) << setfill('0') << setprecision(2) << fixed << cpu_load << endl;

	return 0;
}
