/*
//   TRABALHO 1 DE POD
//
// Denes Vargas e Natan Berwaldt
//
//
*/

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <sstream>


#define NumeroArquivos 4
#define MemoriaRAM     3 //1 MB de memoria

#define ENTRADA "entrada.txt"

using namespace std;

void grava(int vetor[],int nome_arq);
void Juntar(int vetor[], int ini, int meio, int fim, int vetAux[]);
void MergeSort(int vetor[], int inicio, int fim, int vetorAux[]);
void MergeSort(int vetor[], int tamanho);

int main()
{
   cout<<"Carregando...."<<endl;

   fstream fp;
   fp.open(ENTRADA);


   int v[MemoriaRAM];
   int i = 0, j =0;;
   while(fp >> v[i])
   {
        i++;
        if(i==3)
        {
            MergeSort(v,MemoriaRAM);
            i=0;
            if(j==NumeroArquivos/2) j=0;
            grava(v,j);
            j++;
        }
   }
   //interpolacao();

   return 0;
}

void grava(int vetor[],int nome_arq)
{
    fstream fout;
    stringstream num;
    string endereco;
    int i = 0;
    endereco = "in";
    num << nome_arq;
    endereco = endereco+num.str();
    fout.open(endereco, fstream::in | fstream::out | fstream::app);
    while(i < MemoriaRAM)
    {
        fout << vetor[i]<<" ";
        i++;
    }
    fout.close();
}

void interpolacao()
{
    fstream in[NumeroArquivos/2];
    fstream out[NumeroArquivos/2];



    while(int i=0 < NumeroArquivos/2)
    {
        stringstream num = i;
        string enderecoIn= "in"+num;
        in[i].open(enderecoIn, fstream::in);
    }

    while(int i=0 < NumeroArquivos/2)
    {
        stringstream num = i;
        string enderecoOut= "in"+num;
        out[i].open(enderecoOut, fstream::out);
    }

    while(int i=0 < NumeroArquivos/2)
    {
        s
    }
}
