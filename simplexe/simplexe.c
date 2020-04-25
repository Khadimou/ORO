#include "protos.h"

float max_tab(float *tab, int nelt)
{
    int i;
    float maxv;

    maxv = tab[0];
    for( i = 0 ; i < nelt ; i++ )
    {
	    if (tab[i] > maxv)
	    {
	        maxv = tab[i];
	    }
    }
    
    return maxv;
}

float min_tab(float *tab, int nelt)
{
    int i;
    float minv;

    minv = tab[1];
    int inc = 1;
    while(minv<0){
        minv = tab[inc+1];
    }

    for( i = 1 ; i < nelt ; i++ )
    {
	    if ((tab[i] < minv) && (tab[i]>0) )
	    {
	        minv = tab[i];
	    }else
        {
            minv = tab[i];
        }
        
    }
    //printf("pivot min %f\n",minv);
    
    return minv;
}

struct simplexe *read_data(char *file_name){
    FILE *entry = fopen(file_name,"r");
    char data[MAX] = {0};
    struct simplexe *s = (struct simplexe*)malloc(sizeof(struct simplexe)*MAX);
    s->nb_contraintes = 0;

    if(entry != NULL){
        printf("L'ouverture du fichier d'entrée a réussi \n");
    }

    while (fgets(data,MAX,entry) != NULL)
    {
        s->nb_contraintes++;
    }
    printf("Le fichier contient %d contraintes\n",s->nb_contraintes-2);

    s->contraintes = malloc(sizeof(int)*s->nb_contraintes);
    s->borne = malloc(sizeof(int)*s->nb_contraintes);
    s->var1 = malloc(sizeof(int)*s->nb_contraintes);
    s->var2 = malloc(sizeof(int)*s->nb_contraintes);
    s->tableau[s->nb_contraintes-2][MAX];
    memset(s->tableau,0,sizeof(int)*MAX);
    int iter = 0;

    rewind(entry);

    //initialisation
    for(int h=0;h<MAX;h++){
        s->borne[h] = 0;
        s->contraintes[h] = 0;
        s->var1[h] = 0;
        s->var2[h] = 0;
    }

    for(int i=0;i<s->nb_contraintes;i++){
        fgets(data,MAX,entry);
        printf("data: %s",data);

        s->contraintes[i] = atoi(data);
        s->var1[i] = atoi(data+1);
        s->var2[i] = atoi(data+3);
        s->borne[i] = atoi(data+5);
    }
    printf("\n");

    for (int j=1; j<s->nb_contraintes-2; j++){
        printf("\n");
        printf("contrainte : %d\n", s->contraintes[j]);
        printf("x : %d\n", s->var1[s->contraintes[j]]);
        printf("y :  %d\n",s->var2[s->contraintes[j]]);
        printf("bornes : %d\n",s->borne[s->contraintes[j]]);
        s->tableau[0][iter] = s->contraintes[j];
        s->tableau[1][iter] = s->var1[j];
        s->tableau[2][iter] = s->var2[j];
        s->tableau[iter+1][4] = s->borne[j];
        iter++;
    }
    printf("\n");
    s->nb_iter = iter;

    //fonction objective
    printf("contrainte: %d\n",s->contraintes[s->nb_contraintes-1]);

    s->var1[s->contraintes[s->nb_contraintes-1]] = atoi(data+1);
    s->var2[s->contraintes[s->nb_contraintes-1]] = atoi(data+3);
    printf("x : %d\n", s->var1[s->contraintes[s->nb_contraintes-1]]);
    printf("y :  %d\n",s->var2[s->contraintes[s->nb_contraintes-1]]);
    s->tableau[0][4] = s->contraintes[s->nb_contraintes-1];
    s->tableau[3][0] = s->var1[s->contraintes[s->nb_contraintes-1]];
    s->tableau[3][1] = s->var2[s->contraintes[s->nb_contraintes-1]];

    return s;
}

struct simplexe* update_matrice(struct simplexe *s){

    return s;
}

void maximisation(struct simplexe *s){

    float func_objective[s->nb_iter+1];
    for(int j=1;j<s->nb_contraintes-3;j++){
        for(int i=0;i<s->nb_iter+1;i++){
            printf("%f   ",s->tableau[j][i]);
            func_objective[i] = s->tableau[j][i];
        }
        printf("\n");
    }
    float max_obj = max_tab(func_objective,s->nb_iter); // le maximum de la fonction objective
    //printf("max_obj %f\n",max_obj);
    /* Si le max de la fonction objective est négatif l'algo est fini
    et on a plus de valeur positive dans la fonction objective ce qui signifie la fin de l'algorithme*/
    if(max_obj<0){
        printf("Algorithme terminé \n");
        exit(EXIT_FAILURE);
    }
    
    int indice_obj = 0; // indice de la fonction objective
    float col_max[s->nb_contraintes-3]; /* vecteur colonne avec des 0 partout
                                        sauf sur le coeff max qu'on remplace par 1*/
    memset(col_max,0,sizeof(int)*(s->nb_contraintes-3));
    
    for(int i=0;i<s->nb_iter;i++){
        if(func_objective[i] == max_obj){
            indice_obj = i; //indice de la valeur max de la fonction objective
        }
    }

    float pivot[s->nb_contraintes-3]; // tableau qui contient les pivots du col_max
    memset(pivot,0,sizeof(int)*(s->nb_contraintes-3));
    
    float min_pivot,max_vect;
    float coeff[s->nb_contraintes-3]; /* tableau qui contient les coefficients
                                        qui annulent une ligne de col_max de coeff 0*/ 
    memset(coeff,0,sizeof(int)*(s->nb_contraintes-3));

    for(int j=1;j<s->nb_contraintes-4;j++){
        col_max[j] = s->tableau[j][indice_obj];
        pivot[j] = s->tableau[j][s->nb_iter]/col_max[j];
        //printf("pivot %f\n",pivot[j]);
        coeff[j] = 1./col_max[j];
    }
    printf("\n");
    coeff[s->nb_contraintes-4] = 1./s->tableau[s->nb_contraintes-4][indice_obj];

    max_vect = max_tab(col_max,s->nb_contraintes-4); /*valeur max sur la colonne
                                                    qui a le coeff le plus élevé*/

    min_pivot = min_tab(pivot,s->nb_contraintes-4);
    int index; // indice de la ligne qui a le coefficient 1
    for(int a=1;a<s->nb_contraintes-4;a++){
        if(pivot[a] == min_pivot) {
            col_max[a] = 1;
            index = a;
        }else{
            col_max[a] = 0;
        }
        
    }
    
    printf("On remplace dans la base\n");
    float row_index[s->nb_iter+1]; // ligne par laquelle on divise le max_vect
    memset(row_index,0,sizeof(int)*(s->nb_iter+1));

    for(int j=1;j<s->nb_contraintes-3;j++){
        //printf("   %f\n",coeff[j]);
        for(int i=0;i<s->nb_iter+1;i++){
            if(i == indice_obj){
                s->tableau[j][i] = col_max[j];
                row_index[i] = 1;
            }else
            {
                row_index[i] = s->tableau[index][i]/max_vect;
                if(col_max[j] == 1){
                    s->tableau[j][i] = s->tableau[j][i]/max_vect;
                    row_index[i] = s->tableau[j][i];
                }else
                {
                    s->tableau[j][i] = row_index[i] - coeff[j]*s->tableau[j][i];
                } 
            }
            
            printf("%f   ",s->tableau[j][i]);
            func_objective[i] = s->tableau[s->nb_contraintes-4][i];
        }
        printf("\n");
    }  
    printf("solution optimale: \n");
    printf("(%f , %f) de valeur %f\n",s->tableau[1][s->nb_iter],
            s->tableau[2][s->nb_iter],s->var1[s->contraintes[s->nb_contraintes-1]]*
            s->tableau[1][s->nb_iter]+s->var2[s->contraintes[s->nb_contraintes-1]]*
            s->tableau[2][s->nb_iter]);

    s = update_matrice(s);
    if((func_objective[s->nb_iter] > 0) ){
        maximisation(s);
        printf("\n"); 
    }
     
}

void minimisation(struct simplexe *s){

    float func_objective[s->nb_iter+1];
    for(int j=1;j<s->nb_contraintes-3;j++){
        for(int i=0;i<s->nb_iter+1;i++){
            printf("%f   ",s->tableau[j][i]);
            func_objective[i] = s->tableau[j][i];
        }
        printf("\n");
    }
    float min_obj = min_tab(func_objective,s->nb_iter);
    //printf("min_obj %f\n",min_obj);
    
    int indice_obj = 0; // indice de la fonction objective
    float col_max[s->nb_contraintes-3]; /* vecteur colonne avec des 0 partout
                                        sauf sur le coeff max qu'on remplace par 1*/
    memset(col_max,0,sizeof(int)*(s->nb_contraintes-3));
    
    for(int i=0;i<s->nb_iter;i++){
        if(func_objective[i] == min_obj){
            indice_obj = i;
        }
    }

    float pivot[s->nb_contraintes-3];
    memset(pivot,0,sizeof(int)*(s->nb_contraintes-3));
    
    float min_pivot,min_vect;
    float coeff[s->nb_contraintes-3];
    memset(coeff,0,sizeof(int)*(s->nb_contraintes-3));

    for(int j=1;j<s->nb_contraintes-4;j++){
        col_max[j] = s->tableau[j][indice_obj];
        pivot[j] = s->tableau[j][s->nb_iter]/col_max[j];
        //printf("pivot %f\n",pivot[j]);
        coeff[j] = 1./col_max[j];
    }
    printf("\n");
    coeff[s->nb_contraintes-4] = 1./s->tableau[s->nb_contraintes-4][indice_obj];

    min_vect = min_tab(col_max,s->nb_contraintes-4); /*valeur max sur la colonne
                                                    qui a le coeff le plus élevé*/

    min_pivot = min_tab(pivot,s->nb_contraintes-4);
    int index; // indice de la ligne qui a le coefficient 1
    for(int a=1;a<s->nb_contraintes-4;a++){
        if(pivot[a] == min_pivot) {
            col_max[a] = 1;
            index = a;
        }else{
            col_max[a] = 0;
        }
        
    }
    
    printf("On remplace dans la base\n");
    float row_index[s->nb_iter+1]; // ligne par laquelle on divise le max_vect
    memset(row_index,0,sizeof(int)*(s->nb_iter+1));

    for(int j=1;j<s->nb_contraintes-3;j++){
        //printf("   %f\n",coeff[j]);
        for(int i=0;i<s->nb_iter+1;i++){
            if(i == indice_obj){
                s->tableau[j][i] = col_max[j];
                row_index[i] = 1;
            }else
            {
                row_index[i] = s->tableau[index][i]/min_vect;
                if(col_max[j] == 1){
                    s->tableau[j][i] = s->tableau[j][i]/min_vect;
                    row_index[i] = s->tableau[j][i];
                }else
                {
                    s->tableau[j][i] = row_index[i] - coeff[j]*s->tableau[j][i];
                } 
            }
            
            printf("%f   ",s->tableau[j][i]);
            func_objective[i] = s->tableau[s->nb_contraintes-4][i];
        }
        printf("\n");
    }  
    printf("solution optimale: \n");
    printf("(%f , %f) de valeur %f\n",s->tableau[1][s->nb_iter],
            s->tableau[2][s->nb_iter],s->var1[s->contraintes[s->nb_contraintes-1]]*
            s->tableau[1][s->nb_iter]+s->var2[s->contraintes[s->nb_contraintes-1]]*
            s->tableau[2][s->nb_iter]);

    s = update_matrice(s); 
    // itération
    int iter =0;

    if(func_objective[s->nb_iter]>0){
        printf("\n");
        minimisation(s);
    }

    // if((func_objective[iter]>0) || (func_objective[iter+1]>0) || 
    // (func_objective[iter+2]>0) || (func_objective[iter+3]>0) ||
    // (func_objective[iter+4]>0))
    // {
    //     printf("\n");
    //     minimisation(s);
    //     free(s);
    // }
    
}