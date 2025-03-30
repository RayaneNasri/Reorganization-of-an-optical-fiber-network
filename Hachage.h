#ifndef __HACHAGE_H__
#define __HACHAGE_H__

#include "Chaine.h"
#include "Reseau.h"

extern double A;

typedef struct{
    int m; /* Taille de la table */
    int nbElem; /* Nombre d'élements stockés dans la table */
    
    CellNoeud** tab; /* Tableau de pointeurs vers des noeuds */
} TableHachage;

/* ~~~~~~ Fonctions auxiliaires ~~~~~~ */
double key(double x, double y);
int hach(double k, int m);

/* ~~~~~~ Fonctions principales ~~~~~~ */
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y);
Reseau* reconstitueReseauHachage(Chaines *C, int M);

#endif