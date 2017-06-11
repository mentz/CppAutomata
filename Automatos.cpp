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
	AFD automato; 
	if (!automato.lerArquivoAFD(file))
		return;

	string fitaEntrada;
	cout << "Escreva a palavra para testar no autômato:\n";
	cin >> fitaEntrada;

	if(automato.ReadEntry(automato.estadoInicial, fitaEntrada.begin()))
		cout << "Palavra aceita pelo autômato.\n";
	else
		cout << "Palavra não aceita pelo autômato.\n"; 
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
	AFD automato; 
	if (!automato.lerArquivoAFD(file))
		return;

	automato.FazerFuncTotal();
	automato.RemoverEstadosInalcancaveis();
	AFD minimo = automato.Minimizar();
	automato.saveToFile(file);
}



/* ======================= AFD ======================= */

int AFD::lerArquivoAFD(string diretorio){
	ifstream arquivoAFD;
	arquivoAFD.open(diretorio);

	if(!arquivoAFD.is_open()){
		cout << "Desculpe, nao foi possivel abrir o arquivo.\nVerifique se o nome do arquivo foi digitado corretamente.\n";
		return 0;
	}

	string estadoInicial;
	arquivoAFD >> estadoInicial; //Leitura do estado inicial
	this->setEstadoInicial(estadoInicial);

	int numeroEstados;
	arquivoAFD >> numeroEstados;
	for(int i = 0; i < numeroEstados; i++){
		string tmpEstado;
		arquivoAFD >> tmpEstado;
		this->AddStates(tmpEstado);
	}  //Leitura dos estados

	int numeroEstadosFinais;
	arquivoAFD >> numeroEstadosFinais;
	for(int i = 0; i < numeroEstadosFinais; i++){
		string tmpEstado;
		arquivoAFD >> tmpEstado;
		this->AddFinalStates(tmpEstado);
	} //Leitura dos estados finais

	int numeroElementos;
	arquivoAFD >> numeroElementos;
	string Alfabeto;
	for(int i = 0; i < numeroElementos; i++){
		char tmpElemento;
		arquivoAFD >> tmpElemento;
		Alfabeto += tmpElemento;
	} //Leitura dos simbolos do alfabeto
	this->NewAlphabet(Alfabeto);

	int numeroConexoes;
	arquivoAFD >> numeroConexoes;
	for(int i = 0; i < numeroConexoes; i++){
		string tmpEstado1, tmpEstado2;
		char simboloConexao;
		arquivoAFD >> tmpEstado1 >> simboloConexao >> tmpEstado2;
		this->NewConnection(tmpEstado1, tmpEstado2, simboloConexao);
	}

	return 1;
}

bool AFD::ReadEntry(string daVez, string::iterator entry){
	pair<string, char> par = {daVez, *entry};

	if(*entry != '\0')
	{
		if(this->automatoConnection[par] != "") 
			cout << "δ: (" <<  daVez << ", " << *entry << ") →  " 
		         << this->automatoConnection[par] << endl; 

		else if(this->automatoConnection.count(par) < 1)
			return false;
	}
	else
	{
		bool finalState = false;
		for(size_t i = 0; i < this->finalStates.size(); i++){
			if(this->finalStates[i].compare(daVez) == 0)
				finalState = true;
		}

		cout << "\nO estado atual (" << daVez << ") " << (finalState ? "é ":"não é ") << "final.\n";

		return finalState;
	}
	return ReadEntry(this->automatoConnection[par], ++entry); // @.@
}

void AFD::NewConnection(string qx, string qy, char alpha){
	this->automatoConnection[{qx, alpha}] = qy;
}

void AFD::NewAlphabet(string alpha)
{
	this->alphabet = alpha;
}

void AFD::AddFinalStates(string state){
	this->finalStates.push_back(state);
}

void AFD::AddStates(string state){
	this->States.push_back(state);
}

void AFD::setEstadoInicial(string state)
{
	this->estadoInicial = state;
}

int AFD::FazerFuncTotal()
{
	int novoEstado = false;
	int sts = this->States.size();
	int als = this->alphabet.size();
	for (int i = 0; i < sts; i++)
	{
		for (int j = 0; j < als; j++)
		{
			if ( !this->automatoConnection.count({States[i], alphabet[j]}) )
			{
				if (!novoEstado)
				{
					novoEstado = true;
					AddStates("qBlackHole");
				}
				this->NewConnection(States[i], "qBlackHole", alphabet[j]);
			}
		}
	}

	if (novoEstado)
	{
		for (int i = 0; i < als; i++)
			this->NewConnection("qBlackHole", "qBlackHole", alphabet[i]);
	}

/*	// Existe para verificar se faz o correto ao totalizar a função programa.
	for (int i = 0; i < (int)this->States.size(); i++)
	{
		for (int j = 0; j < als; j++)
			cout << "δ: (" <<  States[i] << ", " << alphabet[j] << ") →  " 
		         << this->automatoConnection[{States[i], alphabet[j]}] << endl;
	}
*/

	return novoEstado;
}


void AFD::RemoverEstadosInalcancaveis()
{
	Fecho(this->estadoInicial);
	for (int i = States.size() - 1; i >= 0; i--)
	{
		if (!estadosAlcancaveis.count(States[i]))
		{
			map<pair<string, char>, string>::iterator itFuncProg = this->automatoConnection.begin();

			for(; itFuncProg != this->automatoConnection.end(); itFuncProg++){
				if(itFuncProg ->first.first == States[i] or itFuncProg->second == States[i]){
					this->automatoConnection.erase(itFuncProg);
					itFuncProg--;
				}
			}


			vector<string>::iterator itEstFinais = finalStates.begin();
			for (; itEstFinais != finalStates.end(); itEstFinais++)
			{
				if (States[i].compare(*itEstFinais) == 0)
				{
					finalStates.erase(itEstFinais);
					itEstFinais--;
				}
			}
			States.erase(States.begin() + i);
		}
	}
}

void AFD::Fecho(string daVez)
{
	estadosAlcancaveis[daVez] = true;
	int als = this->alphabet.size();
	string next;
	for (int i = 0; i < als; i++)
	{
		next = this->automatoConnection[{daVez, alphabet[i]}];
		if (!estadosAlcancaveis[next])
			Fecho(this->automatoConnection[{daVez, alphabet[i]}]);
	}
}

AFD AFD::Minimizar()
{
	AFD newMinimo;
	newMinimo.NewAlphabet(this->alphabet);
	map<pair<string, string>, bool> equivalentes;
	int mark = 0, nmark = 0;

	// Passo 1: Criação da tabela de estados equivalentes (completa, não triangular)
	// Passo 2: Marcação dos estados trivialmente não-equivalentes

	for (int i = 0; i < (int)States.size(); i++)
	{
		for (int j = 0; j < (int)States.size(); j++)
		{
			equivalentes[{States[i], States[j]}] = true;
			if ((find(finalStates.begin(), finalStates.end(), States[i]) != finalStates.end()) !=
				(find(finalStates.begin(), finalStates.end(), States[j]) != finalStates.end()))
			{
				equivalentes[{States[i], States[j]}] = false;
				mark++;
			}
		}
	}

	cout << "Marcações triviais:\n";
	for (int i = 0; i < (int)States.size(); i++)
	{
		for (int j = 0; j < (int)States.size(); j++)
			printf("%s ", equivalentes[{States[i], States[j]}] ? "-" : "X");
		printf("\n");
	}

	bool areEqual;
	string r1, r2;
	nmark = mark;

	// Passo 3: Marcação dos estados não equivalentes
	// feito até não marcar mais ninguém

	do
	{
		mark = nmark;
		for (int i = 0; i < (int)States.size(); i++)
		{
			for (int j = 0; j < (int)States.size(); j++)
			{
				areEqual = equivalentes[{States[i], States[j]}];
				for (int k = 0; k < (int)alphabet.size() and areEqual; k++)
				{
					r1 = automatoConnection[{States[i], alphabet[k]}];
					r2 = automatoConnection[{States[j], alphabet[k]}];
					if (!equivalentes[{r1, r2}])
					{
						areEqual = false;
						equivalentes[{States[i], States[j]}] = false;
						nmark++;
					}
				}
			}
		}
	} while (nmark != mark);

	cout << "Finalizado:\n";
	for (int i = 0; i < (int)States.size(); i++)
	{
		for (int j = 0; j < (int)States.size(); j++)
			printf("%s ", equivalentes[{States[i], States[j]}] ? "-" : "X");
		printf("\n");
	}
	
	do
	{
		for (int i = 1; i < (int)States.size(); i++)
		{
			for (int j = 0; j < i; j++)
			{
				if (equivalentes[{States[i], States[j]}])
				{

				}
			}
		}
	} while (nmark != mark);


	return newMinimo;
}

void AFD::saveToFile(string path)
{
	ofstream fout;
	string npath = path; npath.insert(0, "min_");
	fout.open(npath);
	fout << this->estadoInicial << endl << this->States.size();
	for (int i = 0; i < (int)this->States.size(); i++)
		fout << " " << this->States[i];
	fout << endl << this->finalStates.size();
	for (int i = 0; i < (int)this->finalStates.size(); i++)
		fout << " " << this->finalStates[i];
	fout << endl << this->alphabet.size();
	for (int i = 0; i < (int)this->alphabet.size(); i++)
		fout << " " << alphabet[i];
	fout << endl << this->automatoConnection.size() << endl;
	map<pair<string, char> , string > ::iterator it = this->automatoConnection.begin();
	for (; it != this->automatoConnection.end(); it++)
		fout << it->first.first << " " << it->first.second << " " << it->second << endl;
}

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

	//Retirando os estados inúteis -> Não chegam em estado final
	for(int i = 0; i < (int)this -> novosEstados.size(); i++){
		map<string, bool> estadosVisitados;
		if(!this -> verificaSeChegaEmEstadoFinal(this -> novosEstados[i], estadosVisitados)){
			this -> removeDoAFD(this -> novosEstados[i]);
			this -> novosEstados.erase(this -> novosEstados.begin() + i);
			i--;
		}
	}

	//Removendo as transições vazias
	map<pair<string, char>, string>::iterator it = AFD.begin();
	for(; it != AFD.end(); it++){
		if(it -> second.size() == 0){
			this -> AFD.erase(it);
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


