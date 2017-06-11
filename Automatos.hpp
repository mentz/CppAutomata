#include <vector>
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <stdarg.h>
#include <cstring>

#define SSTR( x ) static_cast< ostringstream & >(( ostringstream() << dec << x )).str()

using namespace std;

/* ============== Main Functions =============== */

// print useful information about the program usage
void help();

// Aplicar o autômato no reconhecimento de palavras
void leonardo(string file);

// Converter um AFN em AFD sem estados inúteis e gravar em arquivo
void weiss(string file);

// Minimizar um AFD e gravar em arquivo
void mentz(string file);


/* ==================== AFD ==================== */

class AFD{
	private:
		map<pair<string, char>, string> automatoConnection;
		vector<string> finalStates;
		vector<string> States;
		string alphabet;
		map<string, bool> estadosAlcancaveis;
		map<pair<string, char>, string> AFDMinimo;
	public:
		string estadoInicial;
		bool ReadEntry(string daVez, char * entry);
		void NewConnection(string qx, string qy, char alpha);
		void AddStates(string state);
		void setEstadoInicial(string state);
		void AddFinalStates(string state);
		void NewAlphabet(string alpha);
		int lerArquivoAFD(string diretorio);
		int FazerFuncTotal();
		void RemoverEstadosInalcancaveis();
		void Fecho(string daVez);
		void Minimizar();
		void saveToFile(string file);
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
	AFN(string nomeDoArquivo);
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
