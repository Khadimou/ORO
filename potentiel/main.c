#include "protos.h"


int main()
{
    
    char data[MAX] = "entree.txt";
    struct table *t = read_data(data);
    int *res = date_au_plus_tot(t);
    int *daptard = date_au_plus_tard(t,res); 
    int *mt = marge_totale(res,daptard,t);
    marge_libre(mt,res,t);
    
    free(t);

    return 0;
}