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
using namespace std;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

const int ARRSIZE = 100;
const string DATA_FILE_PATH = "TestData\\";
////////////////////////////
class JsonParser
{
	// Work here!
};
///////////////// Testing ////////////////////////////////
TEST_CASE("Testing my JSON Parser")
{
	JsonParser jp;

	SUBCASE("testing parsing of empty JSON object")
	{
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject1.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.names[0] == "");
		CHECK(jp.values[0] == "");
	}
	SUBCASE("testing parsing of simple JSON object (only strings for values)")
	{
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject2.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.names[0] == "Name");
		CHECK(jp.names[1] == "Symbol");
		CHECK(jp.values[0] == "BitCoin");
		CHECK(jp.values[1] == "BTC");
	}
}
