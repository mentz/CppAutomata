#include <vector>
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <stdarg.h>
#include <cstring>
using namespace std;

class AFD{
	private:
		map<pair<string, char>, string> automatoConnection;
		vector<string> finalStates;
		vector<string> States;
		string alphabet;   
	public:
		bool ReadEntry(string daVez, char * entry);
		void NewConnection(string qx, string qy, char alpha);
		void AddStates(const int n, ...);
		void AddFinalStates(const int n, ...);
		void NewAlphabet(string alpha);
};
