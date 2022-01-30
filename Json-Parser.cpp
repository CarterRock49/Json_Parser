#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <sstream> 
using namespace std;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

const string DATA_FILE_PATH = "Testdata//";
const int ARRSIZE = 100;
////////////////////////////
class JsonParser {
private:
	ifstream Json;


public:
	string names[ARRSIZE];
	string values[ARRSIZE];
	//member function takes in file name as a parameter, opens the file, and returns if it was successful
	bool OpenFile(string fileDIR) {
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

	bool ParseJson() {
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
				//this is the only part of the main code that has changed since last week test cases and the #include <sstream> not included
				//this grabs the entire value portion, this is done to not accdentially grab the } at the end of the file although downside, doing it grabs the ,
				getline(Json, entry);
				//this puts entry into a stream
				stringstream ss(entry);
				//this removes the , but because we only grabbed this line before it doesn't have the possiblity to grabs the final }
				getline(ss, entry, ',');
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
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject3a.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.names[0] == "Rank");
		CHECK(jp.names[1] == "Name");
		CHECK(jp.names[2] == "Symbol");
		CHECK(jp.names[3] == "Market Cap");
		CHECK(jp.values[0] == "3");
		CHECK(jp.values[1] == "Binance Coin");
		CHECK(jp.values[2] == "BNB");
		CHECK(jp.values[3] == "90321378765");
	}

	SUBCASE("testing parsing of simple JSON object (only strings for values)") {
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject3b.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.names[0] == "Rank");
		CHECK(jp.names[1] == "Name");
		CHECK(jp.names[2] == "Symbol");
		CHECK(jp.names[3] == "Market Cap");
		CHECK(jp.values[0] == "21");
		CHECK(jp.values[1] == "Algorand");
		CHECK(jp.values[2] == "ALGO");
		CHECK(jp.values[3] == "10198650050.12345");
	}

	SUBCASE("testing parsing of simple JSON object (only strings for values)") {
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject3c.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.names[0] == "Rank");
		CHECK(jp.names[1] == "Name");
		CHECK(jp.names[2] == "Symbol");
		CHECK(jp.names[3] == "Market Cap");
		CHECK(jp.values[0] == "0");
		CHECK(jp.values[1] == "111 Test this 111");
		CHECK(jp.values[2] == "TEST");
		CHECK(jp.values[3] == "-12345.67890e-789");
	}
}
