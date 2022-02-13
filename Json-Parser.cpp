#include <fstream>
#include <cctype>
#include <string>
#include <sstream>
#include <map>
#include <vector>
using namespace std;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

const string DATA_FILE_PATH = "Testdata//";
////////////////////////////
class JsonParser {
private:
	ifstream Json;


public:
	map<string, vector<string>> nNamesValues;
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
		int test = 0;
		string jsonstring;
		string tempvalue;
		string tempstorage;
		vector<string> value;
		char charentry;
		//gets you to to first string for the loop
		Json.get(charentry);
		//the ws function removes as many whitespace characters as possible
		Json >> ws;
		Json.get(charentry);
		//this is a while loop that continues until the end of the file
			while (!Json.eof()) {
				Json >> ws;
				if(Json.peek() == ','){Json.get(charentry);} //gets rid of ',' to start
				if(Json.peek() == '"'){Json.get(charentry);} //gets rid of '"' to start
				if(Json.peek() == '}'){break;} // breaks on the last loop through
				getline(Json, jsonstring, '"');	//gets the first string
				getline(Json, tempstorage, ':'); //removes anything up to and including the colon
				Json >> ws; //if the next char is a '"' thens its a string and will end with a '"' else its something else and will end with either a ',' or '\n' and the '}' or its an array
				if (Json.peek() == '"') {
					Json.get(charentry); //gets rid of the '"'
					getline(Json, tempvalue, '"');
					value.push_back(tempvalue);
					nNamesValues.insert(pair<string, vector<string>> (jsonstring, value)); //inserts the key and value
					value.clear(); //completely clears the vector
					Json.get(charentry); //gets rid of the ',' or '\n'
				}
				//check if the next character is for the start of a json array
				else if (Json.peek() == '['){
					test++;
					cout<<"\nyou are here" << test;
					Json.get(charentry); //removes [
					//this is a bit messy but this while loop continues until the ] is reached for the array then it uses the break statement
					while(true){
						Json >> ws;
						if(Json.peek() == ']'){Json.get(charentry); nNamesValues.insert(pair<string, vector<string>> (jsonstring, value)); value.clear(); break;}
						//the same code for the outer loop that parses if a value is a string or an int
						if (Json.peek() == '"'){
							Json.get(charentry);
							getline(Json, tempvalue, '"');
							value.push_back(tempvalue);
							Json.get(charentry);
						} else {
							tempvalue.clear();
							tempstorage.clear();
							//this loop iterates through every char until the end of the array aka ']' 
							while(true){
								Json >> ws;
								if(Json.peek() == ']'){value.push_back(tempvalue); break;}
								Json >> ws;
								if(Json.peek() != ','){
									Json.get(charentry); // gets the next char
									tempstorage = charentry; // converts to string
									tempvalue.append(tempstorage); //add to the end of the string
									tempstorage.clear(); // clears the single character
								} else {
									value.push_back(tempvalue);
									Json.get(charentry); //gets rid of the ','
									tempvalue.clear(); // since the value has been added the vector it is no long needed
									tempstorage.clear();
								}
							}
						}
					}
				} else {
					//this grabs the entire value portion, this is done to not accdentially grab the } at the end of the file although downside, doing it grabs the ,
					getline(Json, tempvalue);
					//this puts entry into a stream
					stringstream ss(tempvalue);
					//this removes the , but because we only grabbed this line before it doesn't have the possiblity to grabs the final }
					getline(ss, tempvalue, ',');
					value.push_back(tempvalue);
					nNamesValues.insert(pair<string, vector<string>> (jsonstring, value));
					value.clear();
				}
			}
		return true;
	}

	string GetValueGivenKey(string jsonstring){
		vector<string> value;
		string finalstring;
		value = nNamesValues.find(jsonstring)->second;
			for (int index = 0; index < value.size(); index++){
				if(index > 0){finalstring.append(",");}
				finalstring.append(value[index]);
			}
		return finalstring;
	}
};
///////////////// Testing ////////////////////////////////
TEST_CASE("Testing my JSON Parser"){
	JsonParser jp;

	SUBCASE("testing parsing of simple JSON object and storing the vaules in a STL container") {
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject5a.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.GetValueGivenKey("Rank") == "1");
		CHECK(jp.GetValueGivenKey("Active") == "false");
		CHECK(jp.GetValueGivenKey("Name") == "Bitcoin");
		CHECK(jp.GetValueGivenKey("Symbol") == "BTC");
		CHECK(jp.GetValueGivenKey("Market Cap") == "1000,0,-12345.67890e-789");
	}

	SUBCASE("testing parsing of simple JSON object and storing the vaules in a STL container") {
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject5b.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.GetValueGivenKey("Rank") == "1");
		CHECK(jp.GetValueGivenKey("Name") == "Bitcoin");
		CHECK(jp.GetValueGivenKey("Symbol") == "BTC");
		CHECK(jp.GetValueGivenKey("Market Cap") == "1000,0,-12345.67890e-789");
		CHECK(jp.GetValueGivenKey("Last Active") == "true,false,true");

	}

	SUBCASE("testing parsing of simple JSON object and storing the vaules in a STL container") {
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject5c.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.GetValueGivenKey("Rank") == "1");
		CHECK(jp.GetValueGivenKey("Name") == "Bitcoin");
		CHECK(jp.GetValueGivenKey("Symbol") == "BTC");
		CHECK(jp.GetValueGivenKey("Market Cap") == "1000,0,-12345.67890e-789");
		CHECK(jp.GetValueGivenKey("Last Active") == "true,false,true");
		CHECK(jp.GetValueGivenKey("Last Sold") == "null");
	}
}
