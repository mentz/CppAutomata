#include <vector>
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <stdarg.h>
#include <cstring>

#define SSTR( x ) static_cast< ostringstream & >(( ostringstream() << dec << x )).str()

using namespace std;

/* ==================== AFD ==================== */

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

/* ==================== AFN ==================== */

class AFN {
private:
	//Dados que vem do arquivo .afn, usado para montar o AFN e todos os seus componentes
	//(alfabeto, estados, estados finais, simbolo inicial, ligações entre estado, simbolo lido e proximo estado)
	map<pair<string, char>, vector<string> > AFND;
	vector<string> estados;
	vector<string> estadosFinais;
	string estadoInicial;
	vector<char> alfabeto;

	string nomeDoArquivo;

	//Dados que serão construidos durante a conversão de AFN para AFD
	//(alfabeto(igual ao do AFN), novos estados, novos estados finais, novo simbolo inicial, novas ligações entre estado, simbolo lido e proximo estado)
	map<pair<string, char>, string> AFD;
	map<vector<string>, string> novoNomeDosEstados;
	map<string, vector<string> > estadosOriginaisDoNovoEstado;
	vector<string> novosEstadosFinais;
	vector<string> novosEstados;
	string novoEstadoInicial;
	int numeroEstadosNormais;
	int numeroEstadosFinais;

public:
	AFN();
	void lerAFN();
	bool ordena(const string &a, const string &b);
	string gerarNovoEstado(bool ehEstadoFinal);
	bool estadoEhFinal(vector<string> &estadosFinais, string simbolo);
	bool estadoEhFinal(vector<string> &estadosFinais, vector<string> &simbolos);
	void removeDoAFD(string estadoQueDeveSerRemovido);
	bool verificaSeChegaEmEstadoFinal(string estadoDaVez, map<string, bool> &estadosVisitados);
	void converterAFN_AFD();
	void geraAFD();
	void gramaticaAFN();
	void gramaticaAFD();
};
