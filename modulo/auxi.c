#include "definitions.h"
#include "carta.h"
#include "estrategia.h"
#include "mao.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*Retorna frases em 30% das vezes em que é chamada */
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

/*Envia a ação DISCARD para o gerenciador e atualiza a mao do bot*/
Carta acaoDescarta(Jogador *bot, int indice, char *auxNaipe){
  char naipe[MAX_LINE];
  Carta c = bot->maoDoJogador.cartasDoJogador[indice];
  strcpy(auxNaipe, c.valorNaipe);

  //Se carta descartada for C ou A, envia outro complemento 
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

/*Envia ação BUY para o gerenciador e atualiza a mão do jogador com as cartas compradas*/
void acaoCompra(int qtdCartas, Jogador *bot,Carta totalDeCartas[108]){
  char cartas[qtdCartas][MAX_LINE];
  Carta c;

  printf("BUY %d\n", qtdCartas);

  //Recebe cartas do gerenciador e adiciona uma por uma na mão
  for(int i=0; i<qtdCartas; i++){ 
    scanf(" %s\n", cartas[i]);
    c = gerarCarta(cartas[i]);
    acompanhaTotal(totalDeCartas, c);
    adicionaCarta(bot, c);
  }
}