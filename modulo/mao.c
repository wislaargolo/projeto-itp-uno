#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "debug.h"
#include "definitions.h"
#include "carta.h"

Mao maoInicial(char *mensagem, Carta totalDeCartas[108]){ // Faz a leitura das cartas iniciais do bot, salva na mao e retorna a mao
    Mao saida;
    Carta aux;
    char *pedaco, vetor[10][10];
    int tamanho, cont = 0;

    pedaco = strtok(mensagem, " ");
    while(pedaco != NULL){
        strcpy(vetor[cont],pedaco);
        cont += 1;
        pedaco = strtok(NULL, " ");
    }

    saida.cartasDoJogador = malloc(sizeof(Carta) * (cont-2));

    for (int i = 1; i < cont - 1; i++) {
        tamanho = strlen(vetor[i]);
        aux = gerarCarta(vetor[i]);
        //Retira do cartaTotais o 1º baralho do nosso bot
        acompanhaTotal(totalDeCartas, aux);
        saida.cartasDoJogador[i-1].valorCarta = malloc(sizeof(char) * 3);
        saida.cartasDoJogador[i-1].valorNaipe = malloc(sizeof(char) * 4);
        strcpy(saida.cartasDoJogador[i-1].valorCarta, aux.valorCarta);
        strcpy(saida.cartasDoJogador[i-1].valorNaipe, aux.valorNaipe);
        saida.qtdDeCartas = i;
    }
    free(aux.valorNaipe);
    free(aux.valorCarta);
    return saida;
}

void retiraCarta(Jogador *bot, int indice){ //função que retira carta da mão quando essa é jogada (atualiza mão)
  Carta *aux; //cria um novo vetor de cartas
  int count = 0;
  int qtdCartas = bot->maoDoJogador.qtdDeCartas;

  bot->maoDoJogador.cartasDoJogador[indice].valorCarta = NULL; 
  bot->maoDoJogador.cartasDoJogador[indice].valorNaipe = NULL;
  aux = malloc(sizeof(Carta)*(qtdCartas-1)); //aloca o espaço pra esse vetor como a qtd de cartas atuais - 1

  for(int i=0; i<qtdCartas; i++){
    if(bot->maoDoJogador.cartasDoJogador[i].valorCarta != NULL && bot->maoDoJogador.cartasDoJogador[i].valorNaipe != NULL){
      aux[count++] = bot->maoDoJogador.cartasDoJogador[i]; //atualiza vetor com cartas antigas, exceto a retirada
    }
  }
  bot->maoDoJogador.qtdDeCartas +=-1;
  free(bot->maoDoJogador.cartasDoJogador);
  bot->maoDoJogador.cartasDoJogador = NULL;
  bot->maoDoJogador.cartasDoJogador = aux;
   
} 

void adicionaCarta(Jogador *bot, Carta c){ //adiciona uma carta a mao do bot
  int qtdAtual = bot->maoDoJogador.qtdDeCartas;
  bot->maoDoJogador.cartasDoJogador = realloc(bot->maoDoJogador.cartasDoJogador, sizeof(Carta)*(qtdAtual+1)); //aumenta o tamanho do vetor cartasDoJogador em +1
  bot->maoDoJogador.qtdDeCartas += 1; //aumenta qtd de cartas na mao do bot
  bot->maoDoJogador.cartasDoJogador[qtdAtual] = c; //adiciona carta a mao
}