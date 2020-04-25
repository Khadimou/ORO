#ifndef _PROTOS_H_
#define _PROTOS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX 100

struct simplexe{
    int *contraintes;
    int *var1;
    int *var2;
    int *borne;
    int nb_contraintes;
    int nb_iter;
    float tableau[][MAX];
};

struct simplexe *read_data(char *file_name);

struct simplexe* update_matrice(struct simplexe *s);

void maximisation(struct simplexe *s);

void minimisation(struct simplexe *s);

float min_tab(float *tab, int nelt);

float max_tab(float *tab, int nelt);

#endif