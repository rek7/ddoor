#pragma once

#include <string>
#include <thread>
#include <iostream>

using namespace std;

#include "crypto.h"

class process_commands
{
private:
	string magic_header, encoded_command, decoded_command;
	crypto c;
public:
	process_commands(string, string);
	string get_correct_cmd(void);
	void execute_cmd(string);
	void decode_cmd(void);
	void strip_header(void);
};

