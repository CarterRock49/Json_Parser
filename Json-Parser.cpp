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
	string array_values[ARRSIZE];
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
		int arrayindex = 0;
		//gets you to to first string for the loop
		Json.get(charentry);
		//the ws function removes as many whitespace characters as possible
		Json >> ws;
		Json.get(charentry);
		//this is a while loop that continues until the end of the file
			while (!Json.eof()) {
				Json >> ws;
				if(Json.peek() == '"'){Json.get(charentry);} //gets rid of '"' to start
				getline(Json, entry, '"');	//gets the first string
				names[index] = entry;	
				getline(Json, entry, ':'); //removes anything up to and including the colon
				Json >> ws; //if the next char is a '"' thens its a string and will end with a '"' else its something else and will end with either a ',' or '\n' and the '}' or its an array
				if (Json.peek() == '"') {
					Json.get(charentry); //gets rid of the '"'
					getline(Json, entry, '"');
					values[index] = entry;
					Json.get(charentry); //gets rid of the ',' or '\n'
				}
				//check if the next character is for a json array
				else if (Json.peek() == '['){
					Json.get(charentry); //removes [
					Json >> ws;
					//this is a bit messy but this while loop continues until the ] is reached for the array then it uses the break statement
					while(true){
						Json >> ws;
						if(Json.peek() == ']'){Json.get(charentry); break;}
						//the same code for the outer loop that parses if a value is a string or a number
						if (Json.peek() == '"'){
							Json.get(charentry);
							getline(Json, entry, '"');
							array_values[arrayindex] = entry;
							Json.get(charentry);
						} else {
							getline(Json, entry);
							stringstream ss(entry);
							getline(ss, entry, ',');
							array_values[arrayindex] = entry;
						}
						arrayindex++;
					}
				} else {
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

	SUBCASE("testing parsing of empty JSON object") {
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject4a.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.names[0] == "Rank");
		CHECK(jp.names[1] == "Name");
		CHECK(jp.names[2] == "Symbol");
		CHECK(jp.names[3] == "Market Cap");
		CHECK(jp.values[0] == "1");
		CHECK(jp.values[1] == "Bitcoin");
		CHECK(jp.values[2] == "BTC");
		CHECK(jp.array_values[0] == "");
	}

	SUBCASE("testing parsing of simple JSON object (only strings for values)") {
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject4b.json") == true);

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
		CHECK(jp.array_values[0] == "One Thousand");
		CHECK(jp.array_values[1] == "Two Thousand");
		CHECK(jp.array_values[2] == "Three Thousand");
	}

	SUBCASE("testing parsing of simple JSON object (only strings for values)") {
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject4c.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.names[0] == "Rank");
		CHECK(jp.names[1] == "Name");
		CHECK(jp.names[2] == "Symbol");
		CHECK(jp.names[3] == "Market Cap");
		CHECK(jp.values[0] == "1");
		CHECK(jp.values[1] == "Bitcoin");
		CHECK(jp.values[2] == "BTC");
		CHECK(jp.array_values[0] == "1000");
		CHECK(jp.array_values[1] == "0");
		CHECK(jp.array_values[2] == "-12345.67890e-789");
	}
}
