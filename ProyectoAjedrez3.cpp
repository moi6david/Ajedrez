#include "ProyectoAjedrez3.h"
#include "c:/MaterialCurso/shell.cpp"
#define inf 100000
Window W("Prueba",700,690);

int Maximo(int *m,int n,int &a){
	int max=m[0];
	a=0;
	for(int i=0;i<n;i++){
		if(max<m[i]){
			max=m[i];
			a=i;
		}
	}
	return max;
}

int Minimo(int *m,int n,int &a){
	int min=m[0],i;
	a=0;
	for(i=0;i<n;i++){
		if(min>m[i]){
			a=i;
			min=m[i];
		}
	}
	return min;
}

int TreeP::MinMax(linkP k,int pr,int &nc){
	int minmax[100],max,min,i,n,h,a;
	if(pr==prof){
		Posicion p=Tablero(k);		//profundidad par no ocupa cambiar el turno
		max=p.EvaluaP();
		if(p.turno%2!=0)max=-max;
		return max;
	}
	n=k->get_ncam();
	if(n==0){
		Posicion s=Tablero(k);
		if(pr%2==0){
			if(s.Jacke())return -inf;
			else return 0;
		}
		else {
			if(s.Jacke())return inf;
			else return 0;
		}
	}
	for(i=0;i<n;i++){
		minmax[i]=MinMax(k->caminos[i],pr+1,h);
	}
	if(pr%2==0){
		max=Maximo(minmax,n,a);
		nc=a;
		return max;
	}
	else {
		min=Minimo(minmax,n,a);
		nc=a;
		return min;
	}
}

NodoP* NodoP::get_padre(){
	return Padre;
}

Tiro NodoP::get_tiro(){
	return tir;
}

Tiro::Tiro(int p,int ci,int cf){
	pieza=p;
	coorini=ci;
	coorfin=cf;
	return;
}

NodoP::NodoP(linkP pad,Tiro t){
	tir.pieza=t.pieza;
	tir.coorini=t.coorini;
	tir.coorfin=t.coorfin;
	Padre=pad;
	return;
}

void NodoP::set_ncaminos(int n){
	ncam=n;
	return;
}

Posicion::Posicion(){
	int i;
	turno=0;
	enrdamaB=1;
	enrdamaN=1;
	enrreyN=1;
	enrreyB=1;
	rBe=0;
	rNe=0;
	paso=0;
	pos[0]=4; pos[1]=2; pos[2]=3; pos[3]=5;
	pos[4]=6; pos[5]=3; pos[6]=2; pos[7]=4;
	for(i=8;i<16;i++){
		pos[i]=1;
	} 
	for(i=16;i<48;i++){
		pos[i]=0;
	}
	for(i=48;i<56;i++){
		pos[i]=7;
	}
	pos[56]=10; pos[57]=8; pos[58]=9; pos[59]=11;
	pos[60]=12; pos[61]=9; pos[62]=8; pos[63]=10;
	piezas[0]=1; piezas[1]=6; piezas[2]=2; piezas[3]=5;
	piezas[4]=0; piezas[5]=7; piezas[6]=3; piezas[7]=4;
	for(i=8;i<16;i++){
		piezas[i]=i;
	}
	for(i=24;i<32;i++){
		piezas[i]=i+24;
	}
	piezas[16]=57; piezas[17]=62; piezas[18]=61; piezas[19]=58;
	piezas[20]=56; piezas[21]=63; piezas[22]=59; piezas[23]=60;
}

TreeP::TreeP(Posicion a,int ps){
	Construye(a,ps);
	return;
}

void TreeP::Construye(Posicion a,int ps){
	int i,j,k,f,g,h;
	Tiro s(-1,-1,-1);
	linkP aux,aux2;
	prof=ps;
	P.turno=a.turno;
	P.enrdamaB=a.enrdamaB;
	P.enrdamaN=a.enrdamaN;
	P.enrreyB=a.enrreyB;
	P.enrreyN=a.enrreyN;
	for(i=0;i<64;i++){
		P.pos[i]=a.pos[i];
	}
	for(i=0;i<32;i++){
		P.piezas[i]=a.piezas[i];
	}
	raiz=new NodoP(NULL,s);
	IniciaCaminos(raiz);
	int n=raiz->get_ncam();
	Tiro ra=raiz->caminos[2]->get_tiro();
	for(i=0;i<n;i++){
		IniciaCaminos(raiz->caminos[i]);
		aux=raiz->caminos[i];
		f=aux->get_ncam();
		for(j=0;j<f;j++){
			IniciaCaminos(aux->caminos[j]);
			aux2=aux->caminos[j];
			g=aux2->get_ncam();
			for(k=0;k<g;k++){
				IniciaCaminos(aux2->caminos[k]);
			}
		}
	}
	return;
}

int Posicion::PiezaDefAtacAlfil(int ini,int fin){
	int aux=ini;
	while(aux%8!=7&&aux<56&&pos[aux+9]==0){
		aux+=9;
		if(aux==fin)return 1;
	}
	if(aux%8!=7&&aux<56&&aux+9==fin)return 1;
	aux=ini;
	while(aux%8!=0&&aux<56&&pos[aux+7]==0){
		aux+=7;
		if(aux==fin)return 1;
	}
	if(aux%8!=0&&aux<56&&aux+7==fin)return 1;
	aux=ini;
	while(aux%8!=7&&aux>7&&pos[aux-7]==0){
		aux-=7;
		if(aux==fin)return 1;
	}
	if(aux%8!=7&&aux>7&&aux-7==fin)return 1;
	aux=ini;
	while(aux%8!=0&&aux>7&&pos[aux-9]==0){
		aux-=9;
		if(aux==fin)return 1;
	}
	if(aux%8!=0&&aux>7&&aux-9==fin)return 1;
	return 0;
}

int Posicion::PiezaDefAtacTorre(int ini,int fin){
	int aux;
	if(ini%8==fin%8){
		for(aux=ini;aux<56;aux+=8){
			if(aux+8==fin)return 1;
			if(pos[aux+8]!=0)break;
		}
		for(aux=ini;aux>7;aux-=8){
			if(aux-8==fin)return 1;
			if(pos[aux-8]!=0)break;
		}
	}
	aux=ini;
	while(aux%8!=7){
		if(aux+1==fin){
			return 1;
		}
		if(pos[aux+1]!=0){
			break;
		}
		aux++;
	}
	for(aux=ini;aux%8!=0;aux--){
		if(aux-1==fin)return 1;
		if(pos[aux-1]!=0)break;
	}
	return 0;
}

int Posicion::PiezaDefAtac(int ini,int fin){		//regresa cero si no defiene
	int aux,dir;
	if(ini<0||ini>63){
		//W.printf("Error entrada invalida");
		return 0;
	}
	if(pos[ini]==0)return 0;
	if(ini==fin)return 0;
	if(pos[ini]==2||pos[ini]==8){		//caballo
		if(ini<48){
			if(ini%8!=0&&ini+15==fin)return 1;
			if(ini%8!=7&&ini+17==fin)return 1;	
		}
		if(ini>15){
			if(ini%8!=0&&ini-17==fin)return 1;
			if(ini%8!=7&&ini-15==fin)return 1;
		}
		if(ini%8!=0&&ini%8!=1){
			if(ini<56&&ini+6==fin)return 1;
			if(ini>7&&ini-10==fin)return 1;
		}
		if(ini%8!=7&&ini%8!=6){
			if(ini<56&&ini+10==fin)return 1;
			if(ini>7&&ini-6==fin)return 1;
		}
	}
	if(pos[ini]==3||pos[ini]==9){			//alfil
		if(PiezaDefAtacAlfil(ini,fin))return 1;
	}
	if(pos[ini]==4||pos[ini]==10){			//Torre
		if(PiezaDefAtacTorre(ini,fin))return 1;
	}
	if(pos[ini]==1){		//Peon
		if(ini%8!=0&&ini+7==fin)return 1;
		if(ini%8!=7&&ini+9==fin)return 1;
	}
	if(pos[ini]==7){
		if(ini%8!=0&&ini-9==fin)return 1;
		if(ini%8!=7&&ini-7==fin)return 1;
	}
	if(pos[ini]==5||pos[ini]==11){		//Dama
		if(PiezaDefAtacAlfil(ini,fin))return 1;
		if(PiezaDefAtacTorre(ini,fin))return 1;
	}
	if(pos[ini]==6||pos[ini]==12){		//Rey
		if(ini%8!=0){
			if(ini-1==fin)return 1;
			if(ini-9==fin)return 1;
			if(ini+7==fin)return 1;
		}
		if(ini%8!=7){
			if(ini+1==fin)return 1;
			if(ini+9==fin)return 1;
			if(ini-7==fin)return 1;
		}
		if(ini+8==fin)return 1;
		if(ini-8==fin)return 1;
	}
	return 0;
}

bool Posicion::TiroValido(Tiro t){
	return 1;
}

int TreeP::TiraCom(){
	Tiro tiromax;
	int i,n,max,con=0;
	n=raiz->get_ncam();
	if(n==0)return 0;
	max=MinMax(raiz,0,con);
	Tira(con);
	return n;
}

Posicion::Posicion(int *p,int *f,int t,int edN,int erN,int edB,int erB,int p1){
	int i;
	paso=p1;
	for(i=0;i<64;i++){
		pos[i]=p[i];
	}
	turno=t;
	enrdamaN=edN;
	enrreyN=erN;
	enrdamaB=edB;
	enrreyB=erB;
	for(i=0;i<32;i++){
		piezas[i]=f[i];
	}
	rNe=0;
	rBe=0;
	return;
}

int Posicion::EvaluaP(){			//dif es la diferencia blancas - negras
	int dif=0;
	dif=EvaluaMaterial();
	dif=EvaluaPosicion()+dif;
	dif+=EvaluaEstructuraPeon();
	//dif+=EvaluaProteccion();
	dif+=EvaluaProteccionRey();
	dif+=EvaluaTorres();
	return dif;
}

int Posicion::EvaluaMaterial(){
	int dif,blan=0,neg=0,i;
	for(i = 8;i < 16; i++){
		if(piezas[i]!=-1)blan+=100;
	}
	for(i = 24;i < 32; i++){
		if(piezas[i]!=-1)neg+=100;
	}
	if(piezas[0]!=-1)blan+=300;
	if(piezas[1]!=-1)blan+=300;
	if(piezas[2]!=-1)blan+=305;
	if(piezas[3]!=-1)blan+=305;
	if(piezas[2]!=-1&&piezas[3]!=-1)blan+=20;
	if(piezas[4]!=-1)blan+=500;
	if(piezas[5]!=-1)blan+=500;
	if(piezas[6]!=-1)blan+=900;
	if(piezas[16]!=-1)neg+=300;
	if(piezas[17]!=-1)neg+=300;
	if(piezas[18]!=-1)neg+=305;
	if(piezas[19]!=-1)neg+=305;
	if(piezas[18]!=-1&&piezas[19]!=-1)neg+=20;
	if(piezas[20]!=-1)neg+=500;
	if(piezas[21]!=-1)neg+=500;
	if(piezas[22]!=-1)neg+=900;
	dif=blan-neg;
	return dif;
}

int Posicion::EvaluaPosicion(){
	int dif,col[2]={0,0},i,in,j;			//i es el color
	for(i=0;i<2;i++){
		for(j=0;j<2;j++){			//caballo
			in=j+i*16;
			if(piezas[in]!=-1){
				if(piezas[in]==1+i*56||piezas[in]==6+i*56)col[i]-=20;
				if(piezas[in]<8||piezas[in]>55)col[i]-=10;
				if(piezas[in]%8==0||piezas[in]%8==7)col[i]-=10;
				if(piezas[in]==27||piezas[in]==28||piezas[in]==35||piezas[in]==36)col[i]+=10;
				if((piezas[in]%8==2||piezas[in]%8==5)&&piezas[in]>15&&piezas[in]<55)col[i]+=5;
				if(piezas[in]==19||piezas[in]==20||piezas[in]==43||piezas[in]==44)col[i]+=5;
			}
		}						//Alfil
		for(j=2;j<3;j++){		//%7 es si esta en la diagonal 
			in=j+i*16;
			if(piezas[in!=-1]){
				if(piezas[in]%7==0)col[i]+=10;
				if(piezas[in]%9==0)col[i]+=10;
				if(piezas[in]<8||piezas[in]>55)col[i]-=10;
				if(piezas[in]%8==0||piezas[in]%8==7)col[i]-=5;
				if(piezas[in]==26||piezas[in]==34||piezas[in]==29||piezas[in]==37)col[i]+=5;
			}
		}
		for(j=8;j<16;j++){
			in=j+i*16;
			if(piezas[in]==27||piezas[in]==28||piezas[in]==35||piezas[in]==36)col[i]+=10;
		}
	}
	dif=col[0]-col[1];
	return dif;
}

int Posicion::EvaluaEstructuraPeon(){
	int col[2]={0,0},i,j,dif,in,c[8]={0,0,0,0,0,0,0,0},h,g;
	for(i=0;i<2;i++){
		for(j=8;j<16;j++){
			in=j+i*16;
			if(piezas[in]!=-1){
				c[piezas[in]%8]++;
				if(i==0){						
					if(piezas[in]>39)col[i]+=15;
					if(piezas[in]>47)col[i]+=20;
				}
				else {
					if(piezas[in]<24)col[i]+=15;
					if(piezas[in]<16)col[i]+=20;
				}
			}
		}
		for(h=0;h<8;h++){
			if(c[h]>1)col[i]-=10;
			if(h==0){
				if(c[2]==0)col[i]-=20;
			}
			else if(h==7){
				if(c[6]==0)col[i]-=20;
			}
			else {
				if(c[h-1]==0&&c[h+1]==0)c[i]-=20;
			}
		}
	}
	dif=col[0]-col[1];
	return dif;
}

int Posicion::EvaluaProteccion(){
	int i,col[2]={0,0},j,k,in,dif;
	for(i=0;i<2;i++){
		for(j=0;j<16;j++){
			in=j+i*16;
			if(piezas[in]!=-1){
				for(k=0;k<16;k++){
					if(piezas[k+16*i]!=-1){
						if(PiezaDefAtac(piezas[k+16*i],piezas[in]))col[i]+=5;
					}
				}
			}
		}
	}
	dif=col[0]-col[1];
	return 0;
}

int Posicion::EvaluaProteccionRey(){
	int dif,col[2]={0,0};
	if(!enrreyB)col[0]-=15;
	if(!enrdamaB)col[0]-=10;
	if(rBe)col[0]+=50;
	if(!enrreyN)col[1]-=15;
	if(!enrdamaN)col[1]-=10;
	if(rNe)col[1]+=50;
	dif=col[0]-col[1];
	return dif;
}

int Posicion::EvaluaTorres(){
	int col[2]={0,0},dir=16,i,j;
	for(i=0;i<2;i++){
		if(PiezaDefAtac(piezas[4+dir*i],piezas[5+dir*i]))//Torres dobladas
			col[i]+=40;
		for(j=4;j<6;j++){	
			if(piezas[j+dir*i]!=-1&&PiezaDefAtac(piezas[j+dir*i],(piezas[j+dir*i]%8)+56)){//Torres en columnas abiertas
				if(PiezaDefAtac(piezas[j+dir*i],piezas[j+dir*i]%8))col[i]+=20;
			}
			if(piezas[j+dir*i]>=48-i*40&&piezas[j+dir*i]<56-i*40)//Torres en septima
				col[i]+=20;
		}
	}
	int dif=col[0]-col[1];
	return dif;
}

void Posicion::Pinta(){
	int i,nr,nc,x,y,j,h;
	COLORREF **a,**b;
	DimensionesBmp("c:/Users/moysdavid/Desktop/Ajedrez/Tablero1.bmp",&nr,&nc);
	a=new COLORREF*[nr];
	for(i=0;i<nr;i++){
		a[i]=new COLORREF[nc];
	}
	b=new COLORREF*[80];
	for(i=0;i<80;i++){
		b[i]=new COLORREF[80];
	}
	LeeBmpColor(a,"c:/Users/moysdavid/Desktop/Ajedrez/Tablero1.bmp");
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/PeonBlanco.bmp");
	for(i=0;i<8;i++){
		if(piezas[i+8]!=-1){
			x=piezas[i+8]%8;
			y=piezas[i+8]/8;
			y=7-y;
			x=80*x+30;
			y=80*y+30;
			for(j=0;j<80;j++){
				for(h=0;h<80;h++){
					if(b[j][h]!=RED){
						a[j+y][h+x]=b[j][h];
					}
				}
			}
		}
	}
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/PeonNegro.bmp");
	for(i=0;i<8;i++){
		if(piezas[i+24]!=-1){
			x=piezas[i+24]%8;
			y=piezas[i+24]/8;
			y=7-y;
			x=80*x+30;
			y=80*y+30;
			for(j=0;j<80;j++){
				for(h=0;h<80;h++){
					if(b[j][h]!=RED){
						a[j+y][h+x]=b[j][h];
					}
				}
			}
		}
	}
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/AlfilBlanco.bmp");
	for(i=0;i<2;i++){
		if(piezas[i+2]!=-1){
			x=piezas[i+2]%8;
			y=piezas[i+2]/8;
			y=7-y;
			x=80*x+30;
			y=80*y+30;
			for(j=0;j<80;j++){
				for(h=0;h<80;h++){
					if(b[j][h]!=RED){
						a[j+y][h+x]=b[j][h];
					}
				}
			}
		}
	}
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/AlfilNegro.bmp");
	for(i=0;i<2;i++){
		if(piezas[i+18]!=-1){
			x=piezas[i+18]%8;
			y=piezas[i+18]/8;
			y=7-y;
			x=80*x+30;
			y=80*y+30;
			for(j=0;j<80;j++){
				for(h=0;h<80;h++){
					if(b[j][h]!=RED){
						a[j+y][h+x]=b[j][h];
					}
				}
			}
		}
	}
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/CaballoBlanco.bmp");
	for(i=0;i<2;i++){
		if(piezas[i]!=-1){
			x=piezas[i]%8;
			y=piezas[i]/8;
			y=7-y;
			x=80*x+30;
			y=80*y+30;
			for(j=0;j<80;j++){
				for(h=0;h<80;h++){
					if(b[j][h]!=RED){
						a[j+y][h+x]=b[j][h];
					}
				}
			}
		}
	}
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/CaballoNegro.bmp");
	for(i=0;i<2;i++){
		if(piezas[i+16]!=-1){
			x=piezas[i+16]%8;
			y=piezas[i+16]/8;
			y=7-y;
			x=80*x+30;
			y=80*y+30;
			for(j=0;j<80;j++){
				for(h=0;h<80;h++){
					if(b[j][h]!=RED){
						a[j+y][h+x]=b[j][h];
					}
				}
			}
		}
	}
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/TorreBlanca.bmp");
	for(i=0;i<2;i++){
		if(piezas[i+4]!=-1){
			x=piezas[i+4]%8;
			y=piezas[i+4]/8;
			y=7-y;
			x=80*x+30;
			y=80*y+30;
			for(j=0;j<80;j++){
				for(h=0;h<80;h++){
					if(b[j][h]!=RED){
						a[j+y][h+x]=b[j][h];
					}
				}
			}
		}
	}
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/TorreNegra.bmp");
	for(i=0;i<2;i++){
		if(piezas[i+20]!=-1){
			x=piezas[i+20]%8;
			y=piezas[i+20]/8;
			y=7-y;
			x=80*x+30;
			y=80*y+30;
			for(j=0;j<80;j++){
				for(h=0;h<80;h++){
					if(b[j][h]!=RED){
						a[j+y][h+x]=b[j][h];
					}
				}
			}
		}
	}
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/DamaBlanca.bmp");
	if(piezas[6]!=-1){
		x=piezas[6]%8;
		y=piezas[6]/8;
		y=7-y;
		x=80*x+30;
		y=80*y+30;
		for(j=0;j<80;j++){
			for(h=0;h<80;h++){
				if(b[j][h]!=RED){
					a[j+y][h+x]=b[j][h];
				}
			}
		}
	}
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/DamaNegra.bmp");
	if(piezas[22]!=-1){
		x=piezas[22]%8;
		y=piezas[22]/8;
		y=7-y;
		x=80*x+30;
		y=80*y+30;
		for(j=0;j<80;j++){
			for(h=0;h<80;h++){
				if(b[j][h]!=RED){
					a[j+y][h+x]=b[j][h];
				}
			}
		}
	}
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/ReyBlanco.bmp");
	if(piezas[7]!=-1){
		x=piezas[7]%8;
		y=piezas[7]/8;
		y=7-y;
		x=80*x+30;
		y=80*y+30;
		for(j=0;j<80;j++){
			for(h=0;h<80;h++){
				if(b[j][h]!=RED){
					a[j+y][h+x]=b[j][h];
				}
			}
		}
	}
	LeeBmpColor(b,"c:/Users/moysdavid/Desktop/Ajedrez/ReyNegro.bmp");
	if(piezas[23]!=-1){
		x=piezas[23]%8;
		y=piezas[23]/8;
		y=7-y;
		x=80*x+30;
		y=80*y+30;
		for(j=0;j<80;j++){
			for(h=0;h<80;h++){
				if(b[j][h]!=RED){
					a[j+y][h+x]=b[j][h];
				}
			}
		}
	}
	W.PlotCBitmap(a,nr,nc,0,0,1);
	for(i=0;i<80;i++){
		delete []b[i];
	}
	for(i=0;i<nr;i++){
		delete []a[i];
	}
	delete []b;
	delete []a;
	return;
}

void Posicion::Mueve(Tiro a){
	int i,aux1,aux2=-1;
	if(pos[a.coorini]==1&&a.coorini>47){
		for(i=0;i<16;i++){
			if(piezas[i]==a.coorini)piezas[i]=-1;
		}
		piezas[6]=a.coorfin;
		pos[a.coorfin]=5;
		pos[a.coorini]=0;
		return;
	}
	if(pos[a.coorini]==7&&a.coorini<16){
		for(i=0;i<16;i++){
			if(piezas[i+16]==a.coorini)piezas[i+16]=-1;
		}
		piezas[22]=a.coorfin;
		pos[a.coorfin]=11;
		pos[a.coorini]=0;
		return;
	}
	if(a.pieza==6&&(a.coorfin-a.coorini==2||a.coorfin-a.coorini==-2)){
		if(a.coorfin-a.coorini==2&&enrreyB&&pos[7]==4&&PiezaDefAtac(7,4)){
			piezas[7]=6;		//
			piezas[5]=5;		//5 es la torre del rey
			pos[4]=0;
			pos[7]=0;
			pos[5]=4;
			pos[6]=6;
		}
		if(a.coorfin-a.coorini==-2&&enrdamaB&&pos[0]==5&&PiezaDefAtac(0,4)){
			piezas[7]=2;		//
			piezas[4]=3;		//4 es la torre de la dama
			pos[4]=0;
			pos[0]=0;
			pos[3]=4;
			pos[2]=6;
		}
		rBe=1;
		enrdamaB=0;
		enrreyB=0;
		return;
	}
	if(a.pieza==12&&(a.coorfin-a.coorini==2||a.coorfin-a.coorini==-2)){
		if(a.coorfin-a.coorini==2&&enrreyN&&pos[63]==10&&PiezaDefAtac(63,60)){
			piezas[23]=62;		//
			piezas[21]=61;		//21 es la torre del rey
			pos[60]=0;
			pos[63]=0;
			pos[61]=10;
			pos[62]=12;
		}
		if(a.coorfin-a.coorini==-2&&enrdamaN&&pos[56]==10&&PiezaDefAtac(56,60)){
			piezas[23]=58;		//
			piezas[20]=59;		//20 es la torre de la dama
			pos[60]=0;
			pos[56]=0;
			pos[58]=12;
			pos[59]=10;
		}
		rNe=1;
		enrdamaN=0;
		enrreyN=0;
		return;
	}
	for(i=0;i<32;i++){
		if(piezas[i]==a.coorini)
			aux1=i;
		if(piezas[i]==a.coorfin)
			aux2=i;
	}
	piezas[aux1]=a.coorfin;
	if(aux2!=-1){
		piezas[aux2]=-1;
	}
	pos[a.coorini]=0;
	pos[a.coorfin]=a.pieza;
	return;
}

int Posicion::Jacke(){		//funcion aun no teminda
	int i,j,k=13;
	int misp[16],enem[16],aux;
	if(turno%2==0){
		for(i=0;i<16;i++){
			misp[i]=piezas[i];
			enem[i]=piezas[i+16];
		}
	}
	else {
		for(i=0;i<16;i++){
			misp[i]=piezas[i+16];
			enem[i]=piezas[i];
		}
	}
	for(i=0;i<16;i++){
		if(enem[i]!=-1){
			if(i==0||i==1){			//Caballo
				if(enem[i]%8!=7&&enem[i]+17==misp[7])return 1;
				if(enem[i]%8!=0&&enem[i]+15==misp[7])return 1;
				if(enem[i]%8!=0&&enem[i]%8!=1&&enem[i]+6==misp[7])return 1;
				if(enem[i]%8!=6&&enem[i]%8!=7&&enem[i]+10==misp[7])return 1;
				if(enem[i]%8!=7&&enem[i]-15==misp[7])return 1;
				if(enem[i]%8!=0&&enem[i]-17==misp[7])return 1;
				if(enem[i]%8!=0&&enem[i]%8!=1&&enem[i]-10==misp[7])return 1;
				if(enem[i]%8!=6&&enem[i]%8!=7&&enem[i]-6==misp[7])return 1;
			}						
			else if(i==2||i==3){	//Alfil
				aux=enem[i];
				while((aux<=55)&&aux%8!=7&&pos[aux+9]==0){
					aux=aux+9;
				}
				if(aux%8!=7&&aux+9==misp[7])return 1;
				aux=enem[i];
				while((aux<=55)&&aux%8!=0&&pos[aux+7]==0){
					aux=aux+7;
				}
				if(aux%8!=0&&aux+7==misp[7])return 1;
				aux=enem[i];
				while((aux>=8)&&aux%8!=7&&pos[aux-7]==0){
					aux=aux-7;
				}
				if(aux%8!=7&&aux-7==misp[7])return 1;
				aux=enem[i];
				while((aux>=8)&&aux%8!=0&&pos[aux-9]==0){
					aux=aux-9;
				}
				if(aux%8!=0&&aux-9==misp[7])return 1;
			}
			else if(i==4||i==5){		//Torre
				if((enem[i]-misp[7])%8==0&&enem[i]-misp[7]>0){
					for(j=misp[7]+8;j<enem[i];j+=8)
						if(pos[j]!=0)break;
					if(j==enem[i])return 1;
				}
				if((misp[7]-enem[i])%8==0&&misp[7]-enem[i]>0){
					for(j=enem[i]+8;j<misp[7];j+=8)
						if(pos[j]!=0)break;
					if(j==misp[7])return 1;
				}
				aux=enem[i];
				while(aux%8!=0){
					if(aux-1==misp[7])return 1;
					if(pos[aux-1]==0)aux--;
					else break;
				}
				aux=enem[i];
				while(aux%8!=7){
					if(aux+1==misp[7])return 1;
					if(pos[aux+1]==0)aux++;
					else break;
				}
			}
			else if(i==6){				//Dama
				aux=enem[i];
				while((aux<=55)&&aux%8!=7&&pos[aux+9]==0){
					aux=aux+9;
				}
				if(aux%8!=7&&aux+9==misp[7])return 1;
				aux=enem[i];
				while((aux<=55)&&aux%8!=0&&pos[aux+7]==0){
					aux=aux+7;
				}
				if(aux%8!=0&&aux+7==misp[7])return 1;
				aux=enem[i];
				while((aux>=8)&&aux%8!=7&&pos[aux-7]==0){
					aux=aux-7;
				}
				if(aux%8!=7&&aux-7==misp[7])return 1;
				aux=enem[i];
				while((aux>=8)&&aux%8!=0&&pos[aux-9]==0){
					aux=aux-9;
				}
				if(aux%8!=0&&aux-9==misp[7])return 1;
				if((enem[i]-misp[7])%8==0&&enem[i]-misp[7]>0){
					for(j=misp[7]+8;j<enem[i];j+=8){
						if(pos[j]!=0)break;
					}
					if(j==enem[i])return 1;
				}
				if((misp[7]-enem[i])%8==0&&misp[7]-enem[i]>0){
					for(j=enem[i]+8;j<misp[7];j+=8)
						if(pos[j]!=0)break;
					if(j==misp[7])return 1;
				}
				aux=enem[i];
				while(aux%8!=0){
					if(aux-1==misp[7])return 1;
					if(pos[aux-1]==0)aux--;
					else break;
				}
				aux=enem[i];
				while(aux%8!=7){
					if(aux+1==misp[7])return 1;
					if(pos[aux+1]==0)aux++;
					else break;
				}
			}
			else if(i==7){			//Rey
				aux=enem[i];
				if(aux%8!=7&&aux+9==misp[7])return 1;
				if(aux%8!=0&&aux+7==misp[7])return 1;
				if(aux%8!=7&&aux-7==misp[7])return 1;
				if(aux%8!=0&&aux-9==misp[7])return 1;
				if(aux%8!=7&&aux+1==misp[7])return 1;
				if(aux%8!=0&&aux-1==misp[7])return 1;
				if(aux>7&&aux-8==misp[7])return 1;
				if(aux<56&&aux+8==misp[7])return 1;
			}
			else {
				if(turno%2==0){
					if(enem[i]%8!=0&&enem[i]+7==misp[7])return 1;
					if(enem[i]%8!=7&&enem[i]+9==misp[7])return 1;
				}
				else {
					if(enem[i]%8!=0&&enem[i]-9==misp[7])return 1;
					if(enem[i]%8!=7&&enem[i]-7==misp[7])return 1;
				}
			}
		}
	}
	return 0;
}

Posicion TreeP::Tablero(linkP k){
	if(k==raiz)return P;
	Posicion s=Tablero(k->get_padre());
	Tiro a(k->get_tiro().pieza,k->get_tiro().coorini,k->get_tiro().coorfin);
	s.Mueve(a);
	s.turno++;
	return s;
}

int  TreeP::IniciaCaminosTorre(int m,int h,linkP &k){
	if(m==-1)return h;
	Posicion r=Tablero(k),pa=Tablero(k);
	Tiro ta(r.pos[m],0,0);
	int j,aux=m;
	while(aux%8!=7&&r.pos[aux+1]==0){
		ta.coorini=m;
		ta.coorfin=aux+1;
		pa.Mueve(ta);
		if(!pa.Jacke()){
			k->caminos[h]=new NodoP(k,ta);
			h++;
		}
		ta.coorini=ta.coorfin;
		ta.coorfin=m;
		pa.Mueve(ta);
		aux++;
	}
	if(aux%8!=7){
		if(r.pos[m]<7&&r.pos[aux+1]>=7){
			ta.coorini=m;
			ta.coorfin=aux+1;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=16;j<32;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
		if(r.pos[m]>7&&r.pos[aux+1]<7){
			ta.coorini=m;
			ta.coorfin=aux+1;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=0;j<16;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
	}
	aux=m;
	while(aux%8!=0&&r.pos[aux-1]==0){
		ta.coorini=m;
		ta.coorfin=aux-1;
		pa.Mueve(ta);
		if(!pa.Jacke()){
			k->caminos[h]=new NodoP(k,ta);
			h++;
		}
		ta.coorini=ta.coorfin;
		ta.coorfin=m;
		pa.Mueve(ta);
		aux--;
	}
	if(aux%8!=0){
		if(r.pos[m]<7&&r.pos[aux-1]>=7){
			ta.coorini=m;
			ta.coorfin=aux-1;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=16;j<32;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
		if(r.pos[m]>7&&r.pos[aux-1]<7){
			ta.coorini=m;
			ta.coorfin=aux-1;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=0;j<16;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
	}
	aux=m;
	while(aux>7&&r.pos[aux-8]==0){
		ta.coorini=m;
		ta.coorfin=aux-8;
		pa.Mueve(ta);
		if(!pa.Jacke()){
			k->caminos[h]=new NodoP(k,ta);
			h++;
		}
		ta.coorini=ta.coorfin;
		ta.coorfin=m;
		pa.Mueve(ta);
		aux=aux-8;
	}
	if(aux>7){
		if(r.pos[m]<7&&r.pos[aux-8]>=7){
			ta.coorini=m;
			ta.coorfin=aux-8;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=16;j<32;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
		if(r.pos[m]>4&&r.pos[aux-8]<7){
			ta.coorini=m;
			ta.coorfin=aux-8;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;	
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=0;j<16;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
	}
	aux=m;
	while(aux<=55&&r.pos[aux+8]==0){
		ta.coorini=m;
		ta.coorfin=aux+8;
		pa.Mueve(ta);
		if(!pa.Jacke()){
			k->caminos[h]=new NodoP(k,ta);
			h++;
		}
		ta.coorini=ta.coorfin;
		ta.coorfin=m;
		pa.Mueve(ta);
		aux=aux+8;
	}
	if(aux<=55){
		if(r.pos[m]<7&&r.pos[aux+8]>=7){
			ta.coorini=m;
			ta.coorfin=aux+8;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=16;j<32;j++){
				pa.piezas[j]=r.piezas[j];
			}	
		}
		if(r.pos[m]>7&&r.pos[aux+8]<7){
			ta.coorini=m;
			ta.coorfin=aux+8;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=0;j<16;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
	}
	return h;
}

int TreeP::IniciaCaminosAlfil(int m,int h,linkP &k){
	if(m==-1)return h;
	Posicion r=Tablero(k),pa=Tablero(k);
	Tiro ta(pa.pos[m],0,0);
	int j,aux=m;
	while((aux+9)%8!=0&&(aux+9<64)&&r.pos[aux+9]==0){
		ta.coorini=m;
		ta.coorfin=aux+9;
		pa.Mueve(ta);
		if(!pa.Jacke()){
			k->caminos[h]=new NodoP(k,ta);
			h++;
		}
		ta.coorini=ta.coorfin;
		ta.coorfin=m;
		pa.Mueve(ta);
		aux=aux+9;
	}
	if((aux+9)%8!=0&&(aux+9<64)){
		if(r.pos[m]<7&&r.pos[aux+9]>=7){
			ta.coorini=m;
			ta.coorfin=aux+9;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=16;j<32;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
		if(r.pos[m]>7&&r.pos[aux+9]<7){
			ta.coorini=m;
			ta.coorfin=aux+9;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=0;j<16;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
	}
	aux=m;
	while((aux-9>=0)&&(aux-9)%8!=7&&r.pos[aux-9]==0){
		ta.coorini=m;
		ta.coorfin=aux-9;
		pa.Mueve(ta);
		if(!pa.Jacke()){
			k->caminos[h]=new NodoP(k,ta);
			h++;
		}
		ta.coorini=ta.coorfin;
		ta.coorfin=m;
		pa.Mueve(ta);
		aux=aux-9;
	}
	if((aux-9>=0)&&(aux-9)%8!=7){
		if(r.pos[m]<7&&r.pos[aux-9]>=7){
			ta.coorini=m;
			ta.coorfin=aux-9;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=16;j<32;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
		if(r.pos[m]>7&&r.pos[aux-9]<7){
			ta.coorini=m;
			ta.coorfin=aux-9;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=0;j<16;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
	}
	aux=m;
	while((aux-7>=0)&&(aux-7)%8!=0&&r.pos[aux-7]==0){
		ta.coorini=m;
		ta.coorfin=aux-7;
		pa.Mueve(ta);
		if(!pa.Jacke()){
			k->caminos[h]=new NodoP(k,ta);
			h++;
		}
		ta.coorini=ta.coorfin;
		ta.coorfin=m;
		pa.Mueve(ta);
		aux=aux-7;
	}
	if((aux-7>=0)&&(aux-7)%8!=0){
		if(r.pos[m]<7&&r.pos[aux-7]>=7){
			ta.coorini=m;
			ta.coorfin=aux-7;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=16;j<32;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
		if(r.pos[m]>7&&r.pos[aux-7]<7){
			ta.coorini=m;
			ta.coorfin=aux-7;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=0;j<16;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
	}
	aux=m;
	while((aux+7<64)&&(aux+7)%8!=7&&r.pos[aux+7]==0){
		ta.coorini=m;
		ta.coorfin=aux+7;
		pa.Mueve(ta);
		if(!pa.Jacke()){
			k->caminos[h]=new NodoP(k,ta);
			h++;
		}
		ta.coorini=ta.coorfin;
		ta.coorfin=m;
		pa.Mueve(ta);
		aux=aux+7;
	}
	if((aux+7<64)&&(aux+7)%8!=7){
		if(r.pos[m]<7&&r.pos[aux+7]>=7){
			ta.coorini=m;
			ta.coorfin=aux+7;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=16;j<32;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
		if(r.pos[m]>7&&r.pos[aux+7]<7){
			ta.coorini=m;
			ta.coorfin=aux+7;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=m;
			pa.Mueve(ta);
			pa.pos[ta.coorini]=r.pos[ta.coorini];
			for(j=0;j<16;j++){
				pa.piezas[j]=r.piezas[j];
			}
		}
	}
	return h;
}

void TreeP::IniciaCaminos(linkP &k){
	int misp[16],enem[16],i,j,mov=0,h=0,aux,l;
	Posicion r=Tablero(k),pa=Tablero(k);
	k->caminos=new linkP[100];
	Tiro ta(0,0,0);
	if(r.turno%2==0){
		for(i=0;i<16;i++){
			misp[i]=r.piezas[i];
			enem[i]=r.piezas[i+16];
		}
	}
	else {
		for(i=0;i<16;i++){
			misp[i]=r.piezas[i+16];
			enem[i]=r.piezas[i];
		}
	}
	for(i=8;i<16;i++){				//peon
		if(misp[i]!=-1){
			ta.pieza=r.pos[misp[i]];
			if(r.pos[misp[i]]==1){
				if(misp[i]<=15){
					if(r.pos[misp[i]+8]==0){
						ta.coorini=misp[i];
						ta.coorfin=misp[i]+8;
						pa.Mueve(ta);
						if(!pa.Jacke()){
							k->caminos[h]=new NodoP(k,ta);
							h++;
						}
						ta.coorini=ta.coorfin;
						ta.coorfin=misp[i];
						pa.Mueve(ta);
						if(r.pos[misp[i]+16]==0){
							ta.coorini=misp[i];
							ta.coorfin=misp[i]+16;
							pa.Mueve(ta);
							if(!pa.Jacke()){
								k->caminos[h]=new NodoP(k,ta);
								h++;
							}
							ta.coorini=ta.coorfin;
							ta.coorfin=misp[i];
							pa.Mueve(ta);
						}
					}
				}
				else {
					if(r.pos[misp[i]+8]==0){
						ta.coorini=misp[i];
						ta.coorfin=misp[i]+8;
						pa.Mueve(ta);
						if(!pa.Jacke()){
							k->caminos[h]=new NodoP(k,ta);
							h++;
						}
						ta.coorini=ta.coorfin;
						ta.coorfin=misp[i];
						pa.Mueve(ta);
					}
				}
				if(misp[i]%8!=7){
					if(r.pos[misp[i]+9]>=7){
						ta.coorini=misp[i];
						ta.coorfin=misp[i]+9;
						pa.Mueve(ta);
						if(!pa.Jacke()){
							k->caminos[h]=new NodoP(k,ta);
							h++;
						}
						ta.coorini=ta.coorfin;
						ta.coorfin=misp[i];
						pa.Mueve(ta);
						pa.pos[ta.coorini]=r.pos[ta.coorini];
						for(j=16;j<32;j++){
							pa.piezas[j]=r.piezas[j];
						}
					}
				}
				if(misp[i]%8!=0){
					if(r.pos[misp[i]+7]>=7){
						ta.coorini=misp[i];
						ta.coorfin=misp[i]+7;
						pa.Mueve(ta);
						if(!pa.Jacke()){
							k->caminos[h]=new NodoP(k,ta);
							h++;
						}
						ta.coorini=ta.coorfin;
						ta.coorfin=misp[i];
						pa.Mueve(ta);
						pa.pos[ta.coorini]=r.pos[ta.coorini];
						for(j=16;j<32;j++){
							pa.piezas[j]=r.piezas[j];
						}
					}
				}
			}
			if(r.pos[misp[i]]==7){
				ta.pieza=7;
				if(misp[i]>=48){
					if(r.pos[misp[i]-8]==0){
						ta.coorini=misp[i];
						ta.coorfin=misp[i]-8;
						pa.Mueve(ta);
						if(!pa.Jacke()){
							k->caminos[h]=new NodoP(k,ta);
							h++;
						}
						ta.coorini=ta.coorfin;
						ta.coorfin=misp[i];
						pa.Mueve(ta);
						if(r.pos[misp[i]-16]==0){
							ta.coorini=misp[i];
							ta.coorfin=misp[i]-16;
							pa.Mueve(ta);
							if(!pa.Jacke()){
								k->caminos[h]=new NodoP(k,ta);
								h++;
							}
							ta.coorini=ta.coorfin;
							ta.coorfin=misp[i];
							pa.Mueve(ta);
						}
					}
				}
				else{
					if(r.pos[misp[i]-8]==0){
						ta.coorini=misp[i];
						ta.coorfin=misp[i]-8;
						pa.Mueve(ta);
						if(!pa.Jacke()){
							k->caminos[h]=new NodoP(k,ta);
							h++;
						}
						ta.coorini=ta.coorfin;
						ta.coorfin=misp[i];
						pa.Mueve(ta);
					}
				}
				if(misp[i]%8!=7&&r.pos[misp[i]-7]>0){
					if(r.pos[misp[i]-7]<7){
						ta.coorini=misp[i];
						ta.coorfin=misp[i]-7;
						pa.Mueve(ta);
						if(!pa.Jacke()){
							k->caminos[h]=new NodoP(k,ta);
							h++;
						}
						ta.coorini=ta.coorfin;
						ta.coorfin=misp[i];
						pa.Mueve(ta);
						pa.pos[ta.coorini]=r.pos[ta.coorini];
						for(j=0;j<16;j++){
							pa.piezas[j]=r.piezas[j];
						}
					}
				}
				if(misp[i]%8!=0&&r.pos[misp[i]-9]>0){
					if(r.pos[misp[i]-9]<7){
						ta.coorini=misp[i];
						ta.coorfin=misp[i]-9;
						pa.Mueve(ta);
						if(!pa.Jacke()){
							k->caminos[h]=new NodoP(k,ta);
							h++;
						}
						ta.coorini=ta.coorfin;
						ta.coorfin=misp[i];
						pa.Mueve(ta);
						pa.pos[ta.coorini]=r.pos[ta.coorini];
						for(j=0;j<16;j++){
							pa.piezas[j]=r.piezas[j];
						}
					}
				}
			}
		}
	}////////*****//////Enroque aun no tiene caso en que estan atacadas las posiciones entre la torre y rey
	if(misp[7]==pa.piezas[7]){						//enroque
		if(pa.enrreyB&&misp[5]!=-1&&pa.PiezaDefAtac(misp[5],misp[7])){
			ta.pieza=6;
			ta.coorini=4;
			ta.coorfin=6;
			for(l=0;l<16;l++){
				if(pa.PiezaDefAtac(pa.piezas[l+16],6))break;
			}
			if(l==16){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
		}
		if(pa.enrdamaB&&misp[4]!=0&&pa.PiezaDefAtac(misp[4],misp[7])){
			ta.pieza=6;
			ta.coorini=4;
			ta.coorfin=2;
			for(l=0;l<16;l++){
				if(pa.PiezaDefAtac(pa.piezas[l+16],2))break;
			}
			if(l==16){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
		}			
	}
	else {						//enroque
		if(pa.enrreyN&&misp[5]!=-1&&pa.PiezaDefAtac(misp[5],misp[7])){
			ta.pieza=12;
			ta.coorini=60;
			ta.coorfin=62;
			for(l=0;l<16;l++){
				if(pa.PiezaDefAtac(pa.piezas[l],62))break;
			}
			if(l==16){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
		}
		if(pa.enrdamaN&&misp[4]!=-1&&pa.PiezaDefAtac(misp[4],misp[7])){
			ta.pieza=12;
			ta.coorini=60;
			ta.coorfin=58;
			for(l=0;l<16;l++){
				if(pa.PiezaDefAtac(pa.piezas[l],58))break;
			}
			if(l==16){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
		}
	}
	ta.pieza=r.pos[misp[7]];
	if(misp[7]%8!=7&&r.pos[misp[7]+1]==0){	//Rey
		ta.coorini=misp[7];			
		ta.coorfin=misp[7]+1;
		pa.Mueve(ta);
		if(!pa.Jacke()){
			k->caminos[h]=new NodoP(k,ta);
			h++;
		}
		ta.coorini=ta.coorfin;
		ta.coorfin=misp[7];
		pa.Mueve(ta);
	}
	else {
		if(misp[7]%8!=7){
			if(r.pos[misp[7]]==6&&r.pos[misp[7]+1]>=7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]+1;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=16;j<32;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
			if(r.pos[misp[7]]==12&&r.pos[misp[7]+1]<7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]+1;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=0;j<16;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
		}
	}
	if(misp[7]%8!=0&&r.pos[misp[7]-1]==0){
		ta.coorini=misp[7];
		ta.coorfin=misp[7]-1;
		pa.Mueve(ta);
		if(!pa.Jacke()){
			k->caminos[h]=new NodoP(k,ta);
			h++;
		}
		ta.coorini=ta.coorfin;
		ta.coorfin=misp[7];
		pa.Mueve(ta);
	}
	else {
		if(misp[7]%8!=0){
			if(r.pos[misp[7]]==6&&r.pos[misp[7]-1]>=7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]-1;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=16;j<32;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
			if(r.pos[misp[7]]==12&&r.pos[misp[7]-1]<7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]-1;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=0;j<16;j++){
					pa.piezas[j]=r.piezas[j];
				}
				pa.pos[ta.coorini]=r.pos[ta.coorini];
			}
		}
	}
	if(misp[7]<=55){
		if(r.pos[misp[7]+8]==0){
			ta.coorini=misp[7];
			ta.coorfin=misp[7]+8;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=misp[7];
			pa.Mueve(ta);
		}
		else {
			if(r.pos[misp[7]]==6&&r.pos[misp[7]+8]>=7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]+8;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=16;j<32;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
			if(r.pos[misp[7]]==12&&r.pos[misp[7]+8]<7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]+8;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=0;j<16;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
		}
	}
	if(misp[7]>=8){
		if(r.pos[misp[7]-8]==0){
			ta.coorini=misp[7];
			ta.coorfin=misp[7]-8;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=misp[7];
			pa.Mueve(ta);
		}
		else {
			if(r.pos[misp[7]]==6&&r.pos[misp[7]-8]>=7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]-8;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=16;j<32;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
			if(r.pos[misp[7]]==12&&r.pos[misp[7]-8]<7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]-8;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=0;j<16;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
		}
	}
	if(misp[7]<=54&&misp[7]%8!=7){
		if(r.pos[misp[7]+9]==0){
			ta.coorini=misp[7];
			ta.coorfin=misp[7]+9;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=misp[7];
			pa.Mueve(ta);
		}
		else {
			if(r.pos[misp[7]]==6&&r.pos[misp[7]+9]>=7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]+9;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=16;j<32;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
			if(r.pos[misp[7]]==12&&r.pos[misp[7]+9]<7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]+9;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=0;j<16;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
		}
	}
	if(misp[7]<=55&&misp[7]%8!=0){
		if(r.pos[misp[7]+7]==0){
			ta.coorini=misp[7];
			ta.coorfin=misp[7]+7;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=misp[7];
			pa.Mueve(ta);
		}
		else {
			if(r.pos[misp[7]]==6&&r.pos[misp[7]+7]>=7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]+7;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=16;j<32;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
			if(r.pos[misp[7]]==12&&r.pos[misp[7]+7]<7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]+7;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=0;j<16;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
		}
	}
	if(misp[7]>=8&&misp[7]%8!=7){
		if(r.pos[misp[7]-7]==0){
			ta.coorini=misp[7];
			ta.coorfin=misp[7]-7;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=misp[7];
			pa.Mueve(ta);
		}
		else {
			if(r.pos[misp[7]]==6&&r.pos[misp[7]-7]>=7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]-7;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=16;j<32;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
			if(r.pos[misp[7]]==12&&r.pos[misp[7]-7]<7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]-7;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=0;j<16;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
		}
	}
	if(misp[7]>=9&&misp[7]%8!=0){
		if(r.pos[misp[7]-9]==0){
			ta.coorini=misp[7];
			ta.coorfin=misp[7]-9;
			pa.Mueve(ta);
			if(!pa.Jacke()){
				k->caminos[h]=new NodoP(k,ta);
				h++;
			}
			ta.coorini=ta.coorfin;
			ta.coorfin=misp[7];
			pa.Mueve(ta);
		}
		else {
			if(r.pos[misp[7]]==6&&r.pos[misp[7]-9]>=7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]-9;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=16;j<32;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
			if(r.pos[misp[7]]==12&&r.pos[misp[7]-9]<7){
				ta.coorini=misp[7];
				ta.coorfin=misp[7]-9;
				pa.Mueve(ta);
				if(!pa.Jacke()){
					k->caminos[h]=new NodoP(k,ta);
					h++;
				}
				ta.coorini=ta.coorfin;
				ta.coorfin=misp[7];
				pa.Mueve(ta);
				pa.pos[ta.coorini]=r.pos[ta.coorini];
				for(j=0;j<16;j++){
					pa.piezas[j]=r.piezas[j];
				}
			}
		}
	}
	if(r.turno%2==0)ta.pieza=2;
	else ta.pieza=8;
	for(i=0;i<2;i++){		//Caballo
		if(misp[i]!=-1){
			if(misp[i]<=46&&misp[i]%8!=7){
				if(r.pos[misp[i]+17]==0){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+17;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
				}
				if(r.pos[misp[i]]==2&&r.pos[misp[i]+17]>=7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+17;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=16;j<32;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
				if(r.pos[misp[i]]==8&&r.pos[misp[i]+17]!=0&&r.pos[misp[i]+17]<7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+17;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=0;j<16;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
			}
			if(misp[i]>=17&&misp[i]%8!=0){
				if(r.pos[misp[i]-17]==0){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-17;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
				}
				if(r.pos[misp[i]]==2&&r.pos[misp[i]-17]>=7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-17;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=16;j<32;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
				if(r.pos[misp[i]]==8&&r.pos[misp[i]-17]!=0&&r.pos[misp[i]-17]<7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-17;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=0;j<16;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
			}
			if(misp[i]>=16&&misp[i]%8!=7){
				if(r.pos[misp[i]-15]==0){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-15;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
				}
				if(r.pos[misp[i]]==2&&r.pos[misp[i]-15]>=7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-15;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=16;j<32;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
				if(r.pos[misp[i]]==8&&r.pos[misp[i]-15]!=0&&r.pos[misp[i]-15]<7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-15;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=0;j<16;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
			}
			if(misp[i]<=47&&misp[i]%8!=0){
				if(r.pos[misp[i]+15]==0){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+15;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
				}
				if(r.pos[misp[i]]==2&&r.pos[misp[i]+15]>=7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+15;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=16;j<32;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
				if(r.pos[misp[i]]==8&&r.pos[misp[i]+15]!=0&&r.pos[misp[i]+15]<7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+15;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=0;j<16;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
			}
			if(misp[i]<=53&&misp[i]%8!=7&&misp[i]%8!=6){
				if(r.pos[misp[i]+10]==0){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+10;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
				}
				if(r.pos[misp[i]]==2&&r.pos[misp[i]+10]>=7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+10;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=16;j<32;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
				if(r.pos[misp[i]]==8&&r.pos[misp[i]+10]!=0&&r.pos[misp[i]+10]<7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+10;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
				}
			}
			if(misp[i]>=8&&misp[i]%8!=7&&misp[i]%8!=6){
				if(r.pos[misp[i]-6]==0){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-6;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
				}
				if(r.pos[misp[i]]==2&&r.pos[misp[i]-6]>=7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-6;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=16;j<32;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
				if(r.pos[misp[i]]==8&&r.pos[misp[i]-6]!=0&&r.pos[misp[i]-6]<7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-6;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					for(j=0;j<16;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
			}
			if(misp[i]>=10&&misp[i]%8!=0&&misp[i]%8!=1){
				if(r.pos[misp[i]-10]==0){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-10;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
				}
				if(r.pos[misp[i]]==2&&r.pos[misp[i]-10]>=7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-10;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=16;j<32;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
				if(r.pos[misp[i]]==8&&r.pos[misp[i]-10]!=0&&r.pos[misp[i]-10]<7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]-10;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=0;j<16;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
			}
			if(misp[i]<=55&&misp[i]%8!=0&&misp[i]%8!=1){
				if(r.pos[misp[i]+6]==0){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+6;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
				}
				if(r.pos[misp[i]]==2&&r.pos[misp[i]+6]>=7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+6;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=16;j<32;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
				if(r.pos[misp[i]]==8&&r.pos[misp[i]+6]!=0&&r.pos[misp[i]+6]<7){
					ta.coorini=misp[i];
					ta.coorfin=misp[i]+6;
					pa.Mueve(ta);
					if(!pa.Jacke()){
						k->caminos[h]=new NodoP(k,ta);
						h++;
					}
					ta.coorini=ta.coorfin;
					ta.coorfin=misp[i];
					pa.Mueve(ta);
					pa.pos[ta.coorini]=r.pos[ta.coorini];
					for(j=0;j<16;j++){
						pa.piezas[j]=r.piezas[j];
					}
				}
			}
		}
	}
	if(r.turno%2==0)ta.pieza=3;
	else ta.pieza=9;
	for(i=2;i<4;i++){		//Alfil
		h=IniciaCaminosAlfil(misp[i],h,k);
	}
	if(r.turno%2==0)ta.pieza=4;
	else ta.pieza=10;
	for(i=4;i<6;i++){		//Torre
		h=IniciaCaminosTorre(misp[i],h,k);
	}
	if(r.turno%2==0)ta.pieza=5;
	else ta.pieza=11;
	if(misp[6]!=-1){		//Reyna
		h=IniciaCaminosTorre(misp[6],h,k);
		h=IniciaCaminosAlfil(misp[6],h,k);
	}
	k->set_ncaminos(h);
	return ;
}

TreeP::~TreeP(){
	int i,j,k,h,f,g,i2,j2,h2,g2,n=raiz->get_ncam();
	linkP aux,aux2,aux3,aux4;
	for(i=0;i<n;i++){
		aux=raiz->caminos[i];
		f=aux->get_ncam();
		for(j=0;j<f;j++){
			aux2=aux->caminos[j];
			g=aux2->get_ncam();
			for(h=0;h<g;h++){
				aux3=aux2->caminos[h];
				g2=aux3->get_ncam();
				for(i2=0;i2<g2;i2++){
					aux4=aux3->caminos[i2];
					delete aux4;
				}
				delete []aux3->caminos;
			}
			delete []aux2->caminos;
		}
		delete []aux->caminos;
	}
	return;
}

void TreeP::LiberaPosicionesNoUsadas(int c){
	int i,j,k,h,f,i2,j2,h2,g2,g,n=raiz->get_ncam();
	linkP aux,aux2,aux3,aux4;
	for(i=0;i<c;i++){
		aux=raiz->caminos[i];
		f=aux->get_ncam();
		for(j=0;j<f;j++){
			aux2=aux->caminos[j];
			g=aux2->get_ncam();
			for(h=0;h<g;h++){
				aux3=aux2->caminos[h];
				g2=aux3->get_ncam();
				for(i2=0;i2<g2;i2++){
					aux4=aux3->caminos[i2];
					delete aux4;
				}
				delete []aux3->caminos;
			}
			delete []aux2->caminos;
		}
		delete []aux->caminos;
	}
	for(i=c+1;i<n;i++){
		aux=raiz->caminos[i];
		f=aux->get_ncam();
		for(j=0;j<f;j++){
			aux2=aux->caminos[j];
			g=aux2->get_ncam();
			for(h=0;h<g;h++){
				aux3=aux2->caminos[h];
				g2=aux3->get_ncam();
				for(i2=0;i2<g2;i2++){
					aux4=aux3->caminos[i2];
					delete aux4;
				}
				delete []aux3->caminos;
			}
			delete []aux2->caminos;
		}
		delete []aux->caminos;
	}
	//W.printf("LiberaPosiciones");
	return;
}

void TreeP::Tira(int c){
	int i,j,h,g,f,k,n=raiz->caminos[c]->get_ncam();
	linkP naux=raiz;
	P=Tablero(raiz->caminos[c]);	
	P.Pinta();
	Beep(550,250);
	//W.printf("%d  %d",c,n);
	LiberaPosicionesNoUsadas(c);
	raiz->caminos[c]->set_padre(NULL);
	raiz=raiz->caminos[c];
	delete naux;
	//W.printf("inicie raiz");
	linkP aux,aux2,aux3;
	for(i=0;i<n;i++){
		aux=raiz->caminos[i];
		f=aux->get_ncam();
		for(j=0;j<f;j++){
			aux2=aux->caminos[j];
			k=aux2->get_ncam();
			for(h=0;h<k;h++){
				aux3=aux2->caminos[h];
				IniciaCaminos(aux3);
			}
		}
		//IniciaCaminos(raiz->caminos[i],P.turno-1);
	}
	//W.printf("Tire");
	return;
}

int TreeP::TiraHum(){
	int i,x,y,j,x1,y1,r,r2,x2,y2,n,tur=P.turno;
	Posicion pa=P;
	Tiro ax(1,8,16),ta(-1,-1,-1);
	n=raiz->get_ncam();
	if(n==0)return 0;
	while(1){
		if(MouseHit(&x,&y)){
			if(x<30&&y<30)return 0;
			if(x<30||x>670)continue;
			if(y<30||y>670)continue;
			x1=(x-30)/80;
			y1=(y-30)/80;
			y1=7-y1;
			r=x1+y1*8;
			ta.coorini=r;
			if(P.pos[r]==0)continue;
			if(P.pos[r])
			if(tur%2==0&&P.pos[r]>=7)continue;
			if(tur%2==1&&P.pos[r]<7)continue;
			W.line(x1*80+30,(7-y1)*80+30,x1*80+110,(7-y1)*80+30,RGB(255,0,0),3);
			W.line(x1*80+30,(7-y1)*80+30,x1*80+30,(7-y1)*80+110,RGB(255,0,0),3);
			W.line(x1*80+110,(7-y1)*80+30,x1*80+110,(7-y1)*80+110,RGB(255,0,0),3);
			W.line(x1*80+30,(7-y1)*80+110,x1*80+110,(7-y1)*80+110,RGB(255,0,0),3);
			W.printf("");
			int x3,y3;
			while(1){
				if(MouseHit(&x,&y)){
					if(x<30||x>670)continue;
					if(y<30||y>670)continue;
					x2=(x-30)/80;
					y2=(y-30)/80;
					y2=7-y2;
					r2=x2+y2*8;
					ta.coorfin=r2;
					ta.pieza=P.pos[ta.coorini];
					if(x1==x2&&y1==y2){
						P.Pinta();
						break;
					}
					if(P.TiroValido(ta)){
						for(i=0;i<n;i++){
							ax=raiz->caminos[i]->get_tiro();
							if(ta.coorini==ax.coorini&&ta.coorfin==ax.coorfin){
								Tira(i);
								return n;
							}
						}	
					}
				}
			}	
		}
	}
	return n;	
}

void Juega(){
	int p[64],i,a[32],fin=1,HUM=1,COM=0,x,y;
	p[0]=4;	p[1]=2; p[2]=3; p[3]=5;
	p[4]=6; p[5]=3; p[6]=2; p[7]=4;
	for(i=8;i<16;i++){
		p[i]=1;
	} 
	for(i=16;i<48;i++){
		p[i]=0;
	}
	for(i=48;i<56;i++){
		p[i]=7;
	}
	p[56]=10; p[57]=8; p[58]=9; p[59]=11;
	p[60]=12; p[61]=9; p[62]=8; p[63]=10;
	a[0]=1; a[1]=6; a[2]=2; a[3]=5;
	a[4]=0; a[5]=7; a[6]=3; a[7]=4;
	for(i=8;i<16;i++){
		a[i]=i;
	}
	for(i=24;i<32;i++){
		a[i]=i+24;
	}
	a[16]=57; a[17]=62; a[18]=61; a[19]=58;
	a[20]=56; a[21]=63; a[22]=59; a[23]=60;
	Posicion Actual(p,a,0,1,1,1,1,0);
	/*a[0]=18; a[1]=21; a[2]=2; a[3]=19; a[4]=0; a[5]=5; a[6]=3; a[7]=6;
	a[8]=8; a[9]=9; a[10]=-1; a[11]=27; a[12]=20; a[13]=13; a[14]=14; a[15]=15;
	a[16]=42; a[17]=35; a[18]=54; a[19]=58; a[20]=56; a[21]=61; a[22]=59; a[23]=62;
	a[24]=48; a[25]=49; a[26]=50; a[27]=-1; a[28]=44; a[29]=53; a[30]=46; a[31]=55;
	p[0]=4; p[1]=0; p[2]=3; p[3]=5; p[4]=0; p[5]=4; p[6]=6; p[7]=0; 
	p[8]=1; p[9]=1; p[10]=0; p[11]=0; p[12]=0; p[13]=1; p[14]=1; p[15]=1;
	p[16]=0; p[17]=0; p[18]=2; p[19]=3; p[20]=1; p[21]=2; p[22]=0; p[23]=0;
	p[24]=0; p[25]=0; p[26]=0; p[27]=1; p[28]=0; p[29]=0; p[30]=0; p[31]=0;
	p[32]=0; p[33]=0; p[34]=0; p[35]=8; p[36]=0; p[37]=0; p[38]=0; p[39]=0; 
	p[40]=0; p[41]=0; p[42]=8; p[43]=0; p[44]=7; p[45]=0; p[46]=7; p[47]=0; 
	p[48]=7; p[49]=7; p[50]=7; p[51]=0; p[52]=0; p[53]=7; p[54]=9; p[55]=7;
	p[56]=10; p[57]=0; p[58]=9; p[59]=11; p[60]=0; p[61]=10; p[62]=12; p[63]=0;
	Posicion aux(p,a,0,0,0,0,0,0);*/
	Actual.Pinta();
	TreeP A(Actual,4);
	while(fin){
		if(A.get_P().turno%2==0){
			if(HUM){
				fin=A.TiraHum();
			}
			else {
				fin=A.TiraCom();
			}
		}
		else {
			if(HUM){
				fin=A.TiraCom();
			}
			else fin=A.TiraHum();
		}
		A.get_P().Pinta();
	}
	return;
}
