#include "protos.h"

#define nb_max_antecedent 3 // une tâche a au maximum nb_max_antecedent

int max_tab(int *tab, int nelt)
{
    int i, maxv;

    maxv = tab[0];
    for( i = 1 ; i < nelt ; i++ )
    {
	    if (tab[i] > maxv)
	    {
	        maxv = tab[i];
	    }
    }
    return maxv;
}


struct table *read_data(char *name_file)
{
    FILE *entry = fopen(name_file,"r");
    int i,j,h;
    char donnees[MAX] = {0};
    struct table *t = (struct table*)malloc(sizeof(struct table)*MAX);
    t->nb_tasks = 0;

    printf("Le fichier à ouvrir est : %s\n", name_file);
    
    if(entry != NULL){
        printf("L'ouverture du fichier %s a reussi !\n", name_file);
 
        while (fgets(donnees, MAX, entry) != NULL)
            t->nb_tasks++;
 
        printf("Le fichier contient %d tâches.\n", t->nb_tasks);
 
        t->tasks = malloc(t->nb_tasks * sizeof(int));
        t->duree = malloc(t->nb_tasks * sizeof(int));
        t->antecedent = malloc(nb_max_antecedent*t->nb_tasks * sizeof(int));
        t->nb_antecedents = malloc(t->nb_tasks * sizeof(int));

        if (t->tasks == NULL && (t->duree == NULL) && (t->antecedent == NULL) 
        && (t->nb_antecedents == NULL))
        {
            printf("Erreur allocation !");
            exit(0);
        }
        else
        {
            printf("Allocation reussie !\n");
        }
 
        rewind(entry); // on se place au début du fichier d'entrée

        for (h=0; h<MAX; h++){
            t->tasks[h] = 0.0;
            t->duree[h] = 0.0;
            t->antecedent[h] = 0.0;
            t->nb_antecedents[h] = 0.0;
        }
        char s[6] = ", ,";
        char *token;
        
        t->stock_antecedent[t->nb_tasks][nb_max_antecedent];
        memset(t->stock_antecedent,0,t->nb_tasks*nb_max_antecedent*sizeof(int));
        int inc=0;
        for (i=0; i<t->nb_tasks; i++)
        {
            fgets(donnees, MAX, entry);
            printf("données : %s\n", donnees);
 
            t->tasks[i] = atoi(donnees);
            t->duree[t->tasks[i]] = atoi(donnees+1);
            t->nb_antecedents[t->tasks[i]] = atoi(donnees+3);
            t->antecedent[t->tasks[i]] = atoi(donnees+5);
            t->stock_antecedent[t->tasks[i]][0] = t->antecedent[t->tasks[i]];
            
            inc = 1;
            
            while (inc <= t->nb_antecedents[t->tasks[i]]-1)
            {
                token = strtok(donnees+7, s);   
                //printf("%s\t",token);
                if(inc == 2){
                    token = strtok(donnees+10, s);
                    t->stock_antecedent[t->tasks[i]][nb_max_antecedent-1] = atoi(token);
                }else
                {
                    t->stock_antecedent[t->tasks[i]][1] = atoi(token);
                }

                t->antecedent[t->tasks[i]] = atoi(token);
                inc++;
            }  
             
        }
      
        for (j=0; j<t->nb_tasks; j++){
            printf("\n");
            printf("tâche : %d\n", t->tasks[j]);
            printf("duree : %d\n", t->duree[t->tasks[j]]);
            printf("nombre d'antecedents : %d\n", t->nb_antecedents[t->tasks[j]]);
        }
    }else
        printf("L'ouverture du fichier %s a echouee !\n", name_file);

    return t;
}

int *date_au_plus_tot(struct table *t)
{
    static int c_t[MAX]; //date au plus tôt
    for(int a=0;a<t->nb_tasks;a++){
        if(t->nb_antecedents[t->tasks[a]] == 0){
            c_t[t->tasks[a]] = 0;
        }else if(t->nb_antecedents[t->tasks[a]] == 1){
            c_t[t->tasks[a]] = t->duree[t->stock_antecedent[t->tasks[a]][0]] + c_t[t->stock_antecedent[t->tasks[a]][0]];
        }else
        {
            if(t->nb_antecedents[t->tasks[a]]==2){
                c_t[t->tasks[a]] = max(t->duree[t->stock_antecedent[t->tasks[a]][1]]+c_t[t->stock_antecedent[t->tasks[a]][1]],
                t->duree[t->stock_antecedent[t->tasks[a]][0]]+c_t[t->stock_antecedent[t->tasks[a]][0]]);
            }else
            {
                int tableau[3] = {t->duree[t->stock_antecedent[t->tasks[a]][0]]+c_t[t->stock_antecedent[t->tasks[a]][0]],
                                t->duree[t->stock_antecedent[t->tasks[a]][1]]+c_t[t->stock_antecedent[t->tasks[a]][1]],
                                t->duree[t->stock_antecedent[t->tasks[a]][2]]+c_t[t->stock_antecedent[t->tasks[a]][2]]};
                c_t[t->tasks[a]] = max_tab(tableau,3);
            }
            
        }
    }

    // for (int k=0; k<t->nb_tasks; k++){
    //         for(int col=0;col<3;col++){
    //             printf("col %d stock_antecedent : %d\n",col, t->stock_antecedent[k][col]);
    //         }
    //         printf("-------------------\n");
    //     }
    printf("date au plus tôt: \n");
    for(int i=0;i<t->nb_tasks;i++){
        printf("%d ",c_t[t->tasks[i]]);
    }
    printf("\n");

    return c_t;
}

int *date_au_plus_tard(struct table *t,int *daptot)
{
    int c_t_prime[MAX]; //date au plus tard
    memset(c_t_prime,0,t->nb_tasks*sizeof(int));
    static int result[MAX];
    memset(result,0,t->nb_tasks*sizeof(int));
    c_t_prime[t->tasks[t->nb_tasks-1]] = daptot[t->tasks[t->nb_tasks-1]];
    result[t->tasks[t->nb_tasks-1]] = c_t_prime[t->tasks[t->nb_tasks-1]];
    int stock_task1[MAX];
    int stock_task2[MAX];
    int iter = 0;

    // stockage des tâches et de leur antécédent
    for(int a=t->nb_tasks-2;a>=0;a--){
        
        for(int k=0;k<t->nb_tasks;k++){
            for(int j=0;j<nb_max_antecedent;j++){

                if(t->tasks[a] == t->stock_antecedent[t->tasks[k]][j] ){
            
                    //printf("%d antécédent de %d\n",t->tasks[a],t->tasks[k]);

                    stock_task1[iter] = t->tasks[a];
                    stock_task2[iter] = t->tasks[k];
                    iter++;
                    }
                 
                }  

            }      
        } 

    int stock_daptard[t->nb_tasks][2]; // matrice pour stocker toutes les dates au plus tard de chaque tâche
    memset(stock_daptard,0,t->nb_tasks*sizeof(int));
    int taille = 0;
    int indice[MAX]; //tableau d'indice
    
    // calcul des dates au plus tard
    for(int index=0;index<iter;index++){
            
        if(stock_task1[index] != stock_task1[index+1] && (stock_task1[index] != stock_task1[index-1])){
            c_t_prime[stock_task1[index]] = c_t_prime[stock_task2[index]] - t->duree[stock_task1[index]];
            result[stock_task1[index]] = c_t_prime[stock_task1[index]];
        }
        else
        {
            if(stock_task1[index] != stock_task1[index+1]){

                c_t_prime[stock_task1[index]] = min(c_t_prime[stock_task2[index]],c_t_prime[stock_task2[index-1]]) - t->duree[stock_task1[index]];
                
                stock_daptard[taille][0] = c_t_prime[stock_task1[index]];
                indice[taille] = stock_task1[index];
                taille++;
            }else
            {
                c_t_prime[stock_task1[index]] = min(c_t_prime[stock_task2[index]],c_t_prime[stock_task2[index+1]]) - t->duree[stock_task1[index]];
                
                stock_daptard[taille][1] = c_t_prime[stock_task1[index]];
            }
        
        }
            
    }

    for(int i=0;i<taille;i++){
        result[indice[i]] = min(stock_daptard[i][0],stock_daptard[i][1]);
    }

    printf("date au plus tard: \n");
    for(int i=0;i<t->nb_tasks;i++){
        printf("%d ",result[t->tasks[i]]);
    }
    printf("\n");

    return result;
}

int *marge_totale(int *daptot,int *daptard,struct table *t){
    static int mt[MAX]; // marge totale
    printf("marge totale:\n");
    for(int i=0;i<t->nb_tasks;i++){
        mt[i] = daptard[t->tasks[i]] - daptot[t->tasks[i]];
        printf("%d ",mt[i]);
    }
    printf("\n");

    return mt;
}

void marge_libre(int *mt,int *daptot,struct table *t){
    static int ml[MAX];
    int stock_task1[MAX];
    int stock_task2[MAX];
    int iter = 0;


    for(int index = 0;index<t->nb_tasks;index++){
        if(mt[index] == 0){
            ml[t->tasks[index]] = 0;
        }else
        {
            for(int a=t->nb_tasks-2;a>=0;a--){
            for(int j=0;j<nb_max_antecedent;j++){
                if(t->tasks[index] == t->stock_antecedent[t->tasks[a]][j] ){
                    stock_task1[iter] = t->tasks[index];
                    stock_task2[iter] = t->tasks[a];
                    iter++;

                }
            }
            }

            // calcul des marges libres
            for(int i=0;i<iter;i++){

                if(stock_task1[i] == stock_task1[i+1] && (stock_task1[i] != stock_task1[i-1])){
                    ml[stock_task1[i]] = min(daptot[stock_task2[i]],daptot[stock_task2[i+1]]) - daptot[stock_task1[i]] - t->duree[stock_task1[i]];
                }else if(stock_task1[i] != stock_task1[i+1] && (stock_task1[i] == stock_task1[i-1]))
                {
                    ml[stock_task1[i]] = min(daptot[stock_task2[i]],daptot[stock_task2[i-1]]) - daptot[stock_task1[i]] - t->duree[stock_task1[i]];
                }else
                {
                    ml[stock_task1[i]] = daptot[stock_task2[i]] - daptot[stock_task1[i]] - t->duree[stock_task1[i]];
                }
                
                
            }
        }
        
    }

    printf("MARGE LIBRE: \n");
    for(int i=0;i<t->nb_tasks;i++){
        printf("%d ",ml[t->tasks[i]]);
    }
    printf("\n");
}