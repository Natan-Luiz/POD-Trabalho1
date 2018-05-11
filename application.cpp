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

#define NumeroArquivos 5
#define MemoriaRAM     5 // vai mudar para 1 MB de memoria

#define ENTRADA "entrada.txt"

using namespace std;

void grava(int vetor[],int nome_arq,int val);
void Juntar(int vetor[], int ini, int meio, int fim, int vetAux[]);
int quicksort(int *a, int inicio, int fim);
int particiona(int *a, int inicio, int fim);
void interpolacao();

int main()
{
   cout<<"Carregando...."<<endl;
   fstream fp;
   fp.open(ENTRADA);
   if(fp.is_open() == false)
        std::cout << "Nao foi possivel abrir arquivo entrada" << std::endl;
   cout << "asdfsadfsdf" << endl;
   int v[MemoriaRAM];
   int i = 0, j = 0;
   //enquanto houver dados faça:
   while(fp >> v[i])
   {
        //fp >> v[i];
        cout << "ta no while" << v[i] << endl;
        i++;
        if(i==MemoriaRAM)                       //quando i = memoria ram, faz o quick e poe nos buffers temporários
        {
            quicksort(v, 0, MemoriaRAM-1);
            i=0;
            if(j % NumeroArquivos == 0) j=0;
            grava(v,j,MemoriaRAM);
            j++;
        }
   }
   //erro do que sobra do vetor --> corrigir
   if(i > 0){
     quicksort(v, 0, i-1);
     if(j % NumeroArquivos == 0) j=0;
       grava(v,j,i);
     j++;
   }
   interpolacao();
   return 0;
}

void grava(int vetor[],int nome_arq, int val){
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
  while(i < val){
    fw << vetor[i]<<" ";
    i++;
  }
  fw.close();
}

void interpolacao(){
  //Arquivos de entrada e de saida
  fstream in[NumeroArquivos];
  fstream out[NumeroArquivos];
  //Abre os arquivos de entrada como in
  int i = 0;
  while(i < NumeroArquivos){
    stringstream num;
    num << i;
    string enderecoIn = "temp"+num.str();
    enderecoIn += ".txt";
    const char *nome = enderecoIn.c_str();
    in[i].open(nome, fstream::in);
    i++;
  }
  int j = 0;
  //Abre os arquivos de saida como out
  while(j < NumeroArquivos){
    stringstream num;
    num << j;
    string enderecoOut= "out"+num.str();
    enderecoOut += ".txt";
    const char *nome = enderecoOut.c_str();
    out[j].open(nome, fstream::out);
    j++;
  }
  for(int k = 0; k < NumeroArquivos; k++){
    stringstream num;
    num << k;
    string arqtemp = "temp"+num.str(), arqout = "out"+num.str();
    arqout += ".txt";
    arqtemp += ".txt";
    const char *nome = arqtemp.c_str();
    const char *nomesaida = arqout.c_str();
    in[k].close();
    out[k].close();
    remove(nome);
    remove(nomesaida);
  }
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
