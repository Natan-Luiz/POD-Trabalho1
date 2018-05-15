/*
//   TRABALHO 1 DE POD
//
// Dênes Vargas e Natan Berwaldt
//
//
*/


#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<math.h>

#define NumeroArquivos 10 // 10 arquivos temp e 10 out.
#define MemoriaRAM     200 // Tamanho da memoria disponivel para ordenação, em Bytes.

#define ENTRADA "entrada.txt"
#define SAIDA "saida.txt"

using namespace std;

void grava(string vetor[], int nome_arq, int val);
int quicksort(string *a, int inicio, int fim);
int particiona(string *a, int inicio, int fim);
void interpolacao(int tamParticao, int mRounds, int caminho, int maxCaminhos);
void deletaTemps();
int procura_menor(string *vet, int n);
void encerra(fstream final[]);

int main()
{
  cout << "Carregando...." << endl;
  fstream fp;
  fp.open(ENTRADA);
  if(fp.is_open() == false)
    cout << "Nao foi possivel abrir arquivo entrada" << endl;
  string v[MemoriaRAM], palavra;
  int i = 0, j = 0, particoes = 0, memoria = 0;
  //enquanto houver dados faça:
  while(fp >> palavra){
   // i++;

    if(MemoriaRAM - memoria < palavra.size()){                       //quando i = memoria ram, faz o quick e poe nos buffers temporários
      quicksort(v, 0, i-1);
      memoria = 0;
      if(j % NumeroArquivos == 0) j = 0;
      grava(v,j,i);
      i = 0;
      j++;
      particoes++;
    }

    v[i]=palavra;
    memoria+=palavra.size();
    i++;
  }

  if(i > 0){
    quicksort(v, 0, i-1);
    if(j % NumeroArquivos == 0) j = 0;
      grava(v,j,i);
    j++;
    particoes++;
  }


  // O numero maximo de caminhos é dado pelo numero de vezes que se pode dividir o numero
  // de particoes pelo numero de arquivos.
  int maxCaminhos = 0, result = particoes;
  while(result > 1){
    result = (int)(result/NumeroArquivos) + (result % NumeroArquivos > 0 ? 1 : 0);
    maxCaminhos++;
  }
  result = (int)(particoes/NumeroArquivos) + (particoes % NumeroArquivos > 0 ? 1 : 0);
  cout << "Ordenando...." <<endl;
  interpolacao(MemoriaRAM, result, 1, maxCaminhos);

  //Comentar deletaTemps caso queira ver os arquivos
  deletaTemps();
  return 0;
  }



void grava(string vetor[], int nome_arq, int val){
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
    fw <<  " " << vetor[i];
    i++;
  }
  fw << " ~";
  fw.close();
}

void encerra(int valor){
  cout << "Salvando...." << endl;
  fstream fpout, fpin;
  fpout.open(SAIDA, fstream::out);

  if(valor == 1)
    fpin.open("temp0.txt", fstream::in);
  else
    fpin.open("out0.txt", fstream::in);

  string temp;
  while(fpin >> temp)
    if(temp.compare("~")!=0)
      fpout << temp << " ";
  fpout.close();
}


void interpolacao(int tamParticao, int mRounds, int caminho, int maxCaminhos){
  //Arquivos de entrada e de saida
  fstream in[NumeroArquivos];
  fstream out[NumeroArquivos];
  //Abre os arquivos de entrada como in
  int i = 0;
  //while que coloca todos os arquivos 'temp' no vetor de arquivos 'in[i]' e 'out' no 'out[i]'
  while(i < NumeroArquivos){
    stringstream num;
    num << i;
    //Da o nome tempoN.txt
    string enderecoIn = "temp"+num.str();
    enderecoIn += ".txt";
    const char *nomeI = enderecoIn.c_str();

    //Da o nome outN.txt
    string enderecoOut = "out"+num.str();
    enderecoOut += ".txt";
    const char *nomeO = enderecoOut.c_str();

    //Atribui out.txt ou temp.txt para 'in' ou 'out' de acordo com a rodada atual, para intercalar entre os dois arquivos
    if(caminho % 2 == 0){
      in[i].open(nomeO, fstream::in);
      out[i].open(nomeI, fstream::out);
    }
    else{
      in[i].open(nomeI, fstream::in);
      out[i].open(nomeO, fstream::out);
    }
    //incrementa o indice
    i++;
  }

  int round = 1, part = 0;
  //Chama novamente a interpolacao para a 'proxima rodada'/'proximo caminho'
  while (round <= mRounds){
    string vinicial[NumeroArquivos];
    //coloca menor valor de cada Conjunto de numero arquivos no vetor 'vinicial'
    for(int c = 0; c < NumeroArquivos; c++){
      if(in[c] >> vinicial[c])
      {
        if (vinicial[c].compare("~") == 0)
	   vinicial[c] = "&";
      }
      else{
        vinicial[c] = "&";
      }
    }
    while(true){

      //funcao que retorna o indice de qual arquivo esta o ponteiro de menor valor
      int ind = procura_menor(vinicial, NumeroArquivos);
     // cout << "Menor ta no arquivo:" << ind << endl;
      if(vinicial[ind].compare("&") == 0){
		out[part] << " ~ ";
	break;
      }
      // coloca no vetor de saida o menor valor dos n arquivos
      out[part] <<  " " << vinicial[ind];

      //coloca flag quando ja leu toda a partição de um arquivo, ou seja todos as entradas de algum arquivo ja estão escritos na saida
      if(in[ind].peek() == -1)
        vinicial[ind] = "&";
      else{
        in[ind] >> vinicial[ind];
        if(vinicial[ind].compare("~") == 0)
           vinicial[ind] = "&";
      }
    }
    part++;
    if(part == NumeroArquivos) part = 0;
    round++;
  }
  //aqui fecha os arquivos abertos nessa função
  for(int k = 0; k < NumeroArquivos; k++){
      in[k].close();
      out[k].close();
  }
  //Chama novamente a interpolacao para o proximo caminho
  if(caminho < maxCaminhos){
    int result = (int)(mRounds/NumeroArquivos) + (mRounds % NumeroArquivos > 0 ? 1 : 0);
    interpolacao(tamParticao*NumeroArquivos, result, caminho+1, maxCaminhos);
  }
  //Ou encerra e salva os dados no arquivos de saida.txt
  else{
    if(caminho % 2 == 0)
      encerra(1);
    else
      encerra(0);
  }
}

int procura_menor(string vet[], int n){
  string menor;
  int i = 1, ind = 0;
  //cout << "-" << vet[0] << "-";
  if(vet[0].compare("&") != 0)
    menor = vet[0];
  else
    menor = "~~";
  while(i < n){
    if(menor.compare(vet[i]) > 0 && vet[i].compare("&") != 0){
      menor = vet[i];
      ind = i;
    }
    //cout << "-" << vet[i] << "-";
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
int quicksort(string *a, int inicio, int fim){
  int index;
  if(inicio >= fim)
    return 0;
  index = particiona(a, inicio, fim);
  quicksort(a, inicio, index-1);
  quicksort(a, index+1, fim);
}

//Subfunção do quicksort interno
int particiona(string *a, int inicio, int fim){
  int i,pindex;
  string pivo;
  pindex = inicio;
  pivo = a[fim];
  for(i = inicio; i < fim; i++)
  {
    if(pivo.compare(a[i]) >= 0)
    {
      swap(a[pindex], a[i]);
      pindex++;
    }
  }
  swap(a[pindex], a[fim]);
  return pindex;
}
