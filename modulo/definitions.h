#ifndef DEF
#define DEF
#define MAX_LINE 100
#define MAX_ACTION 10
#define MAX_ID_SIZE 10

typedef struct {     // struct feita para representar uma unica carta
    char *valorCarta;
    char *valorNaipe;
}Carta;

typedef struct  {   // struct para guardar as cartas dos jogadores 
    Carta *cartasDoJogador;
    int qtdDeCartas;
}Mao;

typedef struct {   // representa um jogador 
    char id[MAX_ID_SIZE];
    Mao maoDoJogador;
}Jogador;

#endif