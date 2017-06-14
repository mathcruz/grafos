#include <stdio.h>
#include <stdlib.h>
#include <grafo.h>

int orientado(TG*g);
void pintarBack(TG * g,TNo* no, int cor, int controle);
int pintar(TG * g);
TG* criaGrafo(char* nomeArq);
int main(int argc, char* argv[])
{
    if(argc<2) return 0;
    TG *g = criaGrafo(argv[1]);
    int i, n, r;
    int dir = orientado(g);
    if(!dir) pintar(g);
    printf("dir: %d\n", dir);
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
                    if(!dir) pintar(g);
                }else if(n == 2){
                    printf("\n Digite valor de no1 e no2:");
                    scanf("%d%d",&n,&r);
                    insere_aresta(g,n,r);
                    if(dir == 0){
                        insere_aresta(g,r,n);
                        pintar(g);
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
                }else if(n == 2){
                    printf("\n Digite valor de no1 e no2:");
                    scanf("%d%d",&n,&r);
                    retira_aresta(g,n,r);
                    if(dir == 0){
                        retira_aresta(g,r,n);
                        pintar(g);
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

int pintar(TG * g){
    descolorir(g);
    TNo * p = g->prim;
    g->cores = 0;
    //pintarBack(g,p,1,p->id_no);
    //pinta2(g);
    pintarViz(g,p,1);
    return g->cores;
}
void descolorir(TG *g){
    TNo * p = g->prim;
    while(p){
        p->cor = 0;
        p = p->prox_no;
    }
}
void pintarBack(TG * g,TNo* no, int cor, int controle){
    if(!g || !no) return;

    if(no->cor !=0 ){
        if(no->prox_no) pintarBack(g,no->prox_no, cor, no->prox_no->id_no);
        return;
    }
    no->cor = cor;

    TViz * v = no->prim_viz;
    while(v){
        TNo * viz = busca_no(g, v->id_viz);
        if(viz->cor == 0)pintarBack(g, viz,cor, controle);
        else no->cor = viz->cor;
        v = v->prox_viz;
    }
    if(cor != no->cor) cor--;
    if(cor>g->cores) g->cores = cor;
    if(controle == no->id_no && no->prox_no) pintarBack(g, no->prox_no, cor+1, no->prox_no->id_no);
}
void pintarViz(TG * g,TNo* no, int cor){
    if(!g || !no) return;

    if(no->cor !=0 )cor = no->cor;
    else no->cor = cor;

    TViz * v = no->prim_viz;
    while(v){
        TNo * viz = busca_no(g, v->id_viz);
        viz->cor = cor;
        v = v->prox_viz;
    }
    pintarViz(g, no->prox_no, cor+1);
}
/*
void pinta2(TG *g){
    TNo *no = g->prim;
    int cor=1;
    while(no){
        TNo * no2 = g->prim;
        if(no->cor!=0){
            no = no->prox_no;
            continue;
        }
        no->cor = cor;
        while(no2){
            if(no2->cor==0 && caminho(g,no, no2))
                no2->cor = no->cor;
            no2 = no2->prox_no;
        }
        cor++;
        no=no->prox_no;
    }
}*/

int caminho(TG *g, TNo * no1, TNo * no2){
    if (!g || !no1 || !no2) return 0;
    if(no2->id_no == no1->id_no) return 1;
    if(no1->cor) return 0;
    no1->cor = 1;
    TViz *v = no1->prim_viz;
    while(v){
        TNo * vizNo = busca_no(g, v->id_viz);
        if(caminho(g,vizNo,no2))return 1;
        v = v->prox_viz;
    }
    return 0;
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
