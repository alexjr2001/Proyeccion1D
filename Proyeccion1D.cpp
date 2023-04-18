/*
PROJECT: Projection MultiDimension to 1D
AUTHOR: ALEXANDER SEBASTIAN GOMEZ DEL CARPIO
DATE: 06/30/2022

[EN]
IN THIS CODE THE CONVERSION OF DATA FROM 10 DIMENSIONS TO 1 DIMENSION IS FOUND, IT IS TAKEN AS
THE Z-ORDER CRITERION. THEN, HAVING CONVERTED THESE VALUES, ONE IS CHOOSE, WHICH WILL BE COMPARED TO
SEARCH YOUR 5, 10, 15 AND 20 NEAREST NEIGHBORS IN 10 DIMENSIONS AND IN ONE DIMENSION. FINALLY, WE RETURN
THE PERCENTAGE OF COINCIDENCE BETWEEN THESE TWO PROJECTIONS.

[ES]
EN EL PRESENTE CODIGO SE ENCUENTRA LA CONVERSION DE DATOS DE 10 DIMENSIONES A 1 DIMENSION, SE TOMA COMO 
CRITERIO EL Z-ORDER. LUEGO, DE HABER CONVERTIDO ESTOS VALORES, SE ELIGE UNO, EL CUAL SERÁ COMPARADO PARA 
BUSCAR SUS 5, 10, 15 Y 20 VECINOS MAS CERCANOS EN 10 DIMENSIONES Y EN UNA DIMENSION. FINALMENTE, DEVOLVEMOS 
EL PORCENTAJE DE COINCIDENCIA ENTRE ESTAS DOS PROYECCIONES.
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <random>

using namespace std;

//Calculates the euclidean distance
int distance(vector<int> v1, vector<int> v2){
	int suma=0;
	for(int i=0; i<v1.size();i++){
		suma+=pow(v1[i]-v2[i],2);
	}
	suma=sqrt(suma);
	return suma;
}

//Turns backwards a binary number (110->011)
void reverse(string& str)
{
    int n = str.length();
    for (int i = 0; i < n / 2; i++)
        swap(str[i], str[n - i - 1]);
}

//Converts a decimal number to a binary one
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

//Converts a binary number to a decimal one
long long convertBinLL(string bin){
		long long result=0;
		for(int i=bin.size()-1,j=0;i>=0;i--,j++){
			if(bin[i]=='1'){
				result+=pow(2,j);
			}			
		}
    return result;
}

//Converts a vector of 10 dimensions to a string of 1 dimension with the bits distributed
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
	//Random function
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> distrib(0, 9);

	int data=1000;			//We take 1000 numbers as data, thus that there are some coincidences( with 5000 it is difficult to find coincidences)
	int lim=10;					//The values are from 0 to 9
	int dim=10;					//DIMENSION
	vector<vector<int>> data10D(data,vector<int>(dim));
	
	for(int i=0; i<data;i++){				//Creates the random data
		for(int j=0;j<dim;j++){		
			data10D[i][j]=distrib(gen);			
		}
	}

	
	vector<string> bin;										//Saves from 0 to 9 in binaries
	int tamBits = convert(lim-1).size();
	for(int i=0;i<lim-1;i++){
		bin.push_back(convert(i,tamBits));
	}
	bin.push_back(convert(lim-1));
	

	vector<pair<vector<int>*,int>>::iterator it;
	vector<pair<vector<int>*,int>> knn10D;
	int pos=1;								//Position of data in 10D choosen
	vector<int> choosen=data10D[pos];

	
	it=knn10D.begin();
	int dist; 
	bool find=false;
	for(int i=0;i<data;i++){
		if(i!=pos){
			dist=distance(choosen,data10D[i]);				//Finds out the distance
			find=false;
			for(it=knn10D.begin();it!=knn10D.end();++it){
				if(it->second>=dist){
					knn10D.insert(it, make_pair(&data10D[i],dist));			//INSERT ITS K-NEIGHBORS IN INCREASING ORDER
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
	string binChoosen=vecBin(choosen,tamBits,bin); 		//WE CONVERT THE 10D DATA CHOSEN INTO 1D WITH Z-ORDER
	long long pos1D=convertBinLL(binChoosen);			

	find=false;	
	long long dist2;
	for(int i=0;i<data;i++){
		if(i!=pos){
			dist2=abs(pos1D-convertBinLL(vecBin(data10D[i],tamBits,bin)));		//WE CONVERT THE DATA TO BE COMPARED TO 1D AND CALCULATE ITS DISTANCE
			find=false;
			for(it2=knn1D.begin();it2!=knn1D.end();++it2){
				if(it2->second>=dist2){
					knn1D.insert(it2, make_pair(&data10D[i],dist2));			//INSERT ITS K-NEIGHBORS IN INCREASING ORDER
					find=true;
					break;
				}
			}
			if(find==false){
				knn1D.push_back(make_pair(&data10D[i],dist2));
			}
		}
	}

	//We find out if the K-Neighbours have any coincidence
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

	//Print out the neighbours, then which ones match and finally, the percentage of the matching
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
