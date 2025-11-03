#include <iostream>
#include <string>
#include "header.h"
#include <cassert>
using namespace std;


int main()
{
	string data[5] = { "12.35", "67", "mamaBbcc!", "mamaBbcc!", "mamaBbcc!" };
	string folks[8] = { "samwell", "jon", "margaery", "daenerys", "tyrion", "sansa", "magdalena", "jon" };
	assert(hasTwoOrMoreDuplicates(folks, 8) == false);
	assert(hasTwoOrMoreDuplicates(folks, 3) == false);
	assert(hasTwoOrMoreDuplicates(data, 5) == true);
	assert(identicalValuesTogether(folks, 8) == false);
	assert(identicalValuesTogether(data, 5) == true);
	return 0;
}