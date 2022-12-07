/*  Local para colocar as variaveis :)
    Jogador *jogadores;
    Mao minhaMao;
    Carta cartaInicial;
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "definitions.h"
#include "debug.h"
#include "carta.h"
#include "mao.h"

void retornaFrase(){
  const char *listaFrases[] = {"Poxa vida!", 
                               "Que nervoso...",
                               "UNO!! Brincadeira...",
                               "Alguem ta querendo umas cartas emprestadas?",
                               "TRUCO! Eh mentira...",
                               "Aff...queria estar vendo a copa"};

  int indice = rand()%6;
  int chance = rand()%100;

  if(chance<=30){
    printf("SAY %s\n", listaFrases[indice]);
  }
}

int verificaNaipe(char *valorNaipe, Mao maoJogador, int *indice){
  for(int i=0; i<maoJogador.qtdDeCartas; i++){
    if(strcmp(maoJogador.cartasDoJogador[i].valorNaipe, valorNaipe)==0){
      if(indice!=NULL){
        *indice = i;
      }
      return 1;
    }
  }
  return 0;
}
          
int verificaValor(char *valorCarta, Mao maoJogador, int *indice){
  for(int i=0; i<maoJogador.qtdDeCartas; i++){
    if(strcmp(maoJogador.cartasDoJogador[i].valorCarta, valorCarta)==0){
      if(indice!=NULL){
        *indice = i;
      }
      return 1;
    }
  }
  return 0;
}

int verificaEspecial(char *valorCarta, Mao maoJogador, int *indice){ //exceto As
  if(verificaValor("C", maoJogador, indice)){
    return 1;
  }else if(strcmp(valorCarta, "V") == 0 && verificaValor("V", maoJogador, indice)){
    return 1;
  }else if(strcmp(valorCarta, "R") == 0 && verificaValor("R", maoJogador, indice)){
    return 1;
  }else if(strcmp(valorCarta, "D") == 0 && verificaValor("D", maoJogador, indice)){
    return 1;
  }
  return 0;
}

char *naipeFrequente(Jogador *bot){ //escolhe naipe que aparece com maior frequencia na mao
  char *listaNaipes[] = {"♥","♦","♣","♠"};
  int qtdNaipe[4] = {0};
  int maior, maiorIndice;

  maior = maiorIndice = 0;
   
   for(int i=0; i<4; i++){
      for(int j=0; j<bot->maoDoJogador.qtdDeCartas; j++){
        if(strcmp(listaNaipes[i], bot->maoDoJogador.cartasDoJogador[j].valorNaipe)==0){
          qtdNaipe[i] += 1;
        }
      }
   }

  for(int i=0; i<4; i++){
    if(qtdNaipe[i] > maior){
      maior = qtdNaipe[i];
      maiorIndice = i;
    }
  }

  return listaNaipes[maiorIndice];
}

char *valorFrequente(Jogador *bot){ //escolhe valor que aparece com maior frequencia na mao, exceto As
  char* listaValor[12] = {"2", "3", "4","5","6","7","8","9","10","V","D","R"}; 
  int qtdValor[12] = {0};
  int maior, maiorIndice;

  maior = maiorIndice = 0;
   
   for(int i=0; i<12; i++){
      for(int j=0; j<bot->maoDoJogador.qtdDeCartas; j++){
        if(strcmp(listaValor[i], bot->maoDoJogador.cartasDoJogador[j].valorCarta)==0){
          qtdValor[i] += 1;
        }
      }
   }

  for(int i=0; i<12; i++){
    if(qtdValor[i] > maior){
      maior = qtdValor[i];
      maiorIndice = i;
    }
  }

  return listaValor[maiorIndice];
}

int selecionaCarta(Carta c, Jogador *bot){ //encontra a carta a ser jogada (AINDA VOU MUDAR VARIAS COISAS)
  int indice = 0; //indice da carta que será jogada
  char naipe[MAX_LINE];
  char valor[MAX_LINE];

  strcpy(naipe, naipeFrequente(bot)); //guarda o naipe mais frequente da mao
  naipe[strlen(naipe)] = '\0';

  strcpy(valor, valorFrequente(bot)); //guarda o valor mais frequente da mao
  valor[strlen(valor)] = '\0';

  if(verificaEspecial(c.valorCarta, bot->maoDoJogador, &indice)){ //verifica se pode jogar alguma carta de açao, exceto As
    return indice;
  }else if(strcmp(c.valorNaipe, naipe) == 0 && verificaNaipe(c.valorNaipe, bot->maoDoJogador, &indice)){
    return indice;
  }else if(strcmp(c.valorCarta, valor) == 0 && verificaValor(c.valorCarta, bot->maoDoJogador, &indice)){
    return indice;
  }else if(verificaNaipe(c.valorNaipe, bot->maoDoJogador, &indice)){
    return indice;
  }else if(verificaValor(c.valorCarta, bot->maoDoJogador, &indice)){ //mais dificil ter valor do que naipe na mao
    return indice;
  }else if(verificaValor("A", bot->maoDoJogador, &indice)){
    return indice;
  } 
}

int compraCartas(Carta c, char *naipe, int *especial, Jogador *bot){
  if(strcmp(c.valorCarta, "C")==0 && (*especial) == 1){
    (*especial) = 0;
    return 4;
  }else if(strcmp(c.valorCarta, "V")==0 && (*especial) == 1){
    (*especial) = 0;
    return 2;
  }else if(!verificaNaipe(naipe, bot->maoDoJogador, NULL) &&
           !verificaValor(c.valorCarta, bot->maoDoJogador, NULL) &&
           !verificaEspecial(c.valorCarta, bot->maoDoJogador, NULL) &&
           !verificaValor("A", bot->maoDoJogador, NULL)){
    return 1;
  }
  return 0;
}

void recebeCartas(int qtdCartas, Jogador *bot){
    char cartas[qtdCartas][MAX_LINE];
    Carta c;

    for(int i=0; i<qtdCartas; i++){
        scanf(" %s\n", cartas[i]);
        c = gerarCarta(cartas[i]);
        adicionaCarta(bot, c);
    }
}

Carta acaoDescarta(Jogador *bot, int indice, char *auxNaipe){
  char naipe[MAX_LINE];
  Carta c = bot->maoDoJogador.cartasDoJogador[indice];
  strcpy(auxNaipe, c.valorNaipe);

  if(strcmp(c.valorCarta, "A") == 0 || strcmp(c.valorCarta, "C") == 0){
      strcpy(naipe, naipeFrequente(bot));
      naipe[strlen(naipe)] = '\0';
      printf("DISCARD %s%s %s\n", c.valorCarta, c.valorNaipe, naipe);
      strcpy(auxNaipe, naipe);
    }else{
      printf("DISCARD %s%s\n", c.valorCarta, c.valorNaipe);
    }
    auxNaipe[strlen(auxNaipe)] = '\0';

    retiraCarta(bot, indice); //retira a carta da mao do jogador
    return c;
}

void acaoCompra(int qtdCartas, Jogador *bot,Carta totalDeCartas[108]){
  char cartas[qtdCartas][MAX_LINE];
  Carta c;

  printf("BUY %d\n", qtdCartas);

  for(int i=0; i<qtdCartas; i++){ //recebe cartas do gerenciador e add na mao
    scanf(" %s\n", cartas[i]);
    c = gerarCarta(cartas[i]);
    //Acompanha total de cartas
    acompanhaTotal(totalDeCartas, c);
    adicionaCarta(bot, c);
  }
}

int main() {

    Carta totalDeCartas[108];//eram 54 cm 1 baralho só
    Carta pilhaSobMesa[115];
    int contador = 0, especial = 0;

    char temp[MAX_LINE];  
    char my_id[MAX_ID_SIZE]; 

    Jogador botT;
    Mao minhaMao;

    char complemento2[MAX_LINE];
    char auxNaipe[MAX_LINE]; //guarda o naipe atual da partida
    char id[MAX_ID_SIZE];
    char acao[MAX_ACTION];
    char complemento[MAX_LINE];

    setbuf(stdin, NULL);  
    setbuf(stdout, NULL);  
    setbuf(stderr, NULL);
    srand(time(NULL));

   inicializaBaralho(totalDeCartas);
      
    // Ler quais são os jogadores
    scanf("PLAYERS %[^\n]\n", temp);

    // Id do nosso bot
    scanf("YOU %s\n", my_id);
    strcpy(botT.id,my_id);

    // A mão recebida
    scanf("HAND %[^\n]\n", temp);
    botT.maoDoJogador = maoInicial(temp,totalDeCartas); 
  
    // carta inicial 
    scanf("TABLE %s\n", temp);
    pilhaSobMesa[contador++] = gerarCarta(temp); //atualiza pilha

    strcpy(auxNaipe, pilhaSobMesa[contador-1].valorNaipe); //atualiza auxNaipe
    auxNaipe[strlen(auxNaipe)] = '\0';

    atualizaEspecial(pilhaSobMesa[contador-1], &especial);

    // Fica esperando a vez do seu bot jogar

    while(1) {

    do {

        // ler a jogada do bot anterior a vc 

        scanf(" %s %s", acao, complemento);
      
        //aqui é a adição das cartas na pilhaSobMesa
        if(strcmp(acao, "DISCARD") == 0){
          pilhaSobMesa[contador++] = gerarCarta(complemento);
          strcpy(auxNaipe, pilhaSobMesa[contador-1].valorNaipe);
          //Retira a carta descartada do total de cartas
          acompanhaTotal(totalDeCartas, pilhaSobMesa[contador-1]);

          if(strstr(pilhaSobMesa[contador-1].valorCarta,"A")!= NULL || strstr(pilhaSobMesa[contador-1].valorCarta,"C")!= NULL){
            scanf(" %s", complemento2);
            strcpy(auxNaipe, complemento2);
          }
           atualizaEspecial(pilhaSobMesa[contador-1], &especial);
           auxNaipe[strlen(auxNaipe)] = '\0';
        }

        if(especial == 1 && strcmp(acao, "BUY")==0 && (strcmp(complemento, "2")==0 || 
           strcmp(complemento, "4")==0) || strcmp(acao, "INVALID ACTION") == 0){ //verifica se algum bot respondeu a especial
          especial = 0;
        }

    } while (strcmp(acao, "TURN") || strcmp(complemento, my_id));


    //Vez do bot
    
    int cartasCompradas = 0;
    Carta recebida;

    for(int i=0; i<botT.maoDoJogador.qtdDeCartas; i++){
      debug(botT.maoDoJogador.cartasDoJogador[i].valorCarta);
    }
 
    recebida = inicializaCarta(pilhaSobMesa[contador-1].valorCarta, pilhaSobMesa[contador-1].valorNaipe); //inicializa carta recebida

    cartasCompradas = compraCartas(recebida, auxNaipe, &especial, &botT); //verifica se o jogador terá que comprar cartas e retorna a qtd

    strcpy(recebida.valorNaipe, auxNaipe); //atualizaNaipe de carta recebida;

    if(!cartasCompradas){
        int indice = selecionaCarta(recebida, &botT);  //recebe carta que vai jogar

        pilhaSobMesa[contador++] = acaoDescarta(&botT, indice, auxNaipe); //envia açao ao gerenciador, atualiza naipe e mao do jogador
        atualizaEspecial(pilhaSobMesa[contador-1], &especial); //atualiza o status de especial quando bot joga
    }else{
        retornaFrase();
        acaoCompra(cartasCompradas, &botT,totalDeCartas); //compra cartas e atualiza mao do jogador
    }

  }

  for(int i=0; i<contador; i++){
    desalocaCarta(&pilhaSobMesa[i]);
  }

  free(minhaMao.cartasDoJogador);

    return 0;
}