all: arBonito

arBonito: arBonito.cpp include/BST.h include/nodoT.h include/listaS.h include/nodoS.h
	g++ -Wall -g -o arBonito -I ./include arBonito.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm arBonito
