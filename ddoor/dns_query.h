#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
extern "C" {
	#ifdef __linux__
		#include <resolv.h>
		#include <arpa/inet.h>
	#elif _WIN32
		#define _WINSOCK_DEPRECATED_NO_WARNINGS

		#pragma comment(lib, "Dnsapi.lib")
		#pragma comment(lib, "Ws2_32.lib")
		#include <winsock2.h>
		#include <WinDNS.h>
	#else
	#endif
}

using namespace std;

#include "crypto.h"

class dns_query
{
public:
	dns_query(string, string, string, vector<string>);
	const char* choose_dns_server(void);
	bool is_correct_header(string);
	string get_txt_results(void);
private:
	string domain, magic_header, xor_pass;
	vector<string>dns_resolvers;
	crypto c;
};

