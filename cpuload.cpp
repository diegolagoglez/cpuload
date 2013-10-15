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
get_cpuload_env(const string name, bool put_value = false, int new_value = 0) {
	char* value = getenv(name.c_str());
	if(put_value) {
		set_cpuload_env(name, new_value);
	}
	if(value != NULL) {	
		return string_to_int(string(value));
	} else {
		return 0;
	}
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
cpu_count() {
	return 1;
}

static float
calculate_cpu_load() {
	long hs = time_hs();
	long idle = get_cpu_idle();

	long prev_hs = get_cpuload_env(EnvCpuLoadHS);
	long prev_idle = 0;

	if(prev_hs > 0) {
		// Previous values exist.
		prev_idle = get_cpuload_env(EnvCpuLoadIdle, true, idle);
		set_cpuload_env(EnvCpuLoadHS, hs);
	} else {
		// Previous values do not exist.
		prev_hs = hs;
		prev_idle = idle;
		usleep(500000);
		hs = time_hs();
		idle = get_cpu_idle();
		set_cpuload_env(EnvCpuLoadIdle, idle);
		set_cpuload_env(EnvCpuLoadHS, hs);
	}

	hs -= prev_hs;
	idle -= prev_idle;

	idle /= cpu_count();

	float real_idle;

	// Check some values.
	if(hs == 0 || idle == 0) {
		real_idle = 0;
	} else {
		real_idle = idle * 100 / hs;
	}

	return 100 - real_idle;
}

int
main() {

	float cpu_load = calculate_cpu_load();

	cout << setw(2) << setfill('0') << setprecision(2) << fixed << cpu_load << endl;

	return 0;
}
