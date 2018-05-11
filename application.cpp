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
#include<sstream>

#define NumeroArquivos 4
#define MemoriaRAM     3 // vai mudar para 1 MB de memoria

#define ENTRADA "entrada.txt"

using namespace std;

void grava(int vetor[],int nome_arq);
void Juntar(int vetor[], int ini, int meio, int fim, int vetAux[]);
int quicksort(int *a, int inicio, int fim);
int particiona(int *a, int inicio, int fim);

int main()
{
   cout<<"Carregando...."<<endl;
   fstream fp;
   fp.open(ENTRADA);
   if(fp.is_open() == false)
        std::cout << "Nao foi possivel abrir arquivo entrada" << std::endl;

   int v[MemoriaRAM];
   int i = 0, j = 0;
   //enquanto houver dados faça:
   while(!fp.eof())
   {
        fp >> v[i];
        cout << "ta no while" << v[i] << endl;
        i++;
        if(i==MemoriaRAM)                       //quando i = memoria ram, faz o quick e poe nos buffers temporários
        {
            quicksort(v, 0, MemoriaRAM-1);
            i=0;
            if(j==NumeroArquivos/2) j=0;
            grava(v,j);
            j++;
        }
   }
   //erro do que sobra do vetor --> corrigir
   for(int k = 0; k < 3; k++)
     cout << v[k] << endl;
   //interpolacao();

   return 0;
}

void grava(int vetor[],int nome_arq){
    fstream fw;           //variavel FILE contendo o endereco do buffer
    stringstream num;     //string contendo o numero do buffer temporário
    string endereco;      //string contendo o nome do buffer temporário
    int i = 0;
    endereco = "temp";
    num << nome_arq;
    endereco = endereco+num.str();
    endereco += ".txt";
    //passa os numeros do vetor para o arquivo
    const char *nome = endereco.c_str();
    //cria o arquivo temp
    fw.open(nome, fstream::in | fstream::out | fstream::app);
    while(i < MemoriaRAM)
    {
        fw << vetor[i]<<" ";
        i++;
    }
    fw.close();
}

void interpolacao()
{
   //Arquivos de entrada e de saida
    /*fstream in[NumeroArquivos/2];
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
    }*/
}

//Ordenação dos buffers temporarios é feita com quicksort
int quicksort(int *a, int inicio, int fim){
    int index;
    if(inicio >= fim)
        return 0;
    {
        index = particiona(a, inicio, fim);
        quicksort(a, inicio, index-1);
        quicksort(a, index+1, fim);
    }
}

int particiona(int *a, int inicio, int fim){
    int i,j,pindex,pivo;
    pindex = inicio;
    pivo = a[fim];
    for(i = inicio; i < fim;i++)
    {
        if(a[i] <= pivo)
        {
            swap(a[pindex], a[i]);
            pindex++;
        }
    }
    swap(a[pindex], a[fim]);
    return pindex;
}
