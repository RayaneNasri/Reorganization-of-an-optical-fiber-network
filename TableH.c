#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Graphe.h"
#include "Reseau.h"
#include "TableH.h"


/* Variable globale */
double B = (sqrt(5) - 1) / 2;

/* Fonction auxiliaires */

int hach2(Noeud* n1, Noeud* n2, int m){

    double h1 = (n1-> y) + (((n1-> x) + (n1-> y))*((n1-> x) + (n1-> y) + 1) / 2);
    double h2 = (n2-> y) + (((n2-> x) + (n2-> y))*((n2-> x) + (n2-> y) + 1) / 2);

    double h;
    if(h1 > h2){
        h = h2 + ((h1 + h2)*(h1 + h2 + 1) / 2);
    }
    else{
        h = h1 + ((h1 + h2)*(h1 + h2 + 1) / 2); 
    }

    return (int)(m * (h * B - (int)(h * B)));
}

/* Fonctions principales */

TableH* init_table(int m){

    TableH* H = (TableH*) malloc(sizeof(TableH));
    H-> m = m;
    H-> tab = (Cellule_arete**) calloc(m, sizeof(Cellule_arete *));

    return H;
}

Arete* in_table(TableH* H, Noeud* n1, Noeud* n2){

    int i = hach2(n1, n2, H-> m);

    Cellule_arete* tmp = (H-> tab)[i];
    
    while(tmp != NULL){
        Arete* tmp_Arete = tmp-> a;

        if( ((n1-> num == tmp_Arete-> u) && (n2-> num == tmp_Arete-> v)) || ((n2-> num == tmp_Arete-> u) && (n1-> num == tmp_Arete-> v)) ){
            return tmp_Arete;
        }

        tmp = tmp-> suivant;
    }

    // On crée l'arête :
    Arete* new_Arete = (Arete*) malloc(sizeof(Arete));
    new_Arete-> u = n1-> num;
    new_Arete-> v = n2-> num;

    // On ajoute l'arête dans la table de hachage :
    Cellule_arete* new = malloc(sizeof(Cellule_arete));
    new-> a = new_Arete;
    new-> suivant = NULL;
    inserer_en_tete_arete(&((H-> tab)[i]), new);

    return new_Arete;
}

