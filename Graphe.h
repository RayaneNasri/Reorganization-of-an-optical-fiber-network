#ifndef __GRAPHE_H__
#define __GRAPHE_H__

#include <stdio.h>

#include "Reseau.h"
#include "Struct_File.h"

typedef struct{
    int u, v;
    int poids;
} Arete;

typedef struct cellule_arete{
    Arete* a;

    struct cellule_arete* suivant;
} Cellule_arete;

typedef struct{
    int num;
    double x, y;
    Cellule_arete* L_Voisins;
} Sommet;

typedef struct{
    int e1, e2;
} Commod;

typedef struct{
    int nbSom;
    Sommet** T_som;
    int gamma;
    int nbcommod;
    Commod* T_commod;
} Graphe;

/* ~~~~~~ Fonctions auxiliaires ~~~~~~ */
Sommet* sommetNum(Graphe* G, int num);
void inserer_en_tete_arete(Cellule_arete** L, Cellule_arete* cell_arete);
void detruireGraphe(Graphe* G);


/* ~~~~~~ Fonctions principales ~~~~~~ */
Graphe* creerGraphe(Reseau* R);
int min_nombre_aretes(Graphe* G, Sommet* u, Sommet* v);
S_file* plus_court_chemin(Graphe* G, Sommet* u, Sommet* v);
int reorganiseReseau(Reseau* r);

#endif