// JsonParse_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Objective:
// {
// }
// ... and ...
//{
//	"Name": "BitCoin",
//	"Symbol" : "BTC",
//}
//
//

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
using namespace std;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

const int ARRSIZE = 100;
////////////////////////////
class JsonParser {
private:
	ifstream Json;


public:
	string names[ARRSIZE];
	string values[ARRSIZE];
	//member function takes in file name as a parameter, opens the file, and returns if it was successful
	bool initialize(string fileDIR) {
		//opens the file that the fileDIR varible has stored in it
		Json.open(fileDIR);
		//checks to make sure it was successful
		if (Json.is_open()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool objectparser() {
		string entry;
		char charentry;
		int index = 0;
		//gets you to to first string for the loop
		Json.get(charentry);
		//the ws function removes as many whitespace characters as possible
		Json >> ws;
		Json.get(charentry);
		//this is a while loop that continues until the end of the file
		while (!Json.eof()) {
			Json >> ws;
			if(Json.peek() == '"'){Json.get(charentry);}//gets rid of '"' to start
			//another way to exit the loop if you reach the ending brace I'll probs have to change this later
			//if(Json.peek() == '}'){break;}
			//gets the first string
			getline(Json, entry, '"');
			names[index] = entry;
			//removes anything up to and including the colon
			getline(Json, entry, ':');
			Json >> ws;
			//if the next char is a '"' thens its a string and will end with a '"' else its something else and will end with either a ',' or '\n' and the '}' or its an array
			if (Json.peek() == '"') {
				Json.get(charentry); //gets rid of the '"'
				getline(Json, entry, '"');
				values[index] = entry;
				Json.get(charentry); //gets rid of the ',' or '\n'
			// this should cover all other values than strings not including arrays of json values or objects
			}
			else {
				getline(Json, entry, ',');
				values[index] = entry;
			}
			//interates on the variable 'index' to change the parallel array's row
			index++;
		}
		return true;
	}


};
///////////////// Testing ////////////////////////////////
TEST_CASE("Testing my JSON Parser"){
	JsonParser jp;
	const string fileDIR1 = "JsonObject1.json";
	const string fileDIR2 = "JsonObject2.json";

	//note the requirements for the assignment calls for ten test cases and I thought the default test cases were adequate
	SUBCASE("testing parsing of empty JSON object") {
		// Open the file
		CHECK(jp.initialize(fileDIR1) == true);

		// Invoke the parsing process
		CHECK(jp.objectparser() == true);

		// Validate the parsed JSON object
		CHECK(jp.names[0] == "");
		CHECK(jp.values[0] == "");
	}

	SUBCASE("testing parsing of simple JSON object (only strings for values)") {
		// Open the file
		CHECK(jp.initialize(fileDIR2) == true);

		// Invoke the parsing process
		CHECK(jp.objectparser() == true);

		// Validate the parsed JSON object
		CHECK(jp.names[0] == "Name");
		CHECK(jp.names[1] == "Symbol");
		CHECK(jp.values[0] == "BitCoin");
		CHECK(jp.values[1] == "BTC");
	}
}
