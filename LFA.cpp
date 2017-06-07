#include <vector>
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <string.h>
#include "Automatos.hpp"
using namespace std;

int main(){
	AFD automato;
	
	automato.AddStates(5, "q0", "q1", "q2", "q3", "qf");
	automato.AddFinalStates(1, "qf");
	automato.NewConnection("q0", "q1", 'a');
	automato.NewConnection("q1", "q2", 'b');
	
	if(automato.ReadEntry("q0", (char *)"aba"))
		cout << "Fita de entrada aceita pelo automato\n";
	else
		cout << "Fita de entrada não aceita pelo automato\n"; 
	
	return 0;
}	
