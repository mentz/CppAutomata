#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <utility>
#include <algorithm>
#include <queue>
#include "AFN.h"

using namespace std;

AFN::AFN(){
	this -> numeroEstadosNormais = 0;
	this -> numeroEstadosFinais = 0;
}

void AFN::lerAFN(){
	bool enquantoEstiverLendoArquivo = true;
	ifstream arquivoDeEntrada;
	while(enquantoEstiverLendoArquivo){
		//string nomeArquivo;
		cout << "Digite o nome do arquivo\nSem a extensao .afn (isto eh feito automaticamente): ";
		cin >> nomeDoArquivo;
		nomeDoArquivo += ".afn";
		arquivoDeEntrada.open(nomeDoArquivo.c_str());
		if(!arquivoDeEntrada.is_open()){
			cout << "Desculpe, mas nao foi possivel abrir o arquivo.\nVerifique se o nome do arquivo foi digitado corretamente.\n";
		} else {
			enquantoEstiverLendoArquivo = false;
		}
		for(int i = 0; i < 80; i++){
			cout << "#";
		} cout << endl;
	}

	string estadoInicial;
	getline(arquivoDeEntrada, estadoInicial);
	this -> estadoInicial = estadoInicial;
	string todosOsEstados;
	getline(arquivoDeEntrada, todosOsEstados);
	istringstream iss1(todosOsEstados);
	int quantEstados;
	iss1 >> quantEstados;
	for(int i = 0; i < quantEstados; i++){
		string estado;
		iss1 >> estado;
		this -> estados.push_back(estado);
	}

	string todosOsEstadosFinais;
	getline(arquivoDeEntrada, todosOsEstadosFinais);
	
	istringstream iss2(todosOsEstadosFinais);
	int quantEstadosFinais;
	iss2 >> quantEstadosFinais;
	for(int i = 0; i < quantEstadosFinais; i++){
		string estadoFinal;
		iss2 >> estadoFinal;
		this -> estadosFinais.push_back(estadoFinal);
	}

	int quantSimbolosAlfabeto;
	string todosOsSimbolosAlfabeto;
	getline(arquivoDeEntrada, todosOsSimbolosAlfabeto);
	istringstream iss3(todosOsSimbolosAlfabeto);
	iss3 >> quantSimbolosAlfabeto;
	for(int i = 0; i < quantSimbolosAlfabeto; i++){
		char simboloAlfabeto;
		iss3 >> simboloAlfabeto;
		this -> alfabeto.push_back(simboloAlfabeto);
	}

	int quantTransicoes;
	string quantidadeDeTransicoes;
	getline(arquivoDeEntrada, quantidadeDeTransicoes);
	istringstream iss4(quantidadeDeTransicoes);
	iss4 >> quantTransicoes;
	for(int i = 0; i < quantTransicoes; i++){
		string transicao;
		getline(arquivoDeEntrada, transicao);
		istringstream iss5(transicao);
		string estadoAntes, estadoDepois;
		char simbolo;
		iss5 >> estadoAntes >> simbolo >> estadoDepois;
		this -> AFND[{estadoAntes, simbolo}].push_back(estadoDepois);
	}

	arquivoDeEntrada.close();
}

bool AFN::ordena(const string &a, const string &b){
	return a < b;
}

string AFN::gerarNovoEstado(bool ehEstadoFinal = false){
	string novoNome = "";
	if(ehEstadoFinal){
		novoNome += "qf";
		novoNome += SSTR(this -> numeroEstadosFinais);
		this -> numeroEstadosFinais++;
	} else {
		novoNome += "q";
		novoNome += SSTR(this -> numeroEstadosNormais);
		this -> numeroEstadosNormais++;
	}	
	return novoNome;
}

bool AFN::estadoEhFinal(vector<string> &estadosFinais, string simbolo){
	return find(estadosFinais.begin(), estadosFinais.end(), simbolo) != estadosFinais.end() ? true : false;
}

bool AFN::estadoEhFinal(vector<string> &estadosFinais, vector<string> &simbolos){
	for(int i = 0; i < (int)simbolos.size(); i++){
		string simboloDaVez = simbolos[i];
		if(find(estadosFinais.begin(), estadosFinais.end(), simboloDaVez) != estadosFinais.end()){
			return true;
		}
	}
	return false;
}

void AFN::removeDoAFD(string estadoQueDeveSerRemovido){
	map<pair<string, char>, string>::iterator it = AFD.begin();

	for(; it != this -> AFD.end(); it++){
		if(it -> first.first == estadoQueDeveSerRemovido or it -> second == estadoQueDeveSerRemovido){
			this -> AFD.erase(it);
		}
	}
}

bool AFN::verificaSeChegaEmEstadoFinal(string estadoDaVez, map<string, bool> &estadosVisitados){
	estadosVisitados[estadoDaVez] = true;
	if(estadoEhFinal(this -> novosEstadosFinais, estadoDaVez)){
		return true;
	}
	for(int i = 0; i < (int)this -> alfabeto.size(); i++){
		if(this -> AFD.count( { estadoDaVez, alfabeto[i] } ) > 0){
			string aux = this -> AFD[{ estadoDaVez, alfabeto[i] }];
			if(!estadosVisitados[aux]){
				if(verificaSeChegaEmEstadoFinal(aux, estadosVisitados)){
					return true;
				}
			}
		}
	}

	return false;
}

void AFN::converterAFN_AFD(){
	if(this -> estadoEhFinal(this -> estadosFinais, this -> estadoInicial)){
		this -> novoEstadoInicial = gerarNovoEstado(true);
		this -> novosEstadosFinais.push_back(novoEstadoInicial);
	} else {
		this -> novoEstadoInicial = gerarNovoEstado();
	}
	
	vector<string> sequenciaDeEstados;
	sequenciaDeEstados.push_back(this -> estadoInicial);
	this -> novoNomeDosEstados[sequenciaDeEstados] = novoEstadoInicial;
	this -> estadosOriginaisDoNovoEstado[novoEstadoInicial] = sequenciaDeEstados;

	queue<string> filaDeEstados;
	filaDeEstados.push(novoEstadoInicial);

	this -> novosEstados.push_back(this -> novoEstadoInicial);	

	map<string, bool> novoEstadoJaVisitado;

	while(!filaDeEstados.empty()){
		string estadoDaVez = filaDeEstados.front();
		filaDeEstados.pop();
		novoEstadoJaVisitado[estadoDaVez] = true;
		for(int i = 0; i < (int)this -> alfabeto.size(); i++){
			sequenciaDeEstados.clear();
			map<string, bool> estadoJaVisitado;
			for(int j = 0; j < (int)this -> estadosOriginaisDoNovoEstado[estadoDaVez].size(); j++){
				string aux = this -> estadosOriginaisDoNovoEstado[estadoDaVez][j];
				for(int k = 0; k < (int)this -> AFND[ {aux, this -> alfabeto[i]} ].size(); k++){
					string aux2 = AFND[ {aux, this -> alfabeto[i]} ][k];
					if(!estadoJaVisitado[aux2]){
						sequenciaDeEstados.push_back(aux2);
						estadoJaVisitado[aux2] = true;
					}
				}
			}
			if(!sequenciaDeEstados.empty()){
				sort(sequenciaDeEstados.begin(), sequenciaDeEstados.end());
				string novoEstado;
				if(this -> novoNomeDosEstados.count(sequenciaDeEstados) == 0){
					if(this -> estadoEhFinal(this -> estadosFinais, sequenciaDeEstados)){
						novoEstado = this -> gerarNovoEstado(true);
						this -> novosEstadosFinais.push_back(novoEstado);
					} else {
						novoEstado = this -> gerarNovoEstado();
					}
					
					this -> novoNomeDosEstados[sequenciaDeEstados] = novoEstado;
					this -> estadosOriginaisDoNovoEstado[novoEstado] = sequenciaDeEstados;

					this -> novosEstados.push_back(novoEstado);					
				} else {
					novoEstado = this -> novoNomeDosEstados[sequenciaDeEstados];
				}
				this -> AFD[ {estadoDaVez, this -> alfabeto[i]} ] = novoEstado;

				if(!novoEstadoJaVisitado[novoEstado]){
					filaDeEstados.push(novoEstado);
				}
			}
		}
	}

	for(int i = 0; i < (int)this -> novosEstados.size(); i++){
		map<string, bool> estadosVisitados;
		if(!this -> verificaSeChegaEmEstadoFinal(this -> novosEstados[i], estadosVisitados)){
			this -> removeDoAFD(this -> novosEstados[i]);
			this -> novosEstados.erase(this -> novosEstados.begin() + i);
			i--;
		}
	}
}

void AFN::geraAFD(){
	ofstream arquivoDeSaida;
	nomeDoArquivo.insert(nomeDoArquivo.size() - 4, "_Convertido");
	arquivoDeSaida.open(nomeDoArquivo.c_str());

	if(!arquivoDeSaida.is_open()){
		cout << "Houve um erro ao gerar o arquivo de saida!" << endl;
		for(int i = 0; i < 80; i++){
			cout << "-";
		} cout << endl;
	} else {
		arquivoDeSaida << this -> novoEstadoInicial << endl;
		arquivoDeSaida << (this -> novosEstados.size());
		for(int i = 0; i < (int)this -> novosEstados.size(); i++){
			arquivoDeSaida << " " << this -> novosEstados[i];
		} arquivoDeSaida << endl;
		arquivoDeSaida << (this -> novosEstadosFinais.size());
		for(int i = 0; i < (int)this -> novosEstadosFinais.size(); i++){
			arquivoDeSaida << " " << this -> novosEstadosFinais[i];
		} arquivoDeSaida << endl;
		arquivoDeSaida << (this -> alfabeto.size());
		for(int i = 0; i < (int)this -> alfabeto.size(); i++){
			arquivoDeSaida << " " << this -> alfabeto[i];
		} arquivoDeSaida << endl;
		arquivoDeSaida << (this -> AFD.size()) << endl;

		map<pair<string, char>, string>::iterator it;

		for(it = this -> AFD.begin(); it != this -> AFD.end(); it++){
			arquivoDeSaida << it -> first.first << " " << it -> first.second << " " << it -> second << endl;
		}
		arquivoDeSaida.close();
	}
}

void AFN::gramaticaAFN(){
	string gramatica[estados.size() + 1][alfabeto.size() + 1];
	for(int i = 0; i < (int)estados.size(); i++){
		for(int j = 0; j < (int)alfabeto.size(); j++){
			gramatica[i][j] = "{";
		}
	}

	for(int i = 0; i < (int)estados.size(); i++){
		for(int j = 0; j < (int)alfabeto.size(); j++){
			for(int k = 0; k < (int)AFND[ {estados[i], alfabeto[j]} ].size(); k++){
				if(k){
					gramatica[i][j] += ",";
				}
				gramatica[i][j] += AFND[ {estados[i], alfabeto[j]} ][k];
			}
			gramatica[i][j] += "}";
		}
	}

	cout << "Automato Finito Nao-Deterministico:\n";
	for(int i = 0; i < (int)estados.size(); i++){
		for(int j = 0; j < (int)alfabeto.size(); j++){
			cout << "6(" << estados[i] << ", " << alfabeto[j] << ") -> " << gramatica[i][j] << endl;
		} cout << endl;
	}
	for(int i = 0; i < 80; i++){
		cout << "#";
	} cout << endl;
}

void AFN::gramaticaAFD(){
	string gramatica[novosEstados.size() + 1][alfabeto.size() + 1];
	for(int i = 0; i < (int)novosEstados.size(); i++){
		for(int j = 0; j < (int)alfabeto.size(); j++){
			gramatica[i][j] = "{";
		}
	}

	for(int i = 0; i < (int)novosEstados.size(); i++){
		for(int j = 0; j < (int)alfabeto.size(); j++){				
			gramatica[i][j] += AFD[{ novosEstados[i], alfabeto[j] }];
			gramatica[i][j] += "}";
		}
	}

	cout << "Automato Finito Deterministico:\n";
	for(int i = 0; i < (int)novosEstados.size(); i++){
		for(int j = 0; j < (int)alfabeto.size(); j++){
			cout << "6(" << novosEstados[i] << ", " << alfabeto[j] << ") -> " << gramatica[i][j] << endl;
		} cout << endl;
	}
	for(int i = 0; i < 80; i++){
		cout << "#";
	} cout << endl;

}