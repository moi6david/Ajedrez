#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Tiro{
	public:
	int pieza,coorini,coorfin;
	Tiro(int p=0,int ci=0,int cf=0);
};

class Posicion{
	public:
	int pos[64],piezas[32],turno,enrdamaN,enrreyN,enrdamaB,enrreyB,mov,paso,rBe,rNe;
	Posicion();
	Posicion(int *p,int *f,int t=0,int edN=0,int erN=0,int edB=0,int erB=0,int p1=0);
	int PiezaDefAtac(int ini,int fin);
	int PiezaDefAtacAlfil(int ini,int fin);
	int PiezaDefAtacTorre(int ini,int fin);
	int EvaluaP();
	int EvaluaMaterial();
	int EvaluaPosicion();
	int EvaluaEstructuraPeon();
	int EvaluaProteccionRey();
	int EvaluaProteccion();
	int EvaluaTorres();
	int Jacke();
	void Pinta();
	bool TiroValido(Tiro t);
	void Mueve(Tiro a);
};

class NodoP;

typedef NodoP* linkP;

class NodoP{
	private:
		linkP Padre;
		int ncam;
		Tiro tir;
	public:
		linkP *caminos;
		int get_ncam(){
			return ncam;
		}
		NodoP(linkP pad,Tiro t);	
		NodoP* get_padre();
		void set_ncaminos(int n);
		Tiro get_tiro();
		Posicion get_P();
		void set_padre(NodoP* padre){
			Padre=padre;
		}	
		void set_tiro(Tiro t){
			tir.coorfin=t.coorfin;
			tir.coorini=t.coorini;
			tir.pieza=t.pieza;
			return ;
		}
};

class TreeP{
	 NodoP *raiz;
	 Posicion P;
	public:
	 int prof;
	 TreeP(Posicion a,int ps);
	 ~TreeP();
	 NodoP* get_raiz(){
	 	return raiz;
	 }
	 int MinMax(linkP k,int pr,int &nc);
	 int IniciaCaminosAlfil(int m,int h,linkP &k);
	 int IniciaCaminosTorre(int m,int h,linkP &k);
	 void IniciaCaminos(linkP &k);
	 Posicion Tablero(linkP k);
	 int TiraHum();
	 int TiraCom();
	 int Fin(int tur);
	 void Tira(int c);
	 void Construye(Posicion a,int ps);
	 void LiberaPosicionesNoUsadas(int c);
	 Posicion get_P(){
	 	return P;
	 }
};

int FinJuego(int fin);
void Juega();
