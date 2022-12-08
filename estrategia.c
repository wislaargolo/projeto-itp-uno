#include "definitions.h"
#include "carta.h"
#include "mao.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int verificaNaipe(char *valorNaipe, Mao maoJogador, int *indice){ //verificaçao rapida da existencia de um naipe na mao
  for(int i=0; i<maoJogador.qtdDeCartas; i++){
    if(strcmp(maoJogador.cartasDoJogador[i].valorNaipe, valorNaipe)==0 && strcmp(maoJogador.cartasDoJogador[i].valorCarta, "A")!=0){ //guarda As
      if(indice!=NULL){
        *indice = i;
      }
      return 1;
    }
  }
  return 0;
}
          
int verificaValor(char *valorCarta, Mao maoJogador, int *indice){ //verificaçao rapida da existencia de um valor na mao
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

char *valorRaro(Carta baralho[108]){ //escolhe valor que foi mais jogado e tem menor chance de estar na mao de algum bot
  char* listaValor[12] = {"2", "3", "4","5","6","7","8","9","10","V","D","R"}; 
  int qtdValor[12] = {0};
  int menor, menorIndice = 0;
   
   for(int i=0; i<12; i++){
      for(int j=0; j<108; j++){
          if(strcmp(listaValor[i], baralho[j].valorCarta)==0){
            qtdValor[i] += 1;
          }
      }
   }

  menor = qtdValor[0];

  for(int i=0; i<12; i++){
    if(qtdValor[i] < menor){
      menor = qtdValor[i];
      menorIndice = i;
    }
  }

  return listaValor[menorIndice];
}

char *naipeRaro(Carta baralho[108]){ //escolhe naipe que foi mais jogado e tem menor chance de estar na mao de outro bot
  char *listaNaipes[] = {"♥","♦","♣","♠"};
  int qtdNaipe[4] = {0};
  int menor, menorIndice = 0;
   
   for(int i=0; i<4; i++){
      for(int j=0; j<108; j++){
          if(strcmp(listaNaipes[i], baralho[j].valorNaipe)==0){
            qtdNaipe[i] += 1;
          }
      }
   }

  menor = qtdNaipe[0];

  for(int i=0; i<4; i++){
    if(qtdNaipe[i] < menor){
      menor = qtdNaipe[i];
      menorIndice = i;
    }
  }

  return listaNaipes[menorIndice];
}


int frequenciaBaralho(Jogador *bot, Carta baralho[108], Carta c, int* indice){ //verifica frequencia de valor/naipe nas cartas que restam do baralho
  char valorBaralho[MAX_LINE];
  char naipeBaralho[MAX_LINE];

  strcpy(naipeBaralho, naipeRaro(baralho)); //guarda o naipe mais frequente da mao
  naipeBaralho[strlen(naipeBaralho)] = '\0';

  strcpy(valorBaralho, valorRaro(baralho)); //guarda o valor mais frequente da mao
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

  if(verificaValor("C", bot->maoDoJogador, &indice)){ 
    return indice;
  }else if(verificaEspecial(c, bot, &indice)){
    debug("Entou especial");
    return indice;
  }else if(frequenciaBaralho(bot, baralho, c, &indice)){ //verifica se é possível jogar o valor/naipe menos frequentes no baralho
    debug("Entou baralho");
    return indice; 
  }else if(frequenciaMao(bot, c, &indice)){ //verifica se é possível jogar o  valor/naipe mais frequente da mao do bot
   debug("Entou baralho");
    return indice;
  }else if(verificaNaipe(c.valorNaipe, bot->maoDoJogador, &indice)){
    debug("Entou naipe");
    return indice;
  }else if(verificaValor(c.valorCarta, bot->maoDoJogador, &indice)){ //mais dificil ter valor do que naipe na mao
    return indice;
  }else if(verificaValor("A", bot->maoDoJogador, &indice)){ //guarda o As
    return indice;
  } 
}