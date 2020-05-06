#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>
#include <time.h>


#define MENU_SIZE 30
#define TRUE 1
#define FALSE 0

void barra_sup(int tamanho_x, int pula_linha);
void barra_inf(int tamanho_x, int pula_linha);
void novo_menu_item(int tamanho_x, const char item[], int peso_item, int pula_linha);
void separa_itens(int tamanho_x, int pula_linha);
void menu_titulo(int tamanho_x, const char titulo[], int pula_linha);
void op_ler(int tamanho_x, const char conteudo[], int pos_x, int pos_y);
void gotoxy(int x, int y);
int readOnlyNumber(int tamanho);

typedef struct informacao{ //struct que "modela" o espaço de memória para cada membro da lista encadeada
	int coordX;
	int coordY;
}Dados;

typedef struct noh{ //struct noh, possui estrutura que possui os campos a rem preenchidos. Possui o campo prox que apontará o próximo noh.
	Dados dados;
	struct noh* prox;
}Noh;

typedef struct lista{ //struct cabeça; cabeca aponta para struct que possui struct que possui os campos a serem preenchidos.
	int tamanho; //Guarda o índice de cada noh.
	Noh* cabeca;
}Lista;

Lista* fazer_lista(){ //Aloca um espaço de memória para a lista, ou seja, cria a lista e a inicializa.
	Lista* lista = (Lista*) malloc (sizeof(Lista));
	lista->tamanho = 0;
	lista->cabeca = NULL;
	return lista;
}

void inserir(Lista* lista, Dados dados){ //Insere elementos e aloca dinamicamente.
	Noh* noh = (Noh*)malloc (sizeof(Noh));
	if(lista->tamanho == 0){
		lista->cabeca = noh;
		noh->dados = dados;
		noh->prox = NULL;
		lista->tamanho++;
	} else{
		Noh* tmp;
		tmp = lista->cabeca;
		while (tmp->prox != NULL){
			tmp = tmp->prox;
		}
		noh->dados = dados;
		noh->prox = NULL;
		tmp->prox = noh;
		lista->tamanho++;
	}
}

void imprimir(Lista* lista){ //Imprimi por completo as coordenadas de click, para salvar.
	Noh* aux = lista->cabeca;
	FILE *file;
	file = fopen ("coordenadas.txt", "w");
	while (aux != NULL){
		fprintf(file, "%d %d", aux->dados.coordX, aux->dados.coordY);
		fprintf(file,"\n");
		aux = aux->prox;
	}
	fclose(file);
}

void cadastrarClicks(){
	//Declarations
	POINT pt;
	int registerControl = 0;
	Lista* fazer_lista();
	Dados dados;
	void inserir(Lista* lista, Dados dados);	 
	//Declarations
	
	Lista* listaA = fazer_lista();
	barra_sup(MENU_SIZE, TRUE);
	menu_titulo(MENU_SIZE, "INSTRUÇÕES", TRUE);
	separa_itens(MENU_SIZE, TRUE);
	menu_titulo(MENU_SIZE,"Cadastre com o button esquerdo", TRUE);
	menu_titulo(MENU_SIZE,"Finalize com o button direito ", TRUE);
	barra_inf(MENU_SIZE, TRUE);

	do{
		if((GetKeyState(VK_RBUTTON) & 0x80) != 0){
			registerControl = 1;
		} else
			if((GetKeyState(VK_LBUTTON) & 0x80) != 0){
				GetCursorPos(&pt);
				dados.coordX = pt.x;
				dados.coordY = pt.y;
				printf("%d ", pt.x);
				printf("%d\n\n", pt.y);
				Sleep(500); //Necessário para que o programa não leia mais de uma vez a posição por click.
				printf("PROCESSANDO...\n");
				inserir (listaA, dados); //Chama função que insere um novo noh com os dados lidos.
				Sleep(500);
				printf("AGUARDANDO...\n\n");
			}		
	}while (registerControl == 0);
	imprimir(listaA);
	printf("COORDENADAS SALVAS\n\n");
}
	
void executarClicks(){
	Lista* fazer_lista();
	Dados dados;
	void inserir(Lista* lista, Dados dados);
	FILE *arq;
	int coordX = 0, coordY = 0, result = 0;
	HWND h;
	int timeClick = 0;
	int timeLoop = 0;
	struct tm *data_hora_atual;
	time_t segundos;
	
	Lista* listaA = fazer_lista();
	arq = fopen("coordenadas.txt", "rt");
	if (arq == NULL){
		printf("COORDENADAS NÃO CADASTRADAS\n");
	} else{
		do{
			result = fscanf (arq, "%d%d", &coordX, &coordY);
			if(result != EOF){
				dados.coordX = coordX;
				dados.coordY = coordY;
				inserir (listaA, dados);	
			}		
		}while (result != EOF);
		op_ler(MENU_SIZE, "Intervalo entre os clicks:", 30, 2);
		timeClick = readOnlyNumber(2);
		gotoxy(1, 4);
		
		op_ler(MENU_SIZE, "Intervalo entre os Loops:", 29, 5);
		timeLoop = readOnlyNumber(2);
		gotoxy(1, 8);
		
		printf ("|||||||||||||||| TEMPO DE ESPERA ENTRE CLICKS DEFINIDO EM: %d SEGUNDOS ||||||||||||||||\n", timeClick);
		printf ("|||||||||||||||| TEMPO DE ESPERA ENTRE LOOP DE CLICKS DEFINIDO EM: %d SEGUNDOS ||||||||||||||||\n\n", timeLoop);
		time(&segundos);
		data_hora_atual = localtime(&segundos);
		printf("INICIO EM %d/%d/%d as ", data_hora_atual->tm_mday, data_hora_atual->tm_mon+1, data_hora_atual->tm_year+1900);
		printf("%d:%d:%d\n\n\n", data_hora_atual->tm_hour, data_hora_atual->tm_min, data_hora_atual->tm_sec);
		
		//Função para clicar com o botão esquerdo
		for ( ; ; ){
			Noh* aux = listaA->cabeca;
			
			while (aux != NULL){
				printf("EXECUTANDO CLICK...\n");
				coordX = aux->dados.coordX;
				coordY = aux->dados.coordY;
				aux = aux->prox;
				h = FindWindow(NULL, "Janela");
	    		SetCursorPos(coordX, coordY);
	    		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN, coordX, coordY, 0, 0);
	    		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, coordX, coordY, 0, 0);
				Sleep(timeClick * 1000);
			}
			time(&segundos);
			data_hora_atual = localtime(&segundos);
			printf("_______________________________________________________________________________________________________\n");
			printf("NOVA ESPERA DEFINIDA EM:\n");
			printf("%d/%d/%d as ", data_hora_atual->tm_mday, data_hora_atual->tm_mon+1, data_hora_atual->tm_year+1900);
			
			printf("%d:%d:%d\n", data_hora_atual->tm_hour, data_hora_atual->tm_min, data_hora_atual->tm_sec);
			
			printf("EM ESPERA...\n");
			Sleep(timeLoop * 1000);
		}
	}

	//Função para clicar com o botão direito
	/*void RightClick(int x, int y){
    HWND h;
    h = FindWindow(NULL, "Janela"); //Define h como uma variavel que se refere a janela onde deseja ser trabalhado o clique
    SetCursorPos(x, y);      //Muda a posição do mouse para as coordenadas x e y fornecidas.
    mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN, x, y, 0, 0); //Pressiona o botão do mouse
    mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP, x, y, 0, 0);   //Libera o botão do mouse
	}*/

}

int main(void){
	SetConsoleTitle("Nohara v1.0");
	int repetir_menu;
	int error = 0;
	char msg[MENU_SIZE];
	do{
		repetir_menu = 1;
		barra_sup(MENU_SIZE, TRUE);
		menu_titulo(MENU_SIZE, "SEJA BEM-VINDO AO NOHARA", TRUE);
		separa_itens(MENU_SIZE, TRUE);
		novo_menu_item(MENU_SIZE, "Cadastrar Cliques", 1, TRUE);
		novo_menu_item(MENU_SIZE, "Executar Cliques", 2, TRUE);
		novo_menu_item(MENU_SIZE, "Sair do BOT", 3, TRUE);
		if(error){
			separa_itens(MENU_SIZE, TRUE);
			menu_titulo(MENU_SIZE, msg, TRUE);
		}
		barra_inf(MENU_SIZE, TRUE);
			
		switch (readOnlyNumber(1)){
			case 1:
				system("cls");
				cadastrarClicks();
				break;
			case 2:
				system("cls");
				executarClicks();				
				break;
			case 3:
				repetir_menu = 0;				
				break;
			default:
				error = TRUE;
				strcpy(msg, "Digite um numero entre 1 e 3");
				break;
		}
		system("cls");
	}while (repetir_menu);	
	
	system ("pause");
	return 0;
}

int readOnlyNumber(int tamanho){
	char ch;
	char digitado[tamanho+1];
	int i = 0;
	int repetir_digitacao = TRUE;
	do{
		repetir_digitacao = TRUE;
		fflush(stdin);
		ch = getch();
		if(i >= tamanho && ch != 13 && ch != 8){
			getch();
			fflush(stdin);
		}else if(ch == 13 && i > 0){
			repetir_digitacao = FALSE;
		}else{
			if(isdigit(ch)){
				digitado[i] = ch;
				printf("%c", ch);
				i++;
			}
			else if(ch == 8 && i > 0){
				printf("\b \b");
				digitado[i] = '\0';
				i--;
			}
			else{
				if(i == tamanho)
					repetir_digitacao = FALSE;
				else
	   				repetir_digitacao = TRUE;
			}
		}
	}while(repetir_digitacao);
	digitado[i] = '\0';
	return atoi(digitado);
}

void op_ler(int tamanho_x, const char conteudo[], int pos_x, int pos_y){
	int i = 0;
	barra_sup(tamanho_x, TRUE);
	printf("\xBA ");
	printf("%s", conteudo);
	for(i = strlen(conteudo); i < tamanho_x; i++)
		printf(" ");
	printf(" \xBA\n");
	barra_inf(tamanho_x, TRUE);
	gotoxy(pos_x, pos_y);
}
//===========================================================================================================================================================
void gotoxy(int x, int y){//Função utilizada para selecionar o lugar que o cursor deve ficar no terminal com base na posição X e Y.
  COORD c;
  c.X = x - 1;
  c.Y = y - 1;
  SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void barra_sup(int tamanho_x, int pula_linha){
	int i = 0;
	printf("\xC9");//Serve para desenhar um canto superior esquero.
	for(i = 0; i < tamanho_x+2; i++){
		printf("\xCD");
	}
	printf("\xBB");
	if(pula_linha == TRUE)
		printf("\n");
}
//================================================================================================================================================================
void barra_inf(int tamanho_x, int pula_linha){
	int i = 0;
	printf("\xC8");
	for(i = 0; i < tamanho_x+2; i++)
		printf("\xCD");
	printf("\xBC");
	if(pula_linha == TRUE)
		printf("\n");
}
//==============================================================================================================================================================
void novo_menu_item(int tamanho_x, const char item[], int peso_item, int pula_linha){
	int i, qtd_letras = 0;
	qtd_letras = strlen(item);

	if(peso_item > 1000){
		printf("\n\nERRO - Peso do item '%s' esta muito alto!\n\nRECOMENDACAO: Faca alguns Sub-menus\n", item);
	}
	else{
		printf("\xBA ");
		printf("%s", item);
		if(peso_item < 100){
			while(qtd_letras < tamanho_x - 4){
				printf("_");
				qtd_letras++;
			}
			if(peso_item < 0)
				printf("(__)");
			else if(peso_item < 10)
				printf("(0%d)", peso_item);
			else
				printf("(%d)", peso_item);
		}
		else if(peso_item >= 100 && peso_item < 1000){
			while(qtd_letras < tamanho_x - 5){
				printf("_");
				qtd_letras++;
			}
			printf("(%d)", peso_item);
		}
	}
	printf(" \xBA");
	if(pula_linha == TRUE)
		printf("\n");
}
//==============================================================================================================================================================
void separa_itens(int tamanho_x, int pula_linha){
	int i = 0;
	printf("\xCC");
		for(i = 0; i < tamanho_x+2; i++){
			printf("\xCD");
		}
	printf("\xB9");
	if(pula_linha == TRUE)
		printf("\n");
}
//============================================================================================================================================================
void menu_titulo(int tamanho_x, const char titulo[], int pula_linha){
	int i = 0;
	int tamanho_titulo = strlen(titulo);
	int tamanho_x_novo = tamanho_x - tamanho_titulo;
	int metade_x_novo = tamanho_x_novo / 2;
	
	printf("\xBA ");
	for(i = 0; i < metade_x_novo; i++)
		printf(" ");
	printf("%s", titulo);
	for(i = 0; i < metade_x_novo; i++)
		printf(" ");
	printf(" \xBA");
	if(pula_linha == TRUE)
		printf("\n");
}
//===========================================================================================================================================================
void pula_espaco(int tamanho_x, int pula_linha){//Função responsabel por fazer dois traços separado. EX: "|            |"
	int i = 0;
	printf("\xBA ");
	for (i = 0; i < tamanho_x; i++)
		printf(" ");
	printf(" \xBA");
	if(pula_linha == TRUE)
		printf("\n");
}
