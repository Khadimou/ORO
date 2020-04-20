#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a,b) (a>b) ? (a):b
#define min(a,b) (a<b) ? (a):b

#define MAX 100

struct table{
    int *duree;
    int *antecedent;
    int *nb_antecedents;
    int *tasks; // les tâches
    int nb_tasks; // nombre de taches
    int stock_antecedent[][3] ; // on stocke tous les antécédents dans une matrice
};

int max_tab(int *tab, int nelt);
int min_tab(int *tab, int nelt);
struct table *read_data(char *name_file);
int *date_au_plus_tot(struct table *t);
int *date_au_plus_tard(struct table *t,int *daptot);
int *marge_totale(int *daptot,int *daptard,struct table *t);
void marge_libre(int *mt,int *daptot,struct table *t);