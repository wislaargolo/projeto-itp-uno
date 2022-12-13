
// Para gerar executavel use o comando -> make all <- 

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "definitions.h"
#include "debug.h"
#include "carta.h"
#include "mao.h"
#include "estrategia.h"
#include "auxi.h"

int main() {

    Carta totalDeCartas[108]; //guarda os dois baralhos utilizados na partida
    Carta pilhaSobMesa[115]; //guarda as cartas que vão sendo descartadas
    int contador = 0, especial = 0;

    char temp[MAX_LINE];  
    char my_id[MAX_ID_SIZE]; 

    Jogador botT; 
    Mao minhaMao;

    char complemento2[MAX_LINE];
    char auxNaipe[MAX_LINE]; //guarda o naipe atual da partida
    char acao[MAX_ACTION];
    char complemento[MAX_LINE];

    setbuf(stdin, NULL);  
    setbuf(stdout, NULL);  
    setbuf(stderr, NULL);
    srand(time(NULL));

   inicializaBaralho(totalDeCartas); //inicializa os dois baralhos com todas as cartas 
      
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

        // lê a jogada dos outros bots
        scanf(" %s %s", acao, complemento);
      
        //adiciona cartas descartadas na pilhaSobMesa
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
    Carta recebida; //carta que o bot terá que lidar na sua vez
 
    recebida = inicializaCarta(pilhaSobMesa[contador-1].valorCarta, pilhaSobMesa[contador-1].valorNaipe); //inicializa carta recebida

    cartasCompradas = compraCartas(recebida, auxNaipe, &especial, &botT); //verifica se o jogador terá que comprar cartas e retorna a qtd

    strcpy(recebida.valorNaipe, auxNaipe); //atualizaNaipe de carta recebida;

    if(!cartasCompradas){
        int indice = selecionaCarta(recebida, &botT, totalDeCartas);  //recebe carta que vai jogar

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