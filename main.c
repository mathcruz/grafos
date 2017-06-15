#include <stdio.h>
#include <stdlib.h>
#include <grafo.h>

int orientado(TG*g);
int colorir(TG * g);
void descolorir(TG *g);
void colorirViz(TG * g,TNo* no, int cor);
int caminho(TG *g, TNo * no1, TNo * no2);
int conexo(TG * g);
void pontes(TG * g);
TG* criaGrafo(char* nomeArq);

int main(int argc, char* argv[])
{
    if(argc<2) return 0;
    TG *g = criaGrafo(argv[1]);
    int i, n, r;
    int dir = orientado(g);
    if(!dir) colorir(g);
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
                printf("\n Digite valor de no1 e no2:");
                scanf("%d%d",&n,&r);
                TNo * no1 = busca_no(g,n);
                TNo * no2 = busca_no(g, r);
                if(caminho(g,no1, no2)) printf("ha caminho");
                else printf("nao ha caminho\n");
                pontes(g);
                //se orientado imprimir componentes fortemente conexas
                //se nao orientado imprimir: se eh conectado (pontes e pontos de art); se nao conectado (componentes conectadas)
                break;

        }

    }
}

int orientado(TG*g){ //verifica se eh orientado (1) ou nao orientado(0)
    TNo * p = g->prim;

    while(p){
        TViz * v = p->prim_viz;
        while(v){
            TViz * ida = busca_aresta(g,p->id_no,v->id_viz);
            TViz * volta = busca_aresta(g,v->id_viz,p->id_no);
            if(ida == NULL || volta == NULL) return 1;
            v = v->prox_viz;
        }
        p = p->prox_no;
    }
    return 0;
}

int colorir(TG * g){
    descolorir(g);
    TNo * p = g->prim;
    int cor = 0;
    while(p){
        if(p->cor==0)colorirViz(g, p, ++cor);
        p = p->prox_no;
    }
    g->cores = cor;
    return cor;
}
void descolorir(TG *g){
    TNo * p = g->prim;
    while(p){
        p->cor = 0;
        p = p->prox_no;
    }
    g->cores = 0;
}

void colorirViz(TG * g,TNo* no, int cor){
    if(!g || !no) return;
    if(no->cor !=0) return;

    no->cor = cor;

    TViz * v = no->prim_viz;
    while(v){
        TNo * viz = busca_no(g, v->id_viz);
        colorirViz(g, viz,cor);
        v = v->prox_viz;
    }
}

int caminho(TG *g, TNo * no1, TNo * no2){
    if (!g || !no1 || !no2) return 0;
    if(no2->id_no == no1->id_no) return 1;
    if(no1->cor ==  -1) return 0;
    int cor = no1->cor, resp=0;
    no1->cor = -1;
    TViz *v = no1->prim_viz;
    while(v && resp!=1){
        TNo * vizNo = busca_no(g, v->id_viz);
        resp = caminho(g,vizNo,no2);
        v = v->prox_viz;
    }
    no1->cor = cor;
    return resp;
}

int conexo(TG * g){
    TNo * no = g->prim;
    TNo * no2 = no->prox_no;
    while(no2){
        if(!caminho(g,no, no2)) return 0;
        no2 = no2->prox_no;
    }
    return 1;
}

void pontes(TG * g){
    TNo * no = g->prim;
    int conec = conexo(g);
    if(!conec) return;
    while(no){
        TViz * v = no->prim_viz;
        int prim, temp;
        if(v){
            prim = v->id_viz;
            do{
                temp = v->id_viz;
                retira_aresta(g, no->id_no, temp);
                retira_aresta(g, temp, no->id_no);
                conec = conexo(g);
                insere_aresta(g, no->id_no, temp);
                insere_aresta(g, temp, no->id_no);
                if(!conec) printf("ponte: %d e %d\n", no->id_no, temp);
                v = v->prox_viz;
            }while((v) && (v->id_viz != prim));
        }
        no = no->prox_no;
    }
}

TG* criaGrafo(char* nomeArq){
    FILE* arquivo = fopen(nomeArq,"rt");
    if(!arquivo) exit(1);
    int n,i, r;

    fscanf(arquivo,"%d",&n);

    TG* g = cria();

    for(i = 0; i< n; i++){
        insere_no(g,i+1);
    }

    r = fscanf(arquivo,"%d%d",&n,&i);
    while(r!=EOF){
        insere_aresta(g,n,i);
        r = fscanf(arquivo,"%d%d",&n,&i);
    }

    fclose(arquivo);

    return g;
}
