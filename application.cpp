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

#define NumeroArquivos 5 // 5 arquivos temp e 5 out
#define MemoriaRAM     5 // vai mudar para 1 MB de memoria

#define ENTRADA "entrada.txt"

using namespace std;

void grava(int vetor[],int nome_arq,int val);
void Juntar(int vetor[], int ini, int meio, int fim, int vetAux[]);
int quicksort(int *a, int inicio, int fim);
int particiona(int *a, int inicio, int fim);
void interpolacao();
void deletaTemps();
int procura_menor(int *vet, int n);

int main()
{
   cout << "Carregando...." << endl;
   fstream fp;
   fp.open(ENTRADA);
   if(fp.is_open() == false)
        std::cout << "Nao foi possivel abrir arquivo entrada" << std::endl;
   int v[MemoriaRAM];
   int i = 0, j = 0;
   //enquanto houver dados faça:
   while(fp >> v[i]){
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

   //Comentar deletaTemps caso queira ver os arquivos
   deletaTemps();
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
    fw << vetor[i] << " ";
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
  //while que coloca todos os arquivos 'temp' no vetor de arquivos 'in[i]'
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
  //Abre os arquivos de saida no vetor de arquivos out[n] onde vao ser colocados a interpolação dos arquivos temp
  while(j < NumeroArquivos){
    stringstream num;
    num << j;
    string enderecoOut= "out"+num.str();
    enderecoOut += ".txt";
    const char *nome = enderecoOut.c_str();
    out[j].open(nome, fstream::out);
    j++;
  }
  //a partir daqui esta em implementação
  //A ideia era armazenar no vetor 'v1' os primeiros valores de cada conjunto de MemoriaRam,
  //Em que cada indice desse vetor representa o numero do arquivo temp
  //E o conteúdo desse indice é o menor valor de tal arquivo
  //Para depois pegar o menor valor do vetor e escrever ele no arquivo out
  //Quando pega o menor valor do vetor 'v1' coloca-se outro do arquivo temp daquele indice até o final da palavra
  int vinicial[MemoriaRAM];
  //coloca menor valor de cada Conjunto de numero arquivos no vetor 'vinicial'
  for(int c = 0; c < NumeroArquivos; c++){
    in[c] >> vinicial[c];
  }
  cout << "Prints para nao precisar olhar os arquivos:" << endl;
  //vetor que conta quantos elementos foram lidos de cada arquivo durante as comparacoes -> usado na correção de bugs
  int cont[NumeroArquivos] = {};
  for(int c = 0; c < NumeroArquivos*MemoriaRAM;c++){
    //funcao que retorna o indice de qual arquivo esta o ponteiro de menor valor
    int ind = procura_menor(vinicial, NumeroArquivos);
    cout << "Menor ta no indice:" << ind << endl;
    // coloca no vetor de saida o menor valor dos n arquivos
    out[0] << vinicial[ind] << " ";
    //flags para quando o arquivo tem menos arquivos do que a memoriaRam, para nao gerar bugs
    if(!(in[ind] >> vinicial[ind])){
      //caso nao tenha mais numeros no arquivo ele ve se ele ocupou toda a memoriaRam daquela particao do arquivo
      //no caso de ter espaços vazios, ele desconta esses espaços do 'c' para executar um numero menor de vezes
      vinicial[ind] = -1;
      int dif = (MemoriaRAM - (cont[ind]+1));
      c += dif;
    }
    //incrementa o numero de entradas lidas em um arquivo
    cont[ind] += 1;
    //coloca flag quando ja leu toda a partição de um arquivo, ou seja todos as entradas de algum arquivo ja estão escritos na saida
    if(cont[ind] % MemoriaRAM == 0)
      vinicial[ind] = -1;
  }
  //aqui fecha os arquivos abertos nessa função
  for(int k = 0; k < NumeroArquivos; k++){
    in[k].close();
    out[k].close();
  }
}

int procura_menor(int *vet, int n){
  int menor;
  int i = 1, ind = 0;
  cout << "-" << vet[0] << "-";
  if(vet[0] >= 0)
    menor = vet[0];
  while(i < n){
    if(menor > vet[i] && vet[i] >= 0){
      menor = vet[i];
      ind = i;
    }
    cout << "-" << vet[i] << "-";
    i++;
  }
  return ind;
}

//Deleta os arquivos temporarios
void deletaTemps(){
  for(int k = 0; k < NumeroArquivos; k++){
    stringstream num;
    num << k;
    string arqtemp = "temp"+num.str(), arqout = "out"+num.str();
    arqout += ".txt";
    arqtemp += ".txt";
    const char *nome = arqtemp.c_str();
    const char *nomesaida = arqout.c_str();
    //deleta os buffers temporarios("tempN")
    remove(nome);
    //deleta os arquivos de interpolação("outN")
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

//Subfunção do quicksort interno
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
