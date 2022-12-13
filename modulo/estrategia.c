#include "definitions.h"
#include "carta.h"
#include "mao.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Verifica se existe alguma carta com determinado naipe na mão do jogador, exceto o As*/
int verificaNaipe(char *valorNaipe, Mao maoJogador, int *indice){ 
  for(int i=0; i<maoJogador.qtdDeCartas; i++){
    if(strcmp(maoJogador.cartasDoJogador[i].valorNaipe, valorNaipe)==0 && strcmp(maoJogador.cartasDoJogador[i].valorCarta, "A")!=0){ 
      if(indice!=NULL){
        *indice = i; //salva o indice da carta encontrada
      }
      return 1;
    }
  }
  return 0;
}

/*Verifica se existe alguma carta com determinado valor na mão do jogador, exceto o As*/          
int verificaValor(char *valorCarta, Mao maoJogador, int *indice){ 
  for(int i=0; i<maoJogador.qtdDeCartas; i++){
    if(strcmp(maoJogador.cartasDoJogador[i].valorCarta, valorCarta)==0){
      if(indice!=NULL){
        *indice = i; //salva o indice da carta encontrada
      }
      return 1;
    }
  }
  return 0;
}

/*Retorna o naipe que aparece com maior frequência na mao do bot*/  
char *naipeFrequente(Jogador *bot){ 
  char *listaNaipes[] = {"♥","♦","♣","♠"};
  int qtdNaipe[4] = {0};
  int maior, maiorIndice;

  maior = maiorIndice = 0;
   
   //conta quantas cartas de cada naipe existem na mao do jogador
  for(int i=0; i<4; i++){
    for(int j=0; j<bot->maoDoJogador.qtdDeCartas; j++){ 
      if(strcmp(listaNaipes[i], bot->maoDoJogador.cartasDoJogador[j].valorNaipe)==0){
        qtdNaipe[i] += 1;
      }
    }
  }

  //escolhe qual naipe aparece com maior frequência
  for(int i=0; i<4; i++){
    if(qtdNaipe[i] > maior){
      maior = qtdNaipe[i];
      maiorIndice = i;
    }
  }

  return listaNaipes[maiorIndice];
}

/*Retorna o valor (exceto As) que aparece com maior frequência na mao do bot*/
char *valorFrequente(Jogador *bot){
  char* listaValor[12] = {"2", "3", "4","5","6","7","8","9","10","V","D","R"}; 
  int qtdValor[12] = {0};
  int maior, maiorIndice;

  maior = maiorIndice = 0;
   
  //conta quantas cartas de cada valor existem na mao do jogador 
  for(int i=0; i<12; i++){
    for(int j=0; j<bot->maoDoJogador.qtdDeCartas; j++){
      if(strcmp(listaValor[i], bot->maoDoJogador.cartasDoJogador[j].valorCarta)==0){
        qtdValor[i] += 1;
      }
    }
  }

  //escolhe qual valor aparece com maior frequência
  for(int i=0; i<12; i++){
    if(qtdValor[i] > maior){
      maior = qtdValor[i];
      maiorIndice = i;
    }
  }

  return listaValor[maiorIndice];
}

/*Retorna o valor mais descartado (menos frequente no baralho da partida)*/
char *valorRaro(Carta baralho[108]){
  char* listaValor[12] = {"2", "3", "4","5","6","7","8","9","10","V","D","R"}; 
  int qtdValor[12] = {0};
  int menor, menorIndice = 0;

  //conta quantas cartas de cada valor existem no baralho que resta no jogo 
  for(int i=0; i<12; i++){
    for(int j=0; j<108; j++){
        if(strcmp(listaValor[i], baralho[j].valorCarta)==0){
          qtdValor[i] += 1;
        }
    }
  }

  menor = qtdValor[0];

  //escolhe qual valor aparece com menor frequência
  for(int i=0; i<12; i++){
    if(qtdValor[i] < menor){
      menor = qtdValor[i];
      menorIndice = i;
    }
  }

  return listaValor[menorIndice];
}

/*Retorna o naipe mais descartado (menos frequente no baralho da partida)*/
char *naipeRaro(Carta baralho[108]){ 
  char *listaNaipes[] = {"♥","♦","♣","♠"};
  int qtdNaipe[4] = {0};
  int menor, menorIndice = 0;

  //conta quantas cartas de cada naipe existem no baralho que resta no jogo 
  for(int i=0; i<4; i++){
    for(int j=0; j<108; j++){
        if(strcmp(listaNaipes[i], baralho[j].valorNaipe)==0){
          qtdNaipe[i] += 1;
        }
    }
  }

  menor = qtdNaipe[0];

  //escolhe qual naipe aparece com menor frequência
  for(int i=0; i<4; i++){
    if(qtdNaipe[i] < menor){
      menor = qtdNaipe[i];
      menorIndice = i;
    }
  }

  return listaNaipes[menorIndice];
}

/*Verifica se bot pode jogar valor ou naipe que aparecem com menor frequencia nas cartas que restam do baralho*/
int frequenciaBaralho(Jogador *bot, Carta baralho[108], Carta c, int* indice){ 
  char valorBaralho[MAX_LINE];
  char naipeBaralho[MAX_LINE];

  strcpy(naipeBaralho, naipeRaro(baralho)); //guarda o naipe menos frequente do baralho restante
  naipeBaralho[strlen(naipeBaralho)] = '\0';

  strcpy(valorBaralho, valorRaro(baralho)); //guarda o naipe menos frequente do baralho restante
  valorBaralho[strlen(valorBaralho)] = '\0';

  //verifica valor antes de naipe, pois é mais díficil que os outros bots tenham um determinado valor do que naipe
  if(strcmp(c.valorCarta, valorBaralho) == 0 && verificaValor(c.valorCarta, bot->maoDoJogador, indice)){  
    return 1;
  }else if(strcmp(c.valorNaipe, naipeBaralho) == 0 && verificaNaipe(c.valorNaipe, bot->maoDoJogador, indice)){
    return 1;
  }

  return 0;
}

/*Verifica se bot pode jogar valor ou naipe que aparecem com maior frequencia na mão*/
int frequenciaMao(Jogador *bot, Carta c, int* indice){
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

/*Verifica se bot pode jogar especiais, exceto A e C*/
int verificaEspecial(Carta c, Jogador *bot, int *indice){ 
Carta aux;
  for(int i=0; i<bot->maoDoJogador.qtdDeCartas; i++){
    aux = bot->maoDoJogador.cartasDoJogador[i]; //salva em aux as cartas da mao do jogador
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

/*Seleciona qual a melhor carta a ser jogada*/
int selecionaCarta(Carta c, Jogador *bot, Carta baralho[108]){ 
  int indice = 0; //indice da carta que será jogada

  if(verificaValor("C", bot->maoDoJogador, &indice)){ 
    return indice; //verifica se pode jogar C e retorna indice da carta
  }else if(verificaEspecial(c, bot, &indice)){
    return indice; //verifica se pode jogar especial e retorna indice da carta
  }else if(frequenciaBaralho(bot, baralho, c, &indice)){
    return indice; //verifica se é possível jogar o valor/naipe menos frequentes no baralho restante
  }else if(frequenciaMao(bot, c, &indice)){ 
    return indice; //verifica se é possível jogar o valor/naipe mais frequente da mao do bot
  }else if(verificaNaipe(c.valorNaipe, bot->maoDoJogador, &indice)){
    return indice; //verifica se é possível jogar carta com mesmo naipe da carta da mesa
  }else if(verificaValor(c.valorCarta, bot->maoDoJogador, &indice)){
    return indice; //verifica se é possível jogar carta com mesmo valor da carta da mesa
  }else if(verificaValor("A", bot->maoDoJogador, &indice)){ 
    return indice; //verifica se pode jogar A (por ultimo para guardar cartas desse tipo)
  } 
}

/*Verifica se bot precisa comprar cartas e retorna a quantidade*/
int compraCartas(Carta c, char *naipe, int *especial, Jogador *bot){
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