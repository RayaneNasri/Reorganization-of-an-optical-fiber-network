#ifndef TABLEH_H
#define TABLEH_H

#include <stdio.h>

#include "Graphe.h"

typedef struct
{
    int m;
    Cellule_arete** tab;
} TableH;

/* Fonctions auxiliaires */

int hach2(Noeud* n1, Noeud* n2, int m);

/* Fonctions principales */

TableH* init_table(int m);
Arete* in_table(TableH* H, Noeud* n1, Noeud* n2);

#endif