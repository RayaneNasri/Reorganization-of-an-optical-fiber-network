#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Reseau.h"
#include "Hachage.h"

double A = (sqrt(5) - 1) / 2;

/* ~~~~~~ Fonctions auxiliaires ~~~~~~ */
double key(double x, double y){
    return y + ((x + y)*(x + y + 1) / 2);
}

int hach(double k, int m){
    return (int)(m * (k * A - (int)(k * A)));
}

TableHachage* initTableHachage(int m){
    TableHachage* H = (TableHachage*) malloc(sizeof(TableHachage));
    if(H == NULL){
        fprintf(stderr, "FILE: %s LIGNE: %d Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }

    H-> m = m;
    H-> nbElem = 0;
    H-> tab = (CellNoeud**) malloc(m * sizeof(CellNoeud*));

    if(H-> tab == NULL){
        fprintf(stderr, "FILE: %s LIGNE: %d Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }

    for(int i = 0; i < m; i++){
        H-> tab[i] = NULL;
    }

    return H;
}

void liberer_table_hachage(TableHachage* H){
    if(H == NULL){
        return;
    }

    CellNoeud* tmp;
    CellNoeud* tmp2;

    for(int i = 0; i < H-> m; i++){
        tmp = H-> tab[i];
        while(tmp){
            tmp2 = tmp;
            tmp = tmp-> suiv;
            free(tmp2);
        }
    }

    free(H-> tab);
    free(H);
}

/* ~~~~~~ Fonctions principales ~~~~~~ */
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){

    if(H == NULL){
        fprintf(stderr, "FILE: %s LIGNE: %d Erreur: TableHachage non initialisee\n", __FILE__, __LINE__);
        return NULL;
    }

    int i = hach(key(x, y), H-> m);
    CellNoeud* cell = H-> tab[i];

    while(cell != NULL){
        if(((cell-> nd)-> x == x) && ((cell-> nd)-> y == y)){
            return cell-> nd;
        }
        cell = cell-> suiv;
    }

    Noeud* new = creer_noeud((R-> nbNoeuds)++, x, y);

    inserer_en_tete_noeud(&(R-> noeuds), new);

    inserer_en_tete_noeud(&(H-> tab[i]), new);
    H-> nbElem = (H-> nbElem) + 1;

    return new;
}

Reseau* reconstitueReseauHachage(Chaines* C, int M){
    if(C == NULL){
        return NULL;
    }

    // Allocation et initialisation du reseau.
    Reseau* res = (Reseau*) malloc(sizeof(Reseau));
    if(res == NULL){
        fprintf(stderr, "FILE: %s LIGNE: %d Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }

    res-> nbNoeuds = 0;
    res-> gamma = C-> gamma;
    res-> noeuds = NULL;
    res-> commodites = NULL;

    // Allocation et initialisation de la table de hachage.
    TableHachage* H = initTableHachage(M);

    CellChaine* tmp_chaine = C-> chaines;

    CellPoint* tmp_point;
    Noeud* tmp_nd;
    Noeud* prec_nd;

    Noeud* extrA;
    Noeud* extrB;

    // On parcourt une à une chaque chaîne:
    while(tmp_chaine){
        prec_nd = NULL;
        tmp_point = tmp_chaine-> points;

        // On parcourt chaque point de la chaîne:
        while(tmp_point){
            tmp_nd = rechercheCreeNoeudHachage(res, H, tmp_point-> x, tmp_point-> y);

            if(prec_nd != NULL){

                // On met à jour la liste des voisins de p et celles de ses voisins.
                update_voisins(tmp_nd, prec_nd);
                
            }

            else{
                extrA = tmp_nd;
            }

            if(tmp_point-> suiv == NULL){
                extrB = tmp_nd;
            }

            prec_nd = tmp_nd;
            tmp_point = tmp_point-> suiv;
        }

        // On ajoute les commodités:
        inserer_en_tete_commodite(&(res-> commodites), extrA, extrB);
        tmp_chaine = tmp_chaine-> suiv;
    }

    liberer_table_hachage(H);
    return res;
}

            

        
    
