#include <stdio.h>
#include <stdlib.h>
#include <grafo.h>

TG * criaGrafo(char * nomeArq);

int main(int argc, char* argv[])
{
    if(argc<2) return 0;
    TG *g = criaGrafo(argv[1]);
    int i, n, r;
    int dir = orientado(g);
    colorir(g);
    int con = conexo(g);
    printf("dir: %d\nconexo: %d\n", dir, con);
    while(1){
        printf("\n 1 - Inserir \n 2 - Retirar \n 3 - Buscar \n 4 - Imprimir \n 5 - Especial \n 6 - Sair\n");
        scanf("%d",&i);

        if(i == 6){
          libera(g);
          break;
        }

        switch(i){

            case(1):
                printf("\n 1 - No \n 2 - Aresta \n");
                scanf("%d",&n);
                if(n == 1){
                    printf("\n Digite o valor do no:");
                    scanf("%d",&n);
                    insere_no(g, n);
                    if(!dir) colorir(g);
                }else if(n == 2){
                    printf("\n Digite valor de no1 e no2:");
                    scanf("%d%d",&n,&r);
                    insere_aresta(g,n,r);
                    if(dir == 0){
                        insere_aresta(g,r,n);
                        colorir(g);
                    }
                }
                break;
            case(2):
                printf("\n 1 - No \n 2 - Aresta \n");
                scanf("%d",&n);
                if(n == 1){
                    printf("\n Digite o valor do no:");
                    scanf("%d",&n);
                    retira_no(g, n);
                    if(dir==0) colorir(g);
                }else if(n == 2){
                    printf("\n Digite valor de no1 e no2:");
                    scanf("%d%d",&n,&r);
                    retira_aresta(g,n,r);
                    if(dir == 0){
                        retira_aresta(g,r,n);
                        colorir(g);
                    }
                }
                break;
            case(3):
                printf("\n 1 - No \n 2 - Aresta \n");
                scanf("%d",&n);
                if(n == 1){
                    printf("\n Digite o valor do no:");
                    scanf("%d",&n);
                    TNo *resp = busca_no(g, n);
                    if(!resp) printf("Nó não encontrado");
                    else printf("%d", resp->id_no);
                }else if(n == 2){
                    printf("\n Digite valor de no1 e no2:");
                    scanf("%d%d",&n,&r);
                    TViz *resp = busca_aresta(g,n,r);
                    if(!resp) printf("\n %d e %d nao estao conectados\n", &n, &r);
                    else printf("\n %d e %d estao conectados\n", &n, &r);

                }
                break;

            case(4):
                imprime(g);
                break;
            case(5):
                pontes(g);
                fortementeConexo(g);
                //se orientado imprimir componentes fortemente conexas
                //se nao orientado imprimir: se eh conectado (pontes e pontos de art); se nao conectado (componentes conectadas)
                break;

        }

    }
}

int orientado(TG*g){ 							//verifica se eh orientado (1) ou nao orientado(0)
    TNo * p = g->prim;

    while(p){           						//para cada no...
        TViz * v = p->prim_viz;     			//...verifica se possui aresta de ida e volta com seus vizinhos
        while(v){
            TViz * ida = busca_aresta(g,p->id_no,v->id_viz);
            TViz * volta = busca_aresta(g,v->id_viz,p->id_no);
            if(ida == NULL || volta == NULL) return 1;      //se achar um caso que nao tem, retorna positivo (orientado)
            v = v->prox_viz;
        }
        p = p->prox_no;
    }
    return 0;       							//se passou por todos e nao achou um caso assim, retorna negativo (nao orientado)
}

int colorir(TG * g){
    descolorir(g);      						//o grafo precisa etar descolorido
    TNo * p = g->prim;
    int cor = 0;
    while(p){           						//para cada no do grafo...
        if(p->cor==0)coloreConexos(g, p, ++cor);//...colore ele e os nos conectados a ele
        p = p->prox_no;
    }
    g->cores = cor;
    return cor;
}
void descolorir(TG *g){     					//descolore colocando 0 como cor de todos os nos
    TNo * p = g->prim;
    while(p){
        p->cor = 0;
        p = p->prox_no;
    }
    g->cores = 0;
}
void coloreConexos(TG*g, TNo* no, int cor){    //colore o no e todos os grafos que ele consegue chegar
    TNo * no2 = g->prim;
    no->cor = cor;
    while(no2){         						//para cada no...
        if(no!=no2){							//...procura um caminho de ida e de volta para outro no do grafo
            if(caminho(g, no, no2) && caminho(g,no2, no)) no2->cor = cor;
        }
        no2 = no2->prox_no;     				//serve para direcionado e nao direcionado
    }
}

int caminho(TG *g, TNo * no1, TNo * no2){       //verifica se existe um caminho entre um no e outro
    if (!g || !no1 || !no2) return 0;
    if(no2->id_no == no1->id_no) return 1;      //se achou retorna positivo (1)
    if(no1->cor ==  -1) return 0;               //verifica se ja passou por aquele no. Se ja retorna 0 para evitar loop
    int cor = no1->cor, resp=0;
    no1->cor = -1;                              //pinta com -1 para sinalizar que aquele no ja foi visitado
    TViz *v = no1->prim_viz;
    while(v && resp!=1){
        TNo * vizNo = busca_no(g, v->id_viz);   //confere caminho a partir dos vizinhos
        resp = caminho(g,vizNo,no2);            //se algum vizinho retornou resultado positivo sai do loop
        v = v->prox_viz;
    }
    no1->cor = cor;                             //pinta de volta com a cor original
    return resp;                                //e retorna
}

int conexo(TG * g){                             //verifica se eh conexo
    TNo * no = g->prim;                         //do primeiro no...
    TNo * no2 = no->prox_no;
    while(no2){                                 //...verifica se existe caminho para todos os outros nos
        if(!caminho(g,no, no2)) return 0;       //se nao achou algum caminho, retorna 0
        no2 = no2->prox_no;
    }
    return 1;                                   //se achou caminho para todos eles, eh conexo (1)
}

void fortementeConexo(TG *g){                   //imprime os componentes fortemente conexos
    int i, n = g->cores;
    TNo * no;
    printf("componentes fortemente conexos:\n");
    for(i = 1; i<=n;i++){                       //para cada cor que existe no grafo...
        no = g->prim;
        while(no){								//imprime o no que tem aquela cor
            if(no->cor == i) printf("[%d] ",no->id_no);
            no = no->prox_no;
        }
        printf("\n");
    }
}
void pontes(TG * g){								//imprime as pontes do grafo
    TNo * no = g->prim;
    int conec = conexo(g);
    if(!conec) return;
    while(no){
        TViz * v = no->prim_viz;					//para cada vizinho de um no...
        int prim, temp;
        if(v){
            prim = v->id_viz;
            do{										//retira a aresta de ida e volta...
                temp = v->id_viz;
                retira_aresta(g, no->id_no, temp);
                retira_aresta(g, temp, no->id_no);
                conec = conexo(g);					//...verifica se o grafo continua conectado...
                insere_aresta(g, no->id_no, temp);	//... e coloca as arestas de volta
                insere_aresta(g, temp, no->id_no);
                if(!conec) printf("ponte: %d e %d\n", no->id_no, temp);
                									//se desconectou, imprime o no e o vizinho que causou isso
                v = v->prox_viz;
            }while((v) && (v->id_viz != prim));
        }
        no = no->prox_no;
    }
}

TG* criaGrafo(char* nomeArq){					//le o arquivo e monta o grafo
    FILE* arquivo = fopen(nomeArq,"rt");
    if(!arquivo) exit(1);
    int n,i, r;

    fscanf(arquivo,"%d",&n);					//recebe o tamanho do grafo (1a linha)...

    TG* g = cria();

    for(i = 0; i< n; i++){						//... e insere n nos no grafo
        insere_no(g,i+1);
    }

    r = fscanf(arquivo,"%d%d",&n,&i);			//le as arestas...
    while(r!=EOF){
        insere_aresta(g,n,i);					//...e insere no grafo
        r = fscanf(arquivo,"%d%d",&n,&i);
    }

    fclose(arquivo);							//fecha o arquivo

    return g;									//e retorna o grafo
}
