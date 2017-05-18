#include <vector>
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <string.h>
#include "Automato.h"
using namespace std;

bool Automato::ReadEntry(string daVez, char * entry){
	pair<string, char> par = {daVez, *entry};
	if(*entry != '\0'){
		if(this->automatoConnection[par] != "") 
			cout << "δ: " <<  daVez << " × " << *entry << " → " 
		         <<  this->automatoConnection[par] << endl; 

		else if(this->automatoConnection[par] == "")
			return false; 
	}else{
		for(size_t i = 0; i < this->finalStates.size(); i++){
			if(daVez == this->finalStates[i])
				return true;
		}

		return false;
	}
	return ReadEntry(this->automatoConnection[par], ++entry); //@.@
}

void Automato::NewConnection(string qx, string qy, char alpha){
	this->automatoConnection[{qx, alpha}] = qy;
}

void Automato::AddFinalStates(const int n, ...){
	va_list args;
	va_start(args, n);
	for(int i = 0; i < n; i++)
		this->finalStates.push_back(va_arg(args, char*));
	va_end(args);
}

void Automato::AddStates(const int n, ...){
	va_list args;
	va_start(args, n);
	for(int i = 0; i < n; i++)
		this->States.push_back(va_arg(args, char*));
	va_end(args);
}

void Automato::NewAlphabet(string alpha){
	this -> alphabet = alpha;
}
