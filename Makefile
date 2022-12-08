all: main.c auxi.o carta.o debug.o estrategia.o mao.o
	gcc main.c auxi.o carta.o debug.o estrategia.o mao.o -o bot_T