typedef struct vizi{
	int id_viz, custo;
	struct viz *prox_viz;
}TViz;

typedef struct no{
	int id_no, cor;
	TViz * prim_viz;
	struct no *prox_no;
}TNo;


typedef struct grafo{
	TNo * prim;
    int cores;
}TG;

TNo * busca_no(TG *g, int no){
	TNo *p = g->prim;
	while((p) &&(p->id_no != no)){
		p = p->prox_no;
	}
	return p;
}


void insere_no(TG *g, int no){
	TNo * p = busca_no(g, no);
	if(p) return;
	p = (TNo*) malloc(sizeof(TNo));
	p->id_no = no;
	p->prim_viz = NULL;
	p->prox_no = g->prim;
	p->cor = 0;
	g->prim = p;
}

TG * cria (void){
	TG * g = (TG *) malloc(sizeof(TG));
	g->prim = NULL;
	g->cores = 0;
	return g;
}


TViz * busca_aresta(TG *g,int no1, int no2){
	TNo * viz1= busca_no(g,no1);
	if(!viz1) return NULL;
	TNo *viz2 = busca_no(g, no2);
	if(!viz2) return NULL;

	TViz * p = viz1->prim_viz;
	while((p) && (p->id_viz!=no2)){
		p = p->prox_viz;
	}
	return p;
}

void insere_aresta(TG * g, int id1, int id2){
	TNo * p1 = busca_no(g, id1);
	if(!p1) return;

    TViz * v = busca_aresta(g,id1,id2);
    if(v) return;

	TViz * novoViz = (TViz *) malloc(sizeof(TViz));
	novoViz->id_viz = id2;
	novoViz ->prox_viz = p1->prim_viz;

	p1->prim_viz = novoViz;
}

void imprime(TG *g){
     TNo* p = g->prim;
     while(p){
          printf("%d: cor: %d\n", p->id_no,p->cor);
          TViz * v = p->prim_viz;
          printf("viz:");
          while(v){
               printf("[%d]", v->id_viz, v->custo);
               v = v->prox_viz;
          }
          printf("\n");
          p = p->prox_no;
     }
}

void retira_aresta(TG *g, int id1, int id2){
     TNo *p1 = busca_no(g, id1);
     if(!p1) return;
     TNo * p2 = busca_no(g, id1);
     if(!p2) return;
     TViz *v = p1->prim_viz, *ant = NULL;
     while((v) && (v->id_viz != id2)){
          ant = v;
          v = v->prox_viz;
     }
     if(!v) return;
     if(!ant) p1->prim_viz = v->prox_viz;
     else ant->prox_viz = v->prox_viz;
     free(v);
}

void retira_no(TG * g, int id){
     TNo * p = g->prim, *ant = NULL;
     while((p) && (p->id_no != id)){
          ant = p;
          p = p->prox_no;
     }

     if(!p) return;
     TViz * v = p->prim_viz;
     while(v){
          retira_aresta(g, v->id_viz,id);
          retira_aresta(g, id, v->id_viz);
          v = p->prim_viz;
     }
     if (!ant) g->prim = p->prox_no;
     else ant->prox_no = p->prox_no;
     free(p);
}


void libera (TG *g){
     TNo * p = g->prim;
     while(p){
          TViz *v = p->prim_viz;
          while(v){
               TViz * t = v;
               v = v->prox_viz;
               free(t);
          }
          TNo *q = p;
          p = p->prox_no;
          free(q);
     }
     free(g);
}
