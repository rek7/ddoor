#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "httplib.h"

using namespace std;

#include "crypto.h"

class doh_query
{
public:
	doh_query(string, string, string, vector<string>);
	const char* choose_doh_server(void);
	bool is_correct_header(string);
	string get_txt_results(void);
private:
	string domain, magic_header, xor_pass;
	vector<string>doh_resolvers;
	crypto c;
};

