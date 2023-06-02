#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
/*ADAPTACAO DA FUNCAO CLEAR() PARA LINUX*/
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

/*DIVIDE A GRADE EM BLOCOS E TRATA CADA UM INDIVIDUALMENTE. IMPRIME LINHA A LINHA.
 block_stat: 0 para vazio, 1 para X, 2 para O
 current_line: linha atual da impressao 
 current_pos: posicao exata da impressao
 grid_row: equivale a posicao da linha na tabela inteira
 */
void blockPrint(int block_stat,int current_line,int current_pos, int grid_row){ 
	int block_buffer=0; // VARIAVEL AUXILIAR PARA A POSICAO DO CURSOR, SE 1, EQUIVALE A POSICAO DO JOGADOR
	if(current_pos==grid_row){block_buffer=1;}
	int counter=0;
	if(block_stat==0){ // CELULA VAZIA
		if(block_buffer==1){ //
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
/*FUNCAO PRINCIPAL DE IMPRESSAO 
  *block_stat: vetor de block_stat
   block_buffer: posicao atual do jogador
   round: rodada atual, checa de quem e a vez
*/
void gridPrint(int *block_stat, int block_buffer, int round){
	clear();
	if(round != 10){ // CHECA SE O JOGO NAO ACABOU
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
			entireRowBarPrint(); //IMPRIME A SEQUENCIA QUE DIVIDE AS LINHAS DA TABELA
		}
	}
}
//FIM DA PARTE GRAFICA

//INCIO DA LEITURA
/*MOVIMENTACAO E LEITURA PELA GRADE
  *block_stat: nesse caso e utilizado dentro do gridPrint
   posy: variavel de posicao auxiliar
   round: rodada atual para ser utilizado dentro do gridPrint

RETURN: o gridBuffer retorna um inteiro que equivale a posicao na tabela
*/
int gridBuffer(int *block_stat, int posy, int round){
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

//INICIO DA LOGICA DE JOGO
/* CHECA AS CONDICOES DE VITORIA
 * *player: E o vetor de indices de cada um dos dois jogadores, se *player[i] = 1,
 * esse jogador marcou a celula i
 *
 *RETURN: O gameLogic retorna 1 para vitoria e 0 para derrota
 * */
int gameLogic(int *player){
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

//INICIO DO MENU
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
//FINAL DO MENU

int main(){
	int quit=1; //VARIAVEL AUXILIAR PRA 4a OPCAO DO MENU
	while(quit){
		int menu_option = menu(); // VARIAVEL COM A CONDICAO DO MENU
		int block_stat[9] = {0}; // ESTADO DE CADA CELULA, NEUTRO[0], X[1] OU O[2]
		int playerA[9]={0}, playerB[9]={0}; // ARMAZENA A JOGADA DO JOGADOR, SENDO O INDICE DO VETOR A CELULA DA GRADE
		int win=0,round=0,cell=0; // VARIAVEIS AUXILIARES, WIN: DEFINE QUEM VENCE, ROUND: CONTADOR DE RODADAS, CELL: CELULA LIDA
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

			gridPrint(block_stat, 50, 10); //IMPRESSAO DA TABELA FINAL ESTATICA PARA MOSTRAR O ULTIMO MOVIMENTO DO JOGO
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
