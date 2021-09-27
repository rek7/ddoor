#include "crypto.h"

string crypto::hex_decode(string hex_encoded) {
	string hex_decoded;
	size_t len = hex_encoded.length();
	for (int i = 0; i < len; i += 2) {
		string byte = hex_encoded.substr(i, 2);
		char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
		hex_decoded.push_back(chr);
	}
	return hex_decoded;
}

string crypto::xor_operation(string data, string xor_key) {
	string output;
	for (int i = 0; i < data.size(); i++)
	{
		output += data[i] ^ xor_key[i % (xor_key.size() / sizeof(char))];
	}
	return output;
}