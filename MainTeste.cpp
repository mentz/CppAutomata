#include <iostream>
#include <windows.h>
#include "AFN.h"

using namespace std;

int main(int argc, char const *argv[]){
	AFN *testa = new AFN();
	
	testa -> lerAFN();

	testa -> gramaticaAFN();

	testa -> converterAFN_AFD();

	testa -> gramaticaAFD();

	testa -> geraAFD();

	getchar();
	cout << "Digite qualquer tecla para sair." << endl;
	getchar();

	return 0;
}