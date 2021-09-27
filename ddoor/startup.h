#pragma once

#include <cstdlib>
#include <cstring>

extern "C" {
	#ifdef __linux__
		#include <unistd.h>
		#include <sys/ptrace.h>
		#include <signal.h>
		#include <sys/prctl.h>
	#elif _WIN32
		#include <windows.h>
		// windows code goes here
	#else
	#endif
}

using namespace std;

class startup
{
public:
	void daemonize(void);
	void priv_esc(void);
	void cloak_name(int, char*[], const char*);
	void misc_os(void);
	void detect_debugging(void);
};

