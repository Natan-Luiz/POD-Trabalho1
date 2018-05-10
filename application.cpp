/*
//   TRABALHO 1 DE POD
//
// Denes Vargas e Natan Berwaldt
//
//
*/

// Esta primeira versão está sendo feita ordenando Números, 
// para entender bem o conceito da ordenação externa, pra depois trocar por palavras.

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
   //enquanto houver dados faça:
   while(fp >> v[i])
   {
        i++;
        if(i==MemoriaRAM)                       //quando i = memoria ram, faz o merge e poe no arquivo
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
    fstream fout;       //string contendo o endereco do arquivo
    stringstream num;   //string contendo o numero do arquivo
    string endereco;    //string contendo o nome do arquivo
    int i = 0;
    endereco = "in";
    num << nome_arq;
    endereco = endereco+num.str();
    fout.open(endereco, fstream::in | fstream::out | fstream::app);
   //passa os numeros do vetor para o arquivo
    while(i < MemoriaRAM)
    {
        fout << vetor[i]<<" ";
        i++;
    }
    fout.close();
}

void interpolacao()
{
   //Arquivos de entrada e de saida
    fstream in[NumeroArquivos/2];
    fstream out[NumeroArquivos/2];


   //Abre os arquivos de entrada como in
    while(int i=0 < NumeroArquivos/2)
    {
        stringstream num = i;
        string enderecoIn= "in"+num.str();
        in[i].open(enderecoIn, fstream::in);
    }
   
   //Abre os arquivos de saida como out
    while(int i=0 < NumeroArquivos/2)
    {
        stringstream num = i;
        string enderecoOut= "out"+num.str();
        out[i].open(enderecoOut, fstream::out);
    }

    while(int i=0 < NumeroArquivos/2)
    {
        //FAZER
    }
}
