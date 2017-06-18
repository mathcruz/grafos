#include <stdio.h>
#include <stdlib.h>
#include <grafo.h>

TG * criaGrafo(char * nomeArq);
TG * salvaNo(TNo * no);
int orientado(TG*g); 							//verifica se eh orientado (1) ou nao orientado(0)
int conexo(TG * g);                             //verifica se eh conexo
int colorir(TG * g);
void descolorir(TG *g);     					//descolore colocando 0 como cor de todos os nos
void coloreConexos(TG*g, TNo* no, int cor);     //colore o no e todos os grafos que ele consegue chegar
int caminho(TG *g, TNo * no1, TNo * no2);       //verifica se existe um caminho entre um no e outro
void fortementeConexo(TG *g);                   //imprime os componentes fortemente conexos
void pontes(TG * g);						    //imprime as pontes do grafo
void pontosDeArt(TG * g, TNo * no);


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
        con = conexo(g);
        printf("\n MENU \n 1 - Inserir \n 2 - Retirar \n 3 - Buscar \n 4 - Imprimir \n 5 - Especial \n 6 - Sair\n Digite uma opcao: ");
        scanf("%d",&i);

        if(i == 6){
          libera(g);
          break;
        }

        switch(i){

            case(1):                                        //OPCAO INSERE
                printf("\n 1 - No \n 2 - Aresta \n Digite uma opcao: ");
                scanf("%d",&n);
                if(n == 1){                                 //INSERE NO
                    printf("\n Digite o valor do no: ");
                    scanf("%d",&n);
                    insere_no(g, n);
                    if(!dir) colorir(g);
                    printf("\n %d inserido com sucesso!\n", n);
                }else if(n == 2){                           //INSERE ARESTA
                    printf("\n Digite valor de no1 e no2: ");
                    scanf("%d%d",&n,&r);
                    insere_aresta(g,n,r);
                    TNo * no1 = busca_no(g, n);
                    TNo * no2 = busca_no(g, r);
                    if(dir == 0){                           //se grafo nao orientado
                        insere_aresta(g,r,n);
                        colorir(g);
                    }
                    if(!no1 || !no2) printf("\n Impossivel inserir aresta entre %d e %d \n", n, r);
                    else printf("\n Aresta entre %d e %d inserida com sucesso!\n",n,r);
                }
                break;
            case(2):                                        //OPCAO RETIRA
                printf("\n 1 - No \n 2 - Aresta \n Digite uma opcao: ");
                scanf("%d",&n);
                if(n == 1){                                 //RETIRA NO
                    printf("\n Digite o valor do no: ");
                    scanf("%d",&n);
                    retira_no(g, n);
                    if(dir==0) colorir(g);                  //se grafo nao orientado
                    printf("\n %d removido com sucesso!\n",n);
                }else if(n == 2){                   //RETIRA ARESTA
                    printf("\n Digite valor de no1 e no2: ");
                    scanf("%d%d",&n,&r);
                    retira_aresta(g,n,r);
                    if(dir == 0){                           //se grafo nao orientado
                        retira_aresta(g,r,n);
                        colorir(g);
                    }
                    printf("\n Aresta entre %d e %d removida com sucesso!\n",n,r);
                }
                break;
            case(3):                                        //OPCAO BUSCA
                printf("\n 1 - No \n 2 - Aresta \n Digite uma opcao: ");
                scanf("%d",&n);
                if(n == 1){                                 //BUSCA NO
                    printf("\n Digite o valor do no:");
                    scanf("%d",&n);
                    TNo *resp = busca_no(g, n);
                    if(!resp) printf("\n %d nao pertence ao grafo.\n",n);
                    else printf("\n %d pertence ao grafo.\n",n);
                }else if(n == 2){                           //BUSCA ARESTA
                    printf("\n Digite valor de no1 e no2:");
                    scanf("%d%d",&n,&r);
                    TViz *resp = busca_aresta(g,n,r);
                    if(!resp) printf("\n %d e %d nao estao conectados.\n", n, r);
                    else printf("\n %d e %d estao conectados.\n", n, r);

                }
                break;

            case(4):                                        //OPCAO IMPRIME
                imprime(g);
                break;
            case(5):                                        //OPCAO ESPECIAL
                if(dir == 1){                               //se grafo orientado
                    printf("\n Grafo orientado\n Componentes fortemente conexas:\n");
                    fortementeConexo(g);                    //imprime componentes fortemente conexas
                }else{                                      //se nao orientado
                    printf("\n Grafo nao orientado\n");
                    if(con == 1){                           //se conectado
                        printf("\n Pontes:\n");              //imprime pontes e pontos de articulacao
                        pontes(g);
                        printf("\n Pontos de articulacao:\n");
                        pontosDeArt(g, g->prim);
                    }else{                      //se nao conectado imprime componentes conexas
                        printf("\n Compenentes conexos: \n");
                        fortementeConexo(g);
                    }
                }
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
    for(i = 1; i<=n;i++){                       //para cada cor que existe no grafo...
        no = g->prim;
        while(no){								//imprime o no que tem aquela cor
            if(no->cor == i) printf(" [%d]",no->id_no);
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
                if(!conec && temp > no->id_no) printf(" %d e %d\n", no->id_no, temp);
                                                    //se desconectou, imprime o no e o vizinho que causou isso
                v = v->prox_viz;
            }while((v) && (v->id_viz != prim));
        }
        no = no->prox_no;
    }
}

void pontosDeArt(TG * g, TNo * no){
    if(!g || !no) return;
    pontosDeArt(g, no->prox_no);

    TG * gAux = salvaNo(no);

    int n = no->id_no;
    retira_no(g, n);
    if(!conexo(g)) printf(" %d\n", n);
    insere_no(g,n);
    TNo * noAux = busca_no(gAux, n);
    TViz * v = noAux->prim_viz;
    while(v){
        insere_aresta(g, n, v->id_viz);
        insere_aresta(g, v->id_viz, n);
        v = v->prox_viz;
    }
    libera(gAux);
}

TG * salvaNo(TNo * no){
    TG * g2 = cria();
    insere_no(g2,no->id_no);
    TNo * copia = g2->prim;

    TViz * v = no->prim_viz;
    while(v){
        insere_no(g2, v->id_viz);
        insere_aresta(g2,copia->id_no, v->id_viz);
        v = v->prox_viz;
    }
    return g2;
}

TG* criaGrafo(char* nomeArq){					//le o arquivo e monta o grafo
    FILE* arquivo = fopen(nomeArq,"rt");
    if(!arquivo) exit(1);
    int n,i, r;

    fscanf(arquivo,"%d",&n);					//recebe o tamanho do grafo (1a linha)...

    TG* g = cria();

    for(i = n; i>0; i--){						//... e insere n nos no grafo
        insere_no(g,i);
    }

    r = fscanf(arquivo,"%d%d",&n,&i);			//le as arestas...
    while(r!=EOF){
        insere_aresta(g,n,i);					//...e insere no grafo
        r = fscanf(arquivo,"%d%d",&n,&i);
    }

    fclose(arquivo);							//fecha o arquivo

    return g;									//e retorna o grafo
}
