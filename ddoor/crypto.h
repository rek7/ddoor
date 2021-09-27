#pragma once

#include <cstdlib>
#include <cstring>
#include <string>

using namespace std;

class crypto
{
public:
	string hex_decode(string);
	string xor_operation(string, string);
};

