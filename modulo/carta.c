#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "debug.h"
#include "definitions.h"

Carta gerarCarta(char *mensagem){     // transforma uma string com valor e nipe em uma variavel do tipo Carta
    Carta saida;
    int tamanho = strlen(mensagem);

    if (mensagem[0] == '1' && mensagem[1] == '0')
    {
        char valor[3] = "10";
        saida.valorCarta = malloc(sizeof(char) * 3);
        strcpy(saida.valorCarta, valor);
        saida.valorNaipe = malloc(sizeof(char) * tamanho);

        for (int i = 2; i < tamanho; i++){
            saida.valorNaipe[i-2] = mensagem[i];
        }
    }else{
        saida.valorCarta = malloc(sizeof(char)*2);
        saida.valorCarta[0] = mensagem[0];
        saida.valorCarta[1] = '\0';
        saida.valorNaipe = malloc(sizeof(char) * tamanho);

        for (int i = 1; i < tamanho; i++){
            saida.valorNaipe[i-1] = mensagem[i];
        }
    }

    return saida;
}

/* Essa função acompanha quais cartas da totalidade de cartas dos baralhos ainda não foram jogadas pelos outros jogadores ou compradas pelo nosso bot, utilizada na estrategia */
void acompanhaTotal(Carta totalDeCartas[108], Carta pedaco){
  int i = 0,j=0;
  for(i=0;i < 108; i++){
    if(strcmp(pedaco.valorCarta, totalDeCartas[i].valorCarta) == 0 && strcmp(pedaco.valorNaipe, totalDeCartas[i].valorNaipe) == 0){
      for(j = i; j < 107; j++){
        totalDeCartas[j].valorCarta = totalDeCartas[j+1].valorCarta;
        totalDeCartas[j].valorNaipe = totalDeCartas[j+1].valorNaipe;
      }
      if(j == 107){
        totalDeCartas[j].valorCarta = "";
        totalDeCartas[j].valorNaipe = "";
      }
      break;
    }
  }
}

/* Essa função preenche o vetor totalDecartas com a totalidade de cartas presentes nos 2 baralhos utilizados */
void inicializaBaralho(Carta totalDeCartas[108]){
  char* naipes[4] = {"♥", "♦", "♣", "♠"}; 
  char* valor[13] = {"A", "2", "3", "4","5","6","7","8","9","10","V","D","R"}; 

  int k = 0,q = 0;
  
  
    for(int i = 0; i < 14; i++){
        if(k < 104){
          for(int j=0; j < 4; j++){
            for(q = 0; q < 2; q++){
              totalDeCartas[k].valorCarta = valor[i];
              totalDeCartas[k].valorNaipe = naipes[j];
              k++;
            }
          }
        }
        if(k > 103 && k < 108){
          for(int j=0; j <= 2; j++){
            for(q = 0; q < 2; q++){
              totalDeCartas[k].valorCarta = "C";
              totalDeCartas[k].valorNaipe = naipes[j];
              k++;
            }
            j++;
          }
        }
      }
}

void desalocaCarta(Carta *c){
    free((*c).valorCarta);
    free((*c).valorNaipe);
}

Carta inicializaCarta(char *valorCarta, char *valorNaipe){
  char dados[MAX_LINE];  
  Carta c;

  strcpy(dados, valorCarta);
  strcat(dados, valorNaipe);
  c = gerarCarta(dados);

  return c;
}

void atualizaEspecial(Carta c, int *especial){
  if(strcmp(c.valorCarta, "C") == 0 || strcmp(c.valorCarta, "V") == 0){
    (*especial) = 1;
  }
}