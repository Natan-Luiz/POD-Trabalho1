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
#include<math.h>

#define NumeroArquivos 3 // 5 arquivos temp e 5 out
#define MemoriaRAM     3 // vai mudar para 1 MB de memoria

#define ENTRADA "entrada.txt"
#define SAIDA "saida.txt"

using namespace std;

void grava(int vetor[],int nome_arq,int val);
void Juntar(int vetor[], int ini, int meio, int fim, int vetAux[]);
int quicksort(int *a, int inicio, int fim);
int particiona(int *a, int inicio, int fim);
void interpolacao(int tamParticao,int mRounds, int caminho, int maxCaminhos);
void deletaTemps();
int procura_menor(int *vet, int n);
void encerra(fstream final[]);

int main()
{
   cout << "Carregando...." << endl;
   fstream fp;
   fp.open(ENTRADA);
   if(fp.is_open() == false)
        std::cout << "Nao foi possivel abrir arquivo entrada" << std::endl;
   int v[MemoriaRAM];
   int i = 0, j = 0, particoes = 0;
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
            particoes++;
        }
   }

   if(i > 0){
     quicksort(v, 0, i-1);
     if(j % NumeroArquivos == 0) j=0;
       grava(v,j,i);
     j++;
     particoes++;
   }

   cout << "Ordenando...." << endl;


    // O numero maximo de caminhos é dado pelo numero de vezes que se pode dividir o numero
    // de particoes pelo numero de arquivos.
    int maxCaminhos=0, result=particoes;
    while(result>1){
       result = (int)(result/NumeroArquivos) + (result % NumeroArquivos > 0 ? 1 : 0);
       maxCaminhos++;
    }
    result = (int)(particoes/NumeroArquivos) + (particoes % NumeroArquivos > 0 ? 1 : 0);
    interpolacao(MemoriaRAM,result,1,maxCaminhos);

   //Comentar deletaTemps caso queira ver os arquivos
   //deletaTemps();
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
    fw << " " << vetor[i];
    i++;
  }
  fw.close();
}

void encerra(int valor)
{
   cout << "Salvando...." << endl;
   fstream fpout, fpin;
   fpout.open(SAIDA, fstream::out);

   if(valor == 1)
      fpin.open("temp0.txt",fstream::in);
   else
      fpin.open("out0.txt",fstream::in);

   int temp;
   while(fpin >> temp)
      fpout << temp << " ";
   fpout.close();
}


void interpolacao(int tamParticao,int mRounds,int caminho, int maxCaminhos){
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
    string enderecoOut= "out"+num.str();
    enderecoOut += ".txt";
    const char *nomeO = enderecoOut.c_str();

    //Atribui out.txt ou temp.txt para 'in' ou 'out' de acordo com a rodada atual
    if(caminho%2==0){
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
      //a partir daqui esta em implementação
      //A ideia era armazenar no vetor 'v1' os primeiros valores de cada conjunto de MemoriaRam,
      //Em que cada indice desse vetor representa o numero do arquivo temp
      //E o conteúdo desse indice é o menor valor de tal arquivo
      //Para depois pegar o menor valor do vetor e escrever ele no arquivo out
      //Quando pega o menor valor do vetor 'v1' coloca-se outro do arquivo temp daquele indice até o final da palavra
      int vinicial[MemoriaRAM];
      int count_toC = 0;
      //coloca menor valor de cada Conjunto de numero arquivos no vetor 'vinicial'
      for(int c = 0; c < NumeroArquivos; c++){
        if(in[c] >> vinicial[c]);
        else{
            vinicial[c] = -1;
            count_toC+=NumeroArquivos*caminho;
        }
      }
      cout << "Prints para nao precisar olhar os arquivos:" << endl;
      //vetor que conta quantos elementos foram lidos de cada arquivo durante as comparacoes -> usado na correção de bugs
      int cont[NumeroArquivos] = {};
      for(int c = 0; c < NumeroArquivos*MemoriaRAM*caminho;c++){
        if(count_toC != 0){
            c+= count_toC;
            count_toC = 0;
        }
        //funcao que retorna o indice de qual arquivo esta o ponteiro de menor valor
        int ind = procura_menor(vinicial, NumeroArquivos);
        if(vinicial[ind] == -1) break;
        cout << "Menor ta no indice:" << ind << endl;
        // coloca no vetor de saida o menor valor dos n arquivos
        out[part] <<  " " << vinicial[ind];
        cout << "///Aquivalor:"<<in[ind].peek();
        //flags para quando o arquivo tem menos arquivos do que a memoriaRam, para nao gerar bugs
        if((in[ind].peek() == -1)){
          //caso nao tenha mais numeros no arquivo ele ve se ele ocupou toda a memoriaRam daquela particao do arquivo
          //no caso de ter espaços vazios, ele desconta esses espaços do 'c' para executar um numero menor de vezes
          int dif = (MemoriaRAM - (cont[ind]+1));
          c += dif;
        }
        //incrementa o numero de entradas lidas em um arquivo
        cont[ind] += 1;
        //coloca flag quando ja leu toda a partição de um arquivo, ou seja todos as entradas de algum arquivo ja estão escritos na saida
        if(cont[ind] % tamParticao == 0 || in[ind].peek() == -1)
          vinicial[ind] = -1;
        else
          in[ind] >> vinicial[ind];
      }
      part++;
      //caso a particao seja o numero de arquivos, ela vira a particao zero e voltamos a analisar o primeiro arquivo
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
        interpolacao(tamParticao*NumeroArquivos ,result, caminho+1, maxCaminhos);
   }
  //Ou encerra e salva os dados no arquivos de saida.txt
   else{
      if(caminho % 2 == 0)
         encerra(1);
      else
         encerra(0);
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
  int i,pindex,pivo;
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
