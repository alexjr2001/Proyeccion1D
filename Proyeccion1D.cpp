/*
PROYECTO: PROYECCIONES DE 10D A 1D
AUTOR: ALEXANDER SEBASTI�N G�MEZ DEL CARPIO
FECHA: 30/06/2022

[ES]
EN EL PRESENTE CODIGO SE ENCUENTRA LA CONVERSION DE DATOS DE 10 DIMENSIONES A 1 DIMENSION, SE TOMA COMO CRITERIO EL Z-ORDER. LUEGO, DE HABER CONVERTIDO ESTOS VALORES, SE ELIGE UNO, EL CUAL SERÁ COMPARADO PARA BUSCAR SUS 5, 10, 15 Y 20 VECINOS MAS CERCANOS EN 10 DIMENSIONES Y EN UNA DIMENSION. FINALMENTE, DEVOLVEMOS EL PORCENTAJE DE COINCIDENCIA ENTRE ESTAS DOS PROYECCIONES.
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <random>

using namespace std;

//CALCULA LA DISTANCIA EUCLIDIANA
int distance(vector<int> v1, vector<int> v2){
	int suma=0;
	for(int i=0; i<v1.size();i++){
		suma+=pow(v1[i]-v2[i],2);
	}
	suma=sqrt(suma);
	return suma;
}

//VOLTEA UN NUMERO BINARIO (110->011)
void reverse(string& str)
{
    int n = str.length();
    for (int i = 0; i < n / 2; i++)
        swap(str[i], str[n - i - 1]);
}

//CONVERTIMOS UN NUMERO DECIMAL A UNO BINARIO
string convert(int n, int tam=0){
    string r;
    while (n != 0){
        r += (n % 2 == 0?"0":"1");
        n /= 2;
    }
		reverse(r);
    string r1;
		if(tam!=0){
			for(int i=r.size();i<tam;i++){
				r1+="0";
			}
		}
		r1+=r;
    return r1;
}

//CONVERTIMOS UN NUMERO BINARIO A UN DECIMAL
long long convertBinLL(string bin){
		long long result=0;
		for(int i=bin.size()-1,j=0;i>=0;i--,j++){
			if(bin[i]=='1'){
				result+=pow(2,j);
			}			
		}
    return result;
}

//CONVERTIMOS UN VECTOR DE 10 DIMENSIONES A UN STRING DE 1D CON LOS BITS REPARTIDOS
string vecBin(vector<int> v, int tam, vector<string> bin){
	string D1;  
	for(int i=0; i<tam;i++){
		for(int j=0; j<v.size();j++){
			D1.push_back(bin[v[j]][i]);
		}
	}
	return D1;
}

int main() {
	//FUNCION RANDOM
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> distrib(0, 9);

	int data=1000;			//TOMAMOS 1000 DATOS PARA QUE HAYA ALGUNAS COINCIDENCIAS, CON 5000 ES DIFICIL QUE SE ENCUENTREN
	int lim=10;					//LOS VALORES SON DEL 0 AL 9
	int dim=10;					//DIMENSION
	vector<vector<int>> data10D(data,vector<int>(dim));
	
	for(int i=0; i<data;i++){				//CREAMOS LOS DATOS RANDOM
		for(int j=0;j<dim;j++){		
			data10D[i][j]=distrib(gen);			
		}
	}

	
	vector<string> bin;										//GUARDAMOS DEL 0 AL 9 EN BINARIOS
	int tamBits = convert(lim-1).size();
	for(int i=0;i<lim-1;i++){
		bin.push_back(convert(i,tamBits));
	}
	bin.push_back(convert(lim-1));
	

	vector<pair<vector<int>*,int>>::iterator it;
	vector<pair<vector<int>*,int>> knn10D;
	int pos=1;								//POSICION DEL DATO EN 10D ELEGIDO
	vector<int> choosen=data10D[pos];

	
	it=knn10D.begin();
	int dist; 
	bool find=false;
	for(int i=0;i<data;i++){
		if(i!=pos){
			dist=distance(choosen,data10D[i]);				//HALLAMOS SU DISTANCIA
			find=false;
			for(it=knn10D.begin();it!=knn10D.end();++it){
				if(it->second>=dist){
					knn10D.insert(it, make_pair(&data10D[i],dist));			//INSERTAMOS EN ORDEN CRECIENTE SUS KVECINOS
					find=true;
					break;
				}
			}
			if(find==false){
				knn10D.push_back(make_pair(&data10D[i],dist));
			}
		}
	}

	vector<pair<vector<int>*,long long>>::iterator it2;
	vector<pair<vector<int>*,long long>> knn1D;
	it2=knn1D.begin();
	string binChoosen=vecBin(choosen,tamBits,bin); 		//EL DATO DE 10D ELEGIDO LO CONVERTIMOS EN 1D CON Z-ORDER
	long long pos1D=convertBinLL(binChoosen);			

	find=false;	
	long long dist2;
	for(int i=0;i<data;i++){
		if(i!=pos){
			dist2=abs(pos1D-convertBinLL(vecBin(data10D[i],tamBits,bin)));		//CONVERTIMOS EL DATO A COMPARAR A 1D Y CALCULAMOS SU DISTANCIA
			find=false;
			for(it2=knn1D.begin();it2!=knn1D.end();++it2){
				if(it2->second>=dist2){
					knn1D.insert(it2, make_pair(&data10D[i],dist2));			//INSERTAMOS EN ORDEN CRECIENTE SUS KVECINOS
					find=true;
					break;
				}
			}
			if(find==false){
				knn1D.push_back(make_pair(&data10D[i],dist2));
			}
		}
	}

	//HALLAMOS SI COINCIDEN LOS VECINOS MAS CERCANOS
	bool found=false;
	int cantidad[4]={5,10,15,20};
	int match[4]={5,10,15,20};
	for(int i=0;i<4;i++){
		for(int j=0;j<cantidad[i];j++){
			found=false;
			for(int k=0;k<cantidad[i];k++){
				for(int l=0;l<dim;l++){
					if((*knn10D[j].first)[l]!=(*knn1D[k].first)[l]){
						break;
					}
					if(l==dim-1){
						found=true;
					}
				}
				if(found) break;
			}
			if(!found){
				match[i]-=1;
			}
		}
	}

	//IMPRIMIMOS
	cout<<"VECINOS:  ";
	for(int i=0;i<4;i++){
		cout<< cantidad[i] << " ";
	}
	cout<<"\nIGUALES:  ";
	for(int i=0;i<4;i++){
		cout<< match[i] << "  ";
	}
	cout<<endl<<"PORCENTAJE: ";
	for(int i=0; i<4;i++){
		cout<< (match[i]*100)/cantidad[i] << "% ";
	}

	return 0;
}