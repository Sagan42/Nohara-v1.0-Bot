#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

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
	printf ("CADASTRE COM O BOTAO ESQUERDO\n");
	printf ("FINALIZE COM O BOTAO DIREITO\n");
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
	}
	
	printf("DIGITE EM SEGUNDOS O TEMPO ENTRE OS CLICKS:\n");
	scanf ("%d", &timeClick);
	printf("DIGITE EM SEGUNDOS O TEMPO ENTRE OS LOOPS:\n");
	scanf ("%d", &timeLoop);
	
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
	int menuOp=0;
	do{
		printf("=====SEJA BEM VINDO AO NOHARA=====\n");
		printf("DIGITE A OPCAO PARA O BOT:\n");
		printf("1-CADASTRAR CLIQUES\n");
		printf("2-EXECUTAR CLIQUES\n");
		printf("3-SAIR DO BOT\n");
		
		scanf ("%i", &menuOp);
		
		switch (menuOp){
			case 1:
				cadastrarClicks();
				break;
			case 2:
				executarClicks();				
				break;
			case 3:
				exit(1);				
				break;
			default:
				break;
		}
		menuOp = 0;
	}while (menuOp == 0);	
	
	system ("pause");
	return 0;
}
