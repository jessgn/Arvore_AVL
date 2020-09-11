#include <stdio.h>
#include <stdlib.h>

/* 
UFPR- Universidade Federal do Paraná
TADS- Tecnologia em Análise e Desenvolvimento de Sistemas
Aluna : Jéssica Gustavo do Nascimento - GRR20185004
Implementação de árvore AVL
Estrutura de Dados II- Professora Andreia de Jesus

*/
typedef struct NO* ArvAVL;
ArvAVL* cria_ArvAVL();
int insere_ArvAVL(ArvAVL *raiz, int valor);
int remove_ArvAVL(ArvAVL *raiz, int valor);
void libera_ArvAVL(ArvAVL *raiz);
int consulta_ArvAVL(ArvAVL *raiz, int valor);
void exibeArvore_ArvAVL(ArvAVL *raiz);
int estaVazia_ArvAVL(ArvAVL *raiz);
int alt_ArvAVL(ArvAVL *raiz);

struct NO{
	int info;
	int alt;
	struct NO *esq;
	struct NO *dir;
};

//FUNÇÃO PARA CRIAR ÁRVORE AVL
ArvAVL* cria_ArvAVL(){
	ArvAVL* raiz=(ArvAVL*)malloc(sizeof(ArvAVL));
	if(raiz != NULL)
		*raiz = NULL;
			return raiz;
	
}

//FUNÇÃO PARA DESTRUIR A ÁRVORE AVL
void libera_NO(struct NO* no){
	if(no == NULL)
		return;
	libera_NO(no->esq);
	libera_NO(no->dir);
	free(no);
	no = NULL;
}

void libera_ArvAVL(ArvAVL* raiz){
	if(raiz == NULL)
		return;
	libera_NO(*raiz);//libera cada nó
	free(raiz);//libera a raiz
}

void exibeArvore_ArvAVL(ArvAVL *raiz){
	if (raiz == NULL)
		return;
	if(*raiz !=NULL){
		printf("%d", (*raiz)->info);
		printf("(");
		exibeArvore_ArvAVL(&((*raiz)->esq));
		exibeArvore_ArvAVL(&((*raiz)->dir));
		printf(")");
	}
}

int consulta_ArvAVL(ArvAVL *raiz, int valor){
	if(raiz == NULL)
		return 0;
	struct NO* atual = *raiz;
	while(atual != NULL){
		if(valor == atual->info){
			return 1; 
		}
		if(valor > atual->info)
			atual = atual->dir;
		else
			atual = atual->esq;
	}
	return 0;
}

int alt_ArvAVL(ArvAVL *raiz) {		// a altura é calculada com base no nó mais longe da raiz
	if(raiz == NULL)
		return 0;
	if(*raiz == NULL)
		return 0;
	int alt_esq = alt_ArvAVL(&((*raiz)->esq));
	int alt_dir = alt_ArvAVL(&((*raiz)->dir));
	if(alt_esq > alt_dir)
		return (alt_esq + 1);
	else
		return (alt_dir + 1);
}

int estaVazia_ArvAVL(ArvAVL *raiz) {		//se retorna 1 esta vazia
	if(raiz == NULL)
		return 1;
	if(*raiz == NULL)
		return 1;
	return 0;
}

//Funções auxiliares
//Calcula a altura de um nó
int alt_NO(struct NO* no){
	if( no == NULL)
		return -1;
	else
		return no->alt;
}

//Calcula o fator de balanceamento de um nó
int fatorBalanceamento_NO(struct NO* no){
	return labs(alt_NO(no->esq)- alt_NO(no->dir));
}

//Calcula o maior valor 
int maior(int x, int y){
	if(x > y)
		return x;
	else
		return y;
}

//Rotação simples a direita
void RotacaoLL(ArvAVL *raiz){
		struct NO *no;
		no = (*raiz)->esq;
		(*raiz)->esq = no->dir;
		no->dir = (*raiz);
		(*raiz)->alt = maior(alt_NO((*raiz)->esq), alt_NO((*raiz)->dir))+1;
		no->alt = maior(alt_NO(no->esq), (*raiz)->alt)+1;
		*raiz = no;
}

//Rotação simples a esquerda
void RotacaoRR(ArvAVL *raiz){
	struct NO *no;
	no = (*raiz)->dir;
	(*raiz)->dir = no->esq;
	no->esq = (*raiz);
	(*raiz)->alt = maior(alt_NO((*raiz)->esq),alt_NO((*raiz)->dir))+1;
	no->alt = maior(alt_NO(no->dir),(*raiz)->alt)+1;
	(*raiz) = no;
}

//Rotação dupla Esquerda-Direita
void RotacaoLR(ArvAVL *raiz){
	RotacaoRR(&(*raiz)->esq);
	RotacaoLL(raiz);
}

//Rotação dupla Direita-Esquerda
void RotacaoRL(ArvAVL *raiz){
	RotacaoLL(&(*raiz)->dir);
	RotacaoRR(raiz);
}

int insere_ArvAVL (ArvAVL *raiz, int valor){
	int res;
	if(*raiz == NULL){ //Árvore vazia ou nó folha
		struct NO *novo;
		novo = (struct NO*)malloc(sizeof(struct NO));
		if(novo == NULL)
		return 0;
		
		novo->info = valor;
		novo->alt = 0;
		novo-> esq = NULL;
		novo->dir = NULL;
		*raiz = novo;
			return 1;
	}
		struct NO *atual = *raiz;
		if(valor < atual->info){
			if((res=insere_ArvAVL(&(atual->esq),valor))==1){
				if(fatorBalanceamento_NO(atual) >=2){
					if(valor < (*raiz)->esq->info){
						RotacaoLL(raiz);
					}else{
						RotacaoLR(raiz);
					}
			}
		}
}else{
	if(valor > atual->info){
		if((res = insere_ArvAVL(&(atual->dir),valor))==1){
			if(fatorBalanceamento_NO(atual) >= 2){
				if((*raiz)->dir->info < valor){
					RotacaoRR(raiz);
				}else{
					RotacaoRL(raiz);
			}
		}
	}
 }else{
	 printf("Valor duplicado!!");
	 return 0;
	}
}
	atual->alt = maior(alt_NO(atual->esq), alt_NO(atual->dir)) +1;
	return res;
}

//FUNÇÃO RESPONSÁVEL POR TRATAR A REMOÇÃO DE UM NÓ COM 2 FILHOS
struct NO* procuraMenor( struct NO* atual){
		struct NO *no1 = atual;
		struct NO *no2 = atual->esq;
		while(no2 != NULL){
			no1 = no2;
			no2 = no2->esq;
		}
		return no1;
}

//	FUNÇÃO RESPONSÁVEL PELA BUSCA DO NÓ A SER REMOVIDO
int remove_ArvAVL(ArvAVL *raiz, int valor){
	if(*raiz == NULL){
		printf("O valor nao existe!!\n");
		return 0;
	}
	int res; 
	if(valor < (*raiz)-> info){
		if((res = remove_ArvAVL(&(*raiz)->esq, valor))==1)
		if(fatorBalanceamento_NO(*raiz) >= 2){
			if(alt_NO((*raiz)->dir->esq) <= alt_NO((*raiz)->dir->dir))
				RotacaoRR(raiz);
			else
				RotacaoRL(raiz);
		}
	}
 if((*raiz)->info < valor){
 	if((res=remove_ArvAVL(&(*raiz)->dir, valor))==1){
 		if(fatorBalanceamento_NO(*raiz) >= 2){
 			if(alt_NO((*raiz)->esq->dir) <= alt_NO ((*raiz)->esq->esq) )
 				RotacaoLL(raiz);
 			else
 				RotacaoLR(raiz);
		 }
	 }
 }
 if ((*raiz)->info == valor){
 	if(((*raiz)->esq== NULL || (*raiz)->dir == NULL)){
 		struct NO *oldNode = (*raiz);
 		if((*raiz)->esq != NULL) 
		 	*raiz = (*raiz)->esq;
 		else
 			*raiz = (*raiz)->dir;
 				free(oldNode);
	 }else{// nó tem 2 filhos
	 	struct NO* temp = procuraMenor((*raiz)->dir);
	 	(*raiz)->info = temp->info;
	 	remove_ArvAVL(&(*raiz)->dir,(*raiz)->info);
	 	if(fatorBalanceamento_NO(*raiz) >= 2){
	 		if(alt_NO((*raiz)->esq->dir) <= alt_NO((*raiz)->esq->esq))
	 			RotacaoLL(raiz);
	 		else
	 			RotacaoLR(raiz);
		 }
	 }
	 return 1;
 }
 return res;
}

int main(){
	int valor, op, x;
	ArvAVL* raiz = cria_ArvAVL();
	
	do{
		printf("\n***********************Menu***********************\n");
		printf("1- Inserir na arvore\n");
		printf("2- Remover da arvore\n");
		printf("3- Procurar um Valor\n");	
		printf("4- Exibir a arvore\n");
		printf("5- Exibir a altura da arvore\n");
		printf("6- Verificar se a arvore esta vazia\n");
		printf("7- Destruir uma arvore\n");
		printf("8- Sair do programa\n");
		scanf("%d", &op);
	
		switch(op){
			case 1://Insere na árvore
				printf("Qual o valor deseja inserir?\n");
				scanf("%d", &valor);
				x = insere_ArvAVL (raiz, valor); 	 
				break;
			case 2://Remove da árvore
				printf("Qual o valor a ser removido?\n");
				scanf("%d", &valor);
				x = remove_ArvAVL(raiz,valor);
				if(x=1)
					printf("O noh foi removido\n");
				else
					printf("No nao encontrado\n");
				break;
			case 3: //Consulta se tem na árvore o valor inserido
				printf("Qual o valor a ser procurado?\n");
				scanf("%d", &valor);
				x = consulta_ArvAVL(raiz, valor);
				if(x == 1){
					printf("Encontrado\n",&valor);
				}else{
					printf("Valor nao encontrado\n");
				}break;
			case 4://Exibe a arvore
				  exibeArvore_ArvAVL(raiz);
				printf("\n");
				break;
			case 5://Exibe a altura da arvore
				printf("A altura da arvore e:\n");
				x = alt_ArvAVL(raiz);
				printf("%d\n", x);
				break;
			case 6://Verifica se a árvore está vazia
				if(estaVazia_ArvAVL(raiz))
					printf("A arvore esta vazia\n");
				else
					printf("A arvore nao esta vazia\n");
				break;
			case 7://Destrói a árvore
				libera_ArvAVL(raiz); //chamada da funçao p liberar arvore inteira
				printf("A arvore foi totalmente removida\n");
				break;
			default:
				printf("Opção Invalida\n");
		}
	}while(op != 8);
	
	return 0;
}
