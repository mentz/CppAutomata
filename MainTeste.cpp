#include <iostream>
#include "Automatos.hpp"

using namespace std;

int main(int argc, char const *argv[]){
	int operation = 0;
	string filePath;

	if (argc >= 3)
	{
		filePath = argv[2];
		if (strcmp(argv[1], "-r") == 0)
			operation = 1;
		else if (strcmp(argv[1], "-c") == 0)
			operation = 2;
		else if (strcmp(argv[1], "-m") == 0)
			operation = 3;
	}

	switch(operation)
	{
		case 0:
			help();
			break;
		case 1:
			leonardo(filePath);
			break;
		case 2:
			weiss(filePath);
			break;
		case 3:
			mentz(filePath);
			break;
		default:
			help();
			break;
	}

	cout << "\n\nPressione 'Enter' para sair." << endl;
	getchar();

	return 0;
}