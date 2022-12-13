#include "definitions.h"
#include "carta.h"
#include "mao.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int verificaNaipe(char *valorNaipe, Mao maoJogador, int *indice){ //verifica se o bot possui alguma carta de determinado naipe e salva indice
  for(int i=0; i<maoJogador.qtdDeCartas; i++){
    if(strcmp(maoJogador.cartasDoJogador[i].valorNaipe, valorNaipe)==0 && strcmp(maoJogador.cartasDoJogador[i].valorCarta, "A")!=0){ //nao verifica As, pra guardar cartas desse tipo
      if(indice!=NULL){
        *indice = i;
      }
      return 1;
    }
  }
  return 0;
}
          
int verificaValor(char *valorCarta, Mao maoJogador, int *indice){ //verifica se o bot possui alguma carta de determinado valor e salva indice
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

char *naipeFrequente(Jogador *bot){ //escolhe naipe que aparece com maior frequencia na mao
  char *listaNaipes[] = {"♥","♦","♣","♠"};
  int qtdNaipe[4] = {0};
  int maior, maiorIndice;

  maior = maiorIndice = 0;
   
   for(int i=0; i<4; i++){
      for(int j=0; j<bot->maoDoJogador.qtdDeCartas; j++){ //conta quantas cartas da mao tem determinado naipe
        if(strcmp(listaNaipes[i], bot->maoDoJogador.cartasDoJogador[j].valorNaipe)==0){
          qtdNaipe[i] += 1;
        }
      }
   }

  for(int i=0; i<4; i++){
    if(qtdNaipe[i] > maior){
      maior = qtdNaipe[i]; //escolhe o naipe que mais aparece
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
      for(int j=0; j<bot->maoDoJogador.qtdDeCartas; j++){ //conta quantas cartas da mao tem determinado valor
        if(strcmp(listaValor[i], bot->maoDoJogador.cartasDoJogador[j].valorCarta)==0){
          qtdValor[i] += 1;
        }
      }
   }

  for(int i=0; i<12; i++){
    if(qtdValor[i] > maior){
      maior = qtdValor[i]; //escolhe qual valor aparece mais
      maiorIndice = i;
    }
  }

  return listaValor[maiorIndice];
}

char *valorRaro(Carta baralho[108]){ //verifica qual valor é mais raro no baralho total, ie, valor que tem menor chance de estar presente na mao dos outros bots
  char* listaValor[12] = {"2", "3", "4","5","6","7","8","9","10","V","D","R"}; 
  int qtdValor[12] = {0};
  int menor, menorIndice = 0;
   
   for(int i=0; i<12; i++){
      for(int j=0; j<108; j++){
          if(strcmp(listaValor[i], baralho[j].valorCarta)==0){ //conta quantas cartas de determinado valor existem no baralho restante
            qtdValor[i] += 1;
          }
      }
   }

  menor = qtdValor[0];

  for(int i=0; i<12; i++){
    if(qtdValor[i] < menor){
      menor = qtdValor[i]; //escolhe o valor que menos aparece
      menorIndice = i;
    }
  }

  return listaValor[menorIndice];
}

char *naipeRaro(Carta baralho[108]){ //verifica qual naipe é mais raro no baralho total, ie, naipe que tem menor chance de estar presente na mao dos outros bots
  char *listaNaipes[] = {"♥","♦","♣","♠"};
  int qtdNaipe[4] = {0};
  int menor, menorIndice = 0;
   
   for(int i=0; i<4; i++){
      for(int j=0; j<108; j++){
          if(strcmp(listaNaipes[i], baralho[j].valorNaipe)==0){ //conta quantas cartas de determinado naipe existem no baralho restante
            qtdNaipe[i] += 1;
          }
      }
   }

  menor = qtdNaipe[0];

  for(int i=0; i<4; i++){
    if(qtdNaipe[i] < menor){
      menor = qtdNaipe[i]; //escolhe naipe que menos aparece
      menorIndice = i;
    }
  }

  return listaNaipes[menorIndice];
}


int frequenciaBaralho(Jogador *bot, Carta baralho[108], Carta c, int* indice){ //verifica frequencia de valor/naipe nas cartas que restam do baralho
  char valorBaralho[MAX_LINE];
  char naipeBaralho[MAX_LINE];

  strcpy(naipeBaralho, naipeRaro(baralho)); //guarda o naipe menos frequente no baralho restante
  naipeBaralho[strlen(naipeBaralho)] = '\0';

  strcpy(valorBaralho, valorRaro(baralho)); //guarda o valor menos frequente no baralho restante
  valorBaralho[strlen(valorBaralho)] = '\0';

  if(strcmp(c.valorCarta, valorBaralho) == 0 && verificaValor(c.valorCarta, bot->maoDoJogador, indice)){ //verifica valor antes de naipe, pois é mais díficil que os outros bots tenham um determinado valor doq naipe 
    return 1;
  }else if(strcmp(c.valorNaipe, naipeBaralho) == 0 && verificaNaipe(c.valorNaipe, bot->maoDoJogador, indice)){
    return 1;
  }

  return 0;
}

int frequenciaMao(Jogador *bot, Carta c, int* indice){ //verifica frequencia de valor/naipe na mao
  char valorMao[MAX_LINE];
  char naipeMao[MAX_LINE];

  strcpy(naipeMao, naipeFrequente(bot)); //guarda o naipe mais frequente da mao
  naipeMao[strlen(naipeMao)] = '\0';

  strcpy(valorMao, valorFrequente(bot)); //guarda o valor mais frequente da mao
  valorMao[strlen(valorMao)] = '\0';

  if(strcmp(c.valorNaipe, naipeMao) == 0 && verificaNaipe(c.valorNaipe, bot->maoDoJogador, indice)){
    return 1;
  }else if(strcmp(c.valorCarta, valorMao) == 0 && verificaValor(c.valorCarta, bot->maoDoJogador, indice)){
    return 1;
  }

  return 0;

}

int verificaEspecial(Carta c, Jogador *bot, int *indice){ //verifica se é possível jogar especiais, exceto A e C
Carta aux;
  for(int i=0; i<bot->maoDoJogador.qtdDeCartas; i++){
    aux = bot->maoDoJogador.cartasDoJogador[i];
    if(strcmp(aux.valorCarta, "V")==0 && (strcmp(c.valorCarta, "V") == 0 || strcmp(c.valorNaipe, aux.valorNaipe) == 0)){
      (*indice) = i;
      return 1;
    }else if(strcmp(aux.valorCarta, "R")==0 && (strcmp(c.valorCarta, "R") == 0 || strcmp(c.valorNaipe, aux.valorNaipe) == 0)){
      (*indice) = i;
      return 1;
    }else if(strcmp(aux.valorCarta, "D")==0 && (strcmp(c.valorCarta, "D") == 0 || strcmp(c.valorNaipe, aux.valorNaipe) == 0)){
      (*indice) = i;
      return 1;
    }
  }
  return 0;
}

int selecionaCarta(Carta c, Jogador *bot, Carta baralho[108]){ //encontra a carta a ser jogada (AINDA VOU MUDAR VARIAS COISAS)
  int indice = 0; //indice da carta que será jogada

  if(verificaValor("C", bot->maoDoJogador, &indice)){ //verifica se possui carta com valor C
    return indice;
  }else if(verificaEspecial(c, bot, &indice)){ //verifica se possui especial
    return indice;
  }else if(frequenciaBaralho(bot, baralho, c, &indice)){ //verifica se é possível jogar o valor/naipe menos frequentes no baralho
    return indice; 
  }else if(frequenciaMao(bot, c, &indice)){ //verifica se é possível jogar o valor/naipe mais frequente da mao do bot
    return indice;
  }else if(verificaNaipe(c.valorNaipe, bot->maoDoJogador, &indice)){ //verifica se o jogador possui carta com o naipe da mesa
    return indice;
  }else if(verificaValor(c.valorCarta, bot->maoDoJogador, &indice)){ //verifica se o jogador possui carta com valor da mesa, pois mais dificil ter valor do que naipe na mao
    return indice;
  }else if(verificaValor("A", bot->maoDoJogador, &indice)){ //verifica se possui As e como é o ultimo if, ele guarda o As
    return indice;
  } 
}

int compraCartas(Carta c, char *naipe, int *especial, Jogador *bot){ //verifica se o bot irá precisar comprar e a quantidade a partir da carta da mesa e valor de especial
  if(strcmp(c.valorCarta, "C")==0 && (*especial) == 1){
    (*especial) = 0;
    return 4;
  }else if(strcmp(c.valorCarta, "V")==0 && (*especial) == 1){
    (*especial) = 0;
    return 2;
  }else if(!verificaNaipe(naipe, bot->maoDoJogador, NULL) &&
           !verificaValor(c.valorCarta, bot->maoDoJogador, NULL) &&
           !verificaValor("C", bot->maoDoJogador, NULL) &&
           !verificaValor("A", bot->maoDoJogador, NULL)){
    return 1;
  }
  return 0;
}