#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#define clear() printf("\033[H\033[J")
//TERMIOS
/*DUAS STRUCTS QUE DEFINEM AS CONFIGURACOES DO TERMINAL*/
static struct termios old, current;

/* INICIALIZA NOVAS DEFINICOES PARA O TERMINAL*/
void initTermios(int echo) 
{
	tcgetattr(0, &old); /* CONFIGURACOES ANTIGAS PARA O TERMINAL */
	current = old; /* IGUALA AS CONFIGURACOES ATUAIS AS ANTIGAS*/
	current.c_lflag &= ~ICANON; /* DESABILITA O BUFFER INPUT/OUTPUT, MODO PADRAO*/
	if (echo) {
		current.c_lflag |= ECHO; /* DESLIGA ECHO*/
	} else {
		current.c_lflag &= ~ECHO; /* LIGA ECHO */
	}
	tcsetattr(0, TCSANOW, &current); /* NOVAS CONFIGURACOES DE TERMINAL */
}

/* DESFAZ AS MODIFICACOES NO TERMINAL */
void resetTermios(void) 
{
	tcsetattr(0, TCSANOW, &old);
}

/*FUNCAO GERAL PARA A LEITURA, LE APENAS 1 BYTE, INT ECHO DEFINE O MODO DE FUNCIONAMENTO*/
char getch_(int echo) 
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* LE O CARACTERE SEM ECHO */
char getch(void) 
{
	return getch_(0);
}
//ENDTERMIOS
#endif

//INICIO DAS FUNCOES GRAFICAS
void blankPrint(){// IMPRIME 1 CARACTERE DO TIPO VAZIO
	printf(" ");
}

void barPrint(){ // IMPRIME 1 CARACTERE DO TIPO BARRA
	printf("@");
}

void bar2Print(){ // IMPRIME 1 CARACTERE DO TIPO BARRA_2
	printf("¬");
}

void entireRowBarPrint(){ // IMPRESSAO DE UMA LINHA INTEIRA
	for(int aux=0;aux<46;aux++){
		barPrint();
	}
	printf("\n");
}

void blockPrint(int block_stat,int current_line,int current_pos, int grid_row){ //divide a grade em blocos e trata cada um individualmente
	int block_buffer=0;
	if(current_pos==grid_row){block_buffer=1;}
	int counter=0;
	if(block_stat==0){ // celula vazia
		if(block_buffer==1){
			if(current_line==1 || current_line==7){
				while(counter++ < 14){
					bar2Print();	
				}
			}
			else{
				while(counter++ < 14){
					blankPrint();	
				}
			}
		}
		if(block_buffer==0){
			while(counter++ < 14){
				blankPrint();	
			}
		}

	}

	if(block_stat==1){// X
		if(block_buffer==1){
			if(current_line==1 || current_line==7){
				while(counter++ < 14){
					bar2Print();	
				}
			}
		}

		if(block_buffer==0){
			if(current_line==1 || current_line==7){
				while(counter++ < 14){
					blankPrint();	
				}
			}
		}

		if(current_line==2 || current_line==6){
			for(int counter=0;counter<14;counter++){
				if(counter==2 || counter==3 || counter==10 || counter==11){
					barPrint();
				}
				else{
					blankPrint();
				}
			}
		}
		if(current_line==3 || current_line==5){
			for(int counter=0;counter<14;counter++){
				if(counter==4 || counter==5 || counter==8 || counter==9){
					barPrint();
				}
				else{
					blankPrint();
				}
			}
		}
		if(current_line==4){
			for(int counter=0;counter<14;counter++){
				if(counter == 6 || counter == 7 ){
					barPrint();
				}
				else{
					blankPrint();
				}

			}
		}
	}

	if(block_stat==2){// O
		if(block_buffer==1){
			if(current_line==1 || current_line==7){
				while(counter++ < 14){
					bar2Print();	
				}
			}
		}

		if(block_buffer==0){
			if(current_line==1 || current_line==7){
				while(counter++ < 14){
					blankPrint();	
				}
			}
		}
		if(current_line==2 || current_line==6){
			for(int counter=0;counter<14;counter++){
				if(counter<2 || counter>11){
					blankPrint();
				}
				else{
					barPrint();
				}
			}
		}
		if(current_line>2 && current_line<6){
			for(int counter=0;counter<14;counter++){
				if(counter==2 || counter==3 || counter==10 || counter==11){
					barPrint();
				}
				else{
					blankPrint();
				}
			}
		}
	}
}

void gridPrint(int *block_stat, int block_buffer, int round){//FUNCAO PRINCIPAL DE IMPRESSAO 
	clear();
	if(round != 10){
		printf("\n		VEZ DO JOGADOR %d\n\n", round%2+1);	
	}
	if(round == 10){printf("\n\n\n");}
	for(int grid_row=0;grid_row<9;grid_row=grid_row+3){ //grid_row: coluna da grade
		for(int row=1;row<8;row++){//row: linha de cada celula
			blockPrint(block_stat[grid_row], row, block_buffer, grid_row);
			barPrint();
			barPrint();
			blockPrint(block_stat[grid_row+1], row, block_buffer, grid_row+1);
			barPrint();
			barPrint();
			blockPrint(block_stat[grid_row+2], row, block_buffer, grid_row+2);
			printf("\n");
		}
		if(grid_row<5){
			entireRowBarPrint();
		}
	}
}
//FIM DA PARTE GRAFICA

//INCIO DA LEITURA
int gridBuffer(int *block_stat, int posy, int round){ // movimentacao pela grade
	char buffer_input=0;
	int pos=posy;
	while(buffer_input != 10){
		gridPrint(block_stat, pos, round);
		buffer_input = getch();
		// printf("%c", buffer_input);
		if(buffer_input=='s'){ //desce 
			if(pos<6){
				pos=pos+3;
			}
		}
		if(buffer_input=='w'){//sobe
			if(pos>2){
			pos=pos-3;
			}
		}
		if(buffer_input=='a'){//left
			if(pos%3!=0){
				pos=pos-1;
			}
		}
		if(buffer_input=='d'){
			if((pos+1)%3!=0){//right
				pos=pos+1;
			}
		}
	}
	return pos;
}
//FIM DA LEITURA


int gameLogic(int *player){ // checa as condicoes de vitoria
	int win=0;
	for(int row=0;row<7;row=row+3){ //CHECA AS LINHAS
		if(player[row]+player[row+1]+player[row+2]==3){
			win=1;
		}
	}
	if(win!=1){
		for(int column=0;column<3;column++){ //CHECA AS COLUNAS
			if(player[column]+player[column+3]+player[column+6]==3){
				win=1;
			}
		}
	}
	if(win!=1){//CHECA AS DIAGONAIS ->
		if(player[0]+player[4]+player[8]==3){
			win=1;
		}
	}
	if(win!=1){
		if(player[2]+player[4]+player[6]==3){
			win=1;
		}
	}
	return win;
}

int menu(){
	clear();
	char menu_option;
	printf("\n[1] JOGO");
	printf("\n[2] INSTRUCOES");
	printf("\n[3] CREDITOS");
	printf("\n[4] SAIR");
	do{// DO WHILE DE CHECAGEM DE CONDICAO PARA A LEITURA DOS INPUTS DO MENU
		menu_option = getch();
		menu_option = menu_option - 48;
	}while(menu_option > 4 || menu_option < 1);

	return menu_option;
}

int main(){
	int quit=1; //VARIAVEL AUXILIAR PRA 4a OPCAO DO MENU
	while(quit){
		int menu_option = menu(); // VARIAVEL COM A CONDICAO DO MENU
		int block_stat[9] = {0}; // ESTADO DE CADA CELULA, NEUTRO, X OU O
		int playerA[9]={0}, playerB[9]={0}; // ARMAZENA A JOGADA DO JOGADOR SENDO O INDICE DO VETOR A CELULA DA GRADE
		int win=0,round=0,cell=0; // VARIAVEIS AUXILIARES 
		if(menu_option==1){	
			while(win==0){
				do{
					cell = gridBuffer(block_stat, cell, round);
				} while(block_stat[cell]!=0);
				if(round%2==0){
					playerA[cell] = 1;
					block_stat[cell] = 1;
				}
				if(round%2!=0){
					playerB[cell] = 1;
					block_stat[cell] = 2;
				}
				round++;
				if(gameLogic(playerA)>0){
					win=1;
				}
				if(gameLogic(playerB)>0){
					win=2;
				}
				if(round==9 && win==0){
					win=3;
				}
			}

			gridPrint(block_stat, 50, 10);
			if(win==1)
				printf("\n		JOGADOR 1 GANHOU!\n\n");
			if(win==2)
				printf("\n		JOGADOR 2 GANHOU!\n\n");
			if(win==3)
				printf("\n		   VELHA!\n\n");
			getch();
		}

		if(menu_option==2){
			char temp_char;
			clear();
			FILE *instrucoes;
			instrucoes = fopen("instrucoes.txt","r");
			do{
				temp_char=fgetc(instrucoes);
				printf("%c",temp_char);
			}while(temp_char!=EOF);
			printf("\nQUALQUER TECLA PARA CONTINUAR");
			getch();
			fclose(instrucoes);
		}
		if(menu_option==3){
			char temp_char;
			clear();
			FILE *creditos;
			creditos = fopen("creditos.txt", "r");
			do{
				temp_char=fgetc(creditos);
				printf("%c",temp_char);
			}while(temp_char!=EOF);
			printf("\nQUALQUER TECLA PARA CONTINUAR");
			getch();
			fclose(creditos);
		}
		if(menu_option==4){
			quit=0;
			clear();
			printf("\nQUALQUER TECLA PARA CONTINUAR\n");
			getch();
		}
	}
	return 0;
}
