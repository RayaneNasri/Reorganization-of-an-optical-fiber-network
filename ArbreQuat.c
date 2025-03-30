#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "SVGwriter.h"
#include "Chaine.h"
#include "ArbreQuat.h"

/* ~~~~~~ Fonctions auxiliaires ~~~~~~ */
ArbreQuat* init_parent(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){

    chaineCoordMinMax(C, xmin, ymin, xmax, ymax);

    double xc = (*xmin);
    double yc = (*ymin);
    double coteX = ((*xmax) - (*xmin)) * 2;
    double coteY = ((*ymax) - (*ymin)) * 2;

    return creerArbreQuat(xc, yc, coteX, coteY);
}

void liberer_ArbreQuat(ArbreQuat* aq){
    if(aq == NULL){
        return;
    }

    liberer_ArbreQuat(aq-> so);
    liberer_ArbreQuat(aq-> se);
    liberer_ArbreQuat(aq-> no);
    liberer_ArbreQuat(aq-> ne);

    free(aq);
}

/* ~~~~~~ Fonctions principales ~~~~~~ */
void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    
    CellChaine* tmp_chaine = C-> chaines;
    CellPoint* tmp_point;

    while(tmp_chaine != NULL){
        tmp_point = tmp_chaine-> points;

        while(tmp_point != NULL){

            if((*xmin) > (tmp_point-> x)) *xmin = (tmp_point-> x);
            if((*ymin) > (tmp_point-> y)) *ymin = (tmp_point-> y);

            if((tmp_point-> x) > (*xmax)) *xmax = (tmp_point-> x);
            if((tmp_point-> y) > (*ymax)) *ymax = (tmp_point-> y);

            tmp_point = tmp_point-> suiv;
        }

        tmp_chaine = tmp_chaine-> suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){

    ArbreQuat* res = malloc(sizeof(ArbreQuat));
    if(res == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }

    res-> xc = xc;
    res-> yc = yc;

    res-> coteX = coteX;
    res-> coteY = coteY;

    res-> noeud = NULL;
    res-> so = NULL;
    res-> se = NULL;
    res-> no = NULL;
    res-> ne = NULL;

    return res;
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
    
    double xc, yc, coteX, coteY;
    
    // Si l'arbre est vide:
    if(*a == NULL){
        if( ((parent-> xc) > (n-> x)) && ((parent-> yc) > (n-> y)) ){
            
            xc = parent-> xc - (parent-> coteX / 4);
            yc = parent-> yc - (parent-> coteY / 4);
            coteX = parent-> coteX / 2;
            coteY = parent-> coteY / 2;
            *a = creerArbreQuat(xc, yc, coteX, coteY);
            // On insère le noeud.
            (*a)-> noeud = n;

            // On met à jour le pointeur du parent.
            parent-> so = *a;

            return;
        }

        if( ((parent-> xc) <= (n-> x)) && ((parent-> yc) > (n-> y)) ){
            
            xc = parent-> xc + (parent-> coteX / 4);
            yc = parent-> yc - (parent-> coteY / 4);
            coteX = parent-> coteX / 2;
            coteY = parent-> coteY / 2;
            *a = creerArbreQuat(xc, yc, coteX, coteY);
            // On insère le noeud.
            (*a)-> noeud = n;

            // On met à jour le pointeur du parent.
            parent-> se = *a;

            return;
        }

        if( ((parent-> xc) <= (n-> x)) && ((parent-> yc) <= (n-> y)) ){
            
            xc = parent-> xc + (parent-> coteX / 4);
            yc = parent-> yc + (parent-> coteY / 4);
            coteX = parent-> coteX / 2;
            coteY = parent-> coteY / 2;
            *a = creerArbreQuat(xc, yc, coteX, coteY);
            // On insère le noeud.
            (*a)-> noeud = n;

            // On met à jour le pointeur du parent.
            parent-> ne = *a;

            return;
        }

        if( ((parent-> xc) > (n-> x)) && ((parent-> yc) <= (n-> y)) ){
            
            xc = parent-> xc - (parent-> coteX / 4);
            yc = parent-> yc + (parent-> coteY / 4);
            coteX = parent-> coteX / 2;
            coteY = parent-> coteY / 2;
            *a = creerArbreQuat(xc, yc, coteX, coteY);
            // On insère le noeud.
            (*a)-> noeud = n;

            // On met à jour le pointeur du parent.
            parent-> no = *a;

            return;
        }
    }

    // Si l'arbre est une feuille:
    if ( (*a)-> noeud != NULL ){
        Noeud* tmp = (*a)-> noeud;

        (*a)-> noeud = NULL;

        // On vérifie dans quel quadrant on va insérer le noeud déja présent.
        if (((*a)-> xc) > (tmp-> x) && ((*a)-> yc) > (tmp-> y)){
            insererNoeudArbre(tmp, &((*a)-> so), *a);
        }
        if (((*a)-> xc) <= (tmp-> x) && ((*a)-> yc) > (tmp-> y)){
            insererNoeudArbre(tmp, &((*a)-> se), *a);
        }
        if (((*a)-> xc) <= (tmp-> x) && ((*a)-> yc) <= (tmp-> y)){
            insererNoeudArbre(tmp, &((*a)-> ne), *a);
        }
        if (((*a)-> xc) > (tmp-> x) && ((*a)-> yc) <= (tmp-> y)){
            insererNoeudArbre(tmp, &((*a)-> no), *a);
        }

        // On vérifie dans quel quadrant on va insérer le nouveau noeud.
        if(((*a)-> xc) > (n-> x) && ((*a)-> yc) > (n-> y)){
            insererNoeudArbre(n, &((*a)-> so), *a);
            return;
        }
        if(((*a)-> xc) <= (n-> x) && ((*a)-> yc) > (n-> y)){
            insererNoeudArbre(n, &((*a)-> se), *a);
            return;
        }
        if(((*a)-> xc) <= (n-> x) && ((*a)-> yc) <= (n-> y)){
            insererNoeudArbre(n, &((*a)-> ne), *a);
            return;
        }
        if(((*a)-> xc) > (n-> x) && ((*a)-> yc) <= (n-> y)){
            insererNoeudArbre(n, &((*a)-> no), *a);
            return;
        }
    
    // Si on est dans une cellule interne:
    } else {

        // On vérifie dans quel quadrant on va insérer le nouveau noeud.
        if(((*a)-> xc) > (n-> x) && ((*a)-> yc) > (n-> y)){
            insererNoeudArbre(n, &((*a)-> so), *a);
            return;
        }
        if(((*a)-> xc) <= (n-> x) && ((*a)-> yc) > (n-> y)){
            insererNoeudArbre(n, &((*a)-> se), *a);
            return;
        }
        if(((*a)-> xc) <= (n-> x) && ((*a)-> yc) <= (n-> y)){
            insererNoeudArbre(n, &((*a)-> ne), *a);
            return;
        }
        if(((*a)-> xc) > (n-> x) && ((*a)-> yc) <= (n-> y)){
            insererNoeudArbre(n, &((*a)-> no), *a);
            return;
        }
    }
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){

    // Si l'arbre est vide:
    if(*a == NULL){
        Noeud* nd = creer_noeud((R-> nbNoeuds)++, x, y);
        insererNoeudArbre(nd, a, parent);
        inserer_en_tete_noeud(&(R-> noeuds), nd);
        return nd;
    }

    // Si l'arbre est une feuille:
    if((*a)-> noeud != NULL){

        if( ( ((*a)-> noeud)-> x == x ) && ( ((*a)-> noeud)-> y == y ) ){
            return (*a)-> noeud;
        }

        Noeud* nd = creer_noeud((R-> nbNoeuds)++, x, y);
        insererNoeudArbre(nd, a, parent);
        inserer_en_tete_noeud(&(R-> noeuds), nd);

        return nd;
        
    }

    // Si l'arbre est une cellule interne:
    if((*a)-> noeud == NULL){
        if(  ( ((*a)-> xc) > x ) && ( ((*a)-> yc) > y )  ){
            return rechercheCreeNoeudArbre(R, &((*a)-> so), *a, x, y);
        }
        if(  ( ((*a)-> xc) <= x ) && ( ((*a)-> yc) > y )  ){
            return rechercheCreeNoeudArbre(R, &((*a)-> se), *a, x, y);
        }
        if(  ( ((*a)-> xc) <= x ) && ( ((*a)-> yc) <= y )  ){
            return rechercheCreeNoeudArbre(R, &((*a)-> ne), *a, x, y);
        }
        if(  ( ((*a)-> xc) > x ) && ( ((*a)-> yc) <= y )  ){
            return rechercheCreeNoeudArbre(R, &((*a)-> no), *a, x, y);
        }
    }

    return NULL;
}

Reseau* reconstitueReseauArbre(Chaines* C){
    if(C == NULL){
        return NULL;
    }

    Reseau* res = (Reseau*) malloc(sizeof(Reseau));
    if(res == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }
    
    res-> nbNoeuds = 0;
    res-> gamma = C-> gamma;

    // On utilise un ensemble de nœuds V qui est initialisé vide: V ← ∅
    res-> noeuds = NULL;
    res-> commodites = NULL;

    // Initialisation de l'arbre quaternaire:
    double xmin, xmax, ymin, ymax;
    xmin = ymin = DBL_MAX;
    xmax = ymax = DBL_MIN;

    ArbreQuat* parent = init_parent(C, &xmin, &ymin, &xmax, &ymax);

    ArbreQuat* aq = NULL;

    // Déclaration de variables temporaires:
    CellChaine* tmp_chaine = C-> chaines;

    CellPoint* tmp_point;
    Noeud* tmp_nd;
    Noeud* prec_nd;
    
    Noeud* extrA;
    Noeud* extrB;

    // On parcourt une à une chaque chaîne:
    while(tmp_chaine != NULL){
        prec_nd = NULL;
        tmp_point = tmp_chaine-> points;

        // On parcourt chaque point de la chaîne:
        while(tmp_point != NULL){

            tmp_nd = rechercheCreeNoeudArbre(res, &aq, parent, tmp_point-> x, tmp_point-> y);


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

        inserer_en_tete_commodite(&(res-> commodites), extrA, extrB);
        tmp_chaine = tmp_chaine-> suiv;
    }

    liberer_ArbreQuat(parent);
    return res;
}
