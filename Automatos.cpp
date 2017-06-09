#include <vector>
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <queue>
#include "Automatos.hpp"

using namespace std;

/* =============== Main functions ================== */

void help()
{
	string msg = "Utilitário CppAutomata.\nCopyright 2017"
		" - Felipe Weiss, Leonardo Valério Anastácio, Lucas Litter Mentz."
		"\nAgradecimentos à professora Karina Girardi Roggia pela ideia"
		" da criação deste software didático."
		"\nModo de uso: \n\tcppautomata -r arquivo.afd\n\tcppautomata "
		"-c arquivo.afn\n\tcppautomata -m arquivo.afd\n\n"
	    "   -r   Abre o utilitário no modo de reconhecimento de\n"
	    "        palavras se o arquivo.afd existe.\n\n"
	    "   -c   Converte o autômato finito não-determinístico\n"
	    "        informado em um autômato finito determinístico\n"
	    "        equivalente com o menor número de estados.\n\n"
	    "   -m   Faz a minimização do autômato finito\n"
	    "        determinístico informado e salva em um arquivo\n"
	    "        com prefixo \"min_\".\n\n";

	cout << msg;
}

void leonardo(string file)
{

	std::ifstream arquivoAFD;
	arquivoAFD.open(file); //Abre o arquivo que contém o AFD

	string estadoInicial;
	arquivoAFD >> estadoInicial; //Leitura do estado inicial
	automato.addStates(1, estadoInicial);

	int numeroEstados;
	arquivoAFD >> numeroEstados;
	for(int i = 0; i < numeroEstados; i++){
		string tmpEstado;
		arquivoAFD >> tmpEstado;
		automato.addStates(1, tmpEstado);
	}  //Leitura dos estados

	int numeroEstadosFinais;
	arquivoAFD >> numeroEstadosFinais;
	for(int i = 0; i < numeroEstadosFinais; i++){
		string tmpEstado;
		arquivoAFD >> tmpEstado;
		automato.AddFinalStates(1, tmpEstado);
	} //Leitura dos estados finais

	int numeroElementos;
	arquivoAFD >> numeroElementos;
	for(int i = 0; i < numeroElementos; i++){
		string tmpElemento;
		arquivoAFD >> tmpElemento;
		automato.NewAlphabetSymbol(1, tmpElemento);
	} //Leitura dos simbolos do alfabeto

	int numeroConexoes;
	arquivoAFD >> numeroConexoes;
	for(int i = 0; i < numeroConexoes; i++){
		string tmpEstado1, simboloConexao, tmpEstado2;
		arquivoAFD >> tmpEstado1 >> simboloConexao >> tmpestado2;
		automato.NewConnection(tmpEstado1, tmpEstado2, simboloConexao);
	} //Leitura das conexões do automato
	
	string fitaEntrada;
	cout << "Digite fita de entrata para testar no automato\n";
	cin >> fitaEntrada;

	if(automato.ReadEntry(estadoInicial, fitaEntrada)
		cout << "Fita de entrada aceita pelo automato\n";
	else
		cout << "Fita de entrada não aceita pelo automato\n"; 
}

void weiss(string file)
{
	AFN *testa = new AFN(file);
	
	testa -> lerAFN();
	testa -> gramaticaAFN();
	testa -> converterAFN_AFD();
	testa -> gramaticaAFD();
	testa -> geraAFD();
}

void mentz(string file)
{
	// todo
	printf("Erro. Não implementado.\n");
}



/* ======================= AFD ======================= */

bool AFD::ReadEntry(string daVez, char * entry){
	pair<string, char> par = {daVez, *entry};
	if(*entry != '\0'){
		if(this->automatoConnection[par] != "") 
			cout << "δ: (" <<  daVez << " × " << *entry << ") → " 
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
	return ReadEntry(this->automatoConnection[par], ++entry); // @.@
}

void AFD::NewConnection(string qx, string qy, string alpha){
	this->automatoConnection[{qx, alpha}] = qy;
}

void AFD::AddFinalStates(const int n, ...){
	va_list args;
	va_start(args, n);
	for(int i = 0; i < n; i++)
		this->finalStates.push_back(va_arg(args, char*));
	va_end(args);
}

void AFD::AddStates(const int n, ...){
	va_list args;
	va_start(args, n);
	for(int i = 0; i < n; i++)
		this->States.push_back(va_arg(args, char*));
	va_end(args);
}

void AFD::NewAlphabetSymbol(string symbol){
	this -> alphabet.push_back(symbol);
}


int VerificarFuncProgTotal()
{
	for (int i = 0; i < States.size(); i++)
	{
		if (automatoConnection.count(States[i]) < )
	}
}


int VerificarEstadosInuteis();

/* ======================= AFN ======================= */


AFN::AFN(string nomeDoArquivo){
	this -> numeroEstadosNormais = 0;
	this -> numeroEstadosFinais = 0;
	this -> nomeDoArquivo = nomeDoArquivo;
}

void AFN::lerAFN(){
	ifstream arquivoDeEntrada;
	
	arquivoDeEntrada.open(nomeDoArquivo.c_str());
	if(!arquivoDeEntrada.is_open()){
		cout << "Desculpe, nao foi possivel abrir o arquivo.\nVerifique se o nome do arquivo foi digitado corretamente.\n";
		return;
	}

	for(int i = 0; i < 80; i++){
		cout << "#";
	} cout << endl;
	
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
	nomeDoArquivo.insert(nomeDoArquivo.size() - 4, "_toAFD");
	nomeDoArquivo.at(nomeDoArquivo.size()-1) = 'd';
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

	cout << "Automato Finito Não-Determinístico:\n";
	for(int i = 0; i < (int)estados.size(); i++){
		for(int j = 0; j < (int)alfabeto.size(); j++){
			cout << "δ: (" << estados[i] << ", " << alfabeto[j] << ") -> " << gramatica[i][j] << endl;
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

	cout << "Automato Finito Determinístico:\n";
	for(int i = 0; i < (int)novosEstados.size(); i++){
		for(int j = 0; j < (int)alfabeto.size(); j++){
			cout << "δ: (" << novosEstados[i] << ", " << alfabeto[j] << ") -> " << gramatica[i][j] << endl;
		} cout << endl;
	}
	for(int i = 0; i < 80; i++){
		cout << "#";
	} cout << endl;

}


